cmake_minimum_required (VERSION 3.10)
project(pmgd-tests)
include_directories(../include/ ../util/)

add_executable(aborttest aborttest.cc)
target_link_libraries(aborttest pmgd pmgd-util jsoncpp)

add_executable(allocaborttest allocaborttest.cc)
target_link_libraries(allocaborttest pmgd pmgd-util jsoncpp)

add_executable(alloctest alloctest.cc)
target_link_libraries(alloctest pmgd pmgd-util jsoncpp)

add_executable(avltest avltest.cc)
target_link_libraries(avltest pmgd pmgd-util jsoncpp)

add_executable(chunklisttest chunklisttest.cc)
target_link_libraries(chunklisttest pmgd pmgd-util jsoncpp)

add_executable(edgeindextest edgeindextest.cc)
target_link_libraries(edgeindextest pmgd pmgd-util jsoncpp)

add_executable(emailindextest emailindextest.cc)
target_link_libraries(emailindextest pmgd pmgd-util jsoncpp)

add_executable(filtertest filtertest.cc)
target_link_libraries(filtertest pmgd pmgd-util jsoncpp)

add_executable(indexrangetest indexrangetest.cc)
target_link_libraries(indexrangetest pmgd pmgd-util jsoncpp)

add_executable(indexstringtest indexstringtest.cc)
target_link_libraries(indexstringtest pmgd pmgd-util jsoncpp)

add_executable(indextest indextest.cc)
target_link_libraries(indextest pmgd pmgd-util jsoncpp)

add_executable(listtest listtest.cc)
target_link_libraries(listtest pmgd pmgd-util jsoncpp)

add_executable(load_gson_test load_gson_test.cc)
target_link_libraries(load_gson_test pmgd pmgd-util jsoncpp)

add_executable(load_tsv_test load_tsv_test.cc)
target_link_libraries(load_tsv_test pmgd pmgd-util jsoncpp)

add_executable(mtaddfindremovetest mtaddfindremovetest.cc)
target_link_libraries(mtaddfindremovetest pmgd pmgd-util jsoncpp pthread)

add_executable(mtalloctest mtalloctest.cc)
target_link_libraries(mtalloctest pmgd pmgd-util jsoncpp pthread)

add_executable(mtavltest mtavltest.cc)
target_link_libraries(mtavltest pmgd pmgd-util jsoncpp pthread)

add_executable(neighbortest neighbortest.cc)
target_link_libraries(neighbortest pmgd pmgd-util jsoncpp)

add_executable(nodeedgetest nodeedgetest.cc)
target_link_libraries(nodeedgetest pmgd pmgd-util jsoncpp)

add_executable(propertychunktest propertychunktest.cc)
target_link_libraries(propertychunktest pmgd pmgd-util jsoncpp)

add_executable(propertylisttest propertylisttest.cc)
target_link_libraries(propertylisttest pmgd pmgd-util jsoncpp)

add_executable(propertypredicatetest propertypredicatetest.cc)
target_link_libraries(propertypredicatetest pmgd pmgd-util jsoncpp)

add_executable(propertytest propertytest.cc)
target_link_libraries(propertytest pmgd pmgd-util jsoncpp)

add_executable(removetest removetest.cc)
target_link_libraries(removetest pmgd pmgd-util jsoncpp)

add_executable(reverseindexrangetest reverseindexrangetest.cc)
target_link_libraries(reverseindexrangetest pmgd pmgd-util jsoncpp)

add_executable(rotest rotest.cc)
target_link_libraries(rotest pmgd pmgd-util jsoncpp)

add_executable(setproperty setproperty.cc)
target_link_libraries(setproperty pmgd pmgd-util jsoncpp)

add_executable(soltest soltest.cc)
target_link_libraries(soltest pmgd pmgd-util jsoncpp)

add_executable(statsallocatortest statsallocatortest.cc)
target_link_libraries(statsallocatortest pmgd pmgd-util jsoncpp)

add_executable(statsindextest statsindextest.cc)
target_link_libraries(statsindextest pmgd pmgd-util jsoncpp)

add_executable(stringtabletest stringtabletest.cc)
target_link_libraries(stringtabletest pmgd pmgd-util jsoncpp)

add_executable(stripelocktest stripelocktest.cc)
target_link_libraries(stripelocktest pmgd pmgd-util jsoncpp pthread)

add_executable(test720 test720.cc)
target_link_libraries(test720 pmgd pmgd-util jsoncpp)

add_executable(test750 test750.cc)
target_link_libraries(test750 pmgd pmgd-util jsoncpp)

add_executable(test767 test767.cc)
target_link_libraries(test767 pmgd pmgd-util jsoncpp)

add_executable(txtest txtest.cc)
target_link_libraries(txtest pmgd pmgd-util jsoncpp)
