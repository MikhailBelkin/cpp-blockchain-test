# напишите CMakeLists.txt для сборки трёх программ
# он должен иметь три цели: hmirror, vmirror и sobel

cmake_minimum_required(VERSION 3.11)

project(Block_chain CXX)

find_package(OpenSSL REQUIRED)


set(CMAKE_CXX_STANDARD 17)


set(SYSTEM_LIBS)

add_executable(block_chain  ${SYSTEM_LIBS} Blockchain_test.cpp) 


target_link_libraries(block_chain PRIVATE OpenSSL::SSL OpenSSL::Crypto)



