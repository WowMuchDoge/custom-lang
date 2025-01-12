#include <iostream>

#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"
#include "scanner/Scanner.hpp"

int main() {
    Scanner scanner("var _a =123;");

    Token t(TokenType::UNKNOWN);

    while (t.GetType() != TokenType::END) {
        try {
            t = scanner.ScanToken();
            t.Print();
        } catch (ScannerError e) {
            e.Print();
            break;
        }
    }
}