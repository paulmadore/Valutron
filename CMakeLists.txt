cmake_minimum_required (VERSION 2.4)
project (pprproc)

find_package(FLEX)

add_subdirectory (util/btyacc)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

function (FlexComp path)
GET_FILENAME_COMPONENT (file ${path} NAME_WE)
add_custom_command(
OUTPUT ${PROJECT_BINARY_DIR}/${file}.yy.h ${PROJECT_BINARY_DIR}/${file}.yy.cxx 
COMMAND ${FLEX_EXECUTABLE}
ARGS -o${PROJECT_BINARY_DIR}/${file}.yy.cxx -P ${file}
	--header-file=${PROJECT_BINARY_DIR}/${file}.yy.h
    ${path}
DEPENDS ${path}
)
endfunction (FlexComp)

function (BtYaccComp path)
GET_FILENAME_COMPONENT (file ${path} NAME_WE)
add_custom_command(
OUTPUT ${PROJECT_BINARY_DIR}/${file}.tab.h
    ${PROJECT_BINARY_DIR}/${file}.tab.cxx
COMMAND ${CMAKE_BINARY_DIR}/util/btyacc/BtYACC
ARGS -BdLtvli -b ${file} -o ${PROJECT_BINARY_DIR}/${file}.tab.cxx
    -p ${file} --verbose ${path}
DEPENDS ${CMAKE_BINARY_DIR}/util/btyacc/BtYACC
MAIN_DEPENDENCY ${path}
)
endfunction (BtYaccComp)

add_subdirectory (eval)
