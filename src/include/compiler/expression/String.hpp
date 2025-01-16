#pragma once

#include <cstddef>

class String {
public:
    // String();
    String(const char* str);
    String(String& str);
    String(String&& str);

    ~String();

    String operator+ (const char* other);
    String operator+ (String& other);
    String operator+ (String&& other);

    String operator= (const char* other);
    String operator= (String& other);
    String operator= (String&& other);

    char *GetString() const;
    size_t GetSize();

private:
    char* m_data;
    size_t m_size;
};