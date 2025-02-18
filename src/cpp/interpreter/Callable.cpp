#include "interpreter/Callable.hpp"

#include <iostream>

std::string Callable::ToString() {
	return "<fn " + std::to_string((uintptr_t)this) + ">";
}

ValueType Callable::GetType() {
	return ValueType::FUNCTION;
}

Callable& Callable::AsCallable() {
	return *this;
}

TypePtr Callable::Call(StatementVisitor* visitor, SymbolTable* currentScope, std::vector<TypePtr> args) {
	// If arity isn't the same, we didn't call the function properly

	if (args.size() != m_params.size()) throw;

	currentScope->PrintStack();

	ScopePair scopeDiff = currentScope->Rewind(m_scopeBorder);

	currentScope->NewScope();
	
	for (TypePtr arg : args) {
		// A little strange, but the compiler already populated the
		// ids of the params, so as long as we evaluate and populate
		// the args in the right order we should all be good

		currentScope->Push(arg);
	}
	
	try {
		m_block->Accept(*visitor);
	} catch (TypePtr val) {
		return val;
	}

	currentScope->EndScope();

	currentScope->PushBack(scopeDiff);

	return Value().ToPtr();
}

TypePtr Callable::ToPtr() {
	return TypePtr(new Callable(*this));
}
