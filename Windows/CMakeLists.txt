cmake_minimum_required (VERSION 3.0)

set(Windows-Targets
    "Console.hpp" "Console.cpp"
)

add_library(Windows STATIC ${Windows-Targets})
