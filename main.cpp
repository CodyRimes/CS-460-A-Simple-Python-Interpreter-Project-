#include <iostream>
#include<vector>
/*
 * sum = 0
for( i = 0; i < 5; i = i + 1 ) {
sum = sum + i
for( j = 0; j < i; j = j + 1 ) {
sum = sum + j
}
}
print sum
fib0 = 1
fib1 = 1
for( i = 1; i < 20; i = i + 1 ) {
print fib0
fib0 = fib0 + fib1
fib1 = fib0 - fib1
}
print fib0
print fib1
size = 20 * 10 - 81
print size
width = size * 5 - (3 + 2 * 5) + 2
print width
a = 10 * 5 / 15
print a
yesNo = (100 > 20) == (30 >= 10)
print yesNo
yesNo = 100 > 20 > 40 - 23 + 3 * 3 - 2 == 30 + 5 >= 9 != 10 > 6
print yesNo

 *
 *
 */

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"
#include "Statements.hpp"
#include "SymTab.hpp"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }

    std::ifstream inputStream;

    inputStream.open(argv[1], std::ios::in);
    if (!inputStream.is_open()) {
        std::cout << "Unable top open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
        exit(2);
    }

    Tokenizer tokenizer(inputStream);
    Parser parser(tokenizer);
    Statements *statements = parser.statements();
    Token lastToken = tokenizer.getToken();

    if (!lastToken.eof()) {
        std::cout << "Unexpected token in input." << std::endl;
        lastToken.print();
        exit(1);
    }



    SymTab symTab;

    statements->print();
    statements->evaluate(symTab);
    std::cout << std::endl << "Symbol table contains the following variables.\n";
    symTab.print();

    return 0;
}
