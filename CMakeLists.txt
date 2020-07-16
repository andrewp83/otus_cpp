cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(bulk_server VERSION 0.0.${COMMON_TBN})

set(SOURCES main.cpp
            src/command.cpp
            src/command_observer.cpp
            src/executor.cpp
            src/executor_state.cpp
            src/file_writer.cpp
            src/console_printer.cpp
            src/thread_worker.cpp
            src/event.cpp
)

set(HEADERS include/server.hpp
            include/session.hpp
            include/command.h
            include/command_observer.h
            include/command_publisher.hpp
            include/event.h
            include/executor.h
            include/executor_state.h
            include/file_writer.h
            include/console_printer.h
            include/common_types.h
            include/thread_worker.h
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(bulk_server ${SOURCES} ${HEADERS})

#add_executable(test_async   ${SOURCES}
#                            ${HEADERS}
#                            test/test_async.h
#                            test/test_main.cpp
#)

target_include_directories(bulk_server PRIVATE ./ src/ include/)
target_link_libraries(bulk_server PRIVATE ${CMAKE_THREAD_LIBS_INIT})

#target_include_directories(test_async PRIVATE ${GTEST_INCLUDE_DIRS} ./ test/ include/)
#target_link_libraries(test_async PRIVATE ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
    target_compile_options(bulk_server PRIVATE
        /W4
    )
    target_compile_options(test_async PRIVATE
        /W4
    )    
else ()
    target_compile_options(bulk_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
#    target_compile_options(test_async PRIVATE
#        -Wall -Wextra -pedantic -Werror
#    )
endif()

install(TARGETS bulk_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

#enable_testing()
#add_test(test_async test_async)
