#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct Ville {
    int trajets;
    char nom[30];
    int premier;
}Ville;

typedef struct Trajet{
    int IDTrajet;
    int max;
    int min;
    int total;
    int nb_step;
}Trajet;

typedef struct Arbre_t{
    Ville ville;
    int eq;
    struct Arbre_t* fg;
    struct Arbre_t* fd;
}Arbre_t;

typedef struct Arbre_s{
    int eq;
    Trajet trajet;
    struct Arbre_s* fg;
    struct Arbre_s* fd;
}Arbre_s;


typedef Arbre_t* pArbre_t;
typedef Arbre_s* pArbre_s;

void supprimerFilsDroit(pArbre_t a);
void supprimerFilsGauche(pArbre_t a);


pArbre_t creerArbre_Ville(char name[20], int flag){
    pArbre_t new = malloc(sizeof(Arbre_t));
    if(new == NULL){
        exit(1);
    }
    strcpy(new->ville.nom, name);
    new->ville.trajets = 1;
    new->ville.premier=flag;
    new->eq=0;
    new->fd=NULL;
    new->fg=NULL;
    return new;
}

pArbre_s creerArbre_Trajet(int ID, int distance){
    pArbre_s new = malloc(sizeof(Arbre_s));
    if(new == NULL){
        exit(1);
    }
    new->trajet.IDTrajet = ID;
    new->trajet.max = distance;
    new->trajet.min = distance;
    new->trajet.total = distance;
    new->trajet.nb_step = 1;
    new->eq=0;
    new->fd=NULL;
    new->fg=NULL;
    return new;
}

int hauteur(pArbre_t a) {
    if (a == NULL) {
        return -1;
    } else {
        int hg = hauteur(a->fg);
        int hd = hauteur(a->fd);
        return 1 + (hg > hd ? hg : hd); // remonte de chaque feuille : 1+1+... et a chaque noeud 
        //se demande quel sous arbre est le + grand
    }
}

int existeFilsGauche(pArbre_t a){
    if(a == NULL){
        exit(1);
    }
    return !(a->fg == NULL);
}

int existeFilsDroit(pArbre_t a){
    if(a == NULL){
        exit(1);
    }
    return !(a->fd == NULL);
}

//affiche la valeur dans le noeud
void traiter(pArbre_t a){
    if(a!=NULL){
        printf("%s ; %d trajets\n",a->ville.nom, a->ville.trajets);
    }
}

void supprimerFilsGauche(pArbre_t a){
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

void supprimerFilsDroit(pArbre_t a){
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
void parcoursprefixe(pArbre_t a){
    if(a!=NULL){
        traiter(a);
        parcoursprefixe(a->fg);
        parcoursprefixe(a->fd);
    }
}

//parcours GRD
void parcoursinfixe(pArbre_t a){
    if(a!=NULL){
        parcoursinfixe(a->fg);
        traiter(a);
        parcoursinfixe(a->fd);
    }
}

//parcours GDR
void parcourspostfixe(pArbre_t a){
    if(a!=NULL){
        parcourspostfixe(a->fg);
        parcourspostfixe(a->fd);
        traiter(a);
    }
}

void parcoursDecroissant(pArbre_t a) {
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

pArbre_t rotaGauche(pArbre_t a){
    pArbre_t pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg=a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p,0)-1;
    pivot->eq = min(eq_a-2,min(eq_a+eq_p-2,eq_p-1));
    a = pivot;
    return a;
}

pArbre_t rotaDroite(pArbre_t a){
    pArbre_t pivot = a->fg;
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
pArbre_t doublerotaG(pArbre_t a){
    a->fd = rotaDroite(a->fd);
    return rotaGauche(a);
}

// Double rota D
pArbre_t doublerotaD(pArbre_t a){
    a->fg = rotaGauche(a->fg);
    return rotaDroite(a);
}

pArbre_t equilibrerAVL(pArbre_t a){
    if(a != NULL){
        if(a->eq>=2){
            if(a->fd->eq>=0){
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
    }
    
    return a;
}


// Fonction recursive pour insérer un noeud dans l'arbre AVL selon une chaine de charactere
pArbre_t insert(pArbre_t node, char name[20], int* h, int* count, int flag) {
    if (node == NULL){
        *h=1;
        *count = *count+1;
        return creerArbre_Ville(name, flag);
    }

    if (strcmp(name, node->ville.nom) < 0){
        node->fg = insert(node->fg, name, h, count, flag);
        *h=-*h;
    }
    else if (strcmp(name, node->ville.nom) > 0){
        node->fd = insert(node->fd, name, h, count, flag);
    }
    else {
        // Le nom existe deja, incrementer le nombre de trajets
        *h=0;
        if(flag == 1){
            node->ville.premier+=1;
        }
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

// Fonction recursive pour insérer un noeud dans l'arbre AVL selon une chaine de charactere
pArbre_s insert_trajet(pArbre_s node, int ID, int distance, int* h, int* count) {
    if (node == NULL){
        *h=1;
        *count = *count+1;
        return creerArbre_Trajet(ID, distance);
    }

    if ( ID < node->trajet.IDTrajet ){
        node->fg = insert_trajet(node->fg,ID,distance,h,count);
        *h=-*h;
    }
    else if( ID > node->trajet.IDTrajet ){
        node->fd = insert_trajet(node->fd,ID,distance,h,count);
    }
    else {
        // Le trajet est deja renseigne
        *h=0;
        node->trajet.nb_step++;
        node->trajet.total+=distance;
        node->trajet.max = (node->trajet.max < distance) ? distance : node->trajet.max;
        node->trajet.min = (node->trajet.min > distance) ? distance : node->trajet.min;
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

void free_tree_t(pArbre_t node){
    if (node != NULL) {
        free_tree_t(node->fg);
        free_tree_t(node->fd);
        free(node);
    }
}

void free_tree_s(pArbre_s node){
    if (node != NULL) {
        free_tree_s(node->fg);
        free_tree_s(node->fd);
        free(node);
    }
}

void AVL_to_Tab_t(pArbre_t node,Ville* tab,int *n){
    if(node){

        AVL_to_Tab_t(node->fg,tab,n);


        strcpy(tab[*n].nom,node->ville.nom);
        tab[*n].trajets=node->ville.trajets;
        tab[*n].premier=node->ville.premier;

        (*n)++;
        
        AVL_to_Tab_t(node->fd,tab,n);
    }
}

void AVL_to_Tab_s(pArbre_s node,Trajet* tab,int *n){
    if(node){

        AVL_to_Tab_s(node->fg,tab,n);

        tab[*n].max=node->trajet.max;
        tab[*n].min=node->trajet.min;
        tab[*n].nb_step=node->trajet.nb_step;
        tab[*n].total=node->trajet.total;
        tab[*n].IDTrajet=node->trajet.IDTrajet;

        (*n)++;
        
        AVL_to_Tab_s(node->fd,tab,n);
    }
}


void afficheTab_t(Ville* tab,int n){
    for (int i = 0; i < n; i++){
        printf("\nVille : %s, trajets : %d, premier : %d",tab[i].nom,tab[i].trajets,tab[i].premier);
    }
    
}

void afficheTab_s(Trajet* tab,int n){
    for (int i = 0; i < n; i++){
        printf("\nIDTrajet : %d, min : %d, max : %d, total : %d, steps : %d",tab[i].IDTrajet,tab[i].min,tab[i].max,tab[i].total,tab[i].nb_step);
    }
    
}

void triBulle_t(Ville *tab, int taille){
    int desordre, ncase, etape;
    Ville temp;
    etape = taille - 1;
    do{
        desordre = 0;
        for (ncase = 0; ncase < etape; ncase++){
            if (tab[ncase].trajets < tab[ncase + 1].trajets){
            desordre = 1;
            temp = tab[ncase];
            tab[ncase] = tab[ncase + 1];
            tab[ncase + 1] = temp;
            }
        }
        etape--;
    } while (desordre && etape > 0);
}

int main(int argc, char *argv[]){
    //Traitement -t
    if(!strcmp(argv[1],"-t")){
        FILE* file = fopen(argv[2], "r");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            return 1;
        }

        char line[256];
        pArbre_t AVLroot = NULL;
        int count = 0;

        while (fgets(line, sizeof(line), file)){
            char *token;
            char townB[50];
            int h = 0;
            int IDTrajet;
            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            strcpy(townB, token);

            AVLroot = insert(AVLroot, townB, &h, &count, 0);
        }

        // On revient au debut du fichier pour prendre les villes de départ

        rewind(file);

        while (fgets(line, sizeof(line), file)){
            char *token;
            char townA[50];
            int h = 0;
            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            token = strtok(NULL, ";");
            if (atoi(token) == 1){
                token = strtok(NULL, ";");
                strcpy(townA, token);
                AVLroot = insert(AVLroot, townA, &h, &count, 1);
            }
        }

        fclose(file);

        printf("\nNb villes = %d\n", count);
        Ville* tab = (Ville*)malloc(sizeof(Ville)*count);
        
        int n = 0;

        AVL_to_Tab_t(AVLroot,tab,&n);
        triBulle_t(tab,count);

        afficheTab_t(tab,10);
        //Trier par ordre alpha

        free_tree_t(AVLroot);
        free(tab);
    }
    

    //Traitement -s
    if(!strcmp(argv[1],"-s")){
        FILE* file = fopen(argv[2], "r");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            return 1;
        }

        pArbre_s AVL_Trajet = NULL;
        char line[256];
        int count = 0;

        while (fgets(line, sizeof(line), file)){
            char *token;
            int IDTrajet;
            int distance;
            int h = 0;

            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);

            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            distance = atoi(token);

            AVL_Trajet = insert_trajet(AVL_Trajet, IDTrajet, distance, &h, &count);
        }

        //Mettre en tableau max-min (50 premiers)
        //Moyenne = total/nb pour utiliser en graph
        Trajet* tab = (Trajet*)malloc(sizeof(Trajet)*count);
        int n = 0;

        AVL_to_Tab_s(AVL_Trajet,tab,&n);

        //Trier
        //Tester
        afficheTab_s(tab,50);
        fclose(file);
        free_tree_s(AVL_Trajet);
    }

    return 0;
}