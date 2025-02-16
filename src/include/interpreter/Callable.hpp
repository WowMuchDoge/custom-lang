#pragma once

#include <vector>

#include "compiler/expression/Value.hpp"
#include "compiler/Stmt.hpp"

class CallStack;

class Callable : public IType {
public:
	Callable(StmtPtr toExecutestmt);

	std::string ToString();
	ValueType GetType();
	Callable& AsCallable();

	IType Call(CallStack& interpreter, std::vector<TypePtr> args);
private:

};
