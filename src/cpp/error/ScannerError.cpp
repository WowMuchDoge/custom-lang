#include "error/ScannerError.hpp"

#include <iostream>

void ScannerError::Print() {
    std::cout << "[Line " << m_token.GetLine() << "] Error: " << m_message << std::endl;
}