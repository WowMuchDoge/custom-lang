#pragma once

#include "compiler/expression/Value.hpp"
#include "Constants.hpp"

enum class ExprType {
    BINARY,
    UNARY,
    GROUPING,
    PRIMARY
};

class Expr {
public:
    virtual ExprType GetExprType() = 0;

	// For debugging purposes
	virtual std::string ToString() = 0;
};

class Binary : public Expr {
public:
	Binary(TokenType op, Expr* right, Expr* left)
		: m_op{op}, m_right{right}, m_left{left} {}
	
	ExprType GetExprType();
	std::string ToString();

private:
	TokenType m_op;
	Expr* m_right;
	Expr* m_left;
};

class Unary : public Expr {
public:
	Unary(TokenType op, Expr* right) : m_op{op}, m_right{right} {}

	ExprType GetExprType();
	std::string ToString();

private:
	TokenType m_op;
	Expr* m_right;
};

class Grouping : public Expr {
public:
	Grouping(Expr *expr) : m_expr{expr} {}

	ExprType GetExprType();
	std::string ToString();

private:
	Expr* m_expr;
};

class Primary : public Expr {
public:
	Primary(Value val) : m_value{val} {}

	ExprType GetExprType();
	std::string ToString();

private:
	Value m_value;
};
