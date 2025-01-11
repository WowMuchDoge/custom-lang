#include <iostream>

#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"
#include "scanner/Scanner.hpp"

int main() {
    Scanner scanner("+ _ * //% <= *");

    Token t(TokenType::UNKNOWN, 1, 1);

    while (t.GetType() != TokenType::END) {
        try {
            t = scanner.ScanToken();
        } catch (ScannerError e) {
            e.Print();
            break;
        }
    }
}