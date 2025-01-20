#pragma once

#include "scanner/Scanner.hpp"
#include "compiler/expression/Expr.hpp"

class Parser {
public:
	Parser(std::string source) : m_scanner{source}, m_prev{m_scanner.ScanToken()}, m_next{m_scanner.ScanToken()} {}

	Expr GetAst();
private:
	Token peek();
	Token advance();
	bool atEnd();

	Scanner m_scanner;
	Token m_next;
	Token m_prev;
};
