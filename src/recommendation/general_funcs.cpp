#include "../../include/recommendation/general_funcs.h"

using namespace std;

double add_vectorElements(std::vector<double> A)
{
	double result = 0;
	for (int i=0;i<A.size();i++)
		result += A[i];
	
	return result;
}

std::vector<double> compute_similarity(int& vector_size, std::string& user, std::vector<std::pair <double, std::string>>& neighbor, std::map<std::string, std::vector<double>>& normalized_sentiment, bool metric)
{
	std::vector<double> sim;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt1;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt2;

	norm_sentimentIt1 = normalized_sentiment.find(user);
	pair<double, std::string> Pair;
	// changed it here
	for (int i=0;i<vector_size;i++)
	{
		Pair = neighbor[i];
		norm_sentimentIt2 = normalized_sentiment.find(Pair.second);
		if (metric == 1)
			sim.push_back(1/(1 + Euclidean_Distance(norm_sentimentIt1->second, norm_sentimentIt2->second)));
		else
			sim.push_back(Cosine_Similarity(norm_sentimentIt1->second, norm_sentimentIt2->second));
	}

	return sim;	
}

std::vector<double> cluster_compute_similarity(int& vector_size, std::string& user, std::vector<Info> neighbor, std::map<std::string, std::vector<double>>& normalized_sentiment, bool metric)
{
	std::vector<double> sim;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt1;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt2;

	norm_sentimentIt1 = normalized_sentiment.find(user);
	for (int i=0;i<vector_size;i++)
	{
		norm_sentimentIt2 = normalized_sentiment.find(neighbor[i].get_Point_Id());
		if (metric == 1)
			sim.push_back(1/(1 + Euclidean_Distance(norm_sentimentIt1->second, norm_sentimentIt2->second)));
		else
			sim.push_back(Cosine_Similarity(norm_sentimentIt1->second, norm_sentimentIt2->second));
	}

	return sim;	
}

void sort_vector(std::map<std::string, std::vector<double>>& predicted_values, std::map<std::string, std::vector<int>>& empty_pos, std::vector<std::vector<std::string>>& coins, int& coin_num, std::ofstream& outputfile)
{
	// den xreiazetai nomizw h sort edw, na xanadw
	std::map<std::string, std::vector<int>>::iterator posIt;
	std::map<std::string, std::vector<double>>::iterator predIt;
	for (predIt = predicted_values.begin(); predIt != predicted_values.end(); predIt++)
	{
		posIt = empty_pos.find(predIt->first);

		std::vector<pair<double, int>> vec;
		// reverse iterator
		std::vector<pair<double, int>>::reverse_iterator rit; 
		for (int i=0;i<predIt->second.size();i++)
		{
			vec.push_back(make_pair(predIt->second[i], posIt->second[i]));
		}
		sort(vec.begin(), vec.end());
		int counter = 0;
		// cout <<"---------------------------------------------------------------"<<std::endl;
		// cout <<"User: "<<predIt->first<<std::endl;
		outputfile <<"<"<<predIt->first<<">	";
		// coin_num -> 5 (case A) or 2 (case B)
		for (rit = vec.rbegin(); rit != vec.rend(), counter < coin_num; rit++, counter++)
		{
			// if (rit->first != 0)
			// {
				// cout <<"Coin with value "<<rit->first<<" in position "<<rit->second;
				if (coins[rit->second].size() >= 5)
				{
					// cout <<" is "<<coins[rit->second][4]<<std::endl;
					outputfile <<coins[rit->second][4]<<'	';
				}
				else
				{
					// cout <<" is "<<coins[rit->second][0]<<std::endl;
					outputfile <<coins[rit->second][0]<<'	';
				}	

			// }
		}
		outputfile <<std::endl;
	}
}

