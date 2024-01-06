#include <stdio.h>
#include <stdlib.h>

typedef struct Arbre{
    int n;
    struct Arbre* fg;
    struct Arbre* fd;
    int equilibre;
}Arbre;

Arbre* reequilibrageAVL(Arbre* a);

int max(int a, int b){
    return (a<=b) ? b : a;
}

int min(int a, int b){
    return (a<=b) ? a : b;
}

Arbre* creerArbre(int racine){
    Arbre* arbre = (Arbre*)malloc(sizeof(Arbre));
    if(arbre == NULL){
        exit(1);
    }
    arbre->n = racine;
    arbre->fg = NULL;
    arbre->fd = NULL;
    arbre->equilibre = 0;
    return arbre;
}

Arbre* insertAVL(Arbre* a, int e, int* h){
    if(a == NULL){
        *h = 1;
        return creerArbre(e);
    }
    else if(e < a->n){
        a->fg = (insertAVL(a->fg, e, h));
        *h = -*h;
    }
    else if(e > a->n){
        a->fd = (insertAVL(a->fd, e, h));
    }
    else{
        *h = 0;
        return a;
    }
    if(*h != 0){
        a->equilibre = a->equilibre + *h;
        a = reequilibrageAVL(a);
        if(a->equilibre ==0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return a;
}

int estFeuille(Arbre* a){
    if(a == NULL){
        exit(1);
    }
    if(a->fg == NULL && a->fd == NULL){
        return 1;
    }
    return 0;
}

Arbre* RotationGauche(Arbre* a){
    if(a != NULL){
        Arbre* pivot = a->fd;
        a->fd = pivot->fg;
        pivot->fg = a;
        int eq_a = a->equilibre;
        int eq_p = pivot->equilibre;
        a->equilibre = eq_a - max(eq_p, 0) - 1;
        int mineq = min(eq_a-2, eq_a+eq_p-2);
        pivot->equilibre = min(mineq, eq_p-1);
        a = pivot;
    }
    return a;
}

Arbre* RotationDroite(Arbre* a){
    if(a != NULL){
        Arbre* pivot = a->fg;
        a->fg = pivot->fd;
        pivot->fd = a;
        int eq_a = a->equilibre;
        int eq_p = pivot->equilibre;
        a->equilibre = eq_a - min(eq_p, 0) + 1;
        int maxeq = max(eq_a+2, eq_a+eq_p+2);
        pivot->equilibre = max(maxeq, eq_p+1);
        a = pivot;
    }
    return a;
}

Arbre* doubleRotationGauche(Arbre* a){
    a->fd = RotationDroite(a->fd);
    return RotationGauche(a);
}

Arbre* doubleRotationDroite(Arbre* a){
    a->fg = RotationGauche(a->fg);
    return RotationDroite(a);
}

Arbre* reequilibrageAVL(Arbre* a){
    if(a != NULL){
        if(a->equilibre >= 2){
            if(a->fd->equilibre >= 0){
                return RotationGauche(a);
            }
            else{
                return doubleRotationGauche(a);
            }
        }
        else if (a->equilibre <= -2){
            if(a->fg->equilibre <= 0){
                return RotationDroite(a);
            }
            else{
                return doubleRotationDroite(a);
            }
        }
    }
    return a;
}

Arbre* suppMinAVL(Arbre* a, int* h, int* e){
    if(a->fg == NULL){
        *e = a->n;
        *h = -1;
        Arbre* tmp = a;
        a = a->fd;
        free(tmp);
        return a;
    }
    else{
        a->fg = suppMinAVL(a->fg, h, e);
        *h = -*h;
    }
    if(*h != 0){
        a->equilibre = a->equilibre + *h;
        if(a->equilibre == 0){
            *h = -1;
        }
        else{
            *h = 0;
        }
    }
    return a;
}

Arbre* suppAVL(Arbre* a, int e, int* h){
    if(a == NULL){
        *h = 1;
        return a;
    }
    else if(e > a->n){
        a->fd = suppAVL(a->fd, e, h);
    }
    else if(e < a->n){
        a->fg = suppAVL(a->fg, e, h);
        *h = -*h;
    }
    else if (a->fd != NULL){
        a->fg = suppMinAVL(a->fd, h, &(a->n));
    }
    else{
        Arbre* tmp = a;
        a = a->fg;
        free(tmp);
        *h = -1;
    }
    if(a == NULL){
        *h = 0;
    }
    if(*h != 0){
        a->equilibre = a->equilibre + *h;
        a = reequilibrageAVL(a);
        if(a->equilibre == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }
    }
    return a;
}

void parcoursPrefixe(Arbre* a){
    if(a != NULL){
        printf("%d ", a->n);
        parcoursPrefixe(a->fg);
        parcoursPrefixe(a->fd);
    }
}

void free_tree(Arbre* node){
    if (node != NULL) {
        free_tree(node->fg);
        free_tree(node->fd);
        free(node);
    }
}

int main(){
    int diff_eq = 0;
    Arbre* arbre = insertAVL(arbre, 10, &diff_eq);
    arbre = insertAVL(arbre, 3, &diff_eq);
    arbre = insertAVL(arbre, 5, &diff_eq);
    arbre = insertAVL(arbre, 15, &diff_eq);
    arbre = insertAVL(arbre, 20, &diff_eq);
    arbre = insertAVL(arbre, 12, &diff_eq);
    arbre = insertAVL(arbre, 17, &diff_eq);
    arbre = insertAVL(arbre, 9, &diff_eq);

    parcoursPrefixe(arbre);
    printf("\n");

    free_tree(arbre);
    return 0;
}