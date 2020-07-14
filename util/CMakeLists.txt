
cmake_minimum_required (VERSION 3.10)
project(pmgd-util)
include_directories(../include/)
add_library(pmgd-util SHARED dump_debug.cc dump_gexf.cc dump_pmgd.cc exception.cc loader.cc load_gson.cc load_tsv.cc neighbor.cc scanner.cc text.cc)
