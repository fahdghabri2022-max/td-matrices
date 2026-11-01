/*
 * main.c -- Programme de démonstration de la bibliothèque de matrices.
 *
 * FICHIER À COMPLÉTER (exercice 4, étapes 4.a à 4.d).
 *
 * Usage : ./demo_matrices <add|mul> <fichier_A> <fichier_B> <fichier_R>
 */
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrice.h"
#include "matrice_alloc.h"
#include "matrice_io.h"
#include "matrice_calcul.h"

/* ------------------------------------------------------------------
 * FOURNI -- Ouvre le fichier, lit une matrice, ferme le fichier.
 * Affiche un message précis sur stderr en cas d'erreur.
 * ------------------------------------------------------------------ */
static MatErreur charger(const char *chemin, Matrice *m)
{
    FILE *f = fopen(chemin, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir \"%s\" : %s\n",
                chemin, strerror(errno));
        *m = MATRICE_VIDE;
        return MAT_ERR_LECTURE;
    }
    MatErreur err = mat_lire(f, m);
    fclose(f);
    if (err != MAT_OK)
        fprintf(stderr, "Erreur : lecture de \"%s\" : %s\n",
                chemin, mat_message_erreur(err));
    return err;
}

/* ------------------------------------------------------------------
 * FOURNI -- Ouvre le fichier en écriture, écrit la matrice, ferme le
 * fichier (en testant aussi le résultat de fclose).
 * ------------------------------------------------------------------ */
static MatErreur sauvegarder(const char *chemin, const Matrice *m)
{
    FILE *f = fopen(chemin, "w");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible de créer \"%s\" : %s\n",
                chemin, strerror(errno));
        return MAT_ERR_ECRITURE;
    }
    MatErreur err = mat_ecrire(f, m);
    if (fclose(f) == EOF && err == MAT_OK)
        err = MAT_ERR_ECRITURE;
    if (err != MAT_OK)
        fprintf(stderr, "Erreur : écriture de \"%s\" : %s\n",
                chemin, mat_message_erreur(err));
    return err;
}

int main(int argc, char *argv[])
{
    /* FOURNI : vérification des arguments. */
    if (argc != 5
        || (strcmp(argv[1], "add") != 0 && strcmp(argv[1], "mul") != 0)) {
        fprintf(stderr, "Usage : %s <add|mul> <fichier_A> <fichier_B> "
                        "<fichier_R>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int addition = (strcmp(argv[1], "add") == 0);

    /* FOURNI : toutes les matrices démarrent vides (règle R1) et le
     * programme est en échec tant qu'il n'a pas tout réussi. */
    Matrice a = MATRICE_VIDE, b = MATRICE_VIDE, r = MATRICE_VIDE;
    int statut = EXIT_FAILURE;

    /* FOURNI : chargement des deux matrices. */
    if (charger(argv[2], &a) != MAT_OK || charger(argv[3], &b) != MAT_OK)
        goto nettoyage;

    /* (4.a) Afficher A et B (sur stdout, pour la mise au point). */
    mat_afficher("A", &a);
    mat_afficher("B", &b);

    /* (4.b) Calculer r. La bibliothèque n'affiche rien : c'est ici
     *       qu'on écrit le message, avec les dimensions de A et de B.
     *       En cas d'erreur, r est resté vide (R3) : on saute au
     *       nettoyage, statut vaut encore EXIT_FAILURE. */
    MatErreur err = addition ? mat_additionner(&a, &b, &r)
                             : mat_multiplier(&a, &b, &r);
    if (err != MAT_OK) {
        fprintf(stderr, "Erreur : calcul impossible : %s ",
                mat_message_erreur(err));
        fprintf(stderr, "(A : %d x %d, B : %d x %d)\n",
                a.nb_lignes, a.nb_colonnes, b.nb_lignes, b.nb_colonnes);
        goto nettoyage;
    }

    /* (4.c) Afficher le résultat, avec un nom qui rappelle l'opération. */
    mat_afficher(addition ? "R = A + B" : "R = A x B", &r);

    /* FOURNI : enregistrement de r dans le fichier argv[4]. */
    if (sauvegarder(argv[4], &r) != MAT_OK)
        goto nettoyage;

    /* (4.d) Tout a réussi : on le signale, et seulement maintenant le
     *       statut passe à EXIT_SUCCESS. */
    printf("Résultat enregistré dans \"%s\".\n", argv[4]);
    statut = EXIT_SUCCESS;

nettoyage:
    /* FOURNI : point de sortie unique. Chaque matrice est détruite
     * exactement une fois, quel que soit le chemin suivi. */
    mat_detruire(&r);
    mat_detruire(&b);
    mat_detruire(&a);
    return statut;
}
