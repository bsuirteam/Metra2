#include "../include/branches.h"
#include <iostream>
void print_stack(std::stack<std::string> st){
    auto copy = std::stack<std::string>(st);
    int sz = copy.size();
    for (int i = 0; i < sz; i++){
        std::cout << copy.top() << " ";
        copy.pop();
    }
    std::cout << "\n";
}

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

int count_max_nesting_depth(std::string code) {

    code = remove_useless_braces(code);

    int n = code.size();
    int max_depth = 0;
    int match_count = 0;

    std::stack<std::string> st;

    for (int i = 0; i < n; ) {

        // --- пропуск строк ---
        if (code[i] == '"' || code[i] == '\'') {
            char q = code[i++];
            while (i < n && code[i] != q) {
                if (code[i] == '\\') i++;
                i++;
            }
            if (i < n) i++;
            continue;
        }

        // --- комментарии ---
        if (i + 1 < n && code[i] == '/') {
            if (code[i+1] == '/') {
                i += 2;
                while (i < n && code[i] != '\n') i++;
                continue;
            }
            if (code[i+1] == '*') {
                i += 2;
                while (i < n-1 && !(code[i]=='*' && code[i+1]=='/')) i++;
                if (i < n-1) i += 2;
                continue;
            }
        }

        // --- слова ---
        if (is_identifier_start(code[i])) {

            int start = i;
            while (i < n && is_identifier_char(code[i])) i++;
            std::string word = code.substr(start, i - start);

            if (word == "if") {
                st.push("if");
                //max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);
            }

            else if (word == "else") {
                int j = i;
                while (j < n && std::isspace(code[j])) j++;

                if (j + 1 < n && code.substr(j, 2) == "if") {
                    st.push("else_if");
                    i = j + 2;
                    //max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);
                }
            }

            else if (word == "for") {
                st.push("for");
                //max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);
            }

            else if (word == "while") {
                st.push("while");
                // max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);
            }
            else if (word == "do") {
                st.push("do");
                // max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);
            }

            else if (word == "match") {
                st.push("match");
                match_count++;
            }

            else if (word == "case") {
                st.push("case");
                int j = i;

                while (j < n && std::isspace(code[j])) j++;

                if (j < n && code[j] == '_'){
                    st.pop();
                }

                // max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);
            }

            continue;
        }

        // --- закрытие блока ---
        if (code[i] == '}') {
            // print_stack(st);
            max_depth = std::max(max_depth, (int)st.size() - 1 - match_count);

            if (st.size() != 0){
                if ((st.top() == "if") || (st.top() == "else_if")){
                    int j = i + 1;
                    
                    while (j < n && std::isspace(code[j])) j++;

                    if (!(j + 3 < n && code.substr(j, 4) == "else")){
                        std::string top = st.top();

                        while(st.size() > 1 && top == "else_if"){                       
                            st.pop();
                            top = st.top();
                        }

                        if (st.size() != 0 && top == "if"){
                            st.pop();
                        }

                    }
                }
                else if (st.top() == "case"){
                    int j = i + 1;
                    
                    while (j < n && std::isspace(code[j])) j++;

                    if (!(j + 3 < n && code.substr(j, 4) == "case")){

                        std::string top = st.top();

                        while(st.size() > 1 && top == "case"){                       
                            st.pop();
                            top = st.top();
                        }

                        if (st.size() != 0 && st.top() == "match"){
                            st.pop();
                            match_count--;
                        }

                    }
                }
                else if (st.top() == "do"){
                    int j = i + 1;
                    
                    while (j < n && std::isspace(code[j])) j++;

                    if (j + 4 < n && code.substr(j, 5) == "while"){
                        i = j + 4;
                    }
                    st.pop();
                }
                else{
                    st.pop();
                }
            }
        }

        i++;
    }

    return max_depth;
}