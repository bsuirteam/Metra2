#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <string>
#include <cctype>
#include <set>
#include <iostream>

std::unordered_map<std::string, int> get_operators(std::string text_program);
int count_operators(std::unordered_map<std::string, int> operators);
void print_operators(std::unordered_map<std::string, int> operators);
