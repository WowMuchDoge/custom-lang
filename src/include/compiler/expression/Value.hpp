#pragma once

#include <string>
#include <variant>

enum class ValueType {
    NUMBER,
    BOOL,
    STRING,
    NIL
};

typedef union {
    double number;
    bool boolean;
} ValueLiteral;

class Value {
public:
    Value();

    Value(double v);
    Value(bool b);
    //Value(std::string s);

private:
    ValueType m_type;
    ValueLiteral m_value;
};