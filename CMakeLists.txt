cmake_minimum_required(VERSION 3.27)

project(python_cxx_module C)

find_package(Python3 3.9 REQUIRED COMPONENTS Development.Module)

add_subdirectory(python/iterator)
