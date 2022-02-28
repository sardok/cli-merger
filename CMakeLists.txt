cmake_minimum_required(VERSION 3.10)

project(merger)

add_executable(
    merger 
    src/packet.cpp
    src/parser.cpp
    src/main.cpp
    src/rate_limiter.cpp
    src/message_processor.cpp
)

add_executable(
    tests
    tests/tests.cpp
    tests/rate_limiter_tests.cpp
    src/rate_limiter.cpp
    tests/message_processor_tests.cpp
    src/message_processor.cpp
    src/packet.cpp
)

add_executable(
    gen_input
    src/gen_input.cpp
    src/packet.cpp
)

set_property(TARGET merger PROPERTY CXX_STANDARD 17)
set_property(TARGET tests PROPERTY CXX_STANDARD 17)
set_property(TARGET gen_input PROPERTY CXX_STANDARD 17)