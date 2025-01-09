#include "Token.hpp"

#include <string>

double Token::TokenToNumber() {
    if (std::isnan(m_value)) {
        try {
            m_value = std::stod(m_literal);
        } catch (const std::exception& e) {
            throw e;
        }
    }

    return m_value;
}