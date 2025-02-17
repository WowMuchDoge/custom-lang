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
class Callable;
class IType;

typedef std::shared_ptr<IType> TypePtr;

// We use this so values and functions can be treated the same
// as first class citizens as it will inherit from this class
class IType {
public:
	virtual std::string ToString() = 0;
	virtual ValueType GetType() = 0;

	// I know this is bad OOP, but I really want to avoid a cast
	// since I used to do it and it broke a bunch of stuff. 
	// My sincere apologies. Defaults to throw so each class
	// that implements the interface only has to implement the
	// function for the type it is
	virtual Value& AsValue() {
		throw;
	}

	virtual Callable& AsCallable() {
		throw;
	}

	virtual TypePtr ToPtr() = 0;
};

class Value : public IType {
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
	// If type is number will return the id of the function, otherwise will throw error
	int GetFunctionId();

	// Returns the type of the value
	ValueType GetType();

	std::string ToString();

	Value& AsValue() { return *this; }

	TypePtr ToPtr();
private:
    ValueType m_type;
    ValueVariant m_value;
};
