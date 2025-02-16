#include "compiler/expression/Value.hpp"

Value::Value() {
	m_type = ValueType::NIL;
}

Value::Value(double val) {
	m_type = ValueType::NUMBER;
	m_value = val;
}

Value::Value(bool b) {
	m_type = ValueType::BOOL;
	m_value = b;
}

Value::Value(std::string str) {
	m_type = ValueType::STRING;
	m_value = str;
}

void Value::SetNumber(double val) {
	m_type = ValueType::NUMBER;
	m_value = val;
}


void Value::SetBoolean(bool b) {
	m_type = ValueType::BOOL;
	m_value = b;
}


void Value::SetString(std::string str) {
	m_type = ValueType::STRING;
	m_value = str;
}

void Value::SetNil() {
	m_type = ValueType::NIL;
}

ValueType Value::GetType() {
	return m_type;
}

double Value::GetNumber() {
	return std::get<double>(m_value);
}

bool Value::GetBoolean() {
	return std::get<bool>(m_value);
}

std::string Value::GetString() {
	return std::get<std::string>(m_value);
}

int Value::GetFunctionId() {
	return std::get<FunctionType>(m_value).id;
}

std::string Value::ToString() {
	switch (m_type) {
		case ValueType::NUMBER:
			return std::to_string(std::get<double>(m_value));
		case ValueType::STRING:
			return std::get<std::string>(m_value);
		case ValueType::BOOL:{
			bool val = std::get<bool>(m_value);

			return val ? "true" : "false";
		}
		case ValueType::NIL:
			return "nil";
		case ValueType::FUNCTION:
			return "<fn " + std::get<FunctionType>(m_value).name + ", id " + std::to_string(std::get<FunctionType>(m_value).id) + ">";
		default:
			return "";
	}
}

TypePtr Value::ToPtr() {
	return TypePtr(new Value(*this));
}