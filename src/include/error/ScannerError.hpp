#pragma once

#include <string>

#include "scanner/Token.hpp"

class ScannerError {
public:
    ScannerError(int line, std::string message) : m_line{line}, m_message{message} {}

    void Print();

private:
    std::string m_message;
    int m_line;
};