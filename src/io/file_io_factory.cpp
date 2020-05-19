#include "file_io_factory.h"

FileIOPtr FileIOFactory::create(SaveDocumentFormat format) {
	switch (format) {
		case SaveDocumentFormat::PNG:
			return std::make_shared<FileIOPng>();
		case SaveDocumentFormat::JPEG:
			return std::make_shared<FileIOJpeg>();
	};
	return nullptr;
}