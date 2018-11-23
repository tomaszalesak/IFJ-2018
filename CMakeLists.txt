cmake_minimum_required(VERSION 3.12)
project(IFJ_2018_MINDFCUK C)
set(CMAKE_C_STANDARD 11)
include_directories(.)
add_executable(IFJ_2018_MINDFCUK
        lexicalanalyzer.c
        parser.c
        string.c
        tester.c
        prec_anal.c
        symtable.c
        prec_dl.c
        errors.c)