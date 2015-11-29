cmake_minimum_required (VERSION 2.4)
project (valutron)

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall")

FlexComp (${PROJECT_SOURCE_DIR}/scanner.l)
BtYaccComp (${PROJECT_SOURCE_DIR}/eval.y)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/compile
                    ${CMAKE_CURRENT_SOURCE_DIR}/gc
                    ${CMAKE_CURRENT_SOURCE_DIR}/type
                    ${CMAKE_CURRENT_SOURCE_DIR}/vm)

add_executable (eval automgr/automgr.cxx eval.cxx vm/vm.cxx vm/vmop.cxx
                type/builtin.cxx type/values.cxx
                compile/cbuiltin.cxx compile/compile.cxx
                ${PROJECT_BINARY_DIR}/scanner.yy.cxx
                ${PROJECT_BINARY_DIR}/eval.tab.cxx)
