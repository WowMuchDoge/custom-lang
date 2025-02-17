#include "interpreter/Callable.hpp"

#include <iostream>

void Callable::SetTable(SymbolTable table) {
	m_symbolTable = table;
}

std::string Callable::ToString() {
	return "<fn " + std::to_string((uintptr_t)this) + ">";
}

ValueType Callable::GetType() {
	return ValueType::FUNCTION;
}

Callable& Callable::AsCallable() {
	return *this;
}

TypePtr Callable::Call(StatementVisitor& visitor, SymbolTable *currentScope, std::vector<TypePtr> args) {
	// If arity isn't the same, we didn't call the function properly

	if (args.size() != m_params.size()) throw;
	
	for (TypePtr arg : args) {
		// A little strange, but the compiler already populated the
		// ids of the params, so as long as we evaluate and populate
		// the args in the right order we should be all good

		m_symbolTable.Push(arg);
	}

	std::cout << "On Call (call table)\n";
	m_symbolTable.PrintStack();

	std::cout << "On Call (current table)\n";
	currentScope->PrintStack();

	visitor.ChangeScope(&m_symbolTable);

	m_block->Accept(visitor);

	visitor.ChangeScope(currentScope);

	return Value().ToPtr();
}

TypePtr Callable::ToPtr() {
	return TypePtr(new Callable(*this));
}