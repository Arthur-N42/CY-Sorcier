#ifndef TRAITEMENT_T_H
#define TRAITEMENT_T_H

#include "struct.h"

pArbre_t creerArbre_Ville(char name[30], int flag);
pArbre_t rotaGauche(pArbre_t a);
pArbre_t rotaDroite(pArbre_t a);
pArbre_t doublerotaG(pArbre_t a);
pArbre_t doublerotaD(pArbre_t a);
pArbre_t equilibrerAVL_t(pArbre_t a);
pArbre_t insert_ville(pArbre_t node, char name[30], int *h, int *count, int flag, pArbre_t *insertedNode);
pArbre_s insert_trajet_t(pArbre_s node, int ID, int *h, pArbre_t *city);
void free_tree_t(pArbre_t node);
void AVL_to_Tab_t(pArbre_t node, Ville *tab, int *n);
void afficheTab_t(Ville *tab, int n);
Ville *putInTab_t(Ville *tab, int size, Ville *tab2);
int compareNames(const void *a, const void *b);
int compareTrajets(const void *a, const void *b);

#endif // TRAITEMENT_T_H