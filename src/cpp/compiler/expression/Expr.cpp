#include "compiler/expression/Expr.hpp"

#include "Constants.hpp"
#include "interpreter/ExpressionVisitor.hpp"

std::string GetTokenName(TokenType type) {
	auto it = ScannerConstants::kTokenNames.find(type);
	
	if (it == ScannerConstants::kTokenNames.end()) {
		return "UKNOWN OP";
	}

	return it->second;
}

ExprType Binary::GetExprType() {
	return ExprType::BINARY;
}

std::string Binary::ToString() {
	return "[BINARY, LEFT = " + m_left->ToString() + ", RIGHT = "
		+ m_right->ToString() + ", OP = " + GetTokenName(m_op) + "]";
}

TypePtr Binary::accept(ExpressionVisitor& visitor) {
	return visitor.visitBinaryExpr(*this);
}

ExprType Unary::GetExprType() {
	return ExprType::UNARY;
}

std::string Unary::ToString() {
	return "[UNARY, RIGHT = " + m_right->ToString() + ", OP = " + GetTokenName(m_op) + "]";
}

TypePtr Unary::accept(ExpressionVisitor& visitor) {
	return visitor.visitUnaryExpr(*this);
}

ExprType Grouping::GetExprType() {
	return ExprType::GROUPING;
}

std::string Grouping::ToString() {
	return "[GROUPING, EXPR = " + m_expr->ToString() + "]";
}

TypePtr Grouping::accept(ExpressionVisitor& visitor) {
	return visitor.visitGroupingExpr(*this);
}

ExprType Primary::GetExprType() {
	return ExprType::PRIMARY;
}

std::string Primary::ToString() {

	if (m_value->GetType() == ValueType::FUNCTION) {
		// Handle for function types the header file is made
	}
	
	Value value = m_value->AsValue();

	switch (value.GetType()) {
		case ValueType::NUMBER:
			return std::to_string(value.GetNumber());
		case ValueType::STRING:
			return "\"" + value.GetString() + "\"";
		case ValueType::BOOL:
			if (value.GetBoolean())
				return "TRUE";
			else
				return "FALSE";
		case ValueType::NIL:
			return "NIL";
		default:
			return "Unkown Type " + std::to_string((int)value.GetType()) + ".";
	}
}

TypePtr Primary::accept(ExpressionVisitor& visitor) {
	return visitor.visitPrimaryExpr(*this);
}

std::string Identifier::ToString() {
	return "[Identifier, id = " + std::to_string(m_id) + "]";
}

ExprType Identifier::GetExprType() {
	return ExprType::IDENTIFIER;
}

TypePtr Identifier::accept(ExpressionVisitor& visitor) {
	return visitor.visitIdentifierExpr(*this);
}

std::string Call::ToString() {
	std::string output = "[Call, callee " + m_callee->ToString() + ", args = ";

	for (ExprPtr arg : m_args) {
		output += arg->ToString() + " ";
	}

	output += "]";

	return output;
}

ExprType Call::GetExprType() {
	return ExprType::CALL;
}

TypePtr Call::accept(ExpressionVisitor& visitor) {
	return visitor.visitCallExpr(*this);
}
