#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct Ville {
    int trajets;
    char nom[30];
}Ville;

typedef struct arbre{
    Ville ville;
    int eq;
    struct arbre* fg;
    struct arbre* fd;
}Arbre;

typedef Arbre* pArbre;

void supprimerFilsDroit(pArbre a);
void supprimerFilsGauche(pArbre a);


pArbre creerArbre(char name[20]){
    pArbre new = malloc(sizeof(Arbre));
    if(new == NULL){
        exit(1);
    }
    new->ville.trajets = 1;
    strcpy(new->ville.nom, name);
    new->eq=0;
    new->fd=NULL;
    new->fg=NULL;
    return new;
}

int hauteur(pArbre a) {
    if (a == NULL) {
        return -1;
    } else {
        int hg = hauteur(a->fg);
        int hd = hauteur(a->fd);
        return 1 + (hg > hd ? hg : hd); // remonte de chaque feuille : 1+1+... et a chaque noeud 
        //se demande quel sous arbre est le + grand
    }
}

int existeFilsGauche(pArbre a){
    if(a == NULL){
        exit(1);
    }
    return !(a->fg == NULL);
}

int existeFilsDroit(pArbre a){
    if(a == NULL){
        exit(1);
    }
    return !(a->fd == NULL);
}

pArbre ajouterFilsGauche(pArbre a, char name[20]){
    if(existeFilsGauche(a)){
        return a;
    }
    a->fg = creerArbre(name);
    return a;
}

pArbre ajouterFilsDroit(pArbre a, char name[20]){
    if(existeFilsDroit(a)){
        return a;
    }
    a->fd = creerArbre(name);
    return a;
}

//affiche la valeur dans le noeud
void traiter(pArbre a){
    if(a == NULL){
        printf("");
    }
    else{
        printf("%s ; %d trajets\n",a->ville.nom, a->ville.trajets);
    }
}

void supprimerFilsGauche(pArbre a){
    if(a==NULL){
        printf("La file est vide.\n");
        exit(1);
    }
    else if(existeFilsGauche(a)){

        if(existeFilsGauche(a->fg)){
            supprimerFilsGauche(a->fg);
        }
        
        if(existeFilsDroit(a->fg)){
            supprimerFilsDroit(a->fg);
        }
        a->fg=NULL;
        free(a->fg);
    }
}

void supprimerFilsDroit(pArbre a){
    if(a==NULL){
        printf("La file est vide.\n");
        exit(1);
    }
    else if(existeFilsDroit(a)){

        if(existeFilsGauche(a->fd)){
            supprimerFilsGauche(a->fd);
        }

        if(existeFilsDroit(a->fd)){
            supprimerFilsDroit(a->fd);
        }
        a->fd=NULL;
        free(a->fd);
    }
}

//parcours RGD
void parcoursprefixe(pArbre a){
    if(a!=NULL){
        traiter(a);
        parcoursprefixe(a->fg);
        parcoursprefixe(a->fd);
    }
}

//parcours GRD
void parcoursinfixe(pArbre a){
    if(a!=NULL){
        parcoursinfixe(a->fg);
        traiter(a);
        parcoursinfixe(a->fd);
    }
}

//parcours GDR
void parcourspostfixe(pArbre a){
    if(a!=NULL){
        parcourspostfixe(a->fg);
        parcourspostfixe(a->fd);
        traiter(a);
    }
}

void parcoursDecroissant(pArbre a) {
    if (a != NULL) {
        parcoursDecroissant(a->fd);
        traiter(a);
        parcoursDecroissant(a->fg);
    }
}


int max(int a, int b){
    return (a<=b) ? b : a;
}

int min(int a, int b){
    return (a<=b) ? a : b;
}

pArbre rotaGauche(pArbre a){
    pArbre pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg=a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p,0)-1;
    pivot->eq = min(eq_a-2,min(eq_a+eq_p-2,eq_p-1));
    a = pivot;
    return a;
}

pArbre rotaDroite(pArbre a){
    pArbre pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd=a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - min(eq_p,0)+1;
    pivot->eq = max(eq_a+2,max(eq_a+eq_p+2,eq_p+1));
    a = pivot;
    return a;
}

// Double rota G
pArbre doublerotaG(pArbre a){
    a->fd = rotaDroite(a->fd);
    return rotaGauche(a);
}

// Double rota D
pArbre doublerotaD(pArbre a){
    a->fg = rotaGauche(a->fg);
    return rotaDroite(a);
}

pArbre equilibrerAVL(pArbre a){
    if(a->eq>=2){
        if(a->fg->eq>=0){
            return rotaGauche(a);
        }
        else{
            return doublerotaG(a);
        }
    }
    if(a->eq<=-2){
        if(a->fg->eq<=0){
            return rotaDroite(a);
        }
        else{
            return doublerotaD(a);
        }
    }
    return a;
}

// Fonction récursive pour insérer un nœud dans l'arbre AVL
pArbre insert(pArbre node, char name[20], int* h, int* count) {
    if (node == NULL){
        *h=1;
        *count = *count+1;
        return creerArbre(name);
    }

    if (strcmp(name, node->ville.nom) < 0){
        node->fg = insert(node->fg, name, h, count);
        *h=-*h;
    }
    else if (strcmp(name, node->ville.nom) > 0){
        node->fd = insert(node->fd, name, h, count);
    }
    else {
        // Le nom existe déjà, incrémenter le nombre de trajets
        *h=0;
        node->ville.trajets++;
        return node;
    }

    if(*h!=0){
        node->eq+=*h;
        if(node->eq==0){
            *h=0;
        }
        else{
            *h=1;
        }
    }

    return node;
}

void freeAVL(pArbre root){
    if(root != NULL){
        freeAVL(root->fg);
        freeAVL(root->fd);
        root->fg = NULL;
        root->fd = NULL;
        free(root);
    }
}


int main(int argc, char *argv[]){

    FILE* file = fopen("datatest.csv", "r");
    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    char line[256];
    pArbre AVLroot = NULL;
    int count = 0;

    while (fgets(line, sizeof(line), file)){
        char *token;
        char townB[50];
        int h = 0;

        // Utiliser strtok pour extraire les champs du fichier CSV
        token = strtok(line, ";");
        token = strtok(NULL, ";");
        token = strtok(NULL, ";");
        token = strtok(NULL, ";");
        strcpy(townB, token);

        AVLroot = insert(AVLroot, townB, &h, &count);
    }

    fclose(file);

    parcoursDecroissant(AVLroot);
    printf("\n\n nb villes = %d\n", count);
    free(AVLroot);


    return 0;
}