#pragma once
#include <string>

namespace nc
{
	std::string string_tolower(const std::string& str);
	bool istring_compare(const std::string& str1, const std::string& str2);
	std::string unique_string(const std::string& str);
}