#pragma once

#include <vector>
#include <optional>

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

class IfObject {
public:
	IfObject(std::optional<ExprPtr> expr, StmtPtr statement) : m_expr{expr}, m_statement{statement} {}

	std::string ToString(int depth);

	bool IsElse() { return !m_expr.has_value(); }

	// Will throw an error if trying to get this from an else statement
	ExprPtr GetExpr() { return m_expr.value(); }

	StmtPtr GetStmt() { return m_statement; }
private:

	// There would be no expression in an `else` control path
	std::optional<ExprPtr> m_expr;
	StmtPtr m_statement;
};

class IfStatement : public Stmt {
public:
	IfStatement(std::vector<IfObject> ifChain) : m_ifChain{ifChain} {}

	std::string ToString(int depth);
private:
	std::vector<IfObject> m_ifChain;
};

class WhileStatement : public Stmt {
public:
	WhileStatement(ExprPtr expr, StmtPtr stmt) : m_expr{expr}, m_stmt{stmt} {}

	std::string ToString(int depth);
private:
	ExprPtr m_expr;
	StmtPtr m_stmt;
};

class ExpressionStatement : public Stmt {
public:
	ExpressionStatement(ExprPtr expr) : m_expr{expr} {}

	std::string ToString(int depth);

	ExprPtr GetExpr() { return m_expr; }
private:
	ExprPtr m_expr;
};
