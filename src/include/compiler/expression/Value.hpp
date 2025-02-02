#pragma once

#include <string>
#include <variant>

class test;

// The int type is for IDing functions since the declarations are stored on an array
typedef std::variant<double, bool, std::string, int> ValueVariant; 

enum class ValueType {
    NUMBER,
    BOOL,
    STRING,
	FUNCTION,
    NIL
};

class Value {
public:
    Value();
	Value(double val);
	Value(bool b);
	Value(std::string str);
	Value(int id);

	void SetNumber(double val);
	void SetBoolean(bool b);
	void SetString(std::string str);
	void SetFunction(int id);
	void SetNil();

	// If type is number will return number, otherwise will throw error
	double GetNumber();
	// If type is number will return boolean, otherwise will throw error
	bool GetBoolean();
	// If type is number will return string, otherwise will throw error
	std::string GetString();
	// If type is number will return the id of the function, otherwise will throw error
	int GetFunctionId();

	// Returns the type of the value
	ValueType GetType();

	std::string ToString();
private:
    ValueType m_type;
    ValueVariant m_value;
};
