#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "utils.h"

int main(char* argv, int argc)
{
    std::string set_path = std::string("examples/simple_example1.nano");
    Lexer lexer(set_path);
    lexer.start();

    Nano parser(lexer.get_source());
    parser.run();
    print_errors();
}
