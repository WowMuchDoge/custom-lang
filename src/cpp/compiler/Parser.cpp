#include "compiler/Parser.hpp"

#include <cstdarg>

#include "error/CompilerError.hpp"

Token Parser::peek() {
	return m_next;
}

Token Parser::advance() {
	m_prev = m_next;
	m_next = m_scanner.ScanToken();
	return m_prev;
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

std::shared_ptr<Expr> Parser::GetAst() {
	return expression();
}

std::shared_ptr<Expr> Parser::expression() {
	return term();
}

std::shared_ptr<Expr> Parser::term() {
	std::shared_ptr<Expr> left = factor();
	TokenType op;

	while ((op = match(2, TokenType::PLUS, TokenType::MINUS))
			!= TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = factor();

		left = std::shared_ptr<Expr>(new Binary(op, left, right));
	}

	return left;
}

std::shared_ptr<Expr> Parser::factor() {
	std::shared_ptr<Expr> left = primary();
	TokenType op;

	while ((op = match(2, TokenType::STAR, TokenType::SLASH))
			!= TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = primary();

		left = std::shared_ptr<Expr>(new Binary(op, right, left));
	}

	return left;
}

std::shared_ptr<Expr> Parser::primary() {
	if (peek().GetType() == TokenType::LEFT_PAREN) {
		// This is unreachable
		consume(TokenType::LEFT_PAREN, "Expected '(' before group.");

		std::shared_ptr<Expr> expr = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after group.");

		return expr;
	}

	if (peek().GetType() == TokenType::NUMBER) {
		Token number = advance();
	
		// Implicit constructor, constructor to Value is being called
		// with a double
		return std::shared_ptr<Expr>(new Primary(number.GetValue()));
	}

	throw CompilerError("Unknown primary.", m_scanner.GetLine());
}
