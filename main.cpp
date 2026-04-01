#include "parser/include/file.h"
#include "parser/include/operators.h"
#include <iostream>

int main(){
    std::string fileName = "analyze/cur_test.sc";
    std::string code = proceedFile(fileName);

    auto oper = get_operators(code);

    int result = count_operators(oper);

    print_operators(oper);

    std::cout << result << '\n';
}
