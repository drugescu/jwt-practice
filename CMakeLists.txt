cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 11)

project(l8test)

# Basic encode - decode
add_executable(l8test_encode ${CMAKE_CURRENT_LIST_DIR}/encode.c)
add_executable(l8test_decode ${CMAKE_CURRENT_LIST_DIR}/decode.c)
#add_library(libl8w8jwt)
# UDP basic send-receive

link_directories(${CMAKE_CURRENT_LIST_DIR}/lib)
	
target_link_libraries(l8test_encode ${CMAKE_SOURCE_DIR}/lib/libl8w8jwt.a ${CMAKE_SOURCE_DIR}/lib/libmbedtls.a ${CMAKE_SOURCE_DIR}/lib/libmbedcrypto.a ${CMAKE_SOURCE_DIR}/lib/libmbedx509.a)
target_link_libraries(l8test_decode ${CMAKE_SOURCE_DIR}/lib/libl8w8jwt.a ${CMAKE_SOURCE_DIR}/lib/libmbedtls.a ${CMAKE_SOURCE_DIR}/lib/libmbedcrypto.a ${CMAKE_SOURCE_DIR}/lib/libmbedx509.a)

# Install
set(tool_dest "bin")
set(main_lib_dest "lib")
set(my_libraries "${CMAKE_SOURCE_DIR}/lib/libcmocka.so"  "${CMAKE_SOURCE_DIR}/lib/libcmocka.so.0"  "${CMAKE_SOURCE_DIR}/lib/libcmocka.so.0.7.0" "${CMAKE_SOURCE_DIR}/lib/libl8w8jwt.a" "${CMAKE_SOURCE_DIR}/lib/libmbedtls.a" "${CMAKE_SOURCE_DIR}/lib/libmbedcrypto.a" "${CMAKE_SOURCE_DIR}/lib/libl8w8jwt.a")
install(FILES ${my_libraries} DESTINATION "${main_lib_dest}")
