#include <iostream>

#include "Token.hpp"

int main() {
    Token t(TokenType::AND, "and");

    try {
        double hehe = t.TokenToNumber();
    } catch (const std::exception& e) {
        std::cout << "Uh oh " << e.what() << std::endl;
    }
}
