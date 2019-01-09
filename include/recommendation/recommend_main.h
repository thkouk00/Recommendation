#ifndef __RECOMMEND_MAINH__
#define __RECOMMEND_MAINH__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <limits>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "sentiment_analysis.h"
#include "general_funcs.h"
#include "validation.h"
// #include "../cluster/Cluster.h"
// #include "../cluster/cluster_main_func.h"

void recommend_main(std::vector<std::vector<double>>&, std::vector<std::string>&, std::map<std::string, std::vector<std::vector<std::string>>>&, std::vector<std::string>&, std::map<std::string, std::vector<std::string>>&, std::map<std::string, double>&, std::vector<std::vector<std::string>>&, int&, std::ofstream&, bool);


#endif