#include "compiler/expression/String.hpp"

#include <cstring>
#include <cstdlib>
#include <iostream>

// String::String() {
//     m_data = nullptr;
//     m_size = 0;
// }

String::String(const char* str) {
    if (str != nullptr) {
        m_size = strlen(str) + 1; // strlen() does not include null terminator

        m_data = (char*)calloc(m_size, sizeof(char));

        memcpy(m_data, str, m_size);
    } else {
        m_data = nullptr;
        m_size = 0;
    }
}

String::String(String& str) {
    if (str.m_data != nullptr) {
        m_size = str.m_size;
        m_data = (char*)calloc(str.m_size, sizeof(char));

        memcpy(m_data, str.m_data, m_size);
    }
}

String::String(String&& str) {
    m_data = str.m_data;
    m_size = str.m_size;

    str.m_data = nullptr;
}

String::~String() {
    free(m_data);
    m_size = 0;
}

String String::operator+ (const char* other) {
    String newString(nullptr);

    newString.m_size = m_size + strlen(other);
    newString.m_data = (char*)calloc(newString.m_size, sizeof(char));

    strcat(newString.m_data, m_data);
    strcat(newString.m_data, other);

    return newString;
}

String String::operator+ (String& other) {
    String newString(nullptr);

    newString.m_size = m_size + other.m_size - 1;
    newString.m_data = (char*)calloc(newString.m_size, sizeof(char));

    strcat(newString.m_data, m_data);
    strcat(newString.m_data, other.m_data);

    return newString;
}

String String::operator+ (String&& other) {
    String newString(nullptr);

    newString.m_size = m_size + other.m_size - 1;
    newString.m_data = (char*)calloc(newString.m_size, sizeof(char));

    strcat(newString.m_data, m_data);
    strcat(newString.m_data, other.m_data);

    return newString;
}

String String::operator= (const char* other) {
    return String(other);
}

String String::operator= (String& other) {
    if (other.m_data == m_data)
        return *this;

    return String(other);
}

String String::operator= (String&& other) {

    if (other.m_data != m_data) {
        m_data = other.m_data;
        m_size = other.m_size;

        other.m_data = nullptr;
    }

    return *this;
}

char *String::GetString() const {
    return m_data;
}

size_t String::GetSize() {
    return m_size;
}