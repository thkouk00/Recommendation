#ifndef __VALIDATIONH__
#define __VALIDATIONH__

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
#include "sentiment_analysis.h"
#include "general_funcs.h"

void validation(std::map<std::string, std::vector<double>>& sentiment, std::map<std::string, std::vector<double>>& old_predicted_values, std::map<std::string, std::vector<int>>& old_empty_pos, std::vector<std::string>& users, std::vector<std::vector<std::string>>& coins, int& coin_num, int& P, bool metric, std::ofstream& outputfile);
// void validation(std::map<std::string, std::vector<double>>& sentiment, std::vector<std::string>& users, std::vector<std::vector<std::string>>& coins, int& coin_num, int& P, bool metric);
void take_known_vals(std::map<std::string, std::vector<double>>& new_sentiment, std::vector<std::vector<int>>& positions, std::map<std::string, std::vector<double>>& sentiment);

#endif
