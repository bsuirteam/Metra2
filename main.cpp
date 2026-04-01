#include "parser/include/file.h"
#include "parser/include/operators.h"
#include "parser/include/branches.h"
#include "parser/include/codeservice.h"
#include <iostream>

int main(){
    std::string fileName = "analyze/cur_test.sc";
    std::string code = proceedFile(fileName);
    

    auto oper = get_operators(code);

    int result = count_operators(oper);

    print_operators(oper);

    std::cout << result << "\n\n";

    print_branches(oper);

    std::cout << count_branches(oper) << "\n\n";

    std::cout << "Max nest: " << count_max_nesting_depth(code);

    // std::cout << remove_useless_braces(code);
}
