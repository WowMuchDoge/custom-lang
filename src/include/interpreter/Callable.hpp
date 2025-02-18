#pragma once

#include <vector>

#include "compiler/expression/Value.hpp"
#include "interpreter/StatementVisitor.hpp"
#include "interpreter/SymbolTable.hpp"
#include "compiler/Stmt.hpp"

class Callable : public IType {
public:
	Callable(StmtPtr toExecute, std::vector<std::string> params, int scopeBorder) 
		: m_block{toExecute}, m_params{params}, m_scopeBorder{scopeBorder} {}

	std::string ToString() override;
	ValueType GetType() override;
	Callable& AsCallable() override;
	TypePtr ToPtr() override;

	TypePtr Call(StatementVisitor* visitor, SymbolTable* currentScope, std::vector<TypePtr> args);
private:	
	StmtPtr m_block;
	std::vector<std::string> m_params;

	// Used so when called, the symbol table pops everything to this index
	// in order for it to retain the state it had when the function was
	// declared and so the function populates the arguments in the correct
	// location
	int m_scopeBorder;
};