void make_dataset(std::vector<std::vector<double>>& dataset, std::map<std::string, std::vector<double>>& sentiment, std::vector<std::string>& users)
{
	// // clear structure if not empty
	// dataset.clear();

	std::map<std::string, std::vector<double>>::iterator sentimentIt;
	// copy sentiments vectors of all users in correct form for LSH
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
	// cout <<"Dataset size  "<<dataset.size()<<std::endl;
	Search_Neighbors(neighbors, dataset, users, k, L, w);
	// cout <<"neighbors size "<<neighbors.size()<<std::endl;
	// cout <<"Neighbors1 "<<neighbors[0].size()<<std::endl;
	for (int j=0;j<neighbors.size();j++)
	{	
		sort(neighbors[j].begin(), neighbors[j].end());
		// for (int i=0;i<neighbors[j].size();i++)
		// {
		// 	if (neighbors[j][i].first != 0)
		// 	{
		// 		cout <<"Neighbor "<<users[j]<<' ';
		// 		cout <<neighbors[j][i].first<<" "<<neighbors[j][i].second<<std::endl;
		// 	}

		// }
		// cout <<std::endl;
	}
}

void predict_singleuser_coins(std::map<std::string, std::vector<double>>& predicted_values, std::vector<std::vector<std::pair <double, std::string>>>& neighbors, std::map<std::string, std::vector<int>>& empty_pos, std::string user, std::map<std::string, std::vector<double>>& normalized_sentiment, int& P, bool metric)
{
	// user -> string
	// normalized sentiment olo
	// neighbors mono to 1 stoixeio pou exei


	int vector_size;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt1;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt2;
	// for (int i=0;i<users.size();i++)
	// {
		// double similaritySum = 0;
		std::vector<double> results;
		std::vector<std::pair <double, std::string>> neighbor = neighbors[0];
		
		std::map<std::string, std::vector<int>>::iterator posIt;
		posIt = empty_pos.find(user);
		std::vector<int> positions = posIt->second;
		pair<double, std::string> p;
		
		// if user has less than P neighbors take them all, else take only P 
		if (neighbor.size() > P)
			vector_size = P;
		else
			vector_size = neighbor.size();
		// cout <<"Number of neighbors "<<neighbor.size()<<" final number "<<vector_size<<std::endl;
		
		norm_sentimentIt1 = normalized_sentiment.find(user);
		// compute similarities once
		std::vector<double> sim = compute_similarity(vector_size, user, neighbor, normalized_sentiment, metric);
		// for (int j=0;j<sim.size();j++)
		// 	cout <<sim[j]<<' ';
		// cout <<std::endl;
		for (int j=0;j<positions.size();j++)
		{
			// double sim = 0;
			double prediction = 0;
			pair<double, std::string> Pair;
			for (int k=0;k<vector_size;k++)
			{
				Pair = neighbor[k];
				norm_sentimentIt2 = normalized_sentiment.find(Pair.second);
				// sim = Cosine_Similarity(norm_sentimentIt1->second, norm_sentimentIt2->second);
				// similarity.push_back(sim);
				// similaritySum += sim;
				prediction += sim[k] * norm_sentimentIt2->second[positions[j]];
			}
			// prediction = prediction * (1 / similaritySum);
			double sum = add_vectorElements(sim);
			if (sum != 0)
				prediction = prediction * (1 / add_vectorElements(sim));
			else
				prediction = 0;
			// update vector with new predicted value
			// norm_sentimentIt1->second[positions[j]] = prediction;
			results.push_back(prediction);

			// cout <<"User "<<users[i]<<" Spot "<<positions[j]<<" and new val "<<prediction<<std::endl;
		}
		predicted_values[user] = results;
		results.clear();
		sim.clear();
	// }
}




void predict_coins(std::map<std::string, std::vector<double>>& predicted_values, std::vector<std::vector<std::pair <double, std::string>>>& neighbors, std::map<std::string, std::vector<int>>& empty_pos, std::vector<std::string>& users, std::map<std::string, std::vector<double>>& normalized_sentiment, int& P, bool metric)
{
	int vector_size;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt1;
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt2;
	for (int i=0;i<users.size();i++)
	{
		// double similaritySum = 0;
		std::vector<double> results;
		std::vector<std::pair <double, std::string>> neighbor = neighbors[i];
		
		std::map<std::string, std::vector<int>>::iterator posIt;
		posIt = empty_pos.find(users[i]);
		std::vector<int> positions = posIt->second;
		pair<double, std::string> p;
		
		// if user has less than P neighbors take them all, else take only P 
		if (neighbor.size() > P)
			vector_size = P;
		else
			vector_size = neighbor.size();
		// cout <<"Number of neighbors "<<neighbor.size()<<" final number "<<vector_size<<std::endl;
		
		norm_sentimentIt1 = normalized_sentiment.find(users[i]);
		// compute similarities once
		std::vector<double> sim = compute_similarity(vector_size, users[i], neighbor, normalized_sentiment, metric);
		// for (int j=0;j<sim.size();j++)
		// 	cout <<sim[j]<<' ';
		// cout <<std::endl;
		for (int j=0;j<positions.size();j++)
		{
			// double sim = 0;
			double prediction = 0;
			pair<double, std::string> Pair;
			for (int k=0;k<vector_size;k++)
			{
				Pair = neighbor[k];
				norm_sentimentIt2 = normalized_sentiment.find(Pair.second);
				// sim = Cosine_Similarity(norm_sentimentIt1->second, norm_sentimentIt2->second);
				// similarity.push_back(sim);
				// similaritySum += sim;
				prediction += sim[k] * norm_sentimentIt2->second[positions[j]];
			}
			// prediction = prediction * (1 / similaritySum);
			double sum = add_vectorElements(sim);
			if (sum != 0)
				prediction = prediction * (1 / add_vectorElements(sim));
			else
				prediction = 0;
			// update vector with new predicted value
			// norm_sentimentIt1->second[positions[j]] = prediction;
			results.push_back(prediction);

			// cout <<"User "<<users[i]<<" Spot "<<positions[j]<<" and new val "<<prediction<<std::endl;
		}
		predicted_values[users[i]] = results;
		results.clear();
		sim.clear();
	}
}

void cluster_predict_coins(std::map<std::string, std::vector<double>>& predicted_values, Cluster** cluster, std::map<std::string, std::vector<int>>& empty_pos, std::vector<std::string>& centroids, std::map<std::string, std::vector<double>>& normalized_sentiment, int& P, bool metric)
{
	std::map<std::string, std::vector<double>>::iterator norm_sentimentIt2;
	int vector_size = 0;
	for (int i=0;i<centroids.size();i++)
	{
		std::vector<double> results;
		std::map<std::string, std::vector<int>>::iterator posIt;
		posIt = empty_pos.find(centroids[i]);
		std::vector<int> positions = posIt->second;
		// cout <<"EMPTY POS "<<positions.size()<<" for user "<<centroids[i]<<std::endl;
		std::vector<Info> neighbor = cluster[i]->get_array();
		for (int j=0;j<neighbor.size();j++)
		{
			// if user has less than P neighbors take them all, else take only P 
			if (neighbor.size() > P)
				vector_size = P;
			else
				vector_size = neighbor.size();
			// cout <<"prin "<<j<<std::endl;
			std::vector<double> sim = cluster_compute_similarity(vector_size, centroids[i], neighbor, normalized_sentiment, metric);
			// cout <<"meta "<<j<<std::endl;
			for (int y=0;y<positions.size();y++)
			{
				double prediction = 0;
				for (int k=0;k<vector_size;k++)
				{
					norm_sentimentIt2 = normalized_sentiment.find(neighbor[k].get_Point_Id());
					// sim = Cosine_Similarity(norm_sentimentIt1->second, norm_sentimentIt2->second);
					// similarity.push_back(sim);
					// similaritySum += sim;
					prediction += sim[k] * norm_sentimentIt2->second[positions[y]];
				}
				// prediction = prediction * (1 / similaritySum);
				double sum = add_vectorElements(sim);
				if (sum != 0)
					// prediction = prediction * (1 / add_vectorElements(sim));
					prediction = prediction * (1 / sum);
				else
					prediction = 0;
				// update vector with new predicted value
				// norm_sentimentIt1->second[positions[j]] = prediction;
				results.push_back(prediction);

				// cout <<"User "<<centroids[i]<<" Spot "<<positions[y]<<" and new val "<<prediction<<std::endl;
			}
			predicted_values[centroids[i]] = results;
			results.clear();
			sim.clear();
		}
	}
}
