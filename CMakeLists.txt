cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(editor VERSION 0.0.${COMMON_TBN})

set(SOURCES src/application.cpp
            src/controller.cpp
            src/document.cpp
            src/model.cpp
            src/utils.cpp
            src/shapes/shapes.cpp
            src/io/file_io.cpp
            src/io/file_io_factory.cpp
            main.cpp
)

set(HEADERS include/application.h
            include/common_types.h
            include/controller.h
            include/document.h
            include/model.h
            include/utils.h
            include/io/file_io.h
            include/io/file_io_factory.h
            include/shapes/shapes.h)

add_executable(editor ${SOURCES} ${HEADERS})

set_target_properties(editor PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

target_include_directories(editor PRIVATE   "${CMAKE_CURRENT_SOURCE_DIR}/include" 
                                            "${CMAKE_CURRENT_SOURCE_DIR}/include/shapes" 
                                            "${CMAKE_CURRENT_SOURCE_DIR}/include/io"
)

#target_link_libraries(helloworld_cli PRIVATE
#    helloworld
#)

if (MSVC)
    target_compile_options(editor PRIVATE
        /W4
    )
else ()
    target_compile_options(editor PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS editor RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

#enable_testing()

#add_test(test_version lib/test/test_version)
