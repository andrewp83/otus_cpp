cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(bulk VERSION 0.0.${COMMON_TBN})

set(SOURCES src/application.cpp
            src/command.cpp
            src/command_observer.cpp
            src/executor.cpp
            src/executor_state.cpp
            src/file_writer.cpp
            src/console_printer.cpp
)

set(HEADERS  include/application.h
             include/command.h
             include/command_observer.h
             include/command_publisher.hpp
             include/executor.h
             include/executor_state.h
             include/file_writer.h
             include/console_printer.h
             include/common_types.h
)

find_package(GTest REQUIRED)
find_package(Threads)

add_executable(bulk ${SOURCES} ${HEADERS} main.cpp)

add_executable(test_bulk ${SOURCES}
                         ${HEADERS}
                         test/test_executor.h
                         test/test_observer.h
                         test/test_main.cpp
)

set_target_properties(bulk test_bulk PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(bulk PRIVATE src/ include/)

target_include_directories(test_bulk PRIVATE ${GTEST_INCLUDE_DIRS} test/ include/)

target_link_libraries(test_bulk PRIVATE ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
    target_compile_options(bulk PRIVATE
        /W4
    )
    target_compile_options(test_bulk PRIVATE
        /W4
    )    
else ()
    target_compile_options(bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_bulk PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS bulk RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(test_matrix test_bulk)
