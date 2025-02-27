#include "error/InterpreterError.hpp"

#include <iostream>

void InterpreterError::Print() {
	std::cout << "[Line " << m_line << "] Interpreter Error: " << m_message << std::endl;
}
