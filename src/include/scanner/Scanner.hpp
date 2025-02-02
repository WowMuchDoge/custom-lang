#pragma once

#include <string>

#include "scanner/Token.hpp"
#include "error/ScannerError.hpp"

class Scanner {
public:
    Scanner(std::string source) : m_source{source}, m_line{1}, m_offset{0}, m_currentIndex{0}, m_error{-1, ""} {}

    Token ScanToken();
	int GetLine();

private:
    std::string m_source;

    int m_line;
    int m_offset; // Difference between the current character and the beginning of the line
    int m_currentIndex;

    // Caller interacts with an Error pointer so a pointer must be
    // thrown instead of a copy or reference. If the error is kept in
    // the scope of the function that is throwing it, the pointer
    // will point to memory that is doing something else since
    // the function has been popped from the call stack. Because of
    // this, the error that is thrown must have a memory address that
    // is still valid after the function is exited.
    ScannerError m_error;

    char peek();
    char advance();
    void consume(char expected, std::string message);

    bool atEnd();

    Token scanNumber();
    Token scanIdentifier();
    Token scanString();

    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);
};
