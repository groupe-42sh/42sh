#Nom du projet
project(42sh)

#création de variables avec les fichiers à compiler
set(SRCS
	src/ast.c
	src/extract.c
	src/parser.c
	src/ini.c
	)

set(HEADERS
	src/ast.h
	src/extract.h
	src/parser.h
	)

#on indique que l'on souhaite obtenir un executable "ini"
add_executable(
	ini ${SRCS} ${HEADERS}
	)
