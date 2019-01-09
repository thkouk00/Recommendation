#include "../../include/recommendation/recommend_main.h"

using namespace std;

void recommend_main(std::vector<std::vector<double>>& Points, std::vector<std::string>& tw_id, std::map<string, std::vector<std::vector<std::string>>>& map, std::vector<std::string>& users, std::map<string, std::vector<string>>& tweetId_map, std::map<std::string, double>& lexicon, std::vector<std::vector<std::string>>& coins, int& P, std::ofstream& outputfile, bool validate)
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
	
	auto start = chrono::high_resolution_clock::now();
	outputfile <<"Cosine LSH"<<std::endl<<"Part 1:"<<std::endl;;
	// compute sentiment for every user according to his tweets
	sentiment_score(map, sentiment, lexicon, coins, alpha);
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);
	make_dataset(dataset, normalized_sentiment, users);
	find_neighbors(neighbors, dataset, users);
	predict_coins(predicted_values, neighbors, empty_pos, users, normalized_sentiment, P, metric);
	sort_vector(predicted_values, empty_pos, coins, coin_num, outputfile);

	auto end = chrono::high_resolution_clock::now();
	outputfile <<"Execution time: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<std::endl;
	
	if (validate)
		validation(sentiment, predicted_values, empty_pos, users, coins, coin_num, P, metric, outputfile);
	
	start = chrono::high_resolution_clock::now();
	outputfile <<std::endl<<"Part 2: "<<std::endl;
	// // Part 1, case B 
	// // 1 euclidean , 0 cosine
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
	predict_coins(predicted_values, neighbors, empty_pos, centroids, normalized_sentiment, P, metric);
	coin_num = 2;
	sort_vector(predicted_values, empty_pos, coins, coin_num, outputfile);

	end = chrono::high_resolution_clock::now();
	outputfile <<"Execution time: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<std::endl;

	for (int i=0;i<k;i++)
		cluster[i]->clear_structure();

	start = chrono::high_resolution_clock::now();
	outputfile <<std::endl<<"Clustering"<<std::endl<<"Part 1: "<<std::endl;
	// Part 2 , case A

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
	cluster_predict_coins(predicted_values, cluster, empty_pos, centroids, normalized_sentiment, P, metric);
	coin_num = 5;
	sort_vector(predicted_values, empty_pos, coins, coin_num, outputfile);

	end = chrono::high_resolution_clock::now();
	outputfile <<"Execution time: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<std::endl;

	for (int i=0;i<k;i++)
		cluster[i]->clear_structure();

	start = chrono::high_resolution_clock::now();
	outputfile <<std::endl<<"Part 2: "<<std::endl; 
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
	cluster_sentiment_score(cluster, centroids, tweetId_map, map, sentiment, lexicon, coins, alpha);
	sentiment_normalization(normalized_sentiment, sentiment, empty_pos);
	make_dataset(dataset, normalized_sentiment, centroids);
	find_neighbors(neighbors, dataset, centroids);
	predict_coins(predicted_values, neighbors, empty_pos, centroids, normalized_sentiment, P, metric);
	coin_num = 2;
	sort_vector(predicted_values, empty_pos, coins, coin_num, outputfile);

	end = chrono::high_resolution_clock::now();
	outputfile <<"Execution time: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<std::endl;

	for (int i=0;i<k;i++)
		delete cluster[i];
	delete[] cluster;

	outputfile.close();
}