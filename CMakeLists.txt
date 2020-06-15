project(get_file)
cmake_minimum_required(VERSION 3.0)

set(INC_DIR include)
set(SRC_DIR src)

include_directories(${INC_DIR})

add_executable(${PROJECT_NAME} 
    ${SRC_DIR}/main.c
    ${SRC_DIR}/http_parse_header.c
    ${SRC_DIR}/http_connect.c
    ${SRC_DIR}/aux_func.c)

target_compile_options(${PROJECT_NAME} PUBLIC -O0 -g)
