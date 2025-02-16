#pragma once

#include <memory>
#include <vector>

#include "compiler/expression/Value.hpp"
#include "Constants.hpp"

// Forward declaring the visitor to avoid circular dependencies
class ExpressionVisitor;

enum class ExprType {
    BINARY,
    UNARY,
    GROUPING,
    PRIMARY,
	IDENTIFIER,
	CALL
};

class Expr;
typedef std::shared_ptr<Expr> ExprPtr;

class Expr {
public:
    virtual ExprType GetExprType() = 0;

	// For debugging purposes
	virtual std::string ToString() = 0;

	// For the visitor pattern, must use `std::shared_ptr<Expr>` since `ExprPtr`
	// is defined after the class is declared
	virtual TypePtr accept(ExpressionVisitor& visitor) = 0;

	virtual ExprPtr ToPtr() = 0;
};

class Binary : public Expr {
public:
	Binary(TokenType op, ExprPtr left, ExprPtr right)
		: m_op{op}, m_left{left}, m_right{right} {}
	
	ExprType GetExprType();
	std::string ToString();

	TokenType GetOp() { return m_op; }

	ExprPtr GetLeft() { return m_left; }
	ExprPtr GetRight() { return m_right; }

	TypePtr accept(ExpressionVisitor& visitor);


	ExprPtr ToPtr();
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

	TokenType GetOp() { return m_op; }
	ExprPtr GetRight() { return m_right; }

	TypePtr accept(ExpressionVisitor& visitor);

	ExprPtr ToPtr();
private:
	TokenType m_op;
	ExprPtr m_right;
};

class Grouping : public Expr {
public:
	Grouping(ExprPtr expr) : m_expr{expr} {}

	ExprType GetExprType();
	std::string ToString();

	TypePtr accept(ExpressionVisitor& visitor);

	ExprPtr GetExpr() { return m_expr; }

	ExprPtr ToPtr();
private:
	ExprPtr m_expr;
};

class Primary : public Expr {
public:
	Primary(TypePtr val) : m_value{val} {}

	ExprType GetExprType();
	std::string ToString();

	TypePtr GetValue() { return m_value; }

	TypePtr accept(ExpressionVisitor& visitor);

	ExprPtr ToPtr();
private:
	TypePtr m_value;
};

class Identifier : public Expr {
public:
	Identifier(int id) : m_id{id} {}

	ExprType GetExprType();
	std::string ToString();

	int GetId() { return m_id; }

	TypePtr accept(ExpressionVisitor& visitor);

	ExprPtr ToPtr();
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

	ExprPtr GetCallee() { return m_callee; }
	std::vector<ExprPtr> GetArgs() { return m_args; }

	TypePtr accept(ExpressionVisitor& visitor);
	
	ExprPtr ToPtr();
private:
	ExprPtr m_callee;
	std::vector<ExprPtr> m_args;
};
