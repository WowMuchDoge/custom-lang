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

bool Parser::match(TokenType type) {
	if (peek().GetType() == type) {
		advance();
		return true;
	}

	return false;
}

std::shared_ptr<Expr> Parser::GetAst() {
	return expression();
}

std::shared_ptr<Expr> Parser::expression() {
	return logical();
}

std::shared_ptr<Expr> Parser::logical() {
	std::shared_ptr<Expr> left = comparison();
	TokenType op;

	while ((op = match(2, TokenType::AND, TokenType::OR))
			!= TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = comparison();

		left = std::shared_ptr<Expr>(new Binary(op, left, right));
	}

	return left;
}

std::shared_ptr<Expr> Parser::comparison() {
	std::shared_ptr<Expr> left = term();
	TokenType op;

	while ((op = match(4, TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL, 
			TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL)) != TokenType::UNKNOWN) {
		
		std::shared_ptr<Expr> right = term();

		left = std::shared_ptr<Expr>(new Binary(op, left, right));
	}

	return left;
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
	if (match(TokenType::TRUE)) return std::shared_ptr<Expr>(new Primary(true));
	if (match(TokenType::FALSE)) return std::shared_ptr<Expr>(new Primary(false));

	// Value constructor with no args constructs a nil value
	if (match(TokenType::NIL)) return std::shared_ptr<Expr>(new Primary(Value()));

	if (match(TokenType::LEFT_PAREN)) {
		std::shared_ptr<Expr> expr = expression();

		consume(TokenType::RIGHT_PAREN, "Expected ')' after group.");

		return std::shared_ptr<Expr>(new Grouping(expr));
	}

	if (peek().GetType() == TokenType::NUMBER) {
		Token number = advance();
	
		// Implicit constructor, constructor to Value is being called
		// with a double
		return std::shared_ptr<Expr>(new Primary(number.GetValue()));
	}

	if (peek().GetType() == TokenType::STRING) {
		Token str = advance();

		return std::shared_ptr<Expr>(new Primary(str.GetLiteral()));
	}

	throw CompilerError("Unknown primary.", m_scanner.GetLine());
}
