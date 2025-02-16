#pragma once

#include <vector>

#include "compiler/expression/Value.hpp"

class SymbolTable {
public:
	SymbolTable() : m_scopeBorders({0}) {}

	void NewScope();
	void EndScope();

	void Push(); // Pushes `nil` to the symbol table
	void Push(TypePtr val);

	TypePtr Get(int index);
	
	TypePtr& GetTail();
	TypePtr& GetHead();

	void PrintStack();
private:

	// The borders of the scope so when we leave a scope we know what position to delete to
	std::vector<int> m_scopeBorders;
	std::vector<TypePtr> m_table;

	int getTableHeadIndex();
};
