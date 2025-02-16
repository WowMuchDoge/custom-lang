#pragma once

#include "interpreter/SymbolTable.hpp"
#include "compiler/Stmt.hpp"

class CallStack {
public:
	// We copy the scope in order modify it without modifying the scope of 
	// other stackframes lower in the stack
	CallStack (SymbolTable table, StmtPtr executionBlock) : m_table{table}, m_block{executionBlock} {}

	StmtPtr GetBlock() { return m_block; }
private:
	SymbolTable m_table;
	StmtPtr m_block;
};
