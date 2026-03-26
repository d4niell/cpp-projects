cmake_minimum_required(VERSION 3.16)
project(projekti LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(projekti main.cpp)
add_executable(rps rps.cpp)
add_executable(numGuesser NumGuesser.cpp)
add_executable(Expenses Expenses.cpp)
add_executable(Localhost Localhost.cpp)