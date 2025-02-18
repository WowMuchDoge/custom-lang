#include "interpreter/StatementVisitor.hpp"
#include "interpreter/Callable.hpp"

#include <iostream>

void StatementVisitor::VisitVariableDeclaration(VariableDeclaration stmt) {
	m_table->Push(evaluate(stmt.GetExpr()));
}

void StatementVisitor::VisitPrintStatement(PrintStatement stmt) {
	std::cout << evaluate(stmt.GetExpression())->ToString() << std::endl;
}

void StatementVisitor::VisitBlockStatement(BlockStatement stmt) {
	// std::cout << "Visiting block\n";
	// m_table->PrintStack();
	m_table->NewScope();
	for (StmtPtr statement : stmt.GetStatements()) {
		statement->Accept(*this);
	}
	m_table->EndScope();
}

void StatementVisitor::VisitIfStatement(IfStatement stmt) {
	for (IfObject obj : stmt.GetIfs()) {
		if (obj.IsElse()) {
			obj.GetStmt()->Accept(*this);
		} else {
			Value condition = evaluateValue(obj.GetExpr());

			if (condition.GetType() != ValueType::BOOL) {
				// TODO add some error handling
				throw;
			}

			bool funny = condition.GetBoolean();
			if (condition.GetBoolean()) {
				obj.GetStmt()->Accept(*this);

				// In an if-else chain, the first condition that evaluates to
				// true is ran and breaks out of the chain
				break;
			}
		}
	}
}

void StatementVisitor::VisitWhileStatement(WhileStatement stmt) {
	Value condition = evaluateValue(stmt.GetExpr());

	if (condition.GetType() != ValueType::BOOL) {
		// Condition has to be a boolean
		throw;
	}

	while (evaluateValue(stmt.GetExpr()).GetBoolean()) {
		stmt.GetStmt()->Accept(*this);
	}
}

void StatementVisitor::VisitExpressionStatement(ExpressionStatement stmt) {
	evaluate(stmt.GetExpr());
}

void StatementVisitor::VisitFunctionDeclaration(FunctionDeclaration stmt) {
	m_table->Push(Callable(stmt.GetBlock().ToPtr(), stmt.GetParams(), m_table->GetTailIndex() + 1).ToPtr());
}

void StatementVisitor::VisitReturnStatement(ReturnStatement stmt) {
	// A touch cursed and inneficient, but this will be caught by the function
	// and that is the value it will return
	// std::cout << evaluate(stmt.GetExpr())->ToString() << "Evaluated\n";
	throw evaluate(stmt.GetExpr());
}

TypePtr StatementVisitor::evaluate(ExprPtr expr) {
	return expr->accept(m_exprVisitor);
}

Value StatementVisitor::evaluateValue(ExprPtr expr) {
	TypePtr ptr = evaluate(expr);

	if (ptr->GetType() == ValueType::FUNCTION) throw;

	return ptr->AsValue();
}
