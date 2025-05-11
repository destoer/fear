#set(CMAKE_UNITY_BUILD ON)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

if(WIN32)

else()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Werror -fomit-frame-pointer -Wall -Wextra -O0")

endif()

# build bar
add_library(fear "fear.c" "platform_linux.c")
target_compile_definitions(fear PUBLIC "-DFEAR_LOGGING")
target_include_directories(fear PUBLIC "include")

add_executable(fear_test "test.c")
target_link_libraries(fear_test fear)