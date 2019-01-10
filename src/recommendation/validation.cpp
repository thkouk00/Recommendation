#include "../../include/recommendation/validation.h"

using namespace std;

void validation(std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, std::vector<double>>& predicted_values, std::map<std::string, std::vector<int>>& empty_pos, std::vector<std::string>& users, std::vector<std::vector<std::string>>& coins, int& coin_num, int& P, bool metric, std::ofstream& outputfile)
{
	std::map<std::string, std::vector<double> > new_sentiment;
	std::map<std::string, std::vector<double>>::iterator sentimentIt;
	std::map<std::string, std::vector<double> > normalized_sentiment;
	// // holds empty position in every users coin vector
	std::map<std::string, std::vector<int>> new_empty_pos;
	std::vector<int>::iterator eposIt;
	std::map<std::string, std::vector<int>>::iterator emptyIt;
	std::vector<std::vector<std::pair <double, std::string>>> neighbors;
	std::map<std::string, std::vector<double>> new_predicted_values;
	std::map<std::string, std::vector<double>>::iterator predIt;
	// // correct form for LSH
	std::vector<std::vector<double>> dataset;

	double avgMAE = 0;
	int number_of_groups = sentiment.size()/10;
	int group_counter = 0;
	// cout <<"dataset "<<sentiment.size()<<" and div "<<sentiment.size()/10<<std::endl;
	std::vector<std::string> group;
	for (sentimentIt = sentiment.begin(); sentimentIt != sentiment.end(); sentimentIt++)
	{
		group.push_back(sentimentIt->first);

		group_counter++;
		if (group_counter % number_of_groups == 0)
		{
			cout <<"LOOP "<<group_counter<<std::endl;
			new_sentiment = sentiment;
			int known_vals = 0;
			std::vector<std::string>::iterator vecIt;
			for (vecIt = group.begin(); vecIt != group.end(); vecIt++)
			{
				emptyIt = empty_pos.find(*vecIt);
				known_vals += emptyIt->second.size();
			}
			// number of values to change
			known_vals = known_vals / 10;
			
			std::map<std::string, int> changed_pos;
			std::map<std::string, int>::iterator change_posIt;
			
			int temp_val = 0;
			for (int i=0; i<group.size(); i++)
			{
				if (temp_val == known_vals)
					break;
				for (int j=0; j<empty_pos[group[i]].size(); j++)
				{
					eposIt = find(empty_pos[group[i]].begin(), empty_pos[group[i]].end(), j);
					if (eposIt == empty_pos[group[i]].end())
					{
						// new_sentiment[group[i]][j] = std::numeric_limits<double>::infinity();
						changed_pos[group[i]] = j;
						break;
					}
					
				}

				temp_val++;
			}
			cout <<"Setting new sentiment"<<std::endl;
			// must change values now in new_sentiment
			for (change_posIt = changed_pos.begin(); change_posIt != changed_pos.end(); change_posIt++)
			{
				new_sentiment[change_posIt->first][change_posIt->second] = std::numeric_limits<double>::infinity();
			}

			cout <<"before main func"<<std::endl;
			// now ready to predict coins as always
			sentiment_normalization(normalized_sentiment, new_sentiment, new_empty_pos);
			make_dataset(dataset, normalized_sentiment, users);
			cout <<"before find"<<std::endl;
			find_neighbors(neighbors, dataset, users);
			cout <<"before predict"<<std::endl;
			predict_coins(new_predicted_values, neighbors, new_empty_pos, users, normalized_sentiment, P, metric);
			cout <<"after predict"<<std::endl;
			// sort_vector(predicted_values, empty_pos, coins, coin_num, outputfile);

			double MAE = 0;
			for (change_posIt = changed_pos.begin(); change_posIt != changed_pos.end(); change_posIt++)
			{
				MAE += abs(sentiment[change_posIt->first][change_posIt->second] - new_predicted_values[change_posIt->first][change_posIt->second]);
				cout <<"Sub: "<<sentiment[change_posIt->first][change_posIt->second]<<" - "<<new_predicted_values[change_posIt->first][change_posIt->second]<<" = "<<sentiment[change_posIt->first][change_posIt->second] - new_predicted_values[change_posIt->first][change_posIt->second]<<std::endl;
			}
			avgMAE += MAE / group.size();
			dataset.clear();
			normalized_sentiment.clear();
			neighbors.clear();
			new_sentiment.clear();
			new_predicted_values.clear();
			new_empty_pos.clear();
			changed_pos.clear();
		}
		group.clear();
		if (group_counter == 10*number_of_groups)
			break;
	}

	cout <<"MAE = "<<avgMAE/10<<std::endl;
	outputfile <<"MAE = "<<avgMAE/10<<std::endl;
}
