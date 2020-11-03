cmake_minimum_required(VERSION 3.1)

set(CMAKE_C_STANDARD 11)

project(l8test)

# Patch issue on some computers (fPIE missing on l8w8jwt when building static library)
execute_process (
    COMMAND bash -c "grep -xF 'set_property(TARGET \${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)' ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/CMakeLists.txt || echo 'set_property(TARGET \${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)' >> ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/CMakeLists.txt"
)

# Build static library
execute_process (
    COMMAND bash -c "sed '/cmake -DBUILD_SHARED_LIBS/','/On/s/On/Off/' ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build.sh > ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build_static.sh"
)

# Own the file 
execute_process (
    COMMAND bash -c "chmod +x ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build_static.sh"
)
execute_process (
    COMMAND bash -c "chown $USER ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build_static.sh"
)

# First run cmake in submodule
execute_process (
    COMMAND bash -c "${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build_static.sh"    
)

add_subdirectory(l8w8jwt)
add_compile_options(-fPIC)
find_library(L8W8JWT_LIB libl8w8jwt.a PATHS ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/l8w8jwt/bin/release)
find_library(LMBEDTLS_LIB libmbedtls.a PATHS ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library)
find_library(LMBEDCRYPTO_LIB libmbedcrypto.a PATHS ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library)
find_library(LMBEDX509_LIB libmbedx509.a PATHS ${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library)

# Basic encode - decode
add_executable(l8test_encode ${CMAKE_CURRENT_LIST_DIR}/encode.c)
add_executable(l8test_decode ${CMAKE_CURRENT_LIST_DIR}/decode.c)
set_property(TARGET l8test_decode PROPERTY POSITION_INDEPENDENT_CODE ON)

link_directories(${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/l8w8jwt/bin/release)
link_directories(${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library)

target_link_libraries(l8test_decode PRIVATE "${L8W8JWT_LIB}" "${LMBEDTLS_LIB}" "${LMBEDCRYPTO_LIB}" "${LMBEDX509_LIB}")
target_link_libraries(l8test_encode PRIVATE "${L8W8JWT_LIB}" "${LMBEDTLS_LIB}" "${LMBEDCRYPTO_LIB}" "${LMBEDX509_LIB}")

# Install
set(tool_dest "bin")
set(main_lib_dest "lib")
set(my_libraries "${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/l8w8jwt/bin/release/libl8w8jwt.a" "${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library/libmbedtls.a" "${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library/libmbedcrypto.a" "${CMAKE_CURRENT_LIST_DIR}/l8w8jwt/build/mbedtls/library/libl8w8jwt.a")
install(FILES ${my_libraries} DESTINATION "${main_lib_dest}")
