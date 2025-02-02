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
    DOUBLE_QUOTE,
    LEFT_BRACE,
    RIGHT_BRACE,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    IDENTIFIER,
    UNKNOWN,
    END
};

namespace FileConstants {
    const std::string kExtension = ".culang";
}

namespace ScannerConstants {

    constexpr int kTokenCount = (int)TokenType::END + 1;

    const std::map<std::string, TokenType> kKeywords = {
        {"and", TokenType::AND},
        {"or", TokenType::OR},
        
        {"if", TokenType::IF},
        {"else", TokenType::ELSE},
        {"for", TokenType::FOR},
        {"while", TokenType::WHILE},
        {"do", TokenType::DO},
        {"break", TokenType::BREAK,},
        {"continue", TokenType::CONTINUE},
        {"return", TokenType::RETURN},

        {"var", TokenType::VAR},
        {"public", TokenType::PUBLIC},
        {"private", TokenType::PRIVATE},
        {"protected", TokenType::PROTECTED},
        {"fn", TokenType::FN},
        
        {"print", TokenType::PRINT},
        {"input", TokenType::INPUT},
        {"exit", TokenType::EXIT},
        {"clock", TokenType::CLOCK},

        {"true", TokenType::TRUE},
        {"false", TokenType::FALSE},
        {"nil", TokenType::NIL},
    };

    const std::map<TokenType, std::string> kTokenNames = {
        {TokenType::PLUS, "PLUS"},
        {TokenType::MINUS, "MINUS"},
        {TokenType::STAR, "STAR"},
        {TokenType::SLASH, "SLASH"},
        {TokenType::MOD, "MOD"},
        {TokenType::EQUAL, "EQUAL"},
        {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
        {TokenType::BANG, "BANG"},
        {TokenType::GREATER, "GREATER"},
        {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
        {TokenType::LESS, "LESS"},
        {TokenType::LESS_EQUAL, "LESS_EQUAL"},
        {TokenType::AND, "AND"},
        {TokenType::OR, "OR"},
        
        {TokenType::IF, "IF"},
        {TokenType::ELSE, "ELSE"},
        {TokenType::FOR, "FOR"},
        {TokenType::WHILE, "WHILE"},
        {TokenType::DO, "DO"},
        {TokenType::BREAK, "BREAK"},
        {TokenType::CONTINUE, "CONTINUE"},
        {TokenType::RETURN, "RETURN"},

        {TokenType::VAR, "VAR"},
        {TokenType::PUBLIC, "PUBLIC"},
        {TokenType::PRIVATE, "PRIVATE"},
        {TokenType::PROTECTED, "PROTECTED"},
        {TokenType::FN, "FN"},
        
        {TokenType::PRINT, "PRINT"},
        {TokenType::INPUT, "INPUT"},
        {TokenType::EXIT, "EXIT"},
        {TokenType::CLOCK, "CLOCK"},

        {TokenType::NUMBER, "NUMBER"},
        {TokenType::STRING, "STRING"},
        {TokenType::TRUE, "TRUE"},
        {TokenType::FALSE, "FALSE"},
        {TokenType::NIL, "NIL"},

        {TokenType::COMMA, "COMMA"},
        {TokenType::SEMICOLON, "SEMICOLON"},
        {TokenType::LEFT_BRACE, "LEFT_BRACE"},
        {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
        {TokenType::LEFT_PAREN, "LEFT_PAREN"},
        {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
        {TokenType::COLON, "COLON"},
        {TokenType::UNKNOWN, "UNKNOWN"},
        {TokenType::END, "END"}
    };
}