#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "traitement_s.h"
#include "basic.h"


// Constructeur d'un noeud pour les AVL de Trajet (traitement S)
pArbre_s creerArbre_Trajet(int ID, float distance)
{
    pArbre_s new = malloc(sizeof(Arbre_s));
    if (new == NULL)
    {
        exit(1);
    }
    new->trajet.IDTrajet = ID;
    new->trajet.max = distance;
    new->trajet.min = distance;
    new->trajet.total = distance;
    new->trajet.nb_step = 1;

    for (int i = 0; i < 60; i++)
    {
        new->trajet.villes[i] = NULL;
    }

    new->eq = 0;
    new->fd = NULL;
    new->fg = NULL;
    return new;
}

pArbre_s rotaGauche_s(pArbre_s a)
{
    pArbre_s pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));
    a = pivot;
    return a;
}

pArbre_s rotaDroite_s(pArbre_s a)
{
    pArbre_s pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - min(eq_p, 0) + 1;
    pivot->eq = max(eq_a + 2, max(eq_a + eq_p + 2, eq_p + 1));
    a = pivot;
    return a;
}

// Double rota G
pArbre_s doublerotaG_s(pArbre_s a)
{
    a->fd = rotaDroite_s(a->fd);
    return rotaGauche_s(a);
}

// Double rota D
pArbre_s doublerotaD_s(pArbre_s a)
{
    a->fg = rotaGauche_s(a->fg);
    return rotaDroite_s(a);
}

// Fonction d'équilibrage des AVL de Trajet
pArbre_s equilibrerAVL_s(pArbre_s a)
{
    if (a != NULL)
    {
        if (a->eq >= 2)
        {
            if (a->fd->eq >= 0)
            {
                return rotaGauche_s(a);
            }
            else
            {
                return doublerotaG_s(a);
            }
        }
        if (a->eq <= -2)
        {
            if (a->fg->eq <= 0)
            {
                return rotaDroite_s(a);
            }
            else
            {
                return doublerotaD_s(a);
            }
        }
    }

    return a;
}

// Traitement -s : Fonction recursive pour inserer un noeud dans l'arbre AVL de Trajet selon un trajet
pArbre_s insert_trajet_s(pArbre_s node, int ID, float distance, int *h, int *count){

    if (node == NULL){ // Le trajet n'est pas déjà dans l'AVL, créer son noeud et le rajouter. Incrémenter le compte du nombre d'éléments de l'AVL aussi
        *h = 1;
        *count = *count + 1;
        node = creerArbre_Trajet(ID, distance);
        return node;
    }

    if (ID < node->trajet.IDTrajet){
        node->fg = insert_trajet_s(node->fg, ID, distance, h, count);
        *h = -*h;
    }

    else if (ID > node->trajet.IDTrajet){
        node->fd = insert_trajet_s(node->fd, ID, distance, h, count);
    }

    else{

        // Le trajet est déjà dans l'AVL
        *h = 0;
        node->trajet.nb_step++; // Augmenter son nombre d'étapes
        node->trajet.total += distance; // Ajouter la distance parcourue de l'étape à la distance totale du trajet
        node->trajet.max = (node->trajet.max < distance) ? distance : node->trajet.max; // Mise à jour de la distance min
        node->trajet.min = (node->trajet.min > distance) ? distance : node->trajet.min; // Mise à jour de la distance max

        return node;
    }

    if (*h != 0){ // Rééquilibrage

        node->eq += *h;
        node = equilibrerAVL_s(node);
        if (node->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }

    return node;
}

void free_tree_s(pArbre_s node){
    if (node != NULL){
        free_tree_s(node->fg);
        free_tree_s(node->fd);
        free(node);
    }
}

// Conversion d'AVL de Trajets en un tableau de trajets
void AVL_to_Tab_s(pArbre_s node, Trajet *tab, int *n){
    if (node){

        AVL_to_Tab_s(node->fg, tab, n);

        tab[*n].max = node->trajet.max;
        tab[*n].min = node->trajet.min;
        tab[*n].nb_step = node->trajet.nb_step;
        tab[*n].total = node->trajet.total;
        tab[*n].IDTrajet = node->trajet.IDTrajet;

        (*n)++;

        AVL_to_Tab_s(node->fd, tab, n);
    }
}

// Afficher les éléments d'un tableau de trajets selon le format : ID du trajet, distance minimum parcourue, distance maximum parcourue, distance totale parcourue, nombre d'étapes
void afficheTab_s(Trajet *tab, int n){
    for (int i = 0; i < n; i++){
        printf("\n%d : IDTrajet : %d, min : %f, max : %f, total : %f, steps : %d", i, tab[i].IDTrajet, tab[i].min, tab[i].max, tab[i].total, tab[i].nb_step);
    }
}

// Fonction de comparaison des différences max - min d'un trajet pour trier par ordre décroissant
int compare_distance_diff(const void* a, const void* b){
    float diff_a = ((Trajet*)a)->max - ((Trajet*)a)->min;
    float diff_b = ((Trajet*)b)->max - ((Trajet*)b)->min;

    if(diff_a < diff_b){
        return 1;
    }

    else if (diff_a > diff_b){
        return -1;
    }

    else{
        return 0;
    }
}