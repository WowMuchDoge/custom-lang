#include "compiler/Parser.hpp"

Token Parser::peek() {
	return m_next;
}

Token Parser::advance() {
	m_prev = m_next;
	return (m_next = m_scanner.ScanToken());
}

bool Parser::atEnd() {
	return m_next.GetType() == TokenType::END;
}


