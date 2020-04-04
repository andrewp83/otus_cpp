cmake_minimum_required(VERSION 3.2)

project(Library VERSION 0.0.1$ENV{TRAVIS_BUILD_NUMBER})

add_subdirectory(test)

configure_file(includes/version.h.in version.h)

add_library(helloworld "${CMAKE_CURRENT_SOURCE_DIR}/sources/lib.cpp")

set_target_properties(helloworld PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

message("lib:   ${CMAKE_CURRENT_SOURCE_DIR}/includes")

target_include_directories(helloworld
    PUBLIC "${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/includes"
)

if (MSVC)
    target_compile_options(helloworld PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
