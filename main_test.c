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

typedef struct Arbre_t{
    Ville ville;
    int eq;
    struct Arbre_t* fg;
    struct Arbre_t* fd;
}Arbre_t;

typedef Arbre_t* pArbre_t;

typedef struct Trajet{
    int IDTrajet;
    double max;
    double min;
    double total;
    int nb_step;
    pArbre_t villes[60];         //Note à moi même : peut être possibilité si j'ai le courage d'implémenter une table de hachage au lieu d'un tableau normal pour gagner un peu en vitesse
}Trajet;

typedef struct Arbre_s{
    int eq;
    Trajet trajet;
    struct Arbre_s* fg;
    struct Arbre_s* fd;
}Arbre_s;



typedef Arbre_s* pArbre_s;

void supprimerFilsDroit(pArbre_t a);
void supprimerFilsGauche(pArbre_t a);


pArbre_t creerArbre_Ville(char name[30], int flag){
    pArbre_t new = malloc(sizeof(Arbre_t));
    if(new == NULL){
        exit(1);
    }

    strcpy(new->ville.nom, name);
    new->ville.trajets = 0;
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
    
    for(int i=0; i<60; i++) {
        new->trajet.villes[i] = NULL;
    }

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

pArbre_s rotaGauche_s(pArbre_s a){
    pArbre_s pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg=a;
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - max(eq_p,0)-1;
    pivot->eq = min(eq_a-2,min(eq_a+eq_p-2,eq_p-1));
    a = pivot;
    return a;
}

pArbre_s rotaDroite_s(pArbre_s a){
    pArbre_s pivot = a->fg;
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
pArbre_s doublerotaG_s(pArbre_s a){
    a->fd = rotaDroite_s(a->fd);
    return rotaGauche_s(a);
}

// Double rota D
pArbre_s doublerotaD_s(pArbre_s a){
    a->fg = rotaGauche_s(a->fg);
    return rotaDroite_s(a);
}

pArbre_s equilibrerAVL_s(pArbre_s a){
    if(a != NULL){
        if(a->eq>=2){
            if(a->fd->eq>=0){
                return rotaGauche_s(a);
            }
            else{
                return doublerotaG_s(a);
            }
        }
        if(a->eq<=-2){
            if(a->fg->eq<=0){
                return rotaDroite_s(a);
            }
            else{
                return doublerotaD_s(a);
            }
        }
    }
    
    return a;
}

// Fonction recursive pour inserer un noeud dans l'arbre AVL selon une chaine de charactere (par ordre alphabétique) pour le traitement t
pArbre_t insert(pArbre_t node, char name[30], int* h, int* count, int flag, pArbre_t* insertedNode) {
    if (node == NULL){
        *h=1;
        *count = *count+1;
        node = creerArbre_Ville(name, flag);
        *insertedNode = node;
        return node;
    }

    if (strcmp(name, node->ville.nom) < 0){
        node->fg = insert(node->fg, name, h, count, flag, insertedNode);
        *h=-*h;
    }
    else if (strcmp(name, node->ville.nom) > 0){
        node->fd = insert(node->fd, name, h, count, flag, insertedNode);
    }
    else {
        // Le nom existe deja, incrementer le nombre de trajets
        *h=0;
        *insertedNode = node;
        if(flag == 1){
            node->ville.premier+=1;
        }
        return node;
    }

    if(*h!=0){
        node->eq+=*h;
        node = equilibrerAVL(node);
        if(node->eq==0){
            *h=0;
        }
        else{
            *h=1;
        }
    }

    return node;
}

// Fonction recursive pour inserer un noeud dans l'arbre AVL selon un trajet pour le traitement s
pArbre_s insert_trajet(pArbre_s node, int ID, double distance, int* h, int* count, pArbre_t* city) {
    if (node == NULL){
        *h=1;
        *count = *count+1;
        node = creerArbre_Trajet(ID, distance);
        node->trajet.villes[0] = *city;
        (*city)->ville.trajets+=1;
        return node;
    }

    if ( ID < node->trajet.IDTrajet ){
        node->fg = insert_trajet(node->fg,ID,distance,h,count, city);
        *h=-*h;
    }
    else if( ID > node->trajet.IDTrajet ){
        node->fd = insert_trajet(node->fd,ID,distance,h,count, city);
    }
    else {
        // Le trajet est deja renseigne
        *h=0;
        node->trajet.nb_step++;
        node->trajet.total+=distance;
        node->trajet.max = (node->trajet.max < distance) ? distance : node->trajet.max;
        node->trajet.min = (node->trajet.min > distance) ? distance : node->trajet.min;

        int i = 0;
        while(node->trajet.villes[i]!= NULL && i<59){
            if(node->trajet.villes[i] == *city)
                return node;
            i++;
        }
        node->trajet.villes[i] = *city;
        (*city)->ville.trajets+=1;

        return node;
    }

    if(*h!=0){
        node->eq+=*h;
        node=equilibrerAVL_s(node);
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

Ville* putInTab_t(Ville* tab, int size, Ville* tab2){
    for (int i = 0; i < size; i++){
        tab2[i] = tab[i];
    }
    return tab2;
}

void afficheTab_s(Trajet* tab,int n){
    for (int i = 0; i < n; i++){
        printf("\n%d : IDTrajet : %d, min : %f, max : %f, total : %f, steps : %d",i,tab[i].IDTrajet,tab[i].min,tab[i].max,tab[i].total,tab[i].nb_step);
    }
    
}

int partitionner_trajet(Trajet *tab, int debut, int fin) {
    double pivot = tab[fin].max - tab[fin].min;
    int i = debut - 1;
    Trajet temp;

    for (int j = debut; j < fin; j++) {
        if ((tab[j].max - tab[j].min) > pivot) {
            i++;
            temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
        }
    }
    temp = tab[i+1];
    tab[i+1] = tab[fin];
    tab[fin] = temp;
    return i + 1;
}

void triRapide_trajet(Trajet *tableau, int debut, int fin) {
    if (debut < fin) {
        int pivot = partitionner_trajet(tableau, debut, fin);

        triRapide_trajet(tableau, debut, pivot - 1);
        triRapide_trajet(tableau, pivot + 1, fin);
    }
}

int partitionner_ville(Ville *tab, int debut, int fin) {
    double pivot = tab[fin].trajets;
    int i = debut - 1;
    Ville temp;

    for (int j = debut; j < fin; j++) {
        if ((tab[j].trajets) > pivot) {
            i++;
            temp = tab[i];
            tab[i] = tab[j];
            tab[j] = temp;
        }
    }
    temp = tab[i+1];
    tab[i+1] = tab[fin];
    tab[fin] = temp;
    return i + 1;
}

int compareNames(const void* a, const void* b){
    const Ville *first = (const Ville *)a;
    const Ville *second = (const Ville *)b;

    // Use strcmp to compare the names alphabetically
    return strcmp(first->nom, second->nom);
}

void triRapide_ville(Ville *tableau, int debut, int fin) {
    if (debut < fin) {
        int pivot = partitionner_ville(tableau, debut, fin);

        triRapide_ville(tableau, debut, pivot - 1);
        triRapide_ville(tableau, pivot + 1, fin);
    }
}

int main(int argc, char *argv[]){

    //Traitement -t
    //ATTENTION : prise en compte de argv pour run via shell provisoirement enlevée, à remettre
        FILE* file = fopen("data.csv", "r");
        if (file == NULL) {
            perror("Erreur lors de l'ouverture du fichier");
            return 1;
        }

        char line[256];
        pArbre_t AVLroot = NULL;
        pArbre_s AVL_Trajet = NULL;
        int count1 = 0;
        int count2 = 0;
        pArbre_t insertedCity=NULL;
        double distance;
        int IDTrajet;
        int h1 = 0, h2 = 0;

        while (fgets(line, sizeof(line), file)){
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
            token = strtok(NULL, ";");
            distance = atof(token);

            AVLroot = insert(AVLroot, townB, &h1, &count1, 0, &insertedCity);
            AVL_Trajet = insert_trajet(AVL_Trajet, IDTrajet, distance, &h2, &count2, &insertedCity); // ATTENTION : ici j'ai réutilisé la fonction du traitement s mais y'a des étapes (notamment le countage de trajet et les distances) dont on s'en bats les couilles pour le traitement t, donc on pourrait refaire une copie de la même fonction en enlevant les trucs inutiles pour le -t
            
        }

        // On revient au debut du fichier pour prendre les villes de depart

        rewind(file);

        while (fgets(line, sizeof(line), file)){
            char *token;
            char townA[50];
            h1 = 0;
            h2 = 0;

            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);
            token = strtok(NULL, ";");
            if (atoi(token) == 1){ // Si on a l'étape 1 d'un trajet, on refait pareil
                token = strtok(NULL, ";");
                strcpy(townA, token);
                token = strtok(NULL, ";");
                token = strtok(NULL, ";");
                distance = atof(token);
                
                AVLroot = insert(AVLroot, townA, &h1, &count1, 1, &insertedCity);
                AVL_Trajet = insert_trajet(AVL_Trajet, IDTrajet, distance, &h2, &count2, &insertedCity); // ATTENTION : ici j'ai réutilisé la fonction du traitement s mais y'a des étapes (notamment le countage de trajet et les distances) dont on s'en bats les couilles pour le traitement t, donc on pourrait refaire une copie de la même fonction en enlevant les trucs inutiles pour le -t
            }
        }

        fclose(file);

        printf("\nNb villes = %d\n",count1);
        Ville* tab = (Ville*)malloc(sizeof(Ville)*count1);
        Ville* tab2 = (Ville*)malloc(sizeof(Ville)*10);

        int n = 0;

        AVL_to_Tab_t(AVLroot,tab,&n);
        triRapide_ville(tab,0,count1-1);
        
        tab2 = putInTab_t(tab,10, tab2);
        qsort(tab2, 10, sizeof(Ville), compareNames);
        afficheTab_t(tab2,10);

        free_tree_t(AVLroot);
        free_tree_s(AVL_Trajet);
        free(tab);
        free(tab2);
    
    /*
    //Traitement -s
    if(!strcmp(argv[1],"-s")){
        printf("Coucou ooo");
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
            double distance;
            int h = 0;

            // Utiliser strtok pour extraire les champs du fichier CSV
            token = strtok(line, ";");
            IDTrajet = atoi(token);

            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");
            distance = atof(token);
        
            AVL_Trajet = insert_trajet(AVL_Trajet, IDTrajet, distance, &h, &count);
        }

        fclose(file);
        
        //Mettre en tableau max-min (50 premiers)
        //Moyenne = total/nb pour utiliser en graph
        Trajet* tab = (Trajet*)malloc(sizeof(Trajet)*count);
        int n = 0;
        AVL_to_Tab_s(AVL_Trajet,tab,&n);
        //Trier max - min
        //Tester
        //triBulle_s(tab,count);
        triRapide_trajet(tab,0,count-1);
        afficheTab_s(tab,50);
        free_tree_s(AVL_Trajet);
    }*/

    return 0;
}