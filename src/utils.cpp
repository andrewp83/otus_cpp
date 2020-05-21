#include "utils.h"
#include <string>

namespace utils {
	
SaveDocumentFormat get_file_format(const std::string& /*name*/) {
	return SaveDocumentFormat::PNG;
}

}
