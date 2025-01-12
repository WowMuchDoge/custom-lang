#include "scanner/Scanner.hpp"

#include "error/ScannerError.hpp"

Token Scanner::ScanToken() {
    char cur;

    while (!atEnd()) {
        cur = advance();
        switch (cur) {
            case '+':
                return Token(TokenType::PLUS, "+");
            case '-':
                return Token(TokenType::MINUS, "-");
            case '*':
                return Token(TokenType::STAR, "*");
            case '/':
                if (peek() == '/') {
                    while (!atEnd() && peek() != '\n') advance();
                    break;
                }
                return Token(TokenType::SLASH, "/");
            case '%':
                return Token(TokenType::MOD, "%");
            case '=':
                if (peek() == '=') {
                    advance();
                    return Token(TokenType::EQUAL_EQUAL, "==");
                }
                    return Token(TokenType::EQUAL, "=");
            case '!':
                if (peek() == '=') {
                    advance();
                    return Token(TokenType::BANG_EQUAL, "!=");
                }
                return Token(TokenType::BANG, "!");
            case '>':
                if (peek() == '=') {
                    advance();
                    return Token(TokenType::GREATER_EQUAL, ">=");
                }
                return Token(TokenType::GREATER, ">");
            case '<':
                if (peek() == '=') {
                    advance();
                    return Token(TokenType::LESS_EQUAL, "<=");
                }
                return Token(TokenType::LESS, "<");
            case ';':
                return Token(TokenType::SEMICOLON, ";");
            case ',':
                return Token(TokenType::COMMA, ",");
            case '"':
                return scanString();
            case '{':
                return Token(TokenType::LEFT_BRACE, "{");
            case '}':
                return Token(TokenType::RIGHT_BRACE, "}");
            case '(':
                return Token(TokenType::LEFT_PAREN, "(");
            case ')':
                return Token(TokenType::RIGHT_PAREN, ")");
            case ':':
                return Token(TokenType::COLON, ":");
            case '\n':
                m_line++;
                m_offset = 0;
                break;
            case ' ':
            case '\t':
            case '\v':
            case '\r':
                continue;
            case '\0':
                return Token(TokenType::END);
            default: {
                    if (isDigit(cur)) {
                        return scanNumber();
                    } else if (isAlpha(cur)) {
                        return scanIdentifier();
                    }
                m_error = ScannerError(m_line, "Unexpected token '" + std::string(1, cur) + "'.");
                throw &m_error;
            }
        }
    }

    return Token(TokenType::END);
}

Token Scanner::scanNumber() {
    // m_currentIndex is always the next token to be consumed and 
    // number that is encountered has already been consumed, so
    // it must be subtracted by one
    int startIndex = m_currentIndex - 1;

    while (isDigit(peek())) advance();

    if (peek() == '.') {
        advance();
        while (isDigit(peek())) advance();
    }

    int endIndex = m_currentIndex;

    return Token(TokenType::NUMBER, "", std::stod(m_source.substr(startIndex, endIndex - startIndex)));
}

Token Scanner::scanIdentifier() {
    int start = m_currentIndex - 1;

    while (isAlphaNumeric(peek())) advance();

    std::string identifier = m_source.substr(start, m_currentIndex - start);

    auto it = ScannerConstants::kKeywords.find(identifier);

    if (it != ScannerConstants::kKeywords.end()) {
        return Token(it->second, identifier);
    }

    return Token(TokenType::IDENTIFIER, identifier);
}

Token Scanner::scanString() {
    std::string str = "";

    while (!atEnd() && peek() != '"') {
        if (atEnd() || peek() == '\n') {
            m_error = ScannerError(m_line, "Unterminated string");
            throw &m_error;
        }
        str += advance();
    }

    consume('"', "Expected '\"' token.");

    return Token(TokenType::STRING, str);
}

char Scanner::peek() {
    return m_source[m_currentIndex];
}

void Scanner::consume(char expected, std::string message) {
    if (peek() != expected) {
        m_error = ScannerError(m_line, message);
        throw &m_error;
    }

    advance();
}

char Scanner::advance() {
    if (!atEnd()) {
        return m_source[m_currentIndex++];
    }

    return '\0';
}

bool Scanner::atEnd() {
    return peek() == '\0';
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') ||
           (c >= 'a' && c <= 'z') ||
           c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}