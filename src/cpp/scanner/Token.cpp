#include "scanner/Token.hpp"

#include <string>
#include <iostream>

#include "Constants.hpp"
#include "error/ScannerError.hpp"

std::string Token::GetLiteral() const {
    return m_literal;
}

TokenType Token::GetType() const {
    return m_type;
}

double Token::GetValue() const {
    return m_value;
}

void Token::Print() {
    if (m_type == TokenType::NUMBER) {
        std::cout << "[TOKEN NUMBER" << ", " << m_value << "]" << std::endl;
        return;
    } else if (m_type == TokenType::STRING) {
        std::cout << "[TOKEN STRING" << ", " << m_literal << "]" << std::endl;
        return;
    } else if (m_type == TokenType::IDENTIFIER) {
        std::cout << "[TOKEN IDENTIFIER" << ", " << m_literal << "]" << std::endl;
        return;
    }
    std::cout << "[TOKEN " << ScannerConstants::kTokenNames.at(m_type) << "]" << std::endl;
}