#include "../../include/recommendation/recommend_main.h"

using namespace std;

void recommend_main(std::vector<std::vector<double>>& Points, std::vector<std::string>& tw_id, std::map<string, std::vector<std::vector<std::string>>>& map, std::vector<std::string>& users, std::map<string, std::vector<string>>& tweetId_map, std::map<std::string, double>& lexicon, std::vector<std::vector<std::string>>& coins, int& P)
{
	int coin_num = 5;
	std::map<std::string, std::vector<double> > sentiment;
	std::map<std::string, std::vector<double> > normalized_sentiment;
	// holds empty position in every users coin vector
	std::map<std::string, std::vector<int>> empty_pos;
	std::map<std::string, std::vector<double> >::iterator sentimentIt;

	std::vector<std::vector<std::pair <double, std::string>>> neighbors;
	// correct form for LSH
	std::vector<std::vector<double>> dataset;

	double alpha = 15;		//default
	
	// compute sentiment for every user according to his tweets
	// sentiment_score(map, sentiment, lexicon, coins, alpha);
	// sentiment_normalization(normalized_sentiment, sentiment, empty_pos);

	sentimentIt = normalized_sentiment.find("1589");
	//  check for normalized
	// for (sentimentIt = normalized_sentiment.begin(); sentimentIt!= normalized_sentiment.end();sentimentIt++)
	// // // for (sentimentIt = sentiment.begin(); sentimentIt!= sentiment.end();sentimentIt++)
	// {	
		// cout <<sentimentIt->second.size()<<std::endl;
		for (int i=0;i<sentimentIt->second.size();i++)
		{
			cout <<sentimentIt->second[i]<<' ';
		}
		cout <<std::endl;
		cout <<std::endl;
	// }

	// make_dataset(dataset, normalized_sentiment, users);
	// find_neighbors(neighbors, dataset, users);
	// std::map<std::string, std::vector<double>> predicted_values;
	// predict_coins(predicted_values, neighbors, empty_pos, users, normalized_sentiment, P);
	// sort_vector(predicted_values, empty_pos, coins, coin_num);
	
	// dika mou gia test
	// cout <<"Inf "<<std::numeric_limits<double>::infinity()<<std::endl;
	// sentimentIt = sentiment.find("40");
	// cout <<"Size "<<sentimentIt->second.size()<<std::endl;
	// for (int i=0;i<sentimentIt->second.size();i++)
	// {
	// 	cout <<sentimentIt->second[i]<<' ';
	// }
	// cout <<std::endl;
	// cout <<std::endl;

	// sentimentIt = normalized_sentiment.find("1589");
	// //  check for normalized
	// // for (sentimentIt = normalized_sentiment.begin(); sentimentIt!= normalized_sentiment.end();sentimentIt++)
	// // // // for (sentimentIt = sentiment.begin(); sentimentIt!= sentiment.end();sentimentIt++)
	// // {	
	// 	// cout <<sentimentIt->second.size()<<std::endl;
	// 	for (int i=0;i<sentimentIt->second.size();i++)
	// 	{
	// 		cout <<sentimentIt->second[i]<<' ';
	// 	}
	// 	cout <<std::endl;
	// 	cout <<std::endl;
	// // }

	int k = 100;
	Cluster** cluster = new Cluster*[k];
	for (int i=0;i<k;i++)
		cluster[i] = new Cluster;

	std::vector<std::string> centroids;
	cout <<"Perase apo edw"<<std::endl;
	cluster_main_func(cluster, Points, tw_id, centroids, k);
	cout <<"After cluster main"<<std::endl;

	sentiment.clear();
	normalized_sentiment.clear();
	cluster_sentiment_score(cluster, centroids, tweetId_map, map, sentiment, lexicon, coins, alpha);
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);
	make_dataset(dataset, normalized_sentiment, centroids);
	find_neighbors(neighbors, dataset, centroids);
	std::map<std::string, std::vector<double>> predicted_values;
	predict_coins(predicted_values, neighbors, empty_pos, centroids, normalized_sentiment, P);
	coin_num = 2;
	sort_vector(predicted_values, empty_pos, coins, coin_num);



	for (int i=0;i<k;i++)
		delete cluster[i];
	delete[] cluster;

}