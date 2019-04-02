project(42sh)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -g")
# creation of variables with source and header files
set(SRCS
	src/ast.c
	src/extract.c
	src/parser.c
	src/ini.c
    src/print_ast.c
    src/opt-parser.c
    src/exec.c
    src/hashmap.c
	)

set(HEADERS
	src/ast.h
	src/extract.h
	src/parser.h
    src/print_ast.h
    src/opt-parser.h
    src/exe.h
    src/hashmap.h
	)

#Indicate that we want an executable "42sh" with variables 

add_executable(
	42sh ${SRCS} ${HEADERS}	
	)

#Run Test suite
add_custom_target(check
	COMMAND python tests/test.py
	)

#Run Doxygen
add_custom_target(doc
                  COMMAND doxygen doc/Doxyfile)
