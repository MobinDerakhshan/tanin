cmake_minimum_required(VERSION 3.16)
project(pish_man_in_the_middle)

set(CMAKE_CXX_STANDARD 17)

add_executable(pish_man_in_the_middle main.cpp
        man.cpp
        man.h
)

find_package(PcapPlusPlus REQUIRED)

#set_target_properties("${PROJECT_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries("${PROJECT_NAME}" PUBLIC PcapPlusPlus::Pcap++)