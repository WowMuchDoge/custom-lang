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
	while (m_table.size() - 1 > m_scopeBorders.back()) {
		std::cout << "Popping " << m_table.back()->ToString() << std::endl;
		m_table.pop_back();
	}

	m_scopeBorders.pop_back();
}

void SymbolTable::Push() {
	m_table.push_back(Value().ToPtr());
}

void SymbolTable::Push(TypePtr val) {
	m_table.push_back(val);
}

TypePtr& SymbolTable::Get(int index) {
	return m_table[index];
}

int SymbolTable::getTableHeadIndex() {
	return m_table.size() - 1;
}

ScopePair SymbolTable::Rewind(int index) {
	std::vector<TypePtr> poppedVars;
	std::vector<int> poppedBorders;
	
	for (int i = m_table.size() - 1; i > index; i--) {
		std::cout << i << " is i, index is " << index << std::endl;
		poppedVars.push_back(m_table.back());

		if (m_scopeBorders.back() == i) {
			poppedBorders.push_back(m_scopeBorders.back());
			m_scopeBorders.pop_back();
		}

		m_table.pop_back();
	}
	
	return std::pair<std::vector<TypePtr>, std::vector<int>>(poppedVars, poppedBorders); 
}

void SymbolTable::PushBack(ScopePair pair) {
	for (TypePtr var : pair.first) {
		m_table.push_back(var);
	}

	for (int index: pair.second) {
		m_scopeBorders.push_back(index);
	}
}

int SymbolTable::GetTailIndex() {
	return m_table.size() - 1;
}

void SymbolTable::PrintStack() {
	std::cout << "Symbol stack: [\n";

	for (TypePtr val : m_table) {
		std::cout << val->ToString() << "\n";
	}

	std::cout << "], Borders: [\n";

	for (int index: m_scopeBorders) {
		std::cout << index << "\n";
	}

	std::cout << "]\n";
}
