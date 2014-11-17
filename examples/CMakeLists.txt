set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Werror -std=c++11 -mwindows -static")

find_package(Boost 1.53.0)
include_directories(${Boost_INCLUDE_DIRS})

include_directories(../include/)

add_executable(collatz collatz.cpp)
add_executable(fizzbuzz fizzbuzz.cpp)
add_executable(look_and_say look_and_say.cpp)
add_executable(sieve_of_eratosthenes sieve_of_eratosthenes.cpp)
add_executable(sort_selected sort_selected.cpp)
