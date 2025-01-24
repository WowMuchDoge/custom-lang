#pragma once

#include <memory.h>
#include <memory>

#include "scanner/Scanner.hpp"
#include "compiler/ScopeStack.hpp"
#include "compiler/expression/Expr.hpp"
#include "compiler/Stmt.hpp"

class Parser {
public:
	Parser(std::string source) : m_scanner{source}, m_next{m_scanner.ScanToken()}, m_scope{} {}

	std::shared_ptr<Stmt> GetAst();
private:
	Token peek();
	Token advance();
	bool atEnd();

	Token consume(TokenType type, std::string msg);

	TokenType match(int count, ...);
	bool match(TokenType type);

	// Statements
	std::shared_ptr<Stmt> variableDeclaration(); 

	// Expressions
	std::shared_ptr<Expr> expression();	
	std::shared_ptr<Expr> logical();	
	std::shared_ptr<Expr> comparison();
	std::shared_ptr<Expr> term();
	std::shared_ptr<Expr> factor();
	std::shared_ptr<Expr> primary();

	Scanner m_scanner;
	Token m_next;
	Token m_prev;
	
	ScopeStack m_scope;
};
