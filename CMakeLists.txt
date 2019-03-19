# Nom du projet 
project(42sh)

# Création de variables avec les fichiers à compiler
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

# On indique que l'on souhaite obtenir un executable "parser" compilé à partir des fichiers décrits par les variables créées plus haut
add_executable(
	ini ${SRCS} ${HEADERS}	
	)
