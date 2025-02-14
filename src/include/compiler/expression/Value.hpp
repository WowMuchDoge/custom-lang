#pragma once

#include <string>
#include <variant>
#include <memory>

struct FunctionType {
	std::string name;
	int id;
};

typedef std::variant<double, bool, std::string, FunctionType> ValueVariant; 

enum class ValueType {
    NUMBER,
    BOOL,
    STRING,
	FUNCTION,
    NIL
};


class Value;

// We use this so values and functions can be treated the same
// as first class citizens as it will inherit from this class
class IType {
public:
	virtual std::string ToString() = 0;
	virtual ValueType GetType() = 0;

	virtual Value& AsValue() = 0;
};

typedef std::shared_ptr<IType> TypePtr;

class Value : public IType {
public:
    Value();
	Value(double val);
	Value(bool b);
	Value(std::string str);
	Value(std::string name, int id);

	void SetNumber(double val);
	void SetBoolean(bool b);
	void SetString(std::string str);
	void SetFunction(std::string name, int id);
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

	Value& AsValue() { return *this; }
private:
    ValueType m_type;
    ValueVariant m_value;
};
