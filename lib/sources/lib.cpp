#include "lib.h"

#include <iostream>

#include "version.h"

int versionMajor() {
    return PROJECT_VERSION_MAJOR;
}

int versionMinor() {
    return PROJECT_VERSION_MINOR;
}

int versionPatch() {
    return PROJECT_VERSION_PATCH;
}

void printHello() {
	std::cout << "Hello, World!" << std::endl;
}