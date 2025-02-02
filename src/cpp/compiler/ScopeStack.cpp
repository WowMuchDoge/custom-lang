#include "compiler/ScopeStack.hpp"

void ScopeStack::Push(std::string name) {
	// `m_depth` refers to the head of the stack which is out of the current index
	m_symbolSlots[m_depth - 1].insert({name, m_currentSlot++});
}

void ScopeStack::NewScope() {
	m_depth++;
	m_beginSlot = m_currentSlot;
	m_symbolSlots.push_back(std::unordered_map<std::string, int>());
}

void ScopeStack::EndScope() {
	m_depth--;
	m_currentSlot = m_beginSlot;
	m_symbolSlots.pop_back();
}

int ScopeStack::Resolve(std::string name) {
	auto it = m_symbolSlots[m_depth - 1].find(name);

	int searchIndex = m_depth - 1;

	while (it == m_symbolSlots[searchIndex].end()) {
		if (searchIndex <= 0)
			return -1;

		it = m_symbolSlots[--searchIndex].find(name);
	}

	return it->second;
}
