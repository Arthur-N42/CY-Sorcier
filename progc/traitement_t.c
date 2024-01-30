#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "struct.h"
#include "traitement_t.h"
#include "traitement_s.h"
#include "basic.h"

// Constructeur d'un noeud pour les AVL de Ville
pArbre_t creerArbre_Ville(char name[30], int flag)
{
    pArbre_t new = malloc(sizeof(Arbre_t));
    if (new == NULL)
    {
        exit(1);
    }

    strcpy(new->ville.nom, name);
    new->ville.trajets = 0;
    new->ville.premier = flag; // indicateur booléon si la ville est ville de départ (1) ou non (0)

    new->eq = 0;
    new->fd = NULL;
    new->fg = NULL;
    return new;
}

pArbre_t rotaGauche(pArbre_t a)
{
    pArbre_t pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p, 0) - 1;
    pivot->eq = min(eq_a - 2, min(eq_a + eq_p - 2, eq_p - 1));
    a = pivot;
    return a;
}

pArbre_t rotaDroite(pArbre_t a)
{
    pArbre_t pivot = a->fg;
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
pArbre_t doublerotaG(pArbre_t a)
{
    a->fd = rotaDroite(a->fd);
    return rotaGauche(a);
}

// Double rota D
pArbre_t doublerotaD(pArbre_t a)
{
    a->fg = rotaGauche(a->fg);
    return rotaDroite(a);
}

// Fonction d'équilibrage des AVL de Ville
pArbre_t equilibrerAVL_t(pArbre_t a)
{
    if (a != NULL)
    {
        if (a->eq >= 2)
        {
            if (a->fd->eq >= 0)
            {
                return rotaGauche(a);
            }
            else
            {
                return doublerotaG(a);
            }
        }
        if (a->eq <= -2)
        {
            if (a->fg->eq <= 0)
            {
                return rotaDroite(a);
            }
            else
            {
                return doublerotaD(a);
            }
        }
    }

    return a;
}

// Traitement -t : Fonction recursive pour inserer un noeud dans l'arbre AVL de Ville selon une chaine de caractères (arbre trié par ordre alphabétique de cette chaîne)
pArbre_t insert_ville(pArbre_t node, char name[30], int *h, int *count, int flag, pArbre_t *insertedNode){

    if (node == NULL){ // La ville n'est pas déjà dans l'AVL, créer son noeud et la rajouter. Incrémenter le compte du nombre d'éléments de l'AVL aussi
        *h = 1;
        *count = *count + 1;
        node = creerArbre_Ville(name, flag);
        *insertedNode = node; // La mémoriser comme dernière ville insérée dans l'AVL des Villes
        return node;
    }

    if (strcmp(name, node->ville.nom) < 0){
        node->fg = insert_ville(node->fg, name, h, count, flag, insertedNode);
        *h = -*h;
    }

    else if (strcmp(name, node->ville.nom) > 0){
        node->fd = insert_ville(node->fd, name, h, count, flag, insertedNode);
    }

    else{

        // La ville est déjà dans l'AVL
        *h = 0;
        *insertedNode = node; // La mémoriser comme dernière ville insérée dans l'AVL des Villes

        if (flag == 1){
            node->ville.premier += 1; // Si marquée comme ville de départ, incrémenter le nombre de fois qu'elle est ville de départ
        }

        return node;
    }

    if (*h != 0){ // Rééquilibrage

        node->eq += *h;
        node = equilibrerAVL_t(node);
        if (node->eq == 0){
            *h = 0;
        }

        else{
            *h = 1;
        }
    }

    return node;
}

// REUTILISATION DE pArbre_s MAIS TRAITEMENT -T UNIQUEMENT
// Fonction recursive pour inserer un noeud dans l'arbre AVL de Trajet selon un trajet
pArbre_s insert_trajet_t(pArbre_s node, int ID, int *h, pArbre_t *city)
{
    if (node == NULL){ // Le trajet n'est pas déjà dans l'AVL, créer son noeud et le rajouter.
        *h = 1;
        node = creerArbre_Trajet(ID, 0);
        node->trajet.villes[0] = *city; // Ajouter la dernière ville ajoutée à l'AVL des villes à la liste des villes parcourues par le trajet
        (*city)->ville.trajets += 1; // Incrémenter le nombre de trajets qui parcourent cette ville
        return node;
    }

    if (ID < node->trajet.IDTrajet){
        node->fg = insert_trajet_t(node->fg, ID, h, city);
        *h = -*h;
    }

    else if (ID > node->trajet.IDTrajet){
        node->fd = insert_trajet_t(node->fd, ID, h, city);
    }

    else{

        // Le trajet est deja renseigné
        *h = 0;

        int i = 0;

        // Rajoute la dernière ville enregistrée dans l'AVL des villes au tableau des villes parcourues par le trajet examiné
        while (node->trajet.villes[i] != NULL && i < 59)
        {
            if (node->trajet.villes[i] == *city)
                return node; // Si la ville est déjà dans le tableau, ignorer cette étape
            i++;
        }
        node->trajet.villes[i] = *city; 
        (*city)->ville.trajets += 1; // Incrémenter le nombre de trajets qui parcourent la ville sans compter plusieurs fois un même trajet

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

void free_tree_t(pArbre_t node){
    if (node != NULL){
        free_tree_t(node->fg);
        free_tree_t(node->fd);
        free(node);
    }
}

// Conversion d'AVL de Ville en un tableau de villes
void AVL_to_Tab_t(pArbre_t node, Ville *tab, int *n){
    if (node){

        AVL_to_Tab_t(node->fg, tab, n);

        strcpy(tab[*n].nom, node->ville.nom);
        tab[*n].trajets = node->ville.trajets;
        tab[*n].premier = node->ville.premier;

        (*n)++;

        AVL_to_Tab_t(node->fd, tab, n);
    }
}

// Afficher les éléments d'un tableau de ville selon le format : Ville, nombre de trajets la parcourant, nombre de fois qu'elle est ville de départ
void afficheTab_t(Ville *tab, int n){
    for (int i = 0; i < n; i++){
        printf("\nVille : %s, trajets : %d, premier : %d", tab[i].nom, tab[i].trajets, tab[i].premier);
    }
}

// Traitement -t : Fonction d'insertion des [size] premières villes d'un tableau dans un nouveau tableau
Ville *putInTab_t(Ville *tab, int size, Ville *tab2){
    for (int i = 0; i < size; i++){
        tab2[i] = tab[i];
    }
    return tab2;
}

// Fonction de comparaison de noms de villes pour trier par ordre alphabétique
int compareNames(const void *a, const void *b){
    const Ville *first = (const Ville *)a;
    const Ville *second = (const Ville *)b;

    return strcmp(first->nom, second->nom);
}

// Fonction de comparaison de nombre de trajets pour trier par ordre décroissant
int compareTrajets(const void *a, const void *b){
    const Ville *first = (const Ville *)a;
    const Ville *second = (const Ville *)b;

    if (first->trajets < second->trajets)
    {
        return 1;
    }

    else if (first->trajets > second->trajets)
    {
        return -1;
    }

    else
    {
        return 0;
    }
}