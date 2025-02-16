#include "interpreter/StatementVisitor.hpp"
#include "interpreter/Callable.hpp"

#include <iostream>

void StatementVisitor::VisitVariableDeclaration(VariableDeclaration stmt) {
	m_table.Push(evaluate(stmt.GetExpr()));
}

void StatementVisitor::VisitPrintStatement(PrintStatement stmt) {
	std::cout << evaluate(stmt.GetExpression())->ToString() << std::endl;
}

void StatementVisitor::VisitBlockStatement(BlockStatement stmt) {
	m_table.NewScope();
	for (StmtPtr statement : stmt.GetStatements()) {
		statement->Accept(*this);
	}
	m_table.EndScope();
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

			obj.GetStmt()->Accept(*this);
			// In an if-else chain, the first condition that evaluates to
			// true is ran and breaks out of the chain
			break;
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
	// std::cout << stmt.ToString(0) << std::endl;
	m_table.Push(Callable(stmt.GetBlock(), stmt.GetParams()).ToPtr());

	// Necessary since the table that the callable has must include itself
	m_table.GetHead()->AsCallable().SetTable(m_table);

	std::cout << "Pushed function, stack is ";
	m_table.PrintStack();
}

void StatementVisitor::VisitReturnStatement(ReturnStatement stmt) {

}

TypePtr StatementVisitor::evaluate(ExprPtr expr) {
	return expr->accept(m_exprVisitor);
}

Value StatementVisitor::evaluateValue(ExprPtr expr) {
	TypePtr ptr = evaluate(expr);

	if (ptr->GetType() == ValueType::FUNCTION) throw;

	return ptr->AsValue();
}

void StatementVisitor::ChangeScope(SymbolTable& table) {
	std::cout << "New table for statement visitor is ";
	table.PrintStack();
	m_table = table;
}
