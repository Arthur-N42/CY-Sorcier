#ifndef TRAITEMENT_S_H
#define TRAITEMENT_S_H

#include "struct.h"

pArbre_s creerArbre_Trajet(int ID, float distance);
pArbre_s rotaGauche_s(pArbre_s a);
pArbre_s rotaDroite_s(pArbre_s a);
pArbre_s doublerotaG_s(pArbre_s a);
pArbre_s doublerotaD_s(pArbre_s a);
pArbre_s equilibrerAVL_s(pArbre_s a);
pArbre_s insert_trajet_s(pArbre_s node, int ID, float distance, int *h, int *count);
void free_tree_s(pArbre_s node);
void AVL_to_Tab_s(pArbre_s node, Trajet *tab, int *n);
void afficheTab_s(Trajet *tab, int n);
int compare_distance_diff(const void* a, const void* b);

#endif // TRAITEMENT_S_H