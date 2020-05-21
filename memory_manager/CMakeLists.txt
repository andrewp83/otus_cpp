cmake_minimum_required(VERSION 3.2)

project(TestMemoryMgr VERSION 0.0.1)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)


set(SOURCES
    test/test_memory_mgr.cpp
    src/memory_mgr.cpp)

set(HEADERS
    include/memory_mgr.h)

add_executable(test_memory_mgr ${SOURCES} ${HEADERS})


set_target_properties(test_memory_mgr PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)


set_target_properties(test_memory_mgr PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_include_directories(test_memory_mgr
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(test_memory_mgr
    ${Boost_LIBRARIES}
)

if (MSVC)
    target_compile_options(test_memory_mgr PRIVATE
        /W4
    )
else ()
    target_compile_options(test_memory_mgr PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
