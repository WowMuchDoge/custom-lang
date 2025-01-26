#include "compiler/Stmt.hpp"

#include <iostream>

void VariableDeclaration::Print() {
	std::cout << "Variable declaration, expr = " << m_expr->ToString() << std::endl;
}

void PrintStatement::Print() {
	std::cout << "Print statement, expr = " << m_expr->ToString() << std::endl;
}

void BlockStatement::Print() {
	std::cout << "Block statement, statements = {\n";

	for (auto statement : m_statements) {
		std::cout << "	";
		statement->Print();
	}

	std::cout << "}" << std::endl;
}
