# Makefile du TD « Stockage de matrices en C »
#
#   make         compile les quatre programmes (cible par défaut : all)
#   make clean   supprime les fichiers produits par la compilation
#
# Compilation séparée : chaque .c donne un fichier objet (.o), puis
# l'édition de liens assemble les objets en un exécutable.
# Les programmes de test utilisent des objets compilés avec -DMAT_TEST_ALLOC
# (suffixe _t.o), qui active la simulation d'échec d'allocation ;
# demo_matrices utilise les objets normaux (.o), sans cette option.

CC     = gcc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g

PROGRAMMES = test_ex1 test_ex2 test_ex3 demo_matrices

all: $(PROGRAMMES)

# ---- Édition de liens ------------------------------------------------
demo_matrices: main.o matrice.o matrice_alloc.o matrice_io.o matrice_calcul.o
	$(CC) $(CFLAGS) $^ -o $@

test_ex1: test_ex1_t.o matrice_t.o matrice_alloc_t.o
	$(CC) $(CFLAGS) $^ -o $@

test_ex2: test_ex2_t.o matrice_t.o matrice_alloc_t.o matrice_io_t.o
	$(CC) $(CFLAGS) $^ -o $@

test_ex3: test_ex3_t.o matrice_t.o matrice_alloc_t.o matrice_calcul_t.o
	$(CC) $(CFLAGS) $^ -o $@

# ---- Compilation (règles génériques) ---------------------------------
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%_t.o: %.c
	$(CC) $(CFLAGS) -DMAT_TEST_ALLOC -c $< -o $@

# ---- Dépendances vers les en-têtes (recopient le graphe des inclusions)
matrice.o matrice_t.o:               matrice.h
matrice_alloc.o matrice_alloc_t.o:   matrice_alloc.h matrice.h
matrice_io.o matrice_io_t.o:         matrice_io.h matrice_alloc.h matrice.h
matrice_calcul.o matrice_calcul_t.o: matrice_calcul.h matrice_alloc.h matrice.h
main.o:     matrice.h matrice_alloc.h matrice_io.h matrice_calcul.h
test_ex1_t.o: matrice.h matrice_alloc.h
test_ex2_t.o: matrice.h matrice_alloc.h matrice_io.h
test_ex3_t.o: matrice.h matrice_alloc.h matrice_calcul.h

clean:
	rm -f *.o $(PROGRAMMES) test_ex2_sortie.txt R.txt R2.txt

.PHONY: all clean
