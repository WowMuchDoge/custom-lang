#include "compiler/ScopeStack.hpp"

#include <iostream>

void ScopeStack::Push(std::string name) {
	m_symbolSlots.back().insert({name, m_currentSlot++});
	std::cout << "Pushed " << name << " Slot index " << m_currentSlot - 1 << "\n";
}

void ScopeStack::NewScope() {
	m_beginSlots.push_back(m_currentSlot);
	std::cout << "New scope, last used was " << m_beginSlots.back() << std::endl;
	m_symbolSlots.push_back(std::unordered_map<std::string, int>());
}

void ScopeStack::EndScope() {
	std::cout << "Reverting slot from " << m_currentSlot << " to " << m_beginSlots.back() << std::endl;
	m_currentSlot = m_beginSlots.back();
	m_symbolSlots.pop_back();
	m_beginSlots.pop_back();
}

int ScopeStack::Resolve(std::string name) {
	auto it = m_symbolSlots.back().find(name);

	int searchIndex = m_symbolSlots.size() - 1;

	while (it == m_symbolSlots[searchIndex].end()) {
		if (searchIndex <= 0)
			return -1;

		it = m_symbolSlots[--searchIndex].find(name);
	}

	return it->second;
}
