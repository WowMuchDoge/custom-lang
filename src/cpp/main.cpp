#include <iostream>

#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"

int main() {
    Token t(TokenType::AND, 1, 0);

    try {
        double hehe = t.TokenToNumber();
    } catch (ScannerError e) {
        e.Print();
    }
}
