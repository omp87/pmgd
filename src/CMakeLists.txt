
cmake_minimum_required (VERSION 3.10)

project(pmgd)
include_directories(../include/)

execute_process(COMMAND bash "-c" ../get_commit_it.sh OUTPUT_VARIABLE COMMIT_ID)
execute_process(COMMAND bash "-c" ../get_pmflush.sh OUTPUT_VARIABLE PMFLUSH)
add_definitions(-DCOMMIT_ID=\"${COMMIT_ID}\")
#add_definitions(-DPMFLUSH=${PMFLUSH} )
#message(STATUS \"${COMMIT_ID}\")
message(STATUS ${PMFLUSH})


add_library(pmgd SHARED Allocator.cc AllocatorUnit.cc AvlTree.cc AvlTreeIndex.cc ChunkAllocator.cc edge.cc EdgeIndex.cc FixedAllocator.cc FixSizeAllocator.cc FlexFixedAllocator.cc graph.cc GraphConfig.cc Index.cc IndexManager.cc IndexString.cc linux.cc node.cc property.cc PropertyList.cc stringid.cc StringTable.cc transaction.cc TransactionManager.cc VariableAllocator.cc)
