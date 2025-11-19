cmake_minimum_required(VERSION 3.27)
project(Visualizer)

if (MSVC)
    add_compile_options(/W4)
    add_compile_options(/permissive-)
    add_compile_options(/EHsc)
endif()