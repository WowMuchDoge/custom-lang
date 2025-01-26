#include "compiler/Stmt.hpp"

std::string VariableDeclaration::ToString(int depth = 0) {
	std::string prefix = std::string(" ", depth);

	return prefix + "Variable declaration, expr = " + m_expr->ToString();
}

std::string PrintStatement::ToString(int depth = 0) {
	std::string prefix = std::string(" ", depth);

	return prefix + "Print statement, expr = " + m_expr->ToString();
}

std::string BlockStatement::ToString(int depth = 0) {
	std::string prefix = std::string(" ", depth);
	std::string output = prefix + "Block statement, statements = {\n";

	for (auto statement : m_statements) {
		output += prefix + statement->ToString(depth + 1) + '\n';
	}

	output += prefix + "}";

	return output;
}
