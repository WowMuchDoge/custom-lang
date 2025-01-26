#pragma once

#include <memory.h>
#include <memory>

#include "scanner/Scanner.hpp"
#include "compiler/ScopeStack.hpp"
#include "compiler/expression/Expr.hpp"
#include "compiler/Stmt.hpp"
#include "error/CompilerError.hpp"

class Parser {
public:
	Parser(std::string source) : m_scanner{source}, m_next{m_scanner.ScanToken()}, m_error{"", 0} {
		// Inserting global scope
		m_scope.NewScope();
	}

	std::vector<StmtPtr> GetAst();
private:
	Token peek();
	Token advance();
	bool atEnd();

	Token consume(TokenType type, std::string msg);
	void consumeSemicolon();

	TokenType match(int count, ...);
	bool match(TokenType type);

	CompilerError* makeCompilerError(std::string message);
	CompilerError* makeCompilerError(std::string message, int line);

	// If an error is encountered, we just go to the end of the expression
	void skipStatement();

	// Statements
	StmtPtr statement();
	StmtPtr variableDeclaration();
	StmtPtr printStatement();
	StmtPtr blockStatement();

	// Expressions
	ExprPtr expression();	
	ExprPtr logical();	
	ExprPtr comparison();
	ExprPtr term();
	ExprPtr factor();
	ExprPtr primary();

	Scanner m_scanner;
	Token m_next;
	Token m_prev;
	
	ScopeStack m_scope;

	// Here for the same reason as scanner error,
	// we must return a pointer to an error which 
	// must persist after `throw` so must be declared
	// somewhere outside of the method where the error
	// is thrown.
	CompilerError m_error;
};
