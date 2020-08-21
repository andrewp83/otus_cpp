cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(yamr VERSION 0.0.${COMMON_TBN})

set(SOURCES src/mail_mapper.cpp
            src/mail_reducer.cpp
            src/job.cpp
)

set(HEADERS include/mapper.h
            include/mail_mapper.h
            include/reducer.h
            include/mail_reducer.h
            include/job.h
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
#find_package(Boost REQUIRED system)

#include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)

add_executable(yamr ${SOURCES} ${HEADERS} main.cpp)

add_executable(test_yamr ${SOURCES} ${HEADERS} test/test_yamr.hpp test/test_main.cpp)

target_include_directories(yamr PRIVATE ./ src/ include/)
target_link_libraries(yamr PRIVATE ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(test_yamr PRIVATE ${GTEST_INCLUDE_DIRS} test/ include/)
target_link_libraries(test_yamr PRIVATE ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
    target_compile_options(yamr PRIVATE
        /W4
    )
    target_compile_options(test_yamr PRIVATE
        /W4
    )
else ()
    target_compile_options(yamr PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_yamr PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS yamr RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(test_yamr test_yamr)
