cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
	set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
	set(COMMON_TBN 1)
	message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(Library VERSION 0.0.${COMMON_TBN})

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
