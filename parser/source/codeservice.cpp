#include "../include/codeservice.h"


// найти закрывающую }
int find_matching_brace(const std::string& code, int i) {
    int n = code.size();
    int depth = 1;
    i++;

    while (i < n) {

        if (code[i] == '"' || code[i] == '\'') {
            char q = code[i++];
            while (i < n && code[i] != q) {
                if (code[i] == '\\') i++;
                i++;
            }
        }

        else if (i + 1 < n && code[i] == '/') {
            if (code[i+1] == '/') {
                i += 2;
                while (i < n && code[i] != '\n') i++;
                continue;
            }
            if (code[i+1] == '*') {
                i += 2;
                while (i < n-1 && !(code[i]=='*' && code[i+1]=='/')) i++;
                i += 2;
                continue;
            }
        }

        else if (code[i] == '{') depth++;
        else if (code[i] == '}') {
            depth--;
            if (depth == 0) return i;
        }

        i++;
    }

    return -1;
}

std::string remove_useless_braces(std::string in) {

    std::string code = in;
    bool required = false;
    int n = code.size();

    for (int i = 0; i < n; ) {
        // std::cout << i << '\n';
        
         // --- пропуск строк ---
        if (code[i] == '"' || code[i] == '\'') {
            // std::cout << "String\n";
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
            // std::cout << "Comment\n";
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

            if (word == "if" || word == "else" || word == "case" ||
                word == "for" || word == "while" || word == "do"){

                required = true;
            }
            // if(required){
            //     std::cout << i << ": " << word << '\n';
            // }

            // i++;

            continue;
        }
        

        // --- фигурная скобка ---
        if (code[i] == '{') {
            // std::cout << i << '\n';

            if(required){
                required = false;
                // std::cout << i << '\n';
            }
            else{
                int end = find_matching_brace(code, i);

                code.erase(end, 1);
                code.erase(i, 1);

                n = code.size();
            }
        }
        i++;
    }

    return code;
}