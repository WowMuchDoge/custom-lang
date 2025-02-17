#pragma once

#include <vector>
#include <optional>

#include "compiler/expression/Expr.hpp"

class StatementVisitor;

class Stmt;
typedef std::shared_ptr<Stmt> StmtPtr;

class Stmt {
public:
	// For debugging purposes, depth is recursive depth 
	// used to format block statments nicely
	virtual std::string ToString(int depth = 0) = 0;

	// The function that will accept the statement visitor
	virtual void Accept(StatementVisitor& visitor) = 0;

	virtual StmtPtr ToPtr() = 0;
};

typedef std::shared_ptr<Stmt> StmtPtr;

class VariableDeclaration : public Stmt {
public:
	VariableDeclaration(ExprPtr expr) : m_expr{expr} {}

	ExprPtr GetExpr() { return m_expr; }
	std::string GetName();

	std::string ToString(int depth);

	StmtPtr ToPtr();
private:
	// Since language is statically scoped, the initial
	// expression will only be evaluated when the variable
	// is declared (if there is any)
	ExprPtr m_expr;

	void Accept(StatementVisitor& visitor);
};

class PrintStatement : public Stmt {
public:
	
	PrintStatement(ExprPtr printExpr) : m_expr{printExpr} {}

	std::string ToString(int depth);

	ExprPtr GetExpression() { return m_expr; }

	void Accept(StatementVisitor& visitor);

	StmtPtr ToPtr();
private:
	ExprPtr m_expr;
};

class BlockStatement : public Stmt {
public:	
	BlockStatement(std::vector<StmtPtr> statements, bool isCall) : m_statements{statements}, m_isCall{isCall} {}
	
	void Accept(StatementVisitor& visitor);

	std::vector<StmtPtr> GetStatements() { return m_statements; }

	std::string ToString(int depth);

	StmtPtr ToPtr();
private:
	std::vector<StmtPtr> m_statements;

	// This is the flag for controlling whether this block is part of a function
	// (even nested in a function) or part of the global scope. This will control
	// if a return statement is able to return since you can't return out of global
	// scope
	bool m_isCall;
};

class IfObject {
public:
	IfObject(std::optional<ExprPtr> expr, StmtPtr statement) : m_expr{expr}, m_statement{statement} {}

	std::string ToString(int depth);

	bool IsElse() { return !m_expr.has_value(); }

	// Will throw an error if trying to get this from an else statement
	ExprPtr GetExpr() { return m_expr.value(); }

	StmtPtr GetStmt() { return m_statement; }

	StmtPtr ToPtr();
private:

	// There would be no expression in an `else` control path
	std::optional<ExprPtr> m_expr;
	StmtPtr m_statement;
};

class IfStatement : public Stmt {
public:
	IfStatement(std::vector<IfObject> ifChain) : m_ifChain{ifChain} {}

	std::string ToString(int depth);

	std::vector<IfObject> GetIfs() { return m_ifChain; }

	void Accept(StatementVisitor& visitor);

	StmtPtr ToPtr();
private:
	std::vector<IfObject> m_ifChain;
};

class WhileStatement : public Stmt {
public:
	WhileStatement(ExprPtr expr, StmtPtr stmt) : m_expr{expr}, m_stmt{stmt} {}

	std::string ToString(int depth);

	ExprPtr GetExpr() { return m_expr; }
	StmtPtr GetStmt() { return m_stmt; }

	void Accept(StatementVisitor& visitor);

	StmtPtr ToPtr();
private:
	ExprPtr m_expr;
	StmtPtr m_stmt;
};

class ExpressionStatement : public Stmt {
public:
	ExpressionStatement(ExprPtr expr) : m_expr{expr} {}

	std::string ToString(int depth);

	ExprPtr GetExpr() { return m_expr; }

	void Accept(StatementVisitor& visitor);

	StmtPtr ToPtr();
private:
	ExprPtr m_expr;
};

class FunctionDeclaration : public Stmt {
public:
	FunctionDeclaration(std::vector<std::string> params, BlockStatement block) 
		: m_params{params}, m_block{block} {}

	std::string ToString(int detph);

	void Accept(StatementVisitor& visitor);

	std::vector<std::string> GetParams() { return m_params; }
	BlockStatement GetBlock() { return m_block; }

	StmtPtr ToPtr();
private:
	std::vector<std::string> m_params;
	BlockStatement m_block;
};

class ReturnStatement : public Stmt {
public:
	ReturnStatement(ExprPtr expr) : m_returnExpr{expr} {}

	std::string ToString(int depth);

	void Accept(StatementVisitor& visitor);

	StmtPtr ToPtr();

	ExprPtr GetExpr() { return m_returnExpr; }
private:
	ExprPtr m_returnExpr;
};
