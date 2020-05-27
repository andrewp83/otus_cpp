#include "file_io_factory.h"

std::unique_ptr<FileIO> FileIOFactory::create(SaveDocumentFormat format) {
	switch (format) {
		case SaveDocumentFormat::PNG:
			return std::make_unique<FileIOPng>();
		case SaveDocumentFormat::JPEG:
			return std::make_unique<FileIOJpeg>();
        case SaveDocumentFormat::UNKNOWN:
            return nullptr;
	};
	return nullptr;
}
