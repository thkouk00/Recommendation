#include "../../include/recommendation/recommend_main.h"

using namespace std;

void recommend_main(std::vector<std::vector<double>>& Points, std::vector<std::string>& tw_id, std::map<string, std::vector<std::vector<std::string>>>& map, std::vector<std::string>& users, std::map<string, std::vector<string>>& tweetId_map, std::map<std::string, double>& lexicon, std::vector<std::vector<std::string>>& coins, int& P)
{
	bool metric = 0;
	int coin_num = 5;
	double alpha = 15;		//default
	std::map<std::string, std::vector<double> > sentiment;
	std::map<std::string, std::vector<double> > normalized_sentiment;
	// holds empty position in every users coin vector
	std::map<std::string, std::vector<int>> empty_pos;
	// std::map<std::string, std::vector<double> >::iterator sentimentIt;

	std::vector<std::vector<std::pair <double, std::string>>> neighbors;
	std::map<std::string, std::vector<double>> predicted_values;
	// correct form for LSH
	std::vector<std::vector<double>> dataset;
	std::vector<std::string> centroids;
	
	// compute sentiment for every user according to his tweets
	sentiment_score(map, sentiment, lexicon, coins, alpha);
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);

	make_dataset(dataset, normalized_sentiment, users);
	find_neighbors(neighbors, dataset, users);
	predict_coins(predicted_values, neighbors, empty_pos, users, normalized_sentiment, P);
	sort_vector(predicted_values, empty_pos, coins, coin_num);

	// // Part 1, case B 
	// // 1 euclidean , 0 cosine
	// metric = 0;
	int k = 100;
	Cluster** cluster = new Cluster*[k];
	Cluster** cluster2 = new Cluster*[k];
	for (int i=0;i<k;i++)
	{
		cluster[i] = new Cluster;
		cluster2[i] = new Cluster;
	}


	sentiment.clear();
	normalized_sentiment.clear();
	empty_pos.clear();
	centroids.clear();
	dataset.clear();
	predicted_values.clear();
	neighbors.clear();
	cluster_main_func(cluster, Points, tw_id, centroids, k, metric);
	cluster_sentiment_score(cluster, centroids, tweetId_map, map, sentiment, lexicon, coins, alpha);
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);
	make_dataset(dataset, normalized_sentiment, centroids);
	find_neighbors(neighbors, dataset, centroids);
	predict_coins(predicted_values, neighbors, empty_pos, centroids, normalized_sentiment, P);
	coin_num = 2;
	sort_vector(predicted_values, empty_pos, coins, coin_num);

	for (int i=0;i<k;i++)
		cluster[i]->clear_structure();

	// // Part 2 , case A

	sentiment.clear();
	normalized_sentiment.clear();
	empty_pos.clear();
	centroids.clear();
	dataset.clear();
	predicted_values.clear();
	neighbors.clear();
	// compute sentiment for every user according to his tweets
	sentiment_score(map, sentiment, lexicon, coins, alpha);
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);
	make_dataset(dataset, normalized_sentiment, users);
	metric = 1; 		// euclidean
	cluster_main_func(cluster, dataset, users, centroids, k, metric);
	cluster_predict_coins(predicted_values, cluster, empty_pos, centroids, normalized_sentiment, P);
	coin_num = 5;
	sort_vector(predicted_values, empty_pos, coins, coin_num);

	for (int i=0;i<k;i++)
		cluster[i]->clear_structure();

	// Part 2, case B 
	// 1 euclidean , 0 cosine
	metric = 1;

	sentiment.clear();
	normalized_sentiment.clear();
	empty_pos.clear();
	centroids.clear();
	dataset.clear();
	predicted_values.clear();
	cluster_main_func(cluster, Points, tw_id, centroids, k, metric);
	cout <<"before here1"<<std::endl;
	cluster_sentiment_score(cluster, centroids, tweetId_map, map, sentiment, lexicon, coins, alpha);
	cout <<"after here1"<<std::endl;
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);
	cout <<"after here2"<<std::endl;
	make_dataset(dataset, normalized_sentiment, centroids);
	cout <<"before here1"<<std::endl;
	
	find_neighbors(neighbors, dataset, centroids);
	predict_coins(predicted_values, neighbors, empty_pos, centroids, normalized_sentiment, P);
	coin_num = 2;
	sort_vector(predicted_values, empty_pos, coins, coin_num);


	// std::map<std::string, std::vector<double> > sentiment2;
	// std::map<std::string, std::vector<double> > normalized_sentiment2;
	// // holds empty position in every users coin vector
	// std::map<std::string, std::vector<int>> empty_pos2;
	// // std::map<std::string, std::vector<double> >::iterator sentimentIt;

	// std::vector<std::vector<std::pair <double, std::string>>> neighbors2;
	// std::map<std::string, std::vector<double>> predicted_values2;
	// // correct form for LSH
	// // std::vector<std::vector<double>> dataset;
	// std::vector<std::string> centroids2;

	// cluster_main_func(cluster2, dataset, users, centroids2, k, metric);
	
	// // // // // edw thelei pali cluster
	// // // // // find_neighbors(neighbors, dataset, centroids);
	// // // // // // std::map<std::string, std::vector<double>> predicted_values;
	// // // // // predict_coins(predicted_values, neighbors, empty_pos, centroids, normalized_sentiment, P);
	// // // // cluster_main_func(cluster, dataset, users, centroids, k, metric);
	// cout <<"Before Predict"<<std::endl;
	// cluster_predict_coins(predicted_values2, cluster2, empty_pos2, centroids2, normalized_sentiment2, P);
	// cout <<"After Predict"<<std::endl;
	// coin_num = 2;
	// sort_vector(predicted_values2, empty_pos2, coins, coin_num);
	// cout <<"After sort"<<std::endl;

	for (int i=0;i<k;i++)
		delete cluster[i];
	delete[] cluster;

}