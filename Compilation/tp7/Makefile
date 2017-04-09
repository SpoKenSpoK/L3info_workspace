CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb

OBJ = util.o analyseur_lexical.o analyseur_syntaxique.o premiers.o suivants.o affiche_arbre_abstrait.o syntabs.o tabsymboles.o generationX86.o

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

affiche_arbre_abstrait.o: affiche_arbre_abstrait.c
	$(CC) $(CCFLAGS) -c affiche_arbre_abstrait.c

syntabs.o: syntabs.c
	$(CC) $(CCFLAGS) -c syntabs.c

tabsymboles.o: tabsymboles.c
	$(CC) $(CCFLAGS) -c tabsymboles.c

generationX86.o: generationX86.c
	$(CC) $(CCFLAGS) -c generationX86.c



.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f compilo
