#include "../../include/recommendation/general_funcs.h"

using namespace std;

void make_dataset(std::vector<std::vector<double>>& dataset, std::map<std::string, std::vector<double>>& sentiment, std::vector<std::string>& users)
{
	std::map<std::string, std::vector<double>>::iterator sentimentIt;
	// copy sentiments vectors of all users in correct from for LSH
	// for (sentimentIt = sentiment.begin() ;sentimentIt != sentiment.end(); sentimentIt++)
	for (int i=0;i<users.size();i++)
	{
		sentimentIt = sentiment.find(users[i]);
		dataset.push_back(sentimentIt->second);
	}	
}

// find neighbors from Cosine LSH
void find_neighbors(std::vector<std::vector<std::pair <double, std::string>>>& neighbors, std::vector<std::vector<double>>& dataset, std::vector<std::string>& users)
{
	int k = 4;
	int L = 5;
	int w = 4;
 
	// std::vector<std::vector<std::pair <double, std::string>>> neighbors;
	// run LSH and take for every user their neighbors
	// std::vector<std::vector<double>> queryset = dataset;
	Search_Neighbors(neighbors, dataset, users, k, L, w);
	cout <<"neighbors size "<<neighbors.size()<<std::endl;
	cout <<"Neighbors1 "<<neighbors[0].size()<<std::endl;
	for (int j=0;j<neighbors.size();j++)
	{	
		sort(neighbors[j].begin(), neighbors[j].end());
		for (int i=0;i<neighbors[j].size();i++)
		{
			if (neighbors[j][i].first != 0)
			{
				cout <<"Neighbor "<<users[j]<<' ';
				cout <<neighbors[j][i].first<<" "<<neighbors[j][i].second<<std::endl;
			}

		}
		cout <<std::endl;
	}
}

void predict_coins(std::vector<std::vector<std::pair <double, std::string>>>& neighbors, std::map<std::string, std::vector<int>>& empty_pos, std::vector<std::string>& users,std::map<std::string, std::vector<double>>& normalized_sentiment)
{
	std::vector<double> similarity;
	for (int i=0;i<users.size();i++)
	{
		std::vector<std::pair <double, std::string>> neighbor = neighbors[i];
		for (int j=0;j<neighbor.size();j++)
		{
			// similarity.push_back()
		}
	}
}