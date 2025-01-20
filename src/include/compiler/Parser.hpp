#pragma once

#include <memory.h>
#include <memory>

#include "scanner/Scanner.hpp"
#include "compiler/expression/Expr.hpp"

class Parser {
public:
	Parser(std::string source) : m_scanner{source}, m_next{m_scanner.ScanToken()} {}

	std::shared_ptr<Expr> GetAst();
private:
	Token peek();
	Token advance();
	bool atEnd();

	Token consume(TokenType type, std::string msg);

	TokenType match(int count, ...);

	std::shared_ptr<Expr> expression();
	std::shared_ptr<Expr> term();
	std::shared_ptr<Expr> factor();
	std::shared_ptr<Expr> primary();

	Scanner m_scanner;
	Token m_next;
	Token m_prev;
};
