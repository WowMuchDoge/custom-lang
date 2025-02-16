#include "interpreter/SymbolTable.hpp"

#include <iostream>

TypePtr& SymbolTable::GetTail() {
	return m_table.back();
}

TypePtr& SymbolTable::GetHead() {
	return m_table[0];
}

void SymbolTable::NewScope() {
	m_scopeBorders.push_back(getTableHeadIndex());
}

void SymbolTable::EndScope() {
	std::cout << "Endscope called, Stack is ";
	PrintStack();
	while (getTableHeadIndex() != m_scopeBorders.back()) {
		std::cout << "Removed " << m_table.back()->ToString() << std::endl;
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

void SymbolTable::PrintStack() {
	std::cout << "Symbol stack: [\n";

	for (TypePtr val : m_table) {
		std::cout << val->ToString() << "\n";
	}

	std::cout << "]\n";
}