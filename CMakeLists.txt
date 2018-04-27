cmake_minimum_required(VERSION 3.5)

project(vno-cli)


set(CLI_SOURCES
    ${vno-cli_SOURCE_DIR}/src/main.cpp 
    ${vno-cli_SOURCE_DIR}/src/utils.cpp
    ${vno-cli_SOURCE_DIR}/src/init.cpp
    ${vno-cli_SOURCE_DIR}/src/commit.cpp
    ${vno-cli_SOURCE_DIR}/src/checkout.cpp
    ${vno-cli_SOURCE_DIR}/src/status.cpp 
    ${vno-cli_SOURCE_DIR}/src/log.cpp 
    ${vno-cli_SOURCE_DIR}/src/branch.cpp
	${vno-cli_SOURCE_DIR}/src/rest.cpp
	${vno-cli_SOURCE_DIR}/src/credentials.cpp
	${vno-cli_SOURCE_DIR}/src/base64/cdecode.c
	${vno-cli_SOURCE_DIR}/src/base64/cencode.c
	${vno-cli_SOURCE_DIR}/src/diff.cpp
	${vno-cli_SOURCE_DIR}/src/clone.cpp
)

include_directories("${vno-cli_SOURCE_DIR}/inc")
include_directories("${vno-cli_SOURCE_DIR}/rapidjson/include/rapidjson")

include(FindPkgConfig)
pkg_check_modules(CURLPP REQUIRED curlpp)

set(CMAKE_CXX_STANDARD 11)

add_compile_options(
        -c
        -Wall
        -std=c++11
)

add_executable(vno ${CLI_SOURCES})
target_link_libraries(vno curl curlpp)
