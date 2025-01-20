#pragma once

#include <string>
#include <map>
#include <cmath>
#include <iostream>

#include "Constants.hpp"

#define TOKEN_COUNT 39

class Token {
public:
    Token(TokenType type = TokenType::UNKNOWN, std::string literal = "", double number = NAN) 
    : m_type{type}, m_literal{literal}, m_value{number} {}

    TokenType GetType() const;
    double GetValue() const;
    std::string GetLiteral() const;

    void Print();

private:
    TokenType m_type;
    double m_value;
    std::string m_literal;
};
