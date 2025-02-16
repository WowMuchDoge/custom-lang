#pragma once

#include "compiler/expression/Expr.hpp"
#include "interpreter/SymbolTable.hpp"

class StatementVisitor;

class ExpressionVisitor {
public:
	ExpressionVisitor(SymbolTable* symbols, StatementVisitor& statementVisitor) : m_symbols{symbols}, m_statementVisitor{statementVisitor} {}	

	TypePtr visitBinaryExpr(Binary expr);
	TypePtr visitUnaryExpr(Unary expr);
	TypePtr visitGroupingExpr(Grouping expr);
	TypePtr visitPrimaryExpr(Primary expr);
	TypePtr visitIdentifierExpr(Identifier expr);
	TypePtr visitCallExpr(Call expr);

private:
	static bool isInteger(double n);

	SymbolTable* m_symbols;

	StatementVisitor& m_statementVisitor;
};
