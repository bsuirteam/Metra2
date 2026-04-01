#include "../include/identifiers.h"


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