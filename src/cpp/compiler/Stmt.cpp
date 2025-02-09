#include "compiler/Stmt.hpp"

#include "interpreter/StatementVisitor.hpp"

std::string VariableDeclaration::ToString(int depth = 0) {
	std::string prefix(" ", depth);

	return prefix + "Variable declaration, expr = " + m_expr->ToString();
}

void VariableDeclaration::Accept(StatementVisitor& visitor) {
	visitor.VisitVariableDeclaration(*this);
}

std::string PrintStatement::ToString(int depth = 0) {
	std::string prefix(" ", depth);

	return prefix + "Print statement, expr = " + m_expr->ToString();
}

void PrintStatement::Accept(StatementVisitor& visitor) {
	visitor.VisitPrintStatement(*this);
}

std::string BlockStatement::ToString(int depth = 0) {
	std::string prefix(" ", depth);
	std::string output = prefix + "Block statement, statements = {\n";

	for (auto statement : m_statements) {
		output += prefix + statement->ToString(depth + 1) + '\n';
	}

	output += prefix + "}";

	return output;
}

void BlockStatement::Accept(StatementVisitor& visitor) {
	visitor.VisitBlockStatement(*this);
}

std::string IfObject::ToString(int depth = 0) {
	std::string prefix(" ", depth);
	std::string output = prefix;

	if (m_expr.has_value()) {
		output += "If path, expression = " + m_expr.value()->ToString() + ", Statement =\n";
	} else {
		output += "Else path, Statement =\n";
	}

	output += m_statement->ToString(depth + 1) + "\n";

	return output;
}

std::string IfStatement::ToString(int depth = 0) {
	std::string prefix(" ", depth);

	std::string output = prefix + "If Statements, objects = \n";

	for (IfObject obj : m_ifChain) {
		output += obj.ToString(depth + 1);
	}

	return output;
}

void IfStatement::Accept(StatementVisitor& visitor) {
	visitor.VisitIfStatement(*this);
}

std::string WhileStatement::ToString(int depth = 0) {
	std::string prefix(" ", depth);

	std::string output = prefix + "While Statement, Expr = " + m_expr->ToString() 
		+ ", Statement = \n" + m_stmt->ToString(depth + 1);

	return output;
}

void WhileStatement::Accept(StatementVisitor& visitor) {
	visitor.VisitWhileStatement(*this);
}

std::string ExpressionStatement::ToString(int depth = 0) {
	return std::string(" ", depth) + "Expression statement, expression = " + m_expr->ToString() + "\n";
}

void ExpressionStatement::Accept(StatementVisitor& visitor) {
	visitor.VisitExpressionStatement(*this);
}

std::string FunctionDeclaration::ToString(int depth = 0) {
	std::string prefix(" ", depth);

	std::string output = prefix + "Function Declaration, params = \n" + prefix;

	for (std::string param : m_params) {
		output += param + " ";
	}

	output += "\nBlock = " + m_block.ToString(depth + 1);

	return output;
}

void FunctionDeclaration::Accept(StatementVisitor& visitor) {
	visitor.VisitFunctionDeclaration(*this);
}

std::string ReturnStatement::ToString(int depth = 0) {
	return std::string(" ", depth) + "Return statement, return expr = " + m_returnExpr->ToString();
}

void ReturnStatement::Accept(StatementVisitor& visitor) {
	visitor.VisitReturnStatement(*this);
}
