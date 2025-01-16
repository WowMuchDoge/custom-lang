#include "compiler/expression/Value.hpp"

#include <cstdlib>

Value::Value() {
    m_type = ValueType::NIL;
}

Value::Value(double v) {
    m_type = ValueType::NUMBER;
    m_value.number = v;
}

Value::Value(bool b) {
    m_type = ValueType::BOOL;
    m_value.boolean = b;
}

// Value::Value(std::string s) {
//     m_type = ValueType::STRING;
//     m_value.string = s;
// }