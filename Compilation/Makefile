CC = gcc

LIBS = -lm
CCFLAGS = -Wall -ggdb

OBJ = analyseur_lexical.o analyseur_syntaxique.o util.o

all: test_yylex

test_yylex: test_yylex.c $(OBJ)
	$(CC) $(CCFLAGS) -o test_yylex test_yylex.c $(OBJ)

analyseur_lexical.o: analyseur_lexical.c
	$(CC) $(CCFLAGS) -c analyseur_lexical.c

analyseur_syntaxique.o: analyseur_syntaxique.c
	$(CC) $(CCFLAGS) -c analyseur_syntaxique.c

.PHONY : clean

clean:
	- rm -f $(OBJ)
	- rm -f test_yylex
