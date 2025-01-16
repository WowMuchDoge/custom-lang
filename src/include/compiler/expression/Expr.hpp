#pragma once

#include "compiler/expression/Value.hpp"

enum class Type {
    BINARY,
    UNARY,
    GROUPING,
    PRIMARY
};

class Expr {
public:
    Value* v;
};