#include "compiler/Stmt.hpp"

#include <iostream>

void VariableDeclaration::Print() {
	std::cout << "Variable declaration, expr = " << m_expr->ToString() << std::endl;
}
