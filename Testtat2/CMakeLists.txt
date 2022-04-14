cmake_minimum_required(VERSION 3.16)

project(milestone2)

add_subdirectory(src)


set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)