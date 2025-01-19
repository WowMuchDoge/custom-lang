#pragma once

#include "compiler/expression/Value.hpp"

enum class ExprType {
    BINARY,
    UNARY,
    GROUPING,
    PRIMARY
};

enum class BinaryOp {
	ADD,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	MODULO,
};

enum class UnaryOp {
	NOT,
	NEGATE,
};

class Expr {
public:
    virtual ExprType GetExprType() = 0;
};

class Binary : public Expr {
public:
	Binary(BinaryOp op, Expr* right, Expr* left)
		: m_op{op}, m_right{right}, m_left{left} {}
	
	ExprType GetExprType();

private:
	BinaryOp m_op;
	Expr* m_right;
	Expr* m_left;
};

class Unary : public Expr {
public:
	Unary(UnaryOp op, Expr* left) : m_op{op}, m_left{left} {}

	ExprType GetExprType();

private:
	UnaryOp m_op;
	Expr* m_left;
};

class Grouping : public Expr {
public:
	Grouping(Expr *expr) : m_expr{expr} {}

	ExprType GetExprType();

private:
	Expr* m_expr;
};

class Primary : public Expr {
public:
	Primary(Value val);

	ExprType GetExprType();

private:
	Value m_value;
};
