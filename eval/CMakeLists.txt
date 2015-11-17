cmake_minimum_required (VERSION 2.4)
project (eval)

FlexComp(${PROJECT_SOURCE_DIR}/eval.l)
BtYaccComp(${PROJECT_SOURCE_DIR}/eval.y)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable (eval eval.cxx
                ${PROJECT_BINARY_DIR}/eval.yy.cxx
                ${PROJECT_BINARY_DIR}/eval.tab.cxx)
