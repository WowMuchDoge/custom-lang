#include "error/CompilerError.hpp"

#include <iostream>

void CompilerError::Print() {
	std::cout << "[line " + std::to_string(m_line) + "] Compiler Error: " + m_message;
}
