#pragma once

#include <vector>

#include "compiler/expression/Value.hpp"

typedef std::pair<std::vector<TypePtr>, std::vector<int>> ScopePair; 

class SymbolTable {
public:
	SymbolTable() : m_scopeBorders({0}) {}

	void NewScope();
	void EndScope();

	void Push(); // Pushes `nil` to the symbol table
	void Push(TypePtr val);

	TypePtr& Get(int index);
	
	TypePtr& GetTail();
	TypePtr& GetHead();

	// Returns a pair containing the popped variables and scope borders
	ScopePair Rewind(int index);
	// Push back the pair returned by the `PopUntilIndex` function
	void PushBack(ScopePair pair);

	int GetTailIndex();

	void PrintStack();
private:

	// The borders of the scope so when we leave a scope we know what position to delete to
	std::vector<int> m_scopeBorders;
	std::vector<TypePtr> m_table;

	int getTableHeadIndex();
};
