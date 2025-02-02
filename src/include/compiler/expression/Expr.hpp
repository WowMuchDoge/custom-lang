#pragma once

#include <memory>
#include <vector>

#include "compiler/expression/Value.hpp"
#include "Constants.hpp"

enum class ExprType {
    BINARY,
    UNARY,
    GROUPING,
    PRIMARY,
	IDENTIFIER,
	CALL
};

class Expr {
public:
    virtual ExprType GetExprType() = 0;

	// For debugging purposes
	virtual std::string ToString() = 0;
};

typedef std::shared_ptr<Expr> ExprPtr;

class Binary : public Expr {
public:
	Binary(TokenType op, ExprPtr left, ExprPtr right)
		: m_op{op}, m_left{left}, m_right{right} {}
	
	ExprType GetExprType();
	std::string ToString();

private:
	TokenType m_op;
	ExprPtr m_right;
	ExprPtr m_left;
};

class Unary : public Expr {
public:
	Unary(TokenType op, ExprPtr right) : m_op{op}, m_right{right} {}

	ExprType GetExprType();
	std::string ToString();

private:
	TokenType m_op;
	ExprPtr m_right;
};

class Grouping : public Expr {
public:
	Grouping(ExprPtr expr) : m_expr{expr} {}

	ExprType GetExprType();
	std::string ToString();

private:
	ExprPtr m_expr;
};

class Primary : public Expr {
public:
	Primary(Value val) : m_value{val} {}

	ExprType GetExprType();
	std::string ToString();

private:
	Value m_value;
};

class Identifier : public Expr {
public:
	Identifier(int id) : m_id{id} {}

	ExprType GetExprType();

	std::string ToString();
private:
	// We only need an ID since the value associated with the 
	// identifier will only be deduced at runtime 
	int m_id;
};

class Call : public Expr {
public:
	Call(ExprPtr callee, std::vector<ExprPtr> args) : m_callee{callee}, m_args{args} {}

	ExprType GetExprType();

	std::string ToString();
private:
	ExprPtr m_callee;
	std::vector<ExprPtr> m_args;
};
