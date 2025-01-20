#pragma once

#include "error/Error.hpp"

#include <string>

class CompilerError : public Error {
public:
	CompilerError(std::string message, int line) : m_message{message}, m_line{line} {}

	void Print();

private:
	std::string m_message;
	int m_line;
};
