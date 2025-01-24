#include "compiler/expression/Expr.hpp"

#include "Constants.hpp"

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

ExprType Unary::GetExprType() {
	return ExprType::UNARY;
}

std::string Unary::ToString() {
	return "[UNARY, RIGHT = " + m_right->ToString() + ", OP = " + GetTokenName(m_op) + "]";
}

ExprType Grouping::GetExprType() {
	return ExprType::GROUPING;
}

std::string Grouping::ToString() {
	return "[GROUPING, EXPR = " + m_expr->ToString() + "]";
}

ExprType Primary::GetExprType() {
	return ExprType::PRIMARY;
}

std::string Primary::ToString() {
	switch (m_value.GetType()) {
		case ValueType::NUMBER:
			return std::to_string(m_value.GetNumber());
		case ValueType::STRING:
			return "\"" + m_value.GetString() + "\"";
		case ValueType::BOOL:
			if (m_value.GetBoolean())
				return "TRUE";
			else
				return "FALSE";
		case ValueType::NIL:
			return "NIL";
		default:
			return "Unkown Type " + std::to_string((int)m_value.GetType()) + ".";
	}
}

std::string Identifier::ToString() {
	return "[Identifier, id = " + std::to_string(m_id) + "]";
}

ExprType Identifier::GetExprType() {
	return ExprType::IDENTIFIER;
}
