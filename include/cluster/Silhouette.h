#ifndef __SILHOUETTEH__
#define __SILHOUETTEH__

#include <iostream>
#include <vector>
#include "Cluster.h"
#include <fstream>
#include <sstream>
#include "../LSH/Distance.h"

// double Silhouette(std::vector<std::vector<double>>&, Cluster**, int&, bool&, std::ofstream&);
double Silhouette(std::vector<std::vector<double>>& Cluster_Table, Cluster** cluster, int& number_of_clusters, bool& k_means_flag);
double calc_distance(std::vector<double>&, Cluster**, int);
double avg_dist_sameCluster(Info&, std::vector<Info>&, bool&);
double avg_dist_neighbor(Info&, Cluster**, bool&);

#endif