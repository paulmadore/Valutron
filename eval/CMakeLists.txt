cmake_minimum_required (VERSION 2.4)
project (eval)

FlexComp(${PROJECT_SOURCE_DIR}/scan.l)

add_executable (eval eval.c ${PROJECT_BINARY_DIR}/scan.yy.cxx)
