#pragma once

#include "compiler/Stmt.hpp"
#include "interpreter/SymbolTable.hpp"
#include "interpreter/ExpressionVisitor.hpp"

class StatementVisitor {
public:
	StatementVisitor(SymbolTable* table) : m_table{table}, m_exprVisitor{ExpressionVisitor{m_table, this}} {}
	
	void VisitVariableDeclaration(VariableDeclaration stmt);
	void VisitPrintStatement(PrintStatement stmt);
	void VisitBlockStatement(BlockStatement stmt);
	void VisitIfStatement(IfStatement stmt);
	void VisitWhileStatement(WhileStatement stmt);
	void VisitExpressionStatement(ExpressionStatement stmt);
	void VisitFunctionDeclaration(FunctionDeclaration stmt);
	void VisitReturnStatement(ReturnStatement stmt);
	
private:
	TypePtr evaluate(ExprPtr expr);
	Value evaluateValue(ExprPtr expr);

	SymbolTable* m_table;
	ExpressionVisitor m_exprVisitor;
};
