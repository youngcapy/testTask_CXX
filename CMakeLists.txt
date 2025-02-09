cmake_minimum_required(VERSION 3.10)

# set the project name
project(socket_messager)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include_directories(MessageGetter)
include_directories(MessageSender)

# add the executable
add_executable(sender
MessageSender/sources/main.cpp
MessageSender/sources/stringops.cpp
MessageSender/sources/receiverops.cpp
MessageSender/headers/receiverops.h
MessageSender/headers/stringops.h
)

add_executable(getter
MessageGetter/main.cpp
)