#ifndef XAX_H
#define XAX_H
#include "config.h"
#include "package.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fstream>
#include <queue>
std::string GetStdoutFromCommand(std::string cmd);
void get_packages(std::vector<Package> &p_vect);
int* str_to_int_arr(std::string str);
std::queue<std::string>* split_string(std::string start);
std::vector<Package>* query_packages(std::queue<std::string>* query, std::vector<Package>* queried_from, bool first_run);
void select_install(std::vector<Package>* matched,  std::stringstream &flags, Config conf);

#endif
