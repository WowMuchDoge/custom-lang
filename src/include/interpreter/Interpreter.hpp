#pragma once

#include "interpreter/StatementVisitor.hpp"
#include <utility>

class Interpreter {
public:
	Interpreter();
private:
	ExpressionVisitor m_expressionVisitor;
	StatementVisitor m_statementVisitor;

	std::pair<StmtPtr, SymbolTable> m_callStack;
};
