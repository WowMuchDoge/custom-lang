#include "scanner/Scanner.hpp"

#include "error/ScannerError.hpp"

Token Scanner::ScanToken() {
    skipWhitespace();

    char cur;

    while (!atEnd() && (cur = advance()) != ' ') {
        switch (cur) {
            case '+':
                return makeToken(TokenType::PLUS);
            case '-':
                return makeToken(TokenType::MINUS);
            case '*':
                return makeToken(TokenType::STAR);
            case '/':
                if (peek() == '/') {
                    while (!atEnd() && peek() != '\n') advance();
                    break;
                }
                return makeToken(TokenType::SLASH);
            case '%':
                return makeToken(TokenType::MOD);
            case '=':
                if (peek() == '=') {
                    advance();
                    return makeToken(TokenType::EQUAL_EQUAL);
                }
                return makeToken(TokenType::EQUAL);
            case '!':
                if (peek() == '=') {
                    advance();
                    return makeToken(TokenType::BANG_EQUAL);
                }
                return makeToken(TokenType::BANG);
            case '>':
                if (peek() == '=') {
                    advance();
                    return makeToken(TokenType::GREATER_EQUAL);
                }
                return makeToken(TokenType::GREATER);
            case '<':
                if (peek() == '=') {
                    advance();
                    return makeToken(TokenType::LESS_EQUAL);
                }
                return makeToken(TokenType::LESS);
            case '\0':
                return makeToken(TokenType::END);
            default: {
                //     if (std::isdigit(cur)) {
                //         scanNumber();
                //     } else if (std::isalpha(cur)) {
                //         scanIdentifier();
                //     }
                throw ScannerError(makeToken(TokenType::UNKNOWN), "Unexpected token \"" + std::string(1, cur) + "\".");
            }
        }
    }

    return makeToken(TokenType::END);
}

char Scanner::peek() {
    return m_source[m_currentIndex];
}

char Scanner::advance() {
    if (!atEnd()) {
        m_currentIndex++;
        return m_source[m_currentIndex - 1];
    }

    return '\0';
}

bool Scanner::atEnd() {
    return peek() == '\0';
}

void Scanner::skipWhitespace() {
    while (peek() == ' ' || peek() == '\n') advance();
}

Token Scanner::makeToken(TokenType type) {
    advance();
    return Token(type, m_line, m_offset);
}