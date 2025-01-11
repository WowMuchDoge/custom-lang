#pragma once

#include <map>
#include <string>

enum class TokenType {
    // Operators
    PLUS = 0,
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
    FN,

    // Specific functions
    PRINT,
    INPUT,
    EXIT,
    CLOCK,

    // Literals
    NUMBER,
    STRING,
    TRUE,
    FALSE,
    NIL,

    // Misc
    COMMA,
    SEMICOLON,
    END
};

namespace ScannerConstants {

    const int kTokenCount = 40;

    // Not including NUMBER, STRING, and END tokens since string content is different than token name
    const std::map<TokenType, std::string> kTokenNames = {
        {TokenType::PLUS, "+"},
        {TokenType::MINUS, "-"},
        {TokenType::STAR, "*"},
        {TokenType::SLASH, "/"},
        {TokenType::MOD, "%"},
        {TokenType::EQUAL, "="},
        {TokenType::EQUAL_EQUAL, "=="},
        {TokenType::BANG, "!"},
        {TokenType::GREATER, ">"},
        {TokenType::GREATER_EQUAL, ">="},
        {TokenType::LESS, "<"},
        {TokenType::LESS_EQUAL, "<="},
        {TokenType::AND, "and"},
        {TokenType::OR, "or"},
        
        {TokenType::IF, "if"},
        {TokenType::ELSE, "else"},
        {TokenType::FOR, "for"},
        {TokenType::WHILE, "while"},
        {TokenType::DO, "do"},
        {TokenType::BREAK, "break"},
        {TokenType::CONTINUE, "continue"},
        {TokenType::RETURN, "return"},

        {TokenType::VAR, "var"},
        {TokenType::PUBLIC, "public"},
        {TokenType::PRIVATE, "private"},
        {TokenType::PROTECTED, "protected"},
        {TokenType::FN, "fn"},
        
        {TokenType::PRINT, "print"},
        {TokenType::INPUT, "input"},
        {TokenType::EXIT, "exit"},
        {TokenType::CLOCK, "clock"},

        {TokenType::TRUE, "true"},
        {TokenType::FALSE, "false"},
        {TokenType::NIL, "nil"},

        {TokenType::COMMA, ","},
        {TokenType::SEMICOLON, ";"}
    };
}