#include "parser.hpp"

#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
    if (argc < 2) {
        exit(1);
    }
    std::string in(argv[1]);
    Parser p;
    Settings s;
    s = p.parse(in);
    std::cout << "\n\n\n";
    std::cout << s.find("Wolol");
    exit(0);
}
