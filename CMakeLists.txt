cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(allocator VERSION 0.0.${COMMON_TBN})

#find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# для тестов
add_subdirectory(memory_manager)


set(SOURCES
    main.cpp 
    memory_manager/src/memory_mgr.cpp
    my_allocator/my_allocator.hpp
    my_list/my_list.hpp)

set(HEADERS
    memory_manager/include/memory_mgr.h)

add_executable(allocator ${SOURCES} ${HEADERS})


set(COMPILE_PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

set_target_properties(allocator PROPERTIES
	${COMPILE_PROPERTIES}
)

target_include_directories(allocator
    PRIVATE 
    "${CMAKE_SOURCE_DIR}/my_allocator" "${CMAKE_SOURCE_DIR}/my_list" "${CMAKE_SOURCE_DIR}/memory_manager/include"
)

######

install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_memory_manager memory_manager/test/test_memory_mgr)
