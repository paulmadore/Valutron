cmake_minimum_required (VERSION 2.4)
project (eval)

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall")

FlexComp(${PROJECT_SOURCE_DIR}/eval.l)
BtYaccComp(${PROJECT_SOURCE_DIR}/eval.y)

include_directories(${CMAKE_CURRENT_BINARY_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable (eval eval.cxx
                values.cxx
                ${PROJECT_BINARY_DIR}/eval.yy.cxx
                ${PROJECT_BINARY_DIR}/eval.tab.cxx)
