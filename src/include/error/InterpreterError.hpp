#pragma once

#include <string>

#include "error/Error.hpp"

class InterpreterError : Error {
public:
	InterpreterError(std::string msg, int line) : m_message{msg}, m_line{line} {}

	void Print();	
private:
	std::string m_message;
	int m_line;
};
