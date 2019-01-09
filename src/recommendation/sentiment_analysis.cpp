#include "../../include/recommendation/sentiment_analysis.h"

using namespace std;

void sentiment_score(std::map<std::string, std::vector<std::vector<std::string>>>& map, std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, double>& lexicon, std::vector<std::vector<std::string> >& coins, double& alpha)
{
	// clear structure if not empty
	sentiment.clear();

	std::map<std::string, std::vector<std::vector<std::string>>>::iterator mapIt;
	std::map<std::string, std::vector<double> >::iterator sentimentIt;
	std::map<std::string, double>::iterator lexiconIt;
	std::vector<std::string>::iterator coinsIt;
	
	std::vector<std::vector<std::string> > tmp_vec;
	std::string word;
	double score = 0;
	tmp_vec.clear();
	for (mapIt = map.begin();mapIt != map.end();mapIt++)
	{
		// store the vector index of the coins referred
		std::vector<int> coins_referred;
		std::vector<int>::iterator coins_referredIt;
		tmp_vec = mapIt->second;
		
		// initialize vector with infinity to all values
		std::vector<double> vec(coins.size(),std::numeric_limits<double>::infinity());
		// check every tweet of user
		for (int i=0;i<tmp_vec.size();i++)
		{
			double si = 0;
			score = 0;
			for (int j=0;j<tmp_vec[i].size();j++)
			{
				word = tmp_vec[i][j];
				// cout <<word<<' ';
				lexiconIt = lexicon.find(word);
				if (lexiconIt != lexicon.end())
				{
					// word found in lexicon
					score += lexiconIt->second;
					// cout <<lexiconIt->second<<" word "<<word<<std::endl;
				}
				else
				{	
					for (int x=0;x<coins.size();x++)
					{
						// check if word refers to coin
						coinsIt = find(coins[x].begin(), coins[x].end(), word);
						if (coinsIt != coins[x].end())
						{
							// check if coin is already in coins_referred vector
							coins_referredIt = find(coins_referred.begin(),coins_referred.end(),x);
							if (coins_referredIt != coins_referred.end())
								continue;
							else
								coins_referred.push_back(x);
						}
					}
				}
			}
			si = score / (sqrt((score*score)+alpha));
			
			for (int x=0;x<coins_referred.size();x++)
			{
				if (vec[coins_referred[x]] == std::numeric_limits<double>::infinity())
					vec[coins_referred[x]] = si;
				else
					vec[coins_referred[x]] += si;
			}
			coins_referred.clear();
		}
		sentiment[mapIt->first] = vec;
		vec.clear();
	}
}




void sentiment_normalization(std::map<std::string, std::vector<double>>& normalized_sentiment, std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, std::vector<int>>& empty_pos)
{
	// clear structure if not empty
	normalized_sentiment.clear();

	std::map<std::string, std::vector<double>>::iterator sentimentIt;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt;

	int counter = 0;
	std::vector<int> empty_slots;
	// position of slot with value
	std::vector<double> temp;

	for (sentimentIt = sentiment.begin();sentimentIt!=sentiment.end();sentimentIt++)
	{
		double total_score = 0;
		// initialize vector
		std::vector<double> normalized(sentimentIt->second.size());
		for (int i=0;i<sentimentIt->second.size();i++)
		{
			if (sentimentIt->second[i] != std::numeric_limits<double>::infinity())
			{
				temp.push_back(i);
				total_score += sentimentIt->second[i];
				// counter++;
			}
			else
			{
				// position of empty slot
				empty_slots.push_back(i);
				// put 0 to empty slot in  normalized version
				normalized[i] = 0;
			}
		}	
		double average = 0;
		if (temp.size() == 0)
			average = 0;
		else
			average = total_score / temp.size();
		
		for (int i=0;i<temp.size();i++)
		{
			// new normalized value
			double val = sentimentIt->second[temp[i]] - average;
			normalized[temp[i]] = val;
		} 
		normalized_sentiment[sentimentIt->first] = normalized;

		empty_pos[sentimentIt->first] = empty_slots;
		temp.clear();
		empty_slots.clear();
		normalized.clear();
	}
}

void cluster_sentiment_score(Cluster** cluster, std::vector<std::string>& centroids, std::map<string, std::vector<string>>& tweetId_map, std::map<std::string, std::vector<std::vector<std::string>>>& map, std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, double>& lexicon, std::vector<std::vector<std::string> >& coins, double& alpha)
{
	// clear structure if not empty
	sentiment.clear();

	double score = 0;
	std::string word;
	std::vector<string>::iterator idIt;
	std::map<std::string, double>::iterator lexiconIt;
	std::vector<std::string>::iterator coinsIt;
	std::map<string, std::vector<string>>::iterator tweetIt;
	std::map<std::string, std::vector<std::vector<std::string>>>::iterator mapIt;

	std::vector<int> coins_referred;
	std::vector<int>::iterator coins_referredIt;

	for (int i=0;i<centroids.size();i++)
	{
		// initialize vector with infinity to all values
		std::vector<double> vec(coins.size(),std::numeric_limits<double>::infinity());
		std::vector<Info> v = cluster[i]->get_array();
		for (int j=0;j<v.size();j++)
		{
			// tweet id 
			std::string id = v[j].get_Point_Id();
			// if id == centroid_id continue
			if (id == centroids[i])
				continue;
			// search it in tweetId_map
			for (tweetIt = tweetId_map.begin(); tweetIt != tweetId_map.end(); tweetIt++)
			{
				idIt = find(tweetIt->second.begin(), tweetIt->second.end(), id);
				// id not found 
				if (idIt == tweetIt->second.end())
					continue;

				// id found, find index in vector
				int dist = std::distance(tweetIt->second.begin(), idIt);
				// now we know map key and index so we can calc sentiment
				mapIt = map.find(tweetIt->first);
				std::vector<std::string> temp_tweet = mapIt->second[dist];
				double si = 0;
				score = 0;
				for (int y=0;y<temp_tweet.size();y++)
				{
					word = temp_tweet[y];
					lexiconIt = lexicon.find(word);
					if (lexiconIt != lexicon.end())
					{
						// word found in lexicon
						score += lexiconIt->second;
						// cout <<lexiconIt->second<<" word "<<word<<std::endl;
					}
					else
					{	
						for (int x=0;x<coins.size();x++)
						{
							// check if word refers to coin
							coinsIt = find(coins[x].begin(), coins[x].end(), word);
							if (coinsIt != coins[x].end())
							{
								// check if coin is already in coins_referred vector
								coins_referredIt = find(coins_referred.begin(),coins_referred.end(),x);
								if (coins_referredIt != coins_referred.end())
									continue;
								else
									coins_referred.push_back(x);
							}
						}
					}
				}
				si = score / (sqrt((score*score)+alpha));
				// cout <<"Score for user "<<centroids[i]<<" is "<<score<<" and si "<<si<<std::endl;
				
				for (int x=0;x<coins_referred.size();x++)
				{
					if (vec[coins_referred[x]] == std::numeric_limits<double>::infinity())
						vec[coins_referred[x]] = si;
					else
						vec[coins_referred[x]] += si;
				}
				coins_referred.clear();
			}
		}
		// map sentiment with key the clusters id not the users id that used in lsh
		sentiment[centroids[i]] = vec;
		vec.clear();
	}
}