#pragma once

#include <unordered_map>
#include <vector>
#include <string>

class ScopeStack {
public:
	ScopeStack() : m_depth{0}, m_currentSlot{0} {}

	void Push(std::string name);

	void NewScope();
	void EndScope();

	int Resolve(std::string name);
private:
	int m_depth;

	// So we know how much to reduce the slot number by
	int m_beginSlot;
	int m_currentSlot;

	// String is the name and int is the slot index,
	// names are resolved at compile time for efficiency.
	// This data structure is the stack of symbol tables.
	std::vector<std::unordered_map<std::string, int>> m_symbolSlots;
};
