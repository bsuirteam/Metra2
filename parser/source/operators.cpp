#include"../include/operators.h"

bool is_identifier_char(char c) {
    return std::isalnum(c) || c == '_';
}

bool is_identifier_start(char c) {
    return std::isalpha(c) || c == '_';
}

// Проверка границ слова
bool is_word_boundary(const std::string& s, int pos, int len) {
    if (pos > 0 && is_identifier_char(s[pos - 1])) return false;
    if (pos + len < (int)s.size() && is_identifier_char(s[pos + len])) return false;
    return true;
}

std::unordered_map<std::string, int> get_operators(std::string code){

    std::unordered_map<std::string, int> out  = {

        {"function_call", 0},
        {"f-string", 0},

        {"[", 0},

        // Арифметические
        {"+", 0}, {"-", 0}, {"*", 0}, {"/", 0}, {"%", 0},

        // Унарные
        {"!", 0}, {"~", 0},

        // Сравнение
        {"==", 0}, {"!=", 0}, {"<", 0}, {"<=", 0},
        {">", 0}, {">=", 0},

        // Логические
        {"&&", 0}, {"||", 0},

        // Побитовые
        {"&", 0}, {"|", 0}, {"^", 0}, {"<<", 0}, {">>", 0}, {">>>", 0},

        // Присваивание
        {"=", 0},
        {"+=", 0}, {"-=", 0}, {"*=", 0}, {"/=", 0}, {"%=", 0},
        {"&=", 0}, {"|=", 0}, {"^=", 0},
        {"<<=", 0}, {">>=", 0}, {">>>=", 0},

        // Диапазоны
        {"to", 0}, {"until", 0},

        // Коллекции
        {"::", 0}, {":::", 0},
        {"++", 0}, {"++=", 0},
        {"--", 0}, {"--=", 0},
        {":+", 0}, {"+:", 0},

        // Стрелочные
        {"=>", 0}, {"->", 0}, {"<-", 0},

        // Типовые операторы
        {":", 0},
        {"<:", 0}, {">:", 0},
        {"<:<", 0}, {"=:= ", 0},

        // Сопоставление
        {"match", 0}, 
        
        // Это для подсчета ветвлений
        {"case", 0},

        // Проверка типов
        {"isInstanceOf", 0},
        {"asInstanceOf", 0},

        // Импорт и область
        {".", 0},

        ////Разделители
        // {",", 0}, {";", 0},

        // Управление
        {"if", 0},
        {"for", 0}, {"while", 0},
        {"yield", 0},
        {"try", 0}, {"catch", 0}, {"finally", 0},
        {"throw", 0},
        {"return", 0},

        // Определения
        {"val", 0}, {"var", 0}, {"def", 0},
        {"class", 0}, {"object", 0}, {"trait", 0},

        // Классы
        {"extends", 0}, {"with", 0},
        //{"new", 0}
    };

    std::set<std::string> defines = {
        "def", "class", "object", "trait"
    };


    std::unordered_set<std::string> not_functions = {
        "if", "for", "while", "catch", "match"
    };

    // Отсортированный список операторов по длине (важно!)
    std::vector<std::string> ops = {
        ">>>=", "<<=", ">>=", ">>>", ":::", "<:<", "++=", "--=",
        "==", "!=", "<=", ">=", "&&", "||",
        "<<", ">>", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=",
        "++", "--", "::", ":+", "+:", "=>", "->", "<-", "<:", ">:",
        "+", "-", "*", "/", "%", "!", "~", "<", ">", "&", "|", "^", "=", ".", ",", ";", ":", "["
    };

    int n = code.size();

    for (int i = 0; i < n; ) {

        // Comments
        if((i + 1 < n) && code[i] == '/'){
            if (code[i+1] == '/'){
                i += 2;
                while(i < n && code[i] != '\n'){
                    i++;
                }
                i++;
                continue;
            }
            else if(code[i+1] == '*'){
                i += 2;
                while(i + 1 < n && (code[i] != '*' || code[i + 1] != '/')){
                    i++;
                }
                i+=2;
                continue;
            }
        }

        // --- Пропуск строк ---
        if (code[i] == '\"' || code[i] == '\'') {
            if (i + 2 < n && code[i+1] == '"' && code[i+2] == '"') {
                // """ multiline """
                i += 3;
                while (i + 2 < n && !(code[i] == '"' && code[i+1] == '"' && code[i+2] == '"')) {
                    i++;
                }
                if (i + 2 < n) i += 3;
            } else {
                char quote = code[i++];
                while (i < n && code[i] != quote) {
                    if (code[i] == '\\') i++;
                    i++;
                    if(i >= n) {
                        break;
                    }
                }
                if (i < n) i++;
            }
            continue;
        }

        // --- f-string ---
        if (i + 1 < n && (code[i] == 'f' || code[i] == 's') && code[i+1] == '"' && (i == 0 || !is_identifier_char(code[i - 1]))) {
            out["f-string"]++;
            i += 2;
            while (i < n && code[i] != '"') {
                if (code[i] == '\\') i++;
                i++;
            }
            i++;
            continue;
        }

        // --- identifier ---
        if (is_identifier_start(code[i])) {
            int start = i;
            while (i < n && is_identifier_char(code[i])) i++;

            std::string word = code.substr(start, i - start);

            if (word == "case"){
                while(i < n && std::isspace(code[i])){
                    i++;
                }

                if(i < n && code[i] == '_'){
                    continue;
                }
            }

            // ключевые слова
            if (out.count(word) && is_word_boundary(code, start, word.size())) {
                out[word]++;
            }

            // function_call
            if (i < n && code[i] == '(' && !not_functions.count(word)) {
                out["function_call"]++;
            }

            // sckip defines
            if (word == "def" || word == "class" || word == "object" || word == "trait") {
                
                while (i < n && code[i] != '{') {

                    // пропуск строк
                    if (code[i] == '"' || code[i] == '\'') {
                        char quote = code[i++];
                        while (i < n && code[i] != quote) {
                            if (code[i] == '\\') i++;
                            i++;
                        }
                        if (i < n) i++;
                        continue;
                    }

                    // пропуск комментариев
                    if (i + 1 < n && code[i] == '/') {
                        if (code[i + 1] == '/') {
                            i += 2;
                            while (i < n && code[i] != '\n') i++;
                            continue;
                        }
                        if (code[i + 1] == '*') {
                            i += 2;
                            while (i < n - 1 && !(code[i] == '*' && code[i + 1] == '/')) i++;
                            if (i < n - 1) i += 2;
                            continue;
                        }
                    }

                    i++;
                }

                continue;
            }

            continue;
        }

        // --- операторы ---
        bool matched = false;
        for (const auto& op : ops) {
            if (i + op.size() <= n && code.substr(i, op.size()) == op) {
                out[op]++;
                i += op.size();
                matched = true;
                break;
            }
        }

        if (!matched) i++;
    }

    return out;

}

void print_operators(std::unordered_map<std::string, int> operators){
     for (auto [op, count] : operators){
        if (count != 0){
            std::cout << op << ": " << count << '\n';
        }
    }
}

int count_operators(std::unordered_map<std::string, int> operators){
    int result = 0;

    for (auto [op, count] : operators){
        if (op != "case"){
            result += count;
        }
    }

    return result;
}