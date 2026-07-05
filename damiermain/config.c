#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <graph.h>
#include "config.h"

/* ---------- LOGIQUE METIER ---------- */

void initialiser_jeu(JeuTaquin *jeu, int l, int c) {
    int valeur = 1;
    int i, j;

    if (l < 2) l = 2;
    if (c < 2) c = 2;
    if (l > MAX_DIM) l = MAX_DIM;
    if (c > MAX_DIM) c = MAX_DIM;

    jeu->nb_lignes = l;
    jeu->nb_colonnes = c;
    jeu->compteur_moves = 0;

    for (i = 0; i < jeu->nb_lignes; ++i) {
        for (j = 0; j < jeu->nb_colonnes; ++j) {
            if (i == jeu->nb_lignes - 1 && j == jeu->nb_colonnes - 1) {
                jeu->grille[i][j] = CASE_VIDE;
            } else {
                jeu->grille[i][j] = valeur++;
            }
        }
    }
    jeu->vide_lig = jeu->nb_lignes - 1;
    jeu->vide_col = jeu->nb_colonnes - 1;
}

int deplacer_tuile(JeuTaquin *jeu, int d_lig, int d_col) {
    int nouv_lig = jeu->vide_lig + d_lig;
    int nouv_col = jeu->vide_col + d_col;

    if (nouv_lig < 0 || nouv_lig >= jeu->nb_lignes ||
        nouv_col < 0 || nouv_col >= jeu->nb_colonnes)
        return 0;

    jeu->grille[jeu->vide_lig][jeu->vide_col] = jeu->grille[nouv_lig][nouv_col];
    jeu->grille[nouv_lig][nouv_col] = CASE_VIDE;
    jeu->vide_lig = nouv_lig;
    jeu->vide_col = nouv_col;
    return 1;
}

/* --- Utilitaires de mélange --- */

static void remplir_grille_depuis_permutation(JeuTaquin *jeu, int *perm) {
    int idx = 0;
    int i, j;
    for (i = 0; i < jeu->nb_lignes; ++i) {
        for (j = 0; j < jeu->nb_colonnes; ++j) {
            int val = perm[idx++];
            if (val == 0) {
                jeu->grille[i][j] = CASE_VIDE;
                jeu->vide_lig = i;
                jeu->vide_col = j;
            } else {
                jeu->grille[i][j] = val;
            }
        }
    }
}

static int count_inversions(int *arr, int n) {
    int i, j, inv = 0;
    for (i = 0; i < n; ++i) {
        if (arr[i] == 0) continue;
        for (j = i + 1; j < n; ++j) {
            if (arr[j] == 0) continue;
            if (arr[i] > arr[j]) ++inv;
        }
    }
    return inv;
}

static int est_solvable_permutation(int *perm, int lignes, int colonnes) {
    int n = lignes * colonnes;
    int inv = count_inversions(perm, n);

    if (colonnes % 2 == 1) {
        return (inv % 2) == 0;
    } else {
        int pos = -1;
        int i;
        for (i = 0; i < n; ++i) if (perm[i] == 0) { pos = i; break; }
        int row_from_top = pos / colonnes;
        int row_from_bottom = lignes - row_from_top;
        if ((row_from_bottom % 2) == 0) return (inv % 2) == 1;
        else return (inv % 2) == 0;
    }
}

void melanger_plateau(JeuTaquin *jeu) {
    int n = jeu->nb_lignes * jeu->nb_colonnes;
    int *perm = (int*)malloc(sizeof(int) * n);
    int i;

    if (!perm) return;

    for (i = 0; i < n; ++i) perm[i] = i;

    do {
        for (i = n - 1; i > 0; --i) {
            int j = rand() % (i + 1);
            int tmp = perm[i];
            perm[i] = perm[j];
            perm[j] = tmp;
        }
    } while (!est_solvable_permutation(perm, jeu->nb_lignes, jeu->nb_colonnes)
             || (/* check if already solved */ ({ int equal = 1; for (i=0;i<n;++i){ if (perm[i] != (i==n-1?0:i+1)) { equal = 0; break; }} equal; })));

    remplir_grille_depuis_permutation(jeu, perm);
    jeu->compteur_moves = 0;
    free(perm);
}

int est_resolu(const JeuTaquin *jeu) {
    int val = 1;
    int i, j;
    for (i = 0; i < jeu->nb_lignes; ++i) {
        for (j = 0; j < jeu->nb_colonnes; ++j) {
            if (i == jeu->nb_lignes - 1 && j == jeu->nb_colonnes - 1) {
                if (jeu->grille[i][j] != CASE_VIDE) return 0;
            } else {
                if (jeu->grille[i][j] != val++) return 0;
            }
        }
    }
    return 1;
}

/* ---------- AFFICHAGE OPTIMISÉ ---------- */

/* Calcule les coordonnées précises pour éviter les espaces vides */
static void get_cell_rect(int idx, int total_size, int count, int *start, int *size) {
    int p1 = (idx * total_size) / count;
    int p2 = ((idx + 1) * total_size) / count;
    *start = p1;
    *size = p2 - p1;
}

