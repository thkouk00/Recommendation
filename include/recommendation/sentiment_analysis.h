#ifndef _SENTIMENT_ANALYSIS_H_
#define _SENTIMENT_ANALYSIS_H_

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
#include "../cluster/cluster_main_func.h"


void sentiment_score(std::map<std::string, std::vector<std::vector<std::string>>>&, std::map<std::string, std::vector<double>>&, std::map<std::string, double>&, std::vector<std::vector<std::string>>&, double&);
void sentiment_normalization(std::map<std::string, std::vector<double>>&, std::map<std::string, std::vector<double>>&, std::map<std::string, std::vector<int>>&);
void cluster_sentiment_score(Cluster**, std::vector<std::string>&, std::map<std::string, std::vector<std::string>>&, std::map<std::string, std::vector<std::vector<std::string>>>&, std::map<std::string, std::vector<double>>&, std::map<std::string, double>&, std::vector<std::vector<std::string> >&, double&);

#endif