#pragma once

#include <memory>

#include "compiler/expression/Value.hpp"
#include "Constants.hpp"

enum class ExprType {
    BINARY,
    UNARY,
    GROUPING,
    PRIMARY,
	IDENTIFIER
};

class Expr {
public:
    virtual ExprType GetExprType() = 0;

	// For debugging purposes
	virtual std::string ToString() = 0;
};

class Binary : public Expr {
public:
	Binary(TokenType op, std::shared_ptr<Expr> left, std::shared_ptr<Expr> right)
		: m_op{op}, m_left{left}, m_right{right} {}
	
	ExprType GetExprType();
	std::string ToString();

private:
	TokenType m_op;
	std::shared_ptr<Expr> m_right;
	std::shared_ptr<Expr> m_left;
};

class Unary : public Expr {
public:
	Unary(TokenType op, std::shared_ptr<Expr> right) : m_op{op}, m_right{right} {}

	ExprType GetExprType();
	std::string ToString();

private:
	TokenType m_op;
	std::shared_ptr<Expr> m_right;
};

class Grouping : public Expr {
public:
	Grouping(std::shared_ptr<Expr> expr) : m_expr{expr} {}

	ExprType GetExprType();
	std::string ToString();

private:
	std::shared_ptr<Expr> m_expr;
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
