cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(join_server VERSION 0.0.${COMMON_TBN})

set(SOURCES main.cpp
            src/database.cpp
            src/database_sql.cpp
)

set(HEADERS include/server.hpp
            include/session.hpp
            include/publisher.hpp
            include/database.h
            include/database_sql.h
)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(Boost REQUIRED system)

include_directories(${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

add_executable(join_server ${SOURCES} ${HEADERS})

target_include_directories(join_server PRIVATE ./ src/ include/)
target_link_libraries(join_server PRIVATE ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
    target_compile_options(join_server PRIVATE
        /W4
    )
else ()
    target_compile_options(join_server PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS join_server RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
