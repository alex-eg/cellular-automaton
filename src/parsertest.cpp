#include "parser.hpp"
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2) exit(1);
    std::string in(argv[1]);
    Parser P;
    Settings S;
    S = P.Parse(in);
    std::cout << "\n\n\n";
    std::cout << S.Find("Wolol");
    exit(0);
}
