# Nom du projet 
project(42sh)

# Création de variables avec les fichiers à compiler
set(SRCS
	ast.c
	extract.c
	parser.c
	ini.c
	opt-parser.c
	)

set(HEADERS
	ast.h
	extract.h
	parser.h
	opt-parser.h
	)

# On indique que l'on souhaite obtenir un executable "parser" compilé à partir des fichiers décrits par les variables créées plus haut
add_executable(
	ini ${SRCS} ${HEADERS}	
	)
