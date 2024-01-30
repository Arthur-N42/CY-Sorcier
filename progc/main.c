#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "basic.h"
#include "traitement_t.h"
#include "traitement_s.h"

int main(int argc, char *argv[]){

    // Traitement -t
    if(!strcmp(argv[1],"-t")){

        // Puisque le chemin fourni de data.csv au script shell est relatif au dossier root et pas au dossier progc pour le programme c :
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "../%s", argv[2]);

        FILE *file = fopen(full_path, "r");

        if (file == NULL){
            perror("Erreur lors de l'ouverture du fichier");
            return 1;
        }

        char line[256];

        pArbre_t AVL_Ville = NULL;
        pArbre_s AVL_Trajet = NULL;

        int count = 0;
        pArbre_t insertedCity = NULL;
        int IDTrajet;
        int h1 = 0, h2 = 0;

        // Parcours du fichier et insertion des Villes et Trajets dans leurs AVL respectifs
        while (fgets(line, sizeof(line), file))
        {
            char *token;
            char townB[50];
            h1 = 0;
            h2 = 0;

            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            strcpy(townB, token);

            AVL_Ville = insert_ville(AVL_Ville, townB, &h1, &count, 0, &insertedCity); // Insertion de la ville d'arrivée de la ligne parcourue dans l'AVL des Villes
            AVL_Trajet = insert_trajet_t(AVL_Trajet, IDTrajet, &h2, &insertedCity); // Insertion du trajet de ligne parcourue dans l'AVL des Trajets, ainsi que de la ville d'arrivée qu'il parcourt à cette ligne dans son tableau de villes parcourues
        }

        // On revient au début du fichier pour prendre les villes de départ
        rewind(file);

        // Parcours du fichier et insertion des Villes et Trajets dans leurs AVL respectifs
        while (fgets(line, sizeof(line), file))
        {
            char *token;
            char townA[50];
            h1 = 0;
            h2 = 0;

            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);
            token = strtok(NULL, ";");

            if (atoi(token) == 1)
            { // Si on a l'étape 1 d'un trajet, on refait pareil
                token = strtok(NULL, ";");
                strcpy(townA, token);
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");

                AVL_Ville = insert_ville(AVL_Ville, townA, &h1, &count, 1, &insertedCity); // Insertion de la ville d'arrivée de la ligne parcourue dans l'AVL des Villes
                AVL_Trajet = insert_trajet_t(AVL_Trajet, IDTrajet, &h2, &insertedCity); // Insertion du trajet de ligne parcourue dans l'AVL des Trajets, ainsi que de la ville d'arrivée qu'il parcourt à cette ligne dans son tableau de villes parcourues
            }
        }

        fclose(file);

        Ville *tab = (Ville *)malloc(sizeof(Ville) * count); // tableau de toutes les villes
        Ville *tab2 = (Ville *)malloc(sizeof(Ville) * 10); // tableau des 10 premières villes avec le + de trajets qui la parcourent
        int n = 0;

        AVL_to_Tab_t(AVL_Ville, tab, &n);
        qsort(tab, count, sizeof(Ville), compareTrajets); // Trier toutes les villes par celles qui ont le + de trajets qui les parcourent
        tab2 = putInTab_t(tab, 10, tab2); // mettre les 10 premières villes du tableau dans le nouveau tableau
        qsort(tab2, 10, sizeof(Ville), compareNames); // Trier par ordre alphabétique des noms

        FILE *new = fopen("../temp/data_t.txt", "w");

        // Enregistrer le classement des 10 villes dans un fichier
        for (int i = 0; i < 10; i++)
        {
            if (new)
            {
                fprintf(new, "%s;%d;%d\n", tab2[i].nom, tab2[i].trajets, tab2[i].premier);
            }
        }

        fclose(new);

        free_tree_t(AVL_Ville);
        free_tree_s(AVL_Trajet);
        free(tab);
        free(tab2);
    }

    //Traitement -s
    if(!strcmp(argv[1],"-s")){

        // Puisque le chemin fourni de data.csv au script shell est relatif au dossier root et pas au dossier progc pour le programme c :
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "../%s", argv[2]);

        FILE *file = fopen(full_path, "r");


        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            return 1;
        }

        pArbre_s AVL_Trajet = NULL;
        char line[256];
        int count = 0;

        // Parcours du fichier et insertion des Trajets dans l'AVL
        while (fgets(line, sizeof(line), file)){
            char *token;
            int IDTrajet;
            float distance;
            int h = 0;

            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);

            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            distance = atof(token);

            AVL_Trajet = insert_trajet_s(AVL_Trajet, IDTrajet, distance, &h, &count); // Insertion du trajet et de la distance qu'il parcourt à cette ligne dans l'AVL des Trajets
        }

        fclose(file);

        Trajet* tab = (Trajet*)malloc(sizeof(Trajet)*count); // tableau de tous les trajets
        int n = 0;

        AVL_to_Tab_s(AVL_Trajet,tab,&n);
        qsort(tab, count, sizeof(Trajet), compare_distance_diff); // Trier tous les trajets par ceux qui ont la + grande différence max - min de distance parcourue

        FILE* new = fopen("../temp/data_s.txt", "w");

        for (int i = 0; i < 50; i++){
            if(new){
                // ID, min, max, total, max - min, moyenne
                fprintf(new, "%d;%f;%f;%f;%f\n", tab[i].IDTrajet, tab[i].min, tab[i].max, tab[i].max - tab[i].min, tab[i].total/tab[i].nb_step);
            }
        }
        
        fclose(new);
        

        free_tree_s(AVL_Trajet);
        free(tab);
    }

    return 0;
}