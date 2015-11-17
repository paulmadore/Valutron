cmake_minimum_required (VERSION 2.4)
project (BtYACC)

add_definitions(-DYYBTYACC=1)
add_executable (BtYACC btyaccpar.c closure.c error.c graph.c lalr.c lr0.c
                main.c mkpar.c mstring.c output.c reader.c symtab.c verbose.c
                warshall.c)
