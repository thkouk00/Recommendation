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

void make_dataset(std::vector<std::vector<double>>&, std::map<std::string, std::vector<double>>&, std::vector<std::string>&);
void find_neighbors(std::vector<std::vector<std::pair <double, std::string>>>&, std::vector<std::vector<double>>&, std::vector<std::string>&, std::map<std::string, std::vector<double>>&);

#endif