#pragma once

#include <string>
#include <map>
#include <cmath>
#include <iostream>

#include "Constants.hpp"

#define TOKEN_COUNT 39

class Token {
public:
    Token(TokenType type, int line, int lineOffset, std::string literal = "") : m_type{type}, m_line{line}, m_lineOffset{line}, m_literal{literal}, m_value{NAN} {
        // End is the only non-literal token that does not contain specific text
        if (literal == "" && type != TokenType::END) {
            auto it = ScannerConstants::kTokenStrings.find(type);
            m_literal = it->second;
        }
    }

    double TokenToNumber();

    int GetLine();
    int GetOffset();
    std::string GetLiteral();
    TokenType GetType();

    void Print();

private:
    TokenType m_type;
    double m_value;
    std::string m_literal;

    int m_line;
    int m_lineOffset;
};