cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(task_mgr VERSION 0.0.${COMMON_TBN})


find_package(GTest REQUIRED)
#find_package(Threads REQUIRED)
find_package(Boost COMPONENTS graph REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

set(SOURCES src/task.cpp
            src/job.cpp
            src/job_manager.cpp
            src/thread_pool.cpp
            src/thread_worker.cpp)

set(HEADERS include/task.h
            include/exception.hpp
            include/job_manager.h
            include/job.h
            include/thread_pool.h
            include/thread_worker.h)

set(CMAKE_CXX_STANDARD 17)

add_library(task_mgr SHARED ${SOURCES} ${HEADERS})

set_target_properties(task_mgr PROPERTIES LINKER_LANGUAGE CXX)

add_executable(test_task_mgr ${SOURCES} 
							 ${HEADERS}
							 test/test_task_mgr.hpp
                             test/test_job_mgr.hpp
							 test/test_main.cpp)

target_include_directories(task_mgr PRIVATE . src/ include/)
target_link_libraries(task_mgr PRIVATE ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

target_include_directories(test_task_mgr PRIVATE ${GTEST_INCLUDE_DIRS} ./ test/ include/)
target_link_libraries(test_task_mgr PRIVATE ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

if (MSVC)
    target_compile_options(task_mgr PRIVATE
        /W4
    )
    target_compile_options(test_task_mgr PRIVATE
        /W4
    )    
else ()
    target_compile_options(task_mgr PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_task_mgr PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS task_mgr LIBRARY DESTINATION lib)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(test_task_mgr test_task_mgr)
