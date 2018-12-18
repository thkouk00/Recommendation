#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include "../include/info_table.h"

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

		iss >> str;
		if (counter == 0)
		{
			if (!str.compare("P:"))
			{
				iss >> str;
				cout <<"P found "<<str<<std::endl;
				P = stoi(str);
				cout <<"Final P is "<<P<<std::endl;
			}
			else
			{
				user_id = str;
				iss >> str;
				tweet_id = str;
				cout <<"ID= "<<user_id<<" tweet id= "<<tweet_id<<" ";
				iss >> str;
				while (!iss.eof())
				{
					tmp_tweet.push_back(str);
					cout <<str<<" ";
					iss >> str;
				}
				cout <<std::endl;
			}
		}
		else
		{
			user_id = str;
			iss >> str;
			tweet_id = str;

			cout <<"ID= "<<user_id<<" tweet id= "<<tweet_id<<" ";

			iss >> str;
			while (!iss.eof())
			{
				tmp_tweet.push_back(str);
				cout <<str<<" ";
				iss >> str;
			}
			cout <<std::endl;
		}

		if (!user_id.compare(prev_id))
		{
			mapIt = map.find(user_id);
			cout <<user_id<<" and "<<prev_id<<std::endl;
			// add new tweet to user
			mapIt->second.push_back(tmp_tweet);
			
			tmapIt = tweetId_map.find(user_id);
			tmapIt->second.push_back(tweet_id);
		}
		else
		{
			cout <<"Different "<<user_id<<" and "<<prev_id<<std::endl;
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
		cout <<word<<" "<<word_value<<std::endl;
		lexicon[word] = word_value;
	}

	cout <<"lexicon size "<<lexicon.size()<<std::endl;

	return 0;
}