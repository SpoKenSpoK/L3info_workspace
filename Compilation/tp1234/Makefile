CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb

OBJ = util.o analyseur_lexical.o analyseur_syntaxique.o premiers.o suivants.o

all: compilo

compilo: compilo.c $(OBJ)
	$(CC) $(CCFLAGS) -o compilo compilo.c $(OBJ)

util.o: util.c
	$(CC) $(CCFLAGS) -c util.c

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c analyseur_lexical.c

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c analyseur_syntaxique.c

premiers.o: premiers.c
	$(CC) $(CCFLAGS) -c premiers.c

suivants.o: suivants.c
	$(CC) $(CCFLAGS) -c suivants.c


.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f compilo
