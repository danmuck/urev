cmake_minimum_required(VERSION 3.25)
project(urev C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_FLAGS "-lncurses -lpanel")

add_executable(urev main.c include/files.h files.c include/structs.h include/generators.h generators.c include/status.h status.c)