void dessiner_plateau(const JeuTaquin *jeu, int larg_fen, int haut_fen) {
    int i, j;
    char tampon[80];

    EffacerEcran(CouleurParComposante(40, 40, 40));

    /* 1. DESSIN DE LA GRILLE */
    for (i = 0; i < jeu->nb_lignes; ++i) {
        /* Calcul hauteur case pour cette ligne */
        int y, h;
        get_cell_rect(i, haut_fen, jeu->nb_lignes, &y, &h);

        for (j = 0; j < jeu->nb_colonnes; ++j) {
            /* Calcul largeur case pour cette colonne */
            int x, w;
            get_cell_rect(j, larg_fen, jeu->nb_colonnes, &x, &w);

            int centre_x = x + w / 2 - 8;
            int centre_y = y + h / 2 + 4;

            if (jeu->grille[i][j] == CASE_VIDE) {
                /* Case vide : rectangle sombre */
                ChoisirCouleurDessin(CouleurParComposante(30, 30, 30));
                RemplirRectangle(x, y, w, h);
            } else {
                /* Si pas d'image chargée */
                if (jeu->fichier_image == NULL) {
                    ChoisirCouleurDessin(CouleurParComposante(255, 0, 0));
                    RemplirRectangle(x, y, w - 2, h - 2); /* petit bord */
                    ChoisirCouleurDessin(CouleurParComposante(255, 255, 255));
                    sprintf(tampon, "%d", jeu->grille[i][j]);
                    EcrireTexte(centre_x, centre_y, tampon, 1);
                }
                else {
                    /* CALCUL IMAGE REDIMENSIONNÉE (Mapping) */
                    /* On suppose que l'image source fait la même taille que la fenêtre (480x480) */
                    /* pour obtenir l'effet "redimensionné" complet. */
                   
                    int tuile = jeu->grille[i][j] - 1;
                    int src_row = tuile / jeu->nb_colonnes;
                    int src_col = tuile % jeu->nb_colonnes;

                    /* Calcul des coordonnées source correspondantes */
                    int sx, sw, sy, sh;
                    get_cell_rect(src_col, larg_fen, jeu->nb_colonnes, &sx, &sw);
                    get_cell_rect(src_row, haut_fen, jeu->nb_lignes,   &sy, &sh);

                    /* Note: ChargerImage(file, dest_x, dest_y, src_x, src_y, w, h) */
                    /* On utilise w/h de la destination, ce qui coupe dans la source. */
                    /* Si l'image source est != 480x480, cela paraîtra zoomé ou coupé. */
                    ChargerImage(jeu->fichier_image, x, y, sx, sy, w, h);
                }
            }
        }
    }

    /* 2. ZONE D'INFORMATIONS */
    ChoisirCouleurDessin(CouleurParComposante(220, 100, 50));
    RemplirRectangle(20, haut_fen + 10, 80, 40);
    ChoisirCouleurDessin(CouleurParComposante(170, 170, 170));
    EcrireTexte(35, haut_fen + 35, "MENU", 1);

    ChoisirCouleurDessin(CouleurParComposante(50, 100, 255));
    sprintf(tampon, "Moves: %d", jeu->compteur_moves);
    EcrireTexte(150, haut_fen + 35, tampon, 1);

    if (est_resolu(jeu)) {
        ChoisirCouleurDessin(CouleurParComposante(0, 255, 100));
        EcrireTexte(300, haut_fen + 35, "VICTOIRE !", 1);
    }
}

int gerer_clic_jeu(JeuTaquin *jeu, int larg_fen, int haut_fen, int x, int y) {
    /* Il faut utiliser la même logique de calcul de coordonnées pour trouver la case cliquée */
    /* Simple division suffit ici car l'erreur est minime sur le clic */
    int larg_case_approx = larg_fen / jeu->nb_colonnes;
    int haut_case_approx = haut_fen / jeu->nb_lignes;

    /* On évite la division par zéro si config foireuse */
    if (larg_case_approx == 0) larg_case_approx = 1;
    if (haut_case_approx == 0) haut_case_approx = 1;

    int col = x / larg_case_approx;
    int lig = y / haut_case_approx;

    /* Protection bornes */
    if (col >= jeu->nb_colonnes) col = jeu->nb_colonnes - 1;
    if (lig >= jeu->nb_lignes) lig = jeu->nb_lignes - 1;

    /* Si clic hors zone */
    if (x < 0 || y < 0 || x > larg_fen || y > haut_fen) return 0;

    int success = 0;
    int vl = jeu->vide_lig;
    int vc = jeu->vide_col;

    /* Vérif adjacence stricte */
    if ((lig == vl && (col == vc + 1 || col == vc - 1)) ||
        (col == vc && (lig == vl + 1 || lig == vl - 1))) {
        success = deplacer_tuile(jeu, lig - vl, col - vc);
    }

    if (success) jeu->compteur_moves++;



int gerer_touche_jeu(JeuTaquin *t, unsigned long key) {
    int dr = 0; int dc = 0;
    switch (key) {
        case XK_Left:  dc = 1;  break;
        case XK_Right: dc = -1; break;
        case XK_Up:    dr = 1;  break;
        case XK_Down:  dr = -1; break;
        /* Support ZQSD */
        case 'z': case 'Z':  dr = 1; break;
        case 's': case 'S': dr = -1; break;
        case 'q': case 'Q': dc = 1; break;
        case 'd': case 'D': dc = -1; break;
        default: return 0;
    }
    if (deplacer_tuile(t, dr, dc)) {
        t->compteur_moves++;
        return 1;
    }
    return 0;
}
    return success;
}