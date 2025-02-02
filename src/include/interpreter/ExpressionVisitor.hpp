#pragma once

#include "compiler/expression/Expr.hpp"
#include "interpreter/SymbolTable.hpp"

class ExpressionVisitor {
public:
	ExpressionVisitor(SymbolTable& symbols) : m_symbols{symbols} {}	

	Value visitBinaryExpr(Binary expr);
	Value visitUnaryExpr(Unary expr);
	Value visitGroupingExpr(Grouping expr);
	Value visitPrimaryExpr(Primary expr);
	Value visitIdentifierExpr(Identifier expr);
	Value visitCallExpr(Call expr);

private:
	static bool isInteger(double n);

	SymbolTable& m_symbols;
};
