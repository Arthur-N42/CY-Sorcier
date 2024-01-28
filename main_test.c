#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct Ville
{
    int trajets;
    char nom[30];
    int premier;
} Ville;

typedef struct Arbre_t
{
    Ville ville;
    int eq;
    struct Arbre_t *fg;
    struct Arbre_t *fd;
} Arbre_t;

typedef Arbre_t *pArbre_t;

typedef struct Trajet
{
    int IDTrajet;
    double max;
    double min;
    double total;
    int nb_step;
    pArbre_t villes[60]; // Note à moi même : peut être possibilité si j'ai le courage d'implémenter une table de hachage au lieu d'un tableau normal pour gagner un peu en vitesse
} Trajet;

typedef struct Arbre_s
{
    int eq;
    Trajet trajet;
    struct Arbre_s *fg;
    struct Arbre_s *fd;
} Arbre_s;

typedef Arbre_s *pArbre_s;

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
    new->ville.premier = flag;

    new->eq = 0;
    new->fd = NULL;
    new->fg = NULL;
    return new;
}

// Constructeur d'un noeud pour les AVL de Trajet (traitement S)
pArbre_s creerArbre_Trajet(int ID, int distance)
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

int max(int a, int b)
{
    return (a <= b) ? b : a;
}

int min(int a, int b)
{
    return (a <= b) ? a : b;
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

// Traitement -s : Fonction recursive pour inserer un noeud dans l'arbre AVL de Trajet selon un trajet
pArbre_s insert_trajet_s(pArbre_s node, int ID, double distance, int *h, int *count, pArbre_t *city){

    if (node == NULL){ // Le trajet n'est pas déjà dans l'AVL, créer son noeud et le rajouter. Incrémenter le compte du nombre d'éléments de l'AVL aussi
        *h = 1;
        *count = *count + 1;
        node = creerArbre_Trajet(ID, distance);
        return node;
    }

    if (ID < node->trajet.IDTrajet){
        node->fg = insert_trajet_s(node->fg, ID, distance, h, count, city);
        *h = -*h;
    }

    else if (ID > node->trajet.IDTrajet){
        node->fd = insert_trajet_s(node->fd, ID, distance, h, count, city);
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

// TRAITEMENT -T UNIQUEMENT : Fonction recursive pour inserer un noeud dans l'arbre AVL de Trajet selon un trajet
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

void free_tree_s(pArbre_s node){
    if (node != NULL){
        free_tree_s(node->fg);
        free_tree_s(node->fd);
        free(node);
    }
}

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

// Afficher les éléments d'un tableau de trajets selon le format : ID du trajet, distance minimum parcourue, distance maximum parcourue, distance totale parcourue, nombre d'étapes
void afficheTab_s(Trajet *tab, int n){
    for (int i = 0; i < n; i++){
        printf("\n%d : IDTrajet : %d, min : %f, max : %f, total : %f, steps : %d", i, tab[i].IDTrajet, tab[i].min, tab[i].max, tab[i].total, tab[i].nb_step);
    }
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

int main(int argc, char *argv[]){

    // Traitement -t
    // ATTENTION : prise en compte de argv pour run via shell provisoirement enlevée, à remettre
    FILE *file = fopen("data.csv", "r");
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

    printf("\nNb villes = %d\n", count);
    Ville *tab = (Ville *)malloc(sizeof(Ville) * count); // tableau de toutes les villes
    Ville *tab2 = (Ville *)malloc(sizeof(Ville) * 10); // tableau des 10 premières villes avec le + de trajets qui la parcourent
    int n = 0;

    AVL_to_Tab_t(AVL_Ville, tab, &n);
    qsort(tab, count, sizeof(Ville), compareTrajets); // Trier toutes les villes par celles qui ont le + de trajets qui les parcourent
    tab2 = putInTab_t(tab, 10, tab2); // mettre les 10 premières villes du tableau dans le nouveau tableau
    qsort(tab2, 10, sizeof(Ville), compareNames); // Trier par ordre alphabétique des noms

    afficheTab_t(tab2, 10);

    FILE *new = fopen("temp/data_t.txt", "w");

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

    return 0;
}