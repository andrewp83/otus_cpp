cmake_minimum_required(VERSION 3.2)

if(DEFINED ENV{TRAVIS_BUILD_NUMBER})
    set(COMMON_TBN $ENV{TRAVIS_BUILD_NUMBER})
else()
    set(COMMON_TBN 1)
    message(WARNING "TRAVIS_BUILD_NUMBER env variable is undefined! It is set to 1.")
endif()

project(timus VERSION 0.0.${COMMON_TBN})

add_executable(timus_1196 timus_1196.cpp)
add_executable(timus_1496 timus_1496.cpp)
add_executable(timus_1545 timus_1545.cpp)
add_executable(timus_1837 timus_1837.cpp)

set_target_properties(timus_1196 timus_1496 timus_1545 timus_1837 PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(timus_1196 PRIVATE
        /W4
    )
    target_compile_options(timus_1496 PRIVATE
        /W4
    )
    target_compile_options(timus_1545 PRIVATE
        /W4
    )
    target_compile_options(timus_1837 PRIVATE
        /W4
    )

else ()
    target_compile_options(timus_1196 PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(timus_1496 PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(timus_1545 PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(timus_1837 PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
