#include "interpreter/ExpressionVisitor.hpp"
#include "interpreter/StatementVisitor.hpp"

#include <limits>
#include <cmath>
#include <iostream>

#include "interpreter/Callable.hpp"

TypePtr ExpressionVisitor::visitBinaryExpr(Binary expr) {
	TypePtr leftType = expr.GetLeft()->accept(*this);
	TypePtr rightType = expr.GetRight()->accept(*this);

	// Only one case where function type is valid: where
	// the leftValue side is an identifier and the rightValue side
	// is the function and the function only
	if (expr.GetLeft()->GetExprType() == ExprType::IDENTIFIER &&
			rightType->GetType() == ValueType::FUNCTION && expr.GetOp() == TokenType::EQUAL) {
			Identifier *variable = (Identifier*)expr.GetLeft().get();

			// Line of code is a little cursed, but `Get()` returns a reference
			// so we can assign it however we want
			m_symbols.Get(variable->GetId()) = rightType;

			return rightType;	
	}

	if (leftType->GetType() == ValueType::FUNCTION || rightType->GetType() == ValueType::FUNCTION) {
		// Already did check to see if the function arg is valid,
		// so it is safe to throw from here
		throw;
	}

	Value leftValue = leftType->AsValue();
	Value rightValue = rightType->AsValue();


	// This ain't javascript, we ain't gonna do operations
	// on values with different types (who the hell would
	// want that)
	if (leftValue.GetType() != rightValue.GetType()) {
		throw;
	}

	ValueType type = leftValue.GetType();

	// Appears to just be returning doubles or bools, but implicit constructor constructs value object
	// with the return value as the arg

	switch (expr.GetOp()) {
		case TokenType::PLUS: {
			switch (type) {
				case ValueType::STRING:
					return Value(leftValue.GetString() + rightValue.GetString()).ToPtr();
				case ValueType::NUMBER:
					return Value(leftValue.GetNumber() + rightValue.GetNumber()).ToPtr();
				case ValueType::BOOL:
				case ValueType::FUNCTION:
				case ValueType::NIL:
					// You can't really add bools, functions or nils together so
					// we throw (TODO: add some proper error handling)
					throw;
				}
		
			}
		case TokenType::MINUS: {
			if (leftValue.GetType() != ValueType::NUMBER) {
				// We can't really subtract anything but numbers
				throw;
			}
			
			return Value(leftValue.GetNumber() + rightValue.GetNumber()).ToPtr();
		}
		case TokenType::STAR: {	
			if (type != ValueType::NUMBER) {
				// Same thing as before
				throw;
			}
			
			return Value(leftValue.GetNumber() * rightValue.GetNumber()).ToPtr();
		}
		case TokenType::SLASH: {
			if (type != ValueType::NUMBER) {
				throw;
			}

			if (rightValue.GetNumber() == 0) {
				// Divide by zero error
				throw;
			}
			
			return Value(leftValue.GetNumber() / rightValue.GetNumber()).ToPtr();
		}
		case TokenType::MOD: {
			if (type != ValueType::NUMBER) {
				throw;
			}

			double leftNum = leftValue.GetNumber();
			double rightNum = rightValue.GetNumber();
			
			if (isInteger(leftNum) && isInteger(rightNum)) {
				return Value((double)((int)leftNum % (int)rightNum)).ToPtr();
			}
			
			// Means one or both of the operands was not an integer
			throw;
		}
		case TokenType::EQUAL: {
			// Cannot assign to a non-identifier type, you can't say
			// 1 = 2 or true = false
			if (expr.GetLeft()->GetExprType() != ExprType::IDENTIFIER) throw;

			Identifier *variable = (Identifier*)expr.GetLeft().get();

			// Line of code is a little cursed, but `Get()` returns a reference
			// so we can assign it however we want. Have to do a little pointer
			// trickery to force it to be a reference. Remember, this works since
			// the memory location of the pointer is essentially `const` to us, but
			// we can do whatever we want with the underlying value
			m_symbols.Get(variable->GetId())->AsValue() = rightValue;

			return Value(rightValue).ToPtr();
		}
		case TokenType::EQUAL_EQUAL: {
			switch (type) {
				case ValueType::NUMBER:
					return Value(leftValue.GetNumber() == rightValue.GetNumber()).ToPtr();
				case ValueType::STRING:
					return Value(leftValue.GetString() == rightValue.GetString()).ToPtr();
				case ValueType::BOOL:
					return Value(leftValue.GetBoolean() == rightValue.GetBoolean()).ToPtr();
				case ValueType::NIL:
					// If the type is `nil` then they both must be `nil`
					return Value(true).ToPtr();
				case ValueType::FUNCTION:
					// An odd case but maybe you would want to compare functions ??
					return Value(leftValue.GetFunctionId() == rightValue.GetFunctionId()).ToPtr();
			}
		}
		case TokenType::BANG_EQUAL: {
			switch (type) {
				case ValueType::NUMBER:
					return Value(leftValue.GetNumber() != rightValue.GetNumber()).ToPtr();
				case ValueType::STRING:
					return Value(leftValue.GetString() != rightValue.GetString()).ToPtr();
				case ValueType::BOOL:
					return Value(leftValue.GetBoolean() != rightValue.GetBoolean()).ToPtr();
				case ValueType::NIL:
					// If the type is `nil` then they both must be `nil`
					return Value().ToPtr();
				case ValueType::FUNCTION:
					// An odd case but maybe you would want to compare functions ??
					return Value(leftValue.GetFunctionId() != rightValue.GetFunctionId()).ToPtr();
			}
		}
		case TokenType::GREATER: {
			if (type != ValueType::NUMBER) throw; // I don't believe a string can be greater than a string
			
			return Value(leftValue.GetNumber() > rightValue.GetNumber()).ToPtr();
		}
		case TokenType::GREATER_EQUAL:
			if (type != ValueType::NUMBER) throw;
			
			return Value(leftValue.GetNumber() >= rightValue.GetNumber()).ToPtr();
		case TokenType::LESS:
			if (type != ValueType::NUMBER) throw; 
			
			return Value(leftValue.GetNumber() < rightValue.GetNumber()).ToPtr();
		case TokenType::LESS_EQUAL:
			if (type != ValueType::NUMBER) throw; 
			
			return Value(leftValue.GetNumber() <= rightValue.GetNumber()).ToPtr();
		case TokenType::AND: {
			// Cannot perform `and` operation on non-boolean values
			if (leftValue.GetType() != ValueType::BOOL || rightValue.GetType() != ValueType::BOOL) throw;

			return Value(leftValue.GetBoolean() && leftValue.GetBoolean()).ToPtr();
		}
		case TokenType::OR: {
			// Cannot perform `and` operation on non-boolean values
			if (leftValue.GetType() != ValueType::BOOL || rightValue.GetType() != ValueType::BOOL) throw;

			return Value(leftValue.GetBoolean() || leftValue.GetBoolean()).ToPtr();
		}
		default:
			// Trying to do a binary operation with an invalid op
			throw;
	}
}

