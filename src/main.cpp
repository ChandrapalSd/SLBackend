#include "tokenizer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

int main()
{
    std::ifstream file;
    file.open("input.txt");

    std::stringstream strStream;
    strStream << file.rdbuf(); // read the file
    std::string src = strStream.str();
    file.close();

    // sl::Parser parser(src);
    // sl::Program program = parser.produceAST();
    // std::cout << program;

    sl::Tokens tokens = sl::tokenize(src);
    sl::Program program{ std::span<sl::Token>(tokens) };
    const std::string asm_ = program.getAsm();
    std::cout << asm_;

    std::ofstream myfile;
    myfile.open("output.asm");
    myfile << asm_;
    myfile.close();
    //sl::print(tokens);

    return 0;
}