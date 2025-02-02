#include "interpreter/ExpressionVisitor.hpp"

#include <limits>

Value ExpressionVisitor::visitBinaryExpr(Binary expr) {
	Value left = expr.GetLeft()->accept(*this);
	Value right = expr.GetRight()->accept(*this);

	// This ain't javascript, we ain't gonna do operations
	// on values with different types (who the hell would
	// want that)
	if (left.GetType() != right.GetType()) {
		throw;
	}

	ValueType type = left.GetType();

	// Appears to just be returning doubles or bools, but implicit constructor constructs value object
	// with the return value as the arg

	switch (expr.GetOp()) {
		case TokenType::PLUS: {
			switch (type) {
				case ValueType::STRING:
					return left.GetString() + right.GetString();
				case ValueType::NUMBER:
					return left.GetNumber() + right.GetNumber();
				case ValueType::BOOL:
				case ValueType::FUNCTION:
				case ValueType::NIL:
					// You can't really add bools, functions or nils together so
					// we throw (TODO: add some proper error handling)
					throw;
				}
		
			}
		case TokenType::MINUS: {
			if (left.GetType() != ValueType::NUMBER) {
				// We can't really subtract anything but numbers
				throw;
			}
			
			return left.GetNumber() + right.GetNumber();
		}
		case TokenType::STAR: {	
			if (type != ValueType::NUMBER) {
				// Same thing as before
				throw;
			}
			
			return left.GetNumber() * right.GetNumber();
		}
		case TokenType::SLASH: {
			if (type != ValueType::NUMBER) {
				throw;
			}

			if (right.GetNumber() == 0) {
				// Divide by zero error
				throw;
			}
			
			return left.GetNumber() / right.GetNumber();
		}
		case TokenType::MOD: {
			if (type != ValueType::NUMBER) {
				throw;
			}

			int leftNum = left.GetNumber();
			int rightNum = right.GetNumber();
			
			if (isInteger(leftNum) && isInteger(rightNum)) {
				return (int)leftNum % (int)rightNum;
			}
			
			// Means one or both of the operands was not an integer
			throw;
		}
		case TokenType::EQUAL: // Deal with this later since it requires environment info
		case TokenType::EQUAL_EQUAL: {
			switch (type) {
				case ValueType::NUMBER:
					return left.GetNumber() == right.GetNumber();
				case ValueType::STRING:
					return left.GetString() == right.GetString();
				case ValueType::BOOL:
					return left.GetBoolean() == right.GetBoolean();
				case ValueType::NIL:
					// If the type is `nil` then they both must be `nil`
					return true;
				case ValueType::FUNCTION:
					// An odd case but maybe you would want to compare functions ??
					return left.GetFunctionId() == right.GetFunctionId();
			}
		}
		case TokenType::BANG_EQUAL: {
			switch (type) {
				case ValueType::NUMBER:
					return left.GetNumber() != right.GetNumber();
				case ValueType::STRING:
					return left.GetString() != right.GetString();
				case ValueType::BOOL:
					return left.GetBoolean() != right.GetBoolean();
				case ValueType::NIL:
					// If the type is `nil` then they both must be `nil`
					return false;
				case ValueType::FUNCTION:
					// An odd case but maybe you would want to compare functions ??
					return left.GetFunctionId() != right.GetFunctionId();
			}
		}
		case TokenType::GREATER: {
			if (type != ValueType::NUMBER) throw; // I don't believe a string can be greater than a string
			
			return left.GetNumber() > right.GetNumber();
		}
		case TokenType::GREATER_EQUAL:
			if (type != ValueType::NUMBER) throw;
			
			return left.GetNumber() >= right.GetNumber();
		case TokenType::LESS:
			if (type != ValueType::NUMBER) throw; 
			
			return left.GetNumber() >= right.GetNumber();
		case TokenType::LESS_EQUAL:
			if (type != ValueType::NUMBER) throw; 
			
			return left.GetNumber() >= right.GetNumber();
		default:
			// Trying to do a binary operation with an invalid op
			throw;
	}
}

Value ExpressionVisitor::visitUnaryExpr(Unary expr) {
	Value right = expr.GetRight()->accept(*this);
	ValueType type = right.GetType();

	switch (expr.GetOp()) {
		case TokenType::BANG:
			if (type != ValueType::BOOL) throw;

			return !right.GetBoolean();
		case TokenType::MINUS:
			if (type != ValueType::NUMBER) throw;

			return -right.GetNumber();
		default:
			// (hopefully) unreachable
			throw;
	}
}

Value ExpressionVisitor::visitGroupingExpr(Grouping expr) {
	return expr.GetExpr()->accept(*this);
}

Value ExpressionVisitor::visitPrimaryExpr(Primary expr) {
	return expr.GetValue();
}

Value ExpressionVisitor::visitIdentifierExpr(Identifier expr) {
	return m_symbols.Get(expr.GetId());
}

Value ExpressionVisitor::visitCallExpr(Call expr) {
	// Calls are complicated, will do later
	return Value();
}

bool ExpressionVisitor::isInteger(double n) {
	double comparison = (double)((int)n);

	// Goofy ahh double comparison
	return fabs(n - comparison) < std::numeric_limits<double>::epsilon();
}
