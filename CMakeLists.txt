cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(matrix VERSION 0.0.${COMMON_TBN})

set(SOURCES src/matrix.cpp
            main.cpp
)

add_executable(matrix ${SOURCES})

set_target_properties(matrix PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

#target_include_directories(matrix PRIVATE   "${CMAKE_CURRENT_SOURCE_DIR}/include" 
#                                            "${CMAKE_CURRENT_SOURCE_DIR}/include/shapes" 
#                                            "${CMAKE_CURRENT_SOURCE_DIR}/include/io"
#)

#target_link_libraries(helloworld_cli PRIVATE
#    helloworld
#)

if (MSVC)
    target_compile_options(matrix PRIVATE
        /W4
    )
else ()
    target_compile_options(matrix PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS matrix RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

#enable_testing()

#add_test(test_version lib/test/test_version)
