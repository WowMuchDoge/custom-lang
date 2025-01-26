#pragma once

#include <vector>

#include "compiler/expression/Expr.hpp"

class Stmt {
public:
	// For debugging purposes, depth is recursive depth 
	// used to format block statments nicely
	virtual std::string ToString(int depth = 0) = 0;
};

typedef std::shared_ptr<Stmt> StmtPtr;

class VariableDeclaration : public Stmt {
public:
	VariableDeclaration(ExprPtr expr) : m_expr{expr} {}

	std::string ToString(int depth);
private:
	// Since language is statically scoped, the initial
	// expression will only be evaluated when the variable
	// is declared (if there is any)
	ExprPtr m_expr;
};

class PrintStatement : public Stmt {
public:
	
	PrintStatement(ExprPtr printExpr) : m_expr{printExpr} {}

	std::string ToString(int depth);

private:
	ExprPtr m_expr;
};

class BlockStatement : public Stmt {
public:	
	BlockStatement(std::vector<StmtPtr> statements) : m_statements{statements} {}
	
	std::string ToString(int depth);
private:
	std::vector<StmtPtr> m_statements;
};

class IfStatement : public Stmt {
public:

private:
	StmtPtr m_intialIf;
	std::vector<StmtPtr> m_ifElses;
	StmtPtr m_else;
};