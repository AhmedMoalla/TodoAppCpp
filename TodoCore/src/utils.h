#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <string_view>

inline std::vector<std::string_view> string_split(const std::string_view str, const char delimiter, const bool ignore_string_in_quotes = false) {
    std::vector<std::string_view> result;
    size_t start = 0;
    bool inside_quotes = false;

    for (size_t i = 0; i < str.size(); ++i) {
        char current = str[i];

        if (ignore_string_in_quotes && current == '"') {
            // Toggle the inside_quotes flag
            inside_quotes = !inside_quotes;
        }

        // Split when we encounter the delimiter and we're not inside quotes
        if (current == delimiter && (!ignore_string_in_quotes || !inside_quotes)) {
            if (i > start) {
                result.push_back(str.substr(start, i - start));
            }
            start = i + 1;  // Move past the delimiter
        }
    }

    // Add the last token
    if (start < str.size()) {
        result.push_back(str.substr(start));
    }

    return result;
}

inline std::string string_trim(std::string_view str) {
    auto start = str.begin();
    auto end = str.end();

    // Remove leading spaces
    start = std::find_if_not(start, end, [](const unsigned char ch) {
        return std::isspace(ch);
    });

    // Remove trailing spaces
    end = std::find_if_not(str.rbegin(), str.rend(), [](const unsigned char ch) {
        return std::isspace(ch);
    }).base();

    // Return the trimmed string
    return (start < end ? std::string(start, end) : std::string());
}
