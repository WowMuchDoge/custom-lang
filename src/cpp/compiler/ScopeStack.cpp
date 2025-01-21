#include "compiler/ScopeStack.hpp"

void ScopeStack::Push(std::string name) {
	m_symbolSlots[m_depth].insert({name, m_currentSlot++});
}

void ScopeStack::NewScope() {
	m_depth++;
	m_symbolSlots.push_back(std::unordered_map<std::string, int>());
}

void ScopeStack::EndScope() {
	m_depth--;
	m_symbolSlots.pop_back();
}

int ScopeStack::Resolve(std::string name) {
	auto it = m_symbolSlots[m_depth].find(name);

	int searchIndex = m_depth;

	while (it == m_symbolSlots[searchIndex].end()) {
		if (searchIndex == 0)
			return -1;

		it = m_symbolSlots[--searchIndex].find(name);
	}

	return it->second;
}

bool ScopeStack::DoesExist(std::string name) {
	auto it = m_symbolSlots[m_depth].find(name);

	int searchIndex = m_depth;
	
	while (it == m_symbolSlots[searchIndex].end()) {
		if (searchIndex == 0)
			return false;

		it = m_symbolSlots[--searchIndex].find(name);
	}

	return true;
}
