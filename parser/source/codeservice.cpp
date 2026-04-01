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

int find_statement_end(const std::string& code, int i) {
    int n = code.size();

    while (i < n) {
        if (code[i] == ';' || code[i] == '\n')
            return i;

        if (std::isalpha(code[i])) {
            int j = i;
            while (j < n && std::isalnum(code[j])) j++;

            std::string word = code.substr(i, j - i);
            if (word == "else" || word == "case")
                return i - 1;

            i = j;
            continue;
        }

        i++;
    }

    return n - 1;
}

std::string add_braces(std::string in) {
    std::string code = in;
    int n = code.size();

    for (int i = 0; i < n; ) {
        // std::cout << code[i] << '\n';

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

        if (is_identifier_start(code[i])) {

            int start = i;
            while (i < n && is_identifier_char(code[i])) i++;
            std::string word = code.substr(start, i - start);

            if (word == "if" || word == "for" || word == "while" || word == "do"){
                int j = i;
                while (j < n && std::isspace(code[j])) j++;

                if (j < n && code[j] == '(') {
                    int depth = 1;
                    j++;
                    while (j < n && depth > 0) {
                        if (code[j] == '(') depth++;
                        else if (code[j] == ')') depth--;
                        j++;
                    }
                }

                while (j < n && std::isspace(code[j])) j++;

                // если уже есть { — ничего не делаем
                if (j < n && code[j] == '{') {
                    i = j;
                    continue;
                }
                else{
                    i = j;
                    code.insert(i, 1, '{');

                    int end = find_statement_end(code, j);
                    code.insert(end, 1, '}');

                    n = code.size();

                    i++;
                }
            }
            if (word == "else"){
                int j = i;
                while (j < n && std::isspace(code[j])) j++;

                if (j + 1 < n && code.substr(j, 2) == "if"){
                    continue;
                }

                if (j < n && code[j] == '(') {
                    int depth = 1;
                    j++;
                    while (j < n && depth > 0) {
                        if (code[j] == '(') depth++;
                        else if (code[j] == ')') depth--;
                        j++;
                    }
                }

                while (j < n && std::isspace(code[j])) j++;

                // если уже есть { — ничего не делаем
                if (j < n && code[j] == '{') {
                    i = j;
                    continue;
                }
                else{
                    i = j;
                    code.insert(i, 1, '{');

                    int end = find_statement_end(code, j);
                    code.insert(end, 1, '}');

                    n = code.size();

                    i++;
                }
            }
            if (word == "case"){
                int j = i;
                while (j < n && code[j] != '>') j++;

                j++;
                while (j < n && std::isspace(code[j])) j++;


                if (j < n && code[j] == '(') {
                    int depth = 1;
                    j++;
                    while (j < n && depth > 0) {
                        if (code[j] == '(') depth++;
                        else if (code[j] == ')') depth--;
                        j++;
                    }
                }

                while (j < n && std::isspace(code[j])) j++;

                // если уже есть { — ничего не делаем
                if (j < n && code[j] == '{') {
                    i = j;
                    continue;
                }
                else{
                    i = j;
                    code.insert(i, 1, '{');

                    int end = find_statement_end(code, j);
                    code.insert(end, 1, '}');

                    n = code.size();

                    i++;
                }
            }
            i++;
            continue;
        }

        i++;
    }

    return code;
}