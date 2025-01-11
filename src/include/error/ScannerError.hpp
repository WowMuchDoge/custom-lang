#pragma once

#include <string>

#include "scanner/Token.hpp"

class ScannerError {
public:
    ScannerError(Token& token, std::string message) : m_token{token}, m_message{message} {}

    void Print();

private:
    std::string m_message;
    Token& m_token;
};