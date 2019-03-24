project(42sh)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror -Wextra -pedantic -g")
# Création de variables avec les fichiers à compiler
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

# On indique que l'on souhaite obtenir un executable "42sh" compilé
## à partir des fichiers décrits par les variables créées plus haut

add_executable(
	42sh ${SRCS} ${HEADERS}	
	)
