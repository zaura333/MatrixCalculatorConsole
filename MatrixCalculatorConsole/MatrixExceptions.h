#pragma once
#include <stdexcept>

class Matrix_size_not_match : public std::logic_error {
public:
	explicit Matrix_size_not_match(const std::string& msg) : std::logic_error(msg) {}
};


