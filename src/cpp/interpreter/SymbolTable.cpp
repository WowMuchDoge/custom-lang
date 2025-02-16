#include "interpreter/SymbolTable.hpp"

#include <iostream>

void SymbolTable::NewScope() {
	m_scopeBorders.push_back(getTableHeadIndex());
}

void SymbolTable::EndScope() {
	while (getTableHeadIndex() != m_scopeBorders.back()) {
		m_table.pop_back();
	}
}

void SymbolTable::Push() {
	m_table.push_back(Value().ToPtr());
}

void SymbolTable::Push(TypePtr val) {
	m_table.push_back(val);
}

TypePtr SymbolTable::Get(int index) {
	return m_table[index];
}

int SymbolTable::getTableHeadIndex() {
	return m_table.size() - 1;
}
