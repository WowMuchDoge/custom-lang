#include "interpreter/Interpreter.hpp"

Value Interpreter::evaluate(ExprPtr expr) {
	return expr->accept(m_expressionVisitor);
}
