cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(kkmeans VERSION 0.0.${COMMON_TBN})

#set(SOURCES src/mail_mapper.cpp)

#set(HEADERS include/mapper.h)


find_package(dlib REQUIRED)
set(DLIB dlib::dlib)
include_directories(${dlib_INCLUDE_DIRS})


set(CMAKE_CXX_STANDARD 17)

add_executable(kkmeans main.cpp)

#target_include_directories(kkmeans PRIVATE ./ src/ include/)
target_link_libraries(kkmeans PRIVATE ${DLIB})

if (MSVC)
    target_compile_options(kkmeans PRIVATE
        /W4
    )
else ()
    target_compile_options(kkmeans PRIVATE
        -Wall -Wextra -pedantic
    )
endif()

install(TARGETS kkmeans RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

