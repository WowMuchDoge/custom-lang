#include <iostream>

#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"
#include "scanner/Scanner.hpp"

int main() {
    Scanner scanner("1234.34567 + 345 == 2342");

    Token t(TokenType::UNKNOWN, 1, 1);

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