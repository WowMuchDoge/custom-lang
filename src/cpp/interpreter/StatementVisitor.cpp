#include "interpreter/StatementVisitor.hpp"

#include <iostream>

void StatementVisitor::VisitVariableDeclaration(VariableDeclaration stmt) {
	m_table.Push(evaluate(stmt.GetExpr()));
}

void StatementVisitor::VisitPrintStatement(PrintStatement stmt) {
	std::cout << evaluate(stmt.GetExpression()).ToString() << std::endl;
}

void StatementVisitor::VisitBlockStatement(BlockStatement stmt) {

}

void StatementVisitor::VisitIfStatement(IfStatement stmt) {

}

void StatementVisitor::VisitWhileStatement(WhileStatement stmt) {

}

void StatementVisitor::VisitExpressionStatement(ExpressionStatement stmt) {

}

void StatementVisitor::VisitFunctionDeclaration(FunctionDeclaration stmt) {

}

void StatementVisitor::VisitReturnStatement(ReturnStatement stmt) {

}

Value StatementVisitor::evaluate(ExprPtr expr) {
	return expr->accept(m_exprVisitor);
}