TypePtr ExpressionVisitor::visitUnaryExpr(Unary expr) {
	TypePtr rightType = expr.GetRight()->accept(*this);

	if (rightType->GetType() == ValueType::FUNCTION) throw;

	Value rightValue = rightType->AsValue();
	ValueType type = rightValue.GetType();

	switch (expr.GetOp()) {
		case TokenType::BANG:
			if (type != ValueType::BOOL) throw;

			return Value(!rightValue.GetBoolean()).ToPtr();
		case TokenType::MINUS:
			if (type != ValueType::NUMBER) throw;

			return Value(-rightValue.GetNumber()).ToPtr();
		default:
			// (hopefully) unreachable
			throw;
	}
}

TypePtr ExpressionVisitor::visitGroupingExpr(Grouping expr) {
	return expr.GetExpr()->accept(*this);
}

TypePtr ExpressionVisitor::visitPrimaryExpr(Primary expr) {
	return Value(expr.GetValue()->AsValue()).ToPtr();
}

TypePtr ExpressionVisitor::visitIdentifierExpr(Identifier expr) {
	return m_symbols.Get(expr.GetId());
}

TypePtr ExpressionVisitor::visitCallExpr(Call expr) {
	// If we have functions that return functions, we of course
	// evaluate those first, and then we get our callable type.
	// A bit of a recurseive approach, but eventually our
	// `visitIdentifier()` method will catch it and return the
	// first underlying callable which will then be called and returned
	// so we can find the next and so on

	TypePtr callable = expr.GetCallee()->accept(*this);

	std::cout << "Callable, stack is ";
	m_symbols.PrintStack();


	if (callable->GetType() != ValueType::FUNCTION) {
		// We cannot things that are not callable
		throw;
	}

	std::vector<TypePtr> args;

	for (ExprPtr arg : expr.GetArgs()) {
		args.push_back(arg->accept(*this));
	}

	return callable->AsCallable().Call(m_statementVisitor, m_symbols, args);
}

bool ExpressionVisitor::isInteger(double n) {
	double comparison = (double)((int)n);

	// Goofy ahh double comparison
	return std::fabs(n - comparison) < std::numeric_limits<double>::epsilon();
}
