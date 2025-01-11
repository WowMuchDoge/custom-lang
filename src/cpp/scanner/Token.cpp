#include "scanner/Token.hpp"

#include <string>
#include <iostream>

#include "Constants.hpp"
#include "error/ScannerError.hpp"

int Token::GetLine() {
    return m_line;
}

int Token::GetOffset() {
    return m_lineOffset;
}

std::string Token::GetLiteral() {
    return m_literal;
}

TokenType Token::GetType() {
    return m_type;
}

void Token::Print() {
    if (m_type == TokenType::NUMBER) {
        std::cout << "[TOKEN NUMBER" << ", " << m_value << "]" << std::endl;
        return;
    } else if (m_type == TokenType::STRING) {
        std::cout << "[TOKEN STRING" << ", " << m_literal << "]" << std::endl;
    }
    std::cout << "[TOKEN " << ScannerConstants::kTokenNames.at(m_type) << "]" << std::endl;
}