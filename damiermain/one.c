int main(void) {
    JeuTaquin jeu;
    int lignes, colonnes;
    int hauteur_totale = HAUT_ECRAN + 60;
    int choix_image;
    const char *fichier_image = NULL;

    InitialiserGraphique();
    CreerFenetre(200, 200, LARG_ECRAN, hauteur_totale);

    while (1) {
        /* 0. MENU DE CHOIX D'IMAGE */
        choix_image = menu_images();
 

 if (choix_image == 1)
            fichier_image = "images/image anime.jpg";
        else if (choix_image == 2)
            fichier_image = "images/kaneki.jpg";
        else
            fichier_image = "images/onepiece.jpg";

        /* 1. ECRAN CONFIGURATION */
        ecran_accueil(&lignes, &colonnes);

        /* 2. INITIALISATION JEU avec l'image choisie */
        initialiser_jeu(&jeu, lignes, colonnes, fichier_image);

        int main(void) {
    JeuTaquin jeu;
    int lignes, colonnes;
    int hauteur_totale = HAUT_ECRAN + 60;
    int choix_image;
    const char *fichier_image = NULL;

    InitialiserGraphique();
    CreerFenetre(200, 200, LARG_ECRAN, hauteur_totale);

    while (1) {
        /* 0. MENU DE CHOIX D'IMAGE */
        choix_image = menu_images();

        if (choix_image == 1)
            fichier_image = "images/image anime.jpg";
        else if (choix_image == 2)
            fichier_image = "images/kaneki.jpg";
        else
            fichier_image = "images/onepiece.jpg";

        /* 1. ECRAN CONFIGURATION */
        ecran_accueil(&lignes, &colonnes);

        /* 2. INITIALISATION JEU avec l'image choisie */
        initialiser_jeu(&jeu, lignes, colonnes, fichier_image);
        melanger_plateau(&jeu, 300);
        dessiner_plateau(&jeu, LARG_ECRAN, HAUT_ECRAN);

        /* 3. BOUCLE DE JEU (inchangée) */
        while (1) {
            if (SourisCliquee()) {
                int mx = _X;
                int my = _Y;

                if (my <= HAUT_ECRAN && !est_resolu(&jeu)) {
                    if (gerer_clic_jeu(&jeu, LARG_ECRAN, HAUT_ECRAN, mx, my)) {
                        dessiner_plateau(&jeu, LARG_ECRAN, HAUT_ECRAN);
                    }
                }

                if (mx >= 20 && mx <= 100 &&
                    my >= HAUT_ECRAN + 10 && my <= HAUT_ECRAN + 50) {
                    break; /* retour au menu (image + config) */
                }
            }

            if (ToucheEnAttente()) {
                char k = Touche();
                if (k == 'q' || k == 'Q') {
                    FermerGraphique();
                    return EXIT_SUCCESS;
                }
            }
        }
    }

    FermerGraphique();
    return EXIT_SUCCESS;
}