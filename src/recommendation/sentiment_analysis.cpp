#include "../../include/recommendation/sentiment_analysis.h"

using namespace std;

void sentiment_score(std::map<std::string, std::vector<std::vector<std::string>>>& map, std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, double>& lexicon, std::vector<std::vector<std::string> >& coins, double& alpha)
{
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
			cout <<"Score for user "<<mapIt->first<<" is "<<score<<" and si "<<si<<std::endl;
			
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
		double average = total_score / temp.size();
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