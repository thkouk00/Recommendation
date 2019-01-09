#ifndef __GENERAL_FUNCSH__
#define __GENERAL_FUNCSH__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "math.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>
#include "../LSH/general_functions.h"
#include "../LSH/main_func.h"
#include "../LSH/Distance.h"
// #include "../cluster/cluster_main_func.h"

void make_dataset(std::vector<std::vector<double>>&, std::map<std::string, std::vector<double>>&, std::vector<std::string>&);
void find_neighbors(std::vector<std::vector<std::pair <double, std::string>>>&, std::vector<std::vector<double>>&, std::vector<std::string>&);
void predict_coins(std::map<std::string, std::vector<double>>&, std::vector<std::vector<std::pair <double, std::string>>>&, std::map<std::string, std::vector<int>>&, std::vector<std::string>&, std::map<std::string, std::vector<double>>&, int&, bool);
void predict_singleuser_coins(std::map<std::string, std::vector<double>>& predicted_values, std::vector<std::vector<std::pair <double, std::string>>>& neighbors, std::map<std::string, std::vector<int>>& empty_pos, std::string user, std::map<std::string, std::vector<double>>& normalized_sentiment, int& P, bool metric);
void cluster_predict_coins(std::map<std::string, std::vector<double>>&, Cluster**, std::map<std::string, std::vector<int>>&, std::vector<std::string>&, std::map<std::string, std::vector<double>>&, int&, bool);
void sort_vector(std::map<std::string, std::vector<double>>&, std::map<std::string, std::vector<int>>&, std::vector<std::vector<std::string>>&, int&, std::ofstream&);

#endif