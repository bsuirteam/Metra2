#include "../include/branches.h"

std::set<std::string> branch_operators = {
    "if", "while", "case", "for"
};

int count_branches(std::unordered_map<std::string, int> operators){
    int result = 0;

    for (auto [op, count] : operators){
        if (branch_operators.count(op)){
            result += count;
        }
    }

    return result;
}

void print_branches(std::unordered_map<std::string, int> operators){
    for (auto [op, count] : operators){
        if (branch_operators.count(op) && count != 0){
            std::cout << op << ": " << count << '\n';
        }
    }
}