#pragma once

#include "compiler/expression/Expr.hpp"

class Stmt {
public:
	// For debugging purposes
	virtual void Print() = 0;
};

class VariableDeclaration : public Stmt {
public:
	VariableDeclaration(int id, std::shared_ptr<Expr> expr) : m_id{id}, m_expr{expr} {}

	void Print();
private:
	// Since language is statically scoped, the initial
	// expression will only be evaluated when the variable
	// is declared (if there is any)
	std::shared_ptr<Expr> m_expr;

	// Used for resolving the variable value at run time
	int m_id;
};
