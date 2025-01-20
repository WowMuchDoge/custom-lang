#pragma once

#include <string>
#include <variant>

typedef std::variant<double, bool, std::string> ValueVariant; 

enum class ValueType {
    NUMBER,
    BOOL,
    STRING,
    NIL
};

class Value {
public:

    Value();
	Value(double val);
	Value(bool b);
	Value(std::string str);

	void SetNumber(double val);
	void SetBoolean(bool b);
	void SetString(std::string str);
	void SetNil();

	// If type is number will return number, otherwise will throw error
	double GetNumber();
	// If type is number will return boolean, otherwise will throw error
	bool GetBoolean();
	// If type is number will return string, otherwise will throw error
	std::string GetString();

	// Returns the type of the value
	ValueType GetType();

private:
    ValueType m_type;
    ValueVariant m_value;
};
