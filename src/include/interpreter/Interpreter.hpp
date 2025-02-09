#pragma once

#include "interpreter/ExpressionVisitor.hpp"

class Interpreter {
public:
	Interpreter() : m_expressionVisitor{m_table} {}
private:
	ExpressionVisitor m_expressionVisitor;
	SymbolTable m_table;

	Value evaluate(ExprPtr expr);
};
