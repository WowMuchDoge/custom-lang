#include "scanner/Token.hpp"

#include <string>
#include <iostream>

#include "Constants.hpp"
#include "error/ScannerError.hpp"

double Token::TokenToNumber() {
    if (std::isnan(m_value)) {
        // Seeing if m_literal can be turned into a number
        try {
            m_value = std::stod(m_literal);
        } catch (const std::exception& e) {
            throw ScannerError(*this, "Cannot convert \"" + m_literal + "\" to a number.");
        }
    }

    return m_value;
}

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
    std::cout << "[TOKEN " << ScannerConstants::kTokenNames.at(m_type) << "]" << std::endl;
}