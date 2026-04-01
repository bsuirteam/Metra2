#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <unordered_set>
#include <string>
#include <cctype>
#include <set>
#include <iostream>

bool is_identifier_char(char c);

bool is_identifier_start(char c);

bool is_word_boundary(const std::string &s, int pos, int len);