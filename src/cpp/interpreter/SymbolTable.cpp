#include "interpreter/SymbolTable.hpp"

void SymbolTable::NewScope() {
	m_scopeBorders.push_back(getTableHeadIndex());
}

void SymbolTable::EndScope() {
	while (getTableHeadIndex() != m_scopeBorders.back())
		m_table.pop_back();
}

void SymbolTable::Push() {
	m_table.push_back(Value());
}

void SymbolTable::Push(Value v) {
	m_table.push_back(v);
}

Value SymbolTable::Get(int index) {
	return m_table[index];
}

int SymbolTable::getTableHeadIndex() {
	return m_table.size() - 1;
}
