#include "error/ScannerError.hpp"

#include <iostream>

void ScannerError::Print() {
    std::cout << "[Line " << m_line << "] Error: " << m_message << std::endl;
}