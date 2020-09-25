cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(realty VERSION 0.0.${COMMON_TBN})


find_package(dlib REQUIRED)
set(DLIB dlib::dlib)
include_directories(${dlib_INCLUDE_DIRS})


set(CMAKE_CXX_STANDARD 14)

# rclst

add_executable(rclst    src/rclst.cpp
                        src/k_means.cpp 
                        include/k_means.h
                        include/flat.h)

target_include_directories(rclst PRIVATE include)
target_link_libraries(rclst PRIVATE ${DLIB})

#rclss

add_executable(rclss    src/rclss.cpp
                        src/k_means.cpp 
                        include/k_means.h
                        include/flat.h)

target_include_directories(rclss PRIVATE include)
target_link_libraries(rclss PRIVATE ${DLIB})

if (MSVC)
    target_compile_options(rclst PRIVATE
        /W4
    )
    target_compile_options(rclss PRIVATE
        /W4
    )    
else ()
    target_compile_options(rclst PRIVATE
        -Wall -Wextra -pedantic
    )
    target_compile_options(rclss PRIVATE
        -Wall -Wextra -pedantic
    )    
endif()

install(TARGETS rclst RUNTIME DESTINATION bin)
install(TARGETS rclss RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)
