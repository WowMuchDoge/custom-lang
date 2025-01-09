#pragma once

#include <string>
#include <cmath>

enum class TokenType {
    // Operators
    PLUS,
    MINUS,
    STAR,
    SLASH,
    MOD,
    EQUAL,
    EQUAL_EQUAL,
    BANG,
    BANG_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESS,
    LESS_EQUAL,
    AND,
    OR,

    // Control flow
    IF,
    ELSE,
    FOR,
    WHILE,
    DO,
    BREAK,
    CONTINUE,
    RETURN,

    // Variables and functions
    VAR,
    PUBLIC,
    PRIVATE,
    PROTECTED,
    FUNC,

    // Specific functions
    PRINT,
    READLINE,
    EXIT,
    CLOCK,

    // Literals
    NUMBER,
    STRING,
    TRUE,
    FALSE,
    NIL,

    // Misc
    END
};

class Token {
public:
    Token(TokenType type, std::string literal = "") : m_type{type}, m_literal{literal}, m_value{NAN} {}

    double TokenToNumber();

private:
    TokenType m_type;
    double m_value;
    std::string m_literal;
};