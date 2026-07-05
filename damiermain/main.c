#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <graph.h>
#include "config.h"

#define LARG_ECRAN 480
#define HAUT_ECRAN 480

/* ----------- MENU IMAGES ----------- */
int menu_images(void)
{
    EffacerEcran(CouleurParComposante(30, 30, 30));

    /* Titre */
    ChoisirCouleurDessin(CouleurParComposante(255, 215, 0));
    EcrireTexte(80, 80, "CHOISIS UNE IMAGE", 1);

    /* Afficher les 3 images en petit */
    ChargerImage("images/image anime.jpg",  30, 150, 0, 0, 140, 140);
    ChargerImage("images/Untitled.jpg",      180, 150, 0, 0, 140, 140);
    ChargerImage("images/onepiece.jpg",    330, 150, 0, 0, 140, 140);

    /* Attendre un clic dans une des 3 zones */
    int choix = 0;
    while (choix == 0) {
        if (SourisCliquee()) {
            int x = _X;
            int y = _Y;

            if (y >= 150 && y <= 290) {          /* hauteur commune */
                if (x >= 30  && x <= 170) choix = 1;      /* 1ère image */
                else if (x >= 180 && x <= 320) choix = 2; /* 2ème */
                else if (x >= 330 && x <= 470) choix = 3; /* 3ème */
            }
        }
    }
    return choix;
}

/* ----------- BOUTON CARRE ----------- */
void dessiner_bouton(int x, int y, int taille, char *symbole, int r, int v, int b) {
    ChoisirCouleurDessin(CouleurParComposante(r, v, b));
    RemplirRectangle(x, y, taille, taille);
    ChoisirCouleurDessin(CouleurParComposante(255, 215, 0));
    EcrireTexte(x + 10, y + 25, symbole, 1);
}

/* ----------- ECRAN CONFIG ----------- */
void ecran_accueil(int *l_choisi, int *c_choisi) {
    int l = 4, c = 4; /* Valeurs par défaut */
    int jouer = 0;
    char buffer[20];

    int btn_size = 30;
    int y_lig = 150;
    int x_moins_l = 200, x_plus_l = 300;
    int y_col = 250;
    int x_moins_c = 200, x_plus_c = 300;
    int x_jouer = 140, y_jouer = 350, w_jouer = 200, h_jouer = 50;

    while (!jouer) {
        EffacerEcran(CouleurParComposante(30, 30, 30));

        ChoisirCouleurDessin(CouleurParComposante(255, 215, 0));
        EcrireTexte(100, 80, "CONFIGURATION DU JEU", 2);

        /* LIGNES */
        EcrireTexte(50, y_lig + 25, "LIGNES:", 1);
        dessiner_bouton(x_moins_l, y_lig, btn_size, "-", 200, 50, 50);
        sprintf(buffer, "%d", l);
        EcrireTexte(255, y_lig + 25, buffer, 1);
        dessiner_bouton(x_plus_l, y_lig, btn_size, "+", 50, 200, 50);

        /* COLONNES */
        EcrireTexte(50, y_col + 25, "COLONNES:", 1);
        dessiner_bouton(x_moins_c, y_col, btn_size, "-", 200, 50, 50);
        sprintf(buffer, "%d", c);
        EcrireTexte(255, y_col + 25, buffer, 1);
        dessiner_bouton(x_plus_c, y_col, btn_size, "+", 50, 200, 50);

        /* BOUTON JOUER */
        ChoisirCouleurDessin(CouleurParComposante(255, 192, 203));
        RemplirRectangle(x_jouer, y_jouer, w_jouer, h_jouer);
        ChoisirCouleurDessin(CouleurParComposante(0, 0, 0));
        EcrireTexte(x_jouer + 60, y_jouer + 35, "JOUER", 2);

        while (!SourisCliquee()) {
            if (ToucheEnAttente() && Touche() == 'q') exit(0);
        }

        int x = _X;
        int y = _Y;

        if (y >= y_lig && y <= y_lig + btn_size) {
            if (x >= x_moins_l && x <= x_moins_l + btn_size && l > 3) l--;
            if (x >= x_plus_l && x <= x_plus_l + btn_size && l < 8) l++;
        }

        if (y >= y_col && y <= y_col + btn_size) {
            if (x >= x_moins_c && x <= x_moins_c + btn_size && c > 3) c--;
            if (x >= x_plus_c && x <= x_plus_c + btn_size && c < 8) c++;
        }

        if (x >= x_jouer && x <= x_jouer + w_jouer &&
            y >= y_jouer && y <= y_jouer + h_jouer) {
            jouer = 1;
        }
    }

    *l_choisi = l;
    *c_choisi = c;
}

/* ----------- MAIN ----------- */
int main(void) {
    JeuTaquin jeu;
    int lignes, colonnes;
    int hauteur_totale = HAUT_ECRAN + 60;

    /* Initialisation graphique et RNG */
    InitialiserGraphique();
    CreerFenetre(1000, 1000, LARG_ECRAN, hauteur_totale);
    srand((unsigned int)time(NULL)); /* une seule fois */

    while (1) {

        /* 0. MENU IMAGES */
        int choix_image = menu_images();

        if (choix_image == 1)
            jeu.fichier_image = "images/image anime.jpg";
        else if (choix_image == 2)
            jeu.fichier_image = "images/Untitled.jpg";
        else
            jeu.fichier_image = "images/onepiece.jpg";

        /* 1. ECRAN CONFIGURATION */
        ecran_accueil(&lignes, &colonnes);

        /* 2. INITIALISATION JEU */
        initialiser_jeu(&jeu, lignes, colonnes);

        /* 3. MELANGE (garanti solvable) */
        melanger_plateau(&jeu);

        dessiner_plateau(&jeu, LARG_ECRAN, HAUT_ECRAN);

        /* 4. BOUCLE DE JEU */
        while (1) {
            if (SourisCliquee()) {
                int mx = _X;
                int my = _Y;

                if (my <= HAUT_ECRAN && !est_resolu(&jeu)) {
                    if (gerer_clic_jeu(&jeu, LARG_ECRAN, HAUT_ECRAN, mx, my)) {
                        dessiner_plateau(&jeu, LARG_ECRAN, HAUT_ECRAN);
                    }
                }

                /* bouton MENU */
                if (mx >= 20 && mx <= 100 &&
                    my >= HAUT_ECRAN + 10 && my <= HAUT_ECRAN + 50) {
                    break;
                }
            }

            if (ToucheEnAttente()) {
                char k = Touche();
                if (k == 'p' || k == 'P') {
                    FermerGraphique();

                    return EXIT_SUCCESS;
                }
            }
        }
    }

    FermerGraphique();
    return EXIT_SUCCESS;
}