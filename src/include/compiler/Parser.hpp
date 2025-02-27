#pragma once

#include "scanner/Scanner.hpp"
#include "compiler/ScopeStack.hpp"
#include "compiler/expression/Expr.hpp"
#include "compiler/Stmt.hpp"
#include "error/CompilerError.hpp"

class Parser {
public:
	Parser(std::string source) : m_scanner{source}, m_error{"", 0}, m_inCall{false}, m_encounteredError{false} {
		// Inserting global scope
		m_scope.NewScope();

		// Making sure tokens are properly handled
		m_next = m_scanner.ScanToken();
		m_twoNext = m_scanner.ScanToken();
	}

	StmtPtr GetAst();
	bool EncounteredError();
private:
	Token peek();
	Token peekNext();
	Token advance();
	bool atEnd();

	Token consume(TokenType type, std::string msg);
	void consumeSemicolon();

	TokenType match(int count, ...);
	bool match(TokenType type);

	// Matches multiple tokens in succession, like `if else`
	bool matchTwo(TokenType t1, TokenType t2);

	CompilerError* makeCompilerError(std::string message);
	CompilerError* makeCompilerError(std::string message, int line);

	// Statements
	StmtPtr statement();
	StmtPtr variableDeclaration(bool requireSemicolon = true);
	StmtPtr printStatement();
	StmtPtr blockStatement();
	StmtPtr ifStatement();
	StmtPtr whileStatement();
	StmtPtr forStatement();
	StmtPtr expressionStatement(bool requireSemicolon = true);
	StmtPtr functionDeclaration();
	StmtPtr returnStatement();

	// Expressions
	ExprPtr expression();	
	ExprPtr assignment();
	ExprPtr logical();	
	ExprPtr comparison();
	ExprPtr term();
	ExprPtr factor();
	ExprPtr unary();
	ExprPtr call();
	ExprPtr primary();

	// Our little method to run to the border of a statement. This
	// allows us to keep trucking through errors
	void synchronize();

	Scanner m_scanner;
	Token m_twoNext;
	Token m_next;
	Token m_prev;
	
	ScopeStack m_scope;

	// Here for the same reason as scanner error,
	// we must return a pointer to an error which 
	// must persist after `throw` so must be declared
	// somewhere outside of the method where the error
	// is thrown.
	CompilerError m_error;

	// A flag to keep track of whether we are in a call or in
	// global scope. This is because if we are in global scope
	// and `return` is encountered, there is nothing to catch
	// the value it throws
	bool m_inCall;

	// Flag to see if there were any errors. If so, we really
	// do not want to allow the interpreter to start doin stuff
	bool m_encounteredError;
};
