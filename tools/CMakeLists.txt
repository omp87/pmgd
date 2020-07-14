cmake_minimum_required (VERSION 3.10)

project(pmgd-tools)
include_directories(../include/ ../util/)
add_executable(dumpgraph dumpgraph.cc)
target_link_libraries(dumpgraph pmgd pmgd-util jsoncpp)
add_executable(loadgraph loadgraph.cc)
target_link_libraries(loadgraph pmgd pmgd-util jsoncpp)
add_executable(mkgraph mkgraph.cc)
target_link_libraries(mkgraph pmgd pmgd-util jsoncpp)


