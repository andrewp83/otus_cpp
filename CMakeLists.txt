cmake_minimum_required(VERSION 3.2)

project(helloworld VERSION 0.0.1)

add_subdirectory(lib)

add_executable(helloworld_cli main.cpp)

set_target_properties(helloworld_cli PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(helloworld_cli
    PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/lib/includes"
)

target_link_libraries(helloworld_cli PRIVATE
    helloworld
)

if (MSVC)
    target_compile_options(helloworld_cli PRIVATE
        /W4
    )
else ()
    target_compile_options(helloworld_cli PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
