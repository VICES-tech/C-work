#ifndef CONFIG_H
#define CONFIG_H

#define MAX_DIM   8
#define CASE_VIDE 0

/* Structure du jeu de taquin */
typedef struct {
    int grille[MAX_DIM][MAX_DIM];
    int nb_lignes;
    int nb_colonnes;
    int vide_lig;          /* Position vide (ligne) */
    int vide_col;          /* Position vide (colonne) */
    int compteur_moves;
    char *fichier_image; /* chemin de l'image choisie (optionnel) */
} JeuTaquin;

/* Initialisation du jeu */
void initialiser_jeu(JeuTaquin *jeu, int l, int c);

/* Mélange : garantit une configuration solvable */
void melanger_plateau(JeuTaquin *jeu);

/* Vérification résolution & déplacement */
int  est_resolu(const JeuTaquin *jeu);
int  deplacer_tuile(JeuTaquin *jeu, int d_lig, int d_col);

/* Affichage et gestion */
void dessiner_plateau(const JeuTaquin *jeu, int larg_fen, int haut_fen);
int  gerer_clic_jeu(JeuTaquin *jeu, int larg_fen, int haut_fen, int x, int y);

#endif