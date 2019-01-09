#include "../../include/recommendation/validation.h"

using namespace std;

void validation(std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, std::vector<double>>& old_predicted_values, std::map<std::string, std::vector<int>>& old_empty_pos, std::vector<std::string>& users, std::vector<std::vector<std::string>>& coins, int& coin_num, int& P, bool metric, std::ofstream& outputfile)
{
	std::map<std::string, std::vector<double>>::iterator sentimentIt;
	std::map<std::string, std::vector<double> > normalized_sentiment;
	// // holds empty position in every users coin vector
	std::map<std::string, std::vector<int>> empty_pos;
	std::vector<int>::iterator emptyIt;
	std::vector<std::vector<std::pair <double, std::string>>> neighbors;
	std::map<std::string, std::vector<double>> predicted_values;
	std::map<std::string, std::vector<double>>::iterator predIt;
	// // correct form for LSH
	std::vector<std::vector<double>> dataset;

	double totalMAE = 0;
	// for every user
	for (sentimentIt = sentiment.begin(); sentimentIt != sentiment.end() ; sentimentIt++)
	{
		int start = 0;
		int end = start + 9;	// 10 - 1 = 9 , in order to take every time 10 elements between start - end
		
		std::vector<int> user_empty_pos ; 
		std::vector<int>::iterator emptyIt;
		// 10 - fold validation
		for (int i=0; i<10; i++)
		{
			std::map<std::string, std::vector<double>> new_sentiment = sentiment;
			// copy initial normalized_sentiment vector
			std::vector<double> new_sent = sentimentIt->second;
			
			std::vector<double> new_norm_sent(coins.size(),std::numeric_limits<double>::infinity());
			
			std::vector<int> changed_pos;
			// change different values in every loop according to start variable
			// change known values (if exist) to 0 (unknown)
			for (int j=start; j<=end; j++)
			{
				if (new_sent[j] != std::numeric_limits<double>::infinity())
					changed_pos.push_back(j);
				new_sent[j] = std::numeric_limits<double>::infinity();
			}	
			
			new_sentiment[sentimentIt->first] = new_sent;
			sentiment_normalization(normalized_sentiment, new_sentiment, empty_pos);
			make_dataset(dataset, normalized_sentiment, users);
			std::vector<std::vector<double>> queryset;
			queryset.push_back(normalized_sentiment[sentimentIt->first]);
			
			int k = 4;
			int L = 5;
			int w = 4;
			Search_Neighbors(neighbors, dataset, queryset, users, k, L, w);
			for (int j=0;j<neighbors.size();j++)
				sort(neighbors[j].begin(), neighbors[j].end());

			predict_singleuser_coins(predicted_values, neighbors, empty_pos, sentimentIt->first, normalized_sentiment, P, metric);
			// sort_vector_np(predicted_values, empty_pos, coins, coin_num);


			// prepare for MAE
			double MAE = 0;
			for (int j=0; j<changed_pos.size(); j++)
				MAE += abs(old_predicted_values[sentimentIt->first][changed_pos[j]]-predicted_values[sentimentIt->first][changed_pos[j]]);
			
			if (changed_pos.size() != 0)
			{
				MAE = MAE / changed_pos.size();
				totalMAE += MAE;
			}
			
			start += 10;
			end = start + 9;
			new_sent.clear();
			new_norm_sent.clear();
			user_empty_pos.clear();
			new_norm_sent.clear();
			// new_empty_pos.clear();
			neighbors.clear();
			predicted_values.clear();
			normalized_sentiment.clear();
			dataset.clear();
			queryset.clear();
			new_sentiment.clear();
		}
	}
	totalMAE = totalMAE / sentiment.size();
	cout <<"TOTAL MAE = "<<totalMAE<<std::endl;
}
