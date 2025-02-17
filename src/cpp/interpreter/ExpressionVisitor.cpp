#include "interpreter/ExpressionVisitor.hpp"
#include "interpreter/StatementVisitor.hpp"

#include <limits>
#include <cmath>
#include <iostream>

#include "interpreter/Callable.hpp"

TypePtr ExpressionVisitor::visitBinaryExpr(Binary expr) {
	TypePtr left = expr.GetLeft()->accept(*this);
	TypePtr right = expr.GetRight()->accept(*this);

	// This ain't javascript, we ain't gonna do operations
	// on values with different types (who the hell would
	// want that)
	if (left->GetType() != right->GetType()) {
		throw;
	}

	ValueType type = left->GetType();

	// Appears to just be returning doubles or bools, but implicit constructor constructs value object
	// with the return value as the arg

	switch (expr.GetOp()) {
		case TokenType::PLUS: {
			switch (type) {
				case ValueType::STRING:
					return Value(left->AsValue().GetString() + right->AsValue().GetString()).ToPtr();
				case ValueType::NUMBER:
					return Value(left->AsValue().GetNumber() + right->AsValue().GetNumber()).ToPtr();
				case ValueType::BOOL:
				case ValueType::FUNCTION:
				case ValueType::NIL:
					// You can't really add bools, functions or nils together so
					// we throw (TODO: add some proper error handling)
					throw;
				}
		
			}
		case TokenType::MINUS: {
			if (left->GetType() != ValueType::NUMBER) {
				// We can't really subtract anything but numbers
				throw;
			}
			
			return Value(left->AsValue().GetNumber() + right->AsValue().GetNumber()).ToPtr();
		}
		case TokenType::STAR: {	
			if (type != ValueType::NUMBER) {
				// Same thing as before
				throw;
			}
			
			return Value(left->AsValue().GetNumber() * right->AsValue().GetNumber()).ToPtr();
		}
		case TokenType::SLASH: {
			if (type != ValueType::NUMBER) {
				throw;
			}

			if (right->AsValue().GetNumber() == 0) {
				// Divide by zero error
				throw;
			}
			
			return Value(left->AsValue().GetNumber() / right->AsValue().GetNumber()).ToPtr();
		}
		case TokenType::MOD: {
			if (type != ValueType::NUMBER) {
				throw;
			}

			double leftNum = left->AsValue().GetNumber();
			double rightNum = right->AsValue().GetNumber();
			
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

			Identifier& variable = *(Identifier*)expr.GetLeft().get();

			// Line of code is a little cursed, but `Get()` returns a reference
			// so we can assign it however we want.
			m_symbols->Get(variable.GetId()) = right;

			return right;
		}
		case TokenType::EQUAL_EQUAL: {
			switch (type) {
				case ValueType::NUMBER:
					return Value(left->AsValue().GetNumber() == right->AsValue().GetNumber()).ToPtr();
				case ValueType::STRING:
					return Value(left->AsValue().GetString() == right->AsValue().GetString()).ToPtr();
				case ValueType::BOOL:
					return Value(left->AsValue().GetBoolean() == right->AsValue().GetBoolean()).ToPtr();
				case ValueType::NIL:
					// If the type is `nil` then they both must be `nil`
					return Value(true).ToPtr();
				case ValueType::FUNCTION:
					// An odd case but maybe you would want to compare functions, this is also super inefficient
					// since we are comparing the strings with contains the memory address of the underlying 
					// callable but i do not care
					return Value(left->AsCallable().ToString() == right->AsCallable().ToString()).ToPtr();
			}
		}
		case TokenType::BANG_EQUAL: {
			switch (type) {
				case ValueType::NUMBER:
					return Value(left->AsValue().GetNumber() != right->AsValue().GetNumber()).ToPtr();
				case ValueType::STRING:
					return Value(left->AsValue().GetString() != right->AsValue().GetString()).ToPtr();
				case ValueType::BOOL:
					return Value(left->AsValue().GetBoolean() != right->AsValue().GetBoolean()).ToPtr();
				case ValueType::NIL:
					// If the type is `nil` then they both must be `nil`
					return Value().ToPtr();
				case ValueType::FUNCTION:
					// An odd case but maybe you would want to compare functions ??
					return Value(left->AsValue().GetFunctionId() != right->AsValue().GetFunctionId()).ToPtr();
			}
		}
		case TokenType::GREATER: {
			if (type != ValueType::NUMBER) throw; // I don't believe a string can be greater than a string
			
			return Value(left->AsValue().GetNumber() > right->AsValue().GetNumber()).ToPtr();
		}
		case TokenType::GREATER_EQUAL:
			if (type != ValueType::NUMBER) throw;
			
			return Value(left->AsValue().GetNumber() >= right->AsValue().GetNumber()).ToPtr();
		case TokenType::LESS:
			if (type != ValueType::NUMBER) throw; 
			
			return Value(left->AsValue().GetNumber() < right->AsValue().GetNumber()).ToPtr();
		case TokenType::LESS_EQUAL:
			if (type != ValueType::NUMBER) throw; 
			
			return Value(right->AsValue().GetNumber() <= right->AsValue().GetNumber()).ToPtr();
		case TokenType::AND: {
			// Cannot perform `and` operation on non-boolean values
			if (left->GetType() != ValueType::BOOL || right->GetType() != ValueType::BOOL) throw;

			return Value(left->AsValue().GetBoolean() && left->AsValue().GetBoolean()).ToPtr();
		}
		case TokenType::OR: {
			// Cannot perform `and` operation on non-boolean values
			if (left->GetType() != ValueType::BOOL || right->GetType() != ValueType::BOOL) throw;

			return Value(left->AsValue().GetBoolean() || right->AsValue().GetBoolean()).ToPtr();
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
	// std::cout << "On identifier\n";
	// m_symbols->PrintStack();
	return m_symbols->Get(expr.GetId());
}

TypePtr ExpressionVisitor::visitCallExpr(Call expr) {
	// If we have functions that return functions, we of course
	// evaluate those first, and then we get our callable type.
	// A bit of a recurseive approach, but eventually our
	// `visitIdentifier()` method will catch it and return the
	// first underlying callable which will then be called and returned
	// so we can find the next and so on

	TypePtr callable = expr.GetCallee()->accept(*this);

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