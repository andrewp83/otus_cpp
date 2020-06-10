#pragma once

#include <string>

enum class HashFunc {
	UNKNOWN,
	CRC32,
	MD5
};

HashFunc hash_type_from_string(const std::string& str);
