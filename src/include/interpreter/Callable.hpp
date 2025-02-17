#pragma once

#include <vector>

#include "compiler/expression/Value.hpp"
#include "interpreter/StatementVisitor.hpp"
#include "interpreter/SymbolTable.hpp"
#include "compiler/Stmt.hpp"

class Callable : public IType {
public:
	Callable(StmtPtr toExecute, std::vector<std::string> params) 
		: m_block{toExecute}, m_params{params} {}

	std::string ToString() override;
	ValueType GetType() override;
	Callable& AsCallable() override;
	TypePtr ToPtr() override;

	TypePtr Call(StatementVisitor* visitor, SymbolTable* currentScope, std::vector<TypePtr> args);
private:	
	StmtPtr m_block;
	std::vector<std::string> m_params;
};
