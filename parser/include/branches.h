#pragma once
#include "identifiers.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <string>
#include <cctype>
#include <set>
#include <iostream>
#include <stack>

int count_branches(std::unordered_map<std::string, int> operators);
void print_branches(std::unordered_map<std::string, int> operators);
int count_max_nesting_depth(std::string code);