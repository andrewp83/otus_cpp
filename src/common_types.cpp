#include "common_types.h"

HashFunc hash_type_from_string(const std::string& str) {
	HashFunc func{HashFunc::UNKNOWN};
	if (str == "crc32") {
		func = HashFunc::CRC32;
	} else if (str == "md5") {
		func = HashFunc::MD5;
	}
	return func;
}
