cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(bayan VERSION 0.0.${COMMON_TBN})

find_package(Boost COMPONENTS program_options filesystem regex REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(GTest REQUIRED)
find_package(Threads)

set(SOURCES src/finder.cpp
            src/common_types.cpp
            src/file_data.cpp
            src/hash_value.cpp
            src/filter.cpp
)

set(HEADERS include/finder.h
            include/common_types.h
            include/file_data.h
            include/hash_value.h
            include/filter.h
            include/md5.hpp
)

add_executable(bayan ${SOURCES} ${HEADERS} main.cpp)

add_executable(test_bayan ${SOURCES}
                          ${HEADERS}
                          test/test_main.cpp
)

set_target_properties(bayan test_bayan PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bayan PRIVATE src/ include/)
target_include_directories(test_bayan PRIVATE ${GTEST_INCLUDE_DIRS} test/ include/)

target_link_libraries(bayan PRIVATE ${Boost_LIBRARIES})
target_link_libraries(test_bayan PRIVATE ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

if (MSVC)
    target_compile_options(bayan PRIVATE
        /W4
    )
    target_compile_options(test_bayan PRIVATE
        /W4
    )    
else ()
    target_compile_options(bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_bayan PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

file(COPY ${CMAKE_SOURCE_DIR}/test/data DESTINATION ${CMAKE_BINARY_DIR} )

install(TARGETS bayan RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(test_bayan test_bayan)
