#pragma once

#include <vector>

#include "compiler/expression/Expr.hpp"

class Stmt {
public:
	// For debugging purposes, depth is recursive depth 
	// used to format block statments nicely
	virtual std::string ToString(int depth = 0) = 0;
};

class VariableDeclaration : public Stmt {
public:
	VariableDeclaration(std::shared_ptr<Expr> expr) : m_expr{expr} {}

	std::string ToString(int depth);
private:
	// Since language is statically scoped, the initial
	// expression will only be evaluated when the variable
	// is declared (if there is any)
	std::shared_ptr<Expr> m_expr;
};

class PrintStatement : public Stmt {
public:
	
	PrintStatement(std::shared_ptr<Expr> printExpr) : m_expr{printExpr} {}

	std::string ToString(int depth);

private:
	std::shared_ptr<Expr> m_expr;
};

class BlockStatement : public Stmt {
public:	
	BlockStatement(std::vector<std::shared_ptr<Stmt>> statements) : m_statements{statements} {}
	
	std::string ToString(int depth);
private:
	std::vector<std::shared_ptr<Stmt>> m_statements;
};
