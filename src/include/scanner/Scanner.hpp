#pragma once

#include <string>

#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"

class Scanner {
public:
    Scanner(std::string source) : m_source{source}, m_line{1}, m_offset{0}, m_currentIndex{0} {}

    Token ScanToken();

private:
    std::string m_source;

    int m_line;
    int m_offset; // Difference between the current character and the beginning ofthe line
    int m_currentIndex;

    char peek();
    char advance();

    bool atEnd();

    // Token scanNumber();
    // Token scanIdentifier();

    void skipWhitespace();

    Token makeToken(TokenType type);
};