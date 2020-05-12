cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(ip_filter VERSION 0.0.${COMMON_TBN})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

set(COMPILE_PROPERTIES
	CXX_STANDARD 14
   	CXX_STANDARD_REQUIRED ON
)

set(SOURCES main.cpp ip_filter.cpp)
set(HEADERS ip_filter.h)
add_executable(ip_filter ${SOURCES} ${HEADERS})

set_target_properties(ip_filter PROPERTIES
	${COMPILE_PROPERTIES}
)

# test_filter

set(SOURCES_TEST test_filter.cpp ip_filter.cpp)
set(HEADERS_TEST ip_filter.h)
add_executable(test_filter ${SOURCES_TEST} ${HEADERS_TEST})

set_target_properties(test_filter PROPERTIES
	${COMPILE_PROPERTIES}
)

set_target_properties(test_filter PROPERTIES
    COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
    INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
)

target_link_libraries(test_filter
    ${Boost_LIBRARIES}
)

######

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_filter test_filter)
