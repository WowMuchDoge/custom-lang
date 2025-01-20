#include "compiler/Parser.hpp"

#include <cstdarg>

#include "error/CompilerError.hpp"

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

Token Parser::consume(TokenType type, std::string msg) {
	if (peek().GetType() != type)
		throw CompilerError(msg, m_scanner.GetLine());
	
	return advance();
}

TokenType Parser::match(int count, ...) {
	va_list args;
	va_start(args, count);

	for (int i = 0; i < count; i++) {
		TokenType type = va_arg(args, TokenType);
		
		if (peek().GetType() == type) {
			return advance().GetType();
		}
	}

	return TokenType::UNKNOWN;
}

Expr* Parser::GetAst() {
	return expression();
}

Expr* Parser::expression() {
	return term();
}

Expr* Parser::term() {
	Expr* left = factor();
	TokenType op;

	while ((op = match(2, TokenType::PLUS, TokenType::MINUS))
			!= TokenType::UNKNOWN) {
		
		Expr* right = factor();

		left = new Binary(op, right, left);
	}

	return left;
}

Expr* Parser::factor() {
	Expr* left = primary();
	TokenType op;

	while ((op = match(2, TokenType::STAR, TokenType::SLASH))
			!= TokenType::UNKNOWN) {
		
		Expr* right = primary();

		left = new Binary(op, right, left);
	}

	return left;
}

Expr* Parser::primary() {
	if (peek().GetType() == TokenType::LEFT_PAREN) {
		// This is unreachable
		consume(TokenType::LEFT_PAREN, "Expected '(' before group.");

		Expr* expr = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after group.");

		return expr;
	}

	if (peek().GetType() == TokenType::NUMBER) {
		Token number = advance();
	
		// Implicit constructor, constructor to Value is being called
		// with a double
		return new Primary(number.GetValue());
	}

	throw CompilerError("Unknown primary.", m_scanner.GetLine());
}
