#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../../include/recommendation/recommend_main.h"
#include "../../include/recommendation/sentiment_analysis.h"
#include "../../include/recommendation/general_funcs.h"
#include "../../include/cluster/Cluster.h"

using namespace std;

void Usage()
{
	cerr <<"./recommendation -d <input file> –o <output file> -validate (optional)"<<endl;
}

int main(int argc, char const *argv[])
{
	int P = 20; 	//default value
	std::string input_file;
	std::string output_file;
	bool validate = 0;
	for (int i=0;i<argc;i++)
	{
		if (!strcmp(argv[i],"-h"))
		{
			cout <<argv[0]<<" -d <input file> –o <output file> -validate (optional)"<<endl;
			exit(1);
		}
		else if (!strcmp(argv[i],"-d"))
		{
			if (i+1 < argc)
			{
				input_file = argv[i+1];	
				cout << "Input file: " <<input_file<< endl;
			}
			else
			{
				Usage();
				exit(1);
			}
		}
		else if (!strcmp(argv[i],"-o"))
		{
			if (i+1 < argc)
			{
				output_file = argv[i+1];
				cout << "Output file: " << output_file << endl;
			}
			else
			{
				Usage();
				exit(1);
			}
		}
		else if (!strcmp(argv[i],"-validate"))
		{
			validate = 1;
		}
	}
	
	// check if arguments are correct
	if (input_file.size() == 0 || output_file.size() == 0)
	{
		cout <<"Please give correct input and output file"<<std::endl;
		exit(-1);
	}

	std::vector<std::string> users;
	// map with user_id as key and 2d vector as value , where 2d vector holds every tweet from user
	std::map<string, std::vector<std::vector<std::string> > > map;
	// map with user_id as key and vector to hold tweet_id as value
	std::map<string, std::vector<string> > tweetId_map;
	// iterator for map
	std::map<string, std::vector<std::vector<std::string> > >::iterator mapIt;
	// iterator for tweetId_map
	std::map<string, std::vector<string> >::iterator tmapIt;
	// first column of dataset
	string user_id;
	string prev_id;
	// second column of dataset
	string tweet_id;
	// temp vector to hold tweet
	std::vector<std::string> tmp_tweet;
	// temp 2d vector , used for map
	std::vector<std::vector<std::string> > tmp_vec;
	
	string line;                      
	string str;
	std::ifstream file(input_file);
	int counter = 0;
	while (getline(file, line))
	{
		std::istringstream iss (line);
		if (counter == 0)
		{
			getline(iss,str,'\t');
			if (!str.compare("P:"))
			{
				// iss >> str;
				getline(iss,str,'\t');
				// cout <<"P found "<<str<<std::endl;
				P = stoi(str);
				// cout <<"Final P is "<<P<<std::endl;
				counter++;
				continue;
			}
			else
			{
				user_id = str;
				// getline(iss,user_id,'\t');
				getline(iss,tweet_id,'\t');
				// cout <<"ID= "<<user_id<<" tweet id= "<<tweet_id<<"	";
				while (getline(iss, str, '\t'))
				{
					tmp_tweet.push_back(str);
					// cout <<str<<"	";
				}

				// cout <<std::endl;
			}
		}
		else
		{
			getline(iss,user_id,'\t');
			getline(iss,tweet_id,'\t');
			// cout <<"ID= "<<user_id<<" tweet id= "<<tweet_id<<"	";
			while (getline(iss, str, '\t'))
			{
				tmp_tweet.push_back(str);
				// cout <<str<<"	";
			}
			// cout <<std::endl;
		}

		if (!user_id.compare(prev_id))
		{
			mapIt = map.find(user_id);
			// add new tweet to user
			mapIt->second.push_back(tmp_tweet);
			
			tmapIt = tweetId_map.find(user_id);
			tmapIt->second.push_back(tweet_id);
		}
		else
		{
			users.push_back(user_id);
			// better way than iterating map every time
			prev_id = user_id;
			tmp_vec.push_back(tmp_tweet);
			map[user_id] = tmp_vec;

			tmp_tweet.clear();
			tmp_tweet.push_back(tweet_id);
			tweetId_map[user_id] = tmp_tweet;
		}


		counter++;
		tmp_tweet.clear();
		tmp_vec.clear();
	}
	
	// cout <<"Total size "<<map.size()<<" and  "<<counter<<std::endl;
	// cout <<"SSize "<<users.size()<<std::endl;

	string word;
	double word_value;
	// map lexicon
	std::map<std::string, double> lexicon;
	std::map<std::string, double>::iterator lexiconIt;
	std::ifstream file2("vader_lexicon.csv");
	while (getline(file2, line))
	{
		std::istringstream iss (line);
		// extract word and value , tab separated
		getline(iss, word,'\t');
		getline(iss, str,'\t');
		word_value = stod(str);
		// cout <<word<<" "<<word_value<<std::endl;
		lexicon[word] = word_value;
	}
	cout <<"lexicon size "<<lexicon.size()<<std::endl;

	// reuse temp vector
	tmp_tweet.clear();
	std::vector<std::vector<std::string> > coins;
	std::vector<std::string>::iterator coinsIt;

	std::ifstream file3("coins_queries.csv");
	while (getline(file3, line))
	{
		std::istringstream iss (line);
		// getline(iss, word,'\t');
		// iss >> str;
		while(getline(iss, str, '\t'))
        {
            // cout << str << '\n';
            tmp_tweet.push_back(str);
			// 	cout <<str<<std::endl;
        }
		coins.push_back(tmp_tweet);
		tmp_tweet.clear();
		// while (!iss.eof())
		// {
		// 	tmp_tweet.push_back(str);
		// 	cout <<str<<std::endl;
		// 	iss >> str;
		// }
	}

	std::vector<std::string> tw_id;
	std::vector<std::vector<double>> Points;
	std::vector<double> tempv;

	// string str;                      // This will store your tokens
	std::ifstream file4("twitter_dataset_small_v2.csv");
	counter = 0;
	while (getline(file4, line))
	{
		// cout <<line<<std::endl;
		std::istringstream ss(line);
		std::string token;
		// int id ;
		getline(ss,token,',');
		tw_id.push_back(token);
		counter = 0;
		while (getline(ss, token, ','))   // You can have a different delimiter
		{
			tempv.push_back(stod(token));
			
			// cout <<tempv[counter]<<std::endl;
			counter++;	
		}
		Points.push_back(tempv);
		tempv.clear();
	}
	
	std::ofstream outputfile;
	outputfile.open (output_file, ios::out | ios::trunc);
	recommend_main(Points, tw_id, map, users, tweetId_map, lexicon, coins, P, outputfile, validate);
	outputfile.close();
	
	return 0;
}