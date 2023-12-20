#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct arbre{
    int val;
    int eq;
    struct arbre* fg;
    struct arbre* fd;
}Arbre;

typedef Arbre* pArbre;

typedef struct Chainon {
    pArbre arbre;
    struct Chainon* next;
}Chainon;


void supprimerFilsDroit(pArbre a);
void supprimerFilsGauche(pArbre a);

void test(pArbre a){
    if(a == NULL){
        printf("Erreur ");
        exit(1);
    }
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

pArbre creerArbre(int n){
    pArbre new = malloc(sizeof(Arbre));
    test(new);
    new->val=n;
    new->eq=0;
    new->fd=NULL;
    new->fg=NULL;
    return new;
}

int existeFilsGauche(pArbre a){
    test(a);
    return !(a->fg == NULL);
}

int existeFilsDroit(pArbre a){
    test(a);
    return !(a->fd == NULL);
}

pArbre ajouterFilsGauche(pArbre a, int e){
    if(existeFilsGauche(a)){
        return a;
    }
    a->fg = creerArbre(e);
    return a;
}

pArbre ajouterFilsDroit(pArbre a, int e){
    if(existeFilsDroit(a)){
        return a;
    }
    a->fd = creerArbre(e);
    return a;
}
//affiche la valeur dans le noeud
void traiter(pArbre a){
    if(a == NULL){
        printf("");
    }
    else{
        printf("%d ",a->val);
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

int recherche(pArbre a, int e){
    if(a){
        if(a->val==e){
            return 1;
        }
        else if(a->val>e){
            recherche(a->fg,e);
        }
        else{
            recherche(a->fd,e);
        }
    }
    else{
        return 0;
    }
}

// Nombre de noeuds parcourus
int countrecherche(pArbre a, int e, int i){ 
    if(a){
        if(a->val==e){
            return i+1;
        }
        else if(a->val>e){
            countrecherche(a->fg,e,i+1);
        }
        else{
            countrecherche(a->fd,e,i+1);
        }
    }
    else{
        printf("\nGros y a pas");
        return i+1;
    }
}

void tritabcroi(int tab[], pArbre a, int *i){
    if(a!=NULL && *i != 15){
        tritabcroi(tab,a->fg,i);
        tab[*i] = a->val;
        *i +=1;
        tritabcroi(tab,a->fd,i);
    }
}

// On suppose e pas dans a
pArbre insertABR(pArbre a, int e){ 
    if(!a){
        return creerArbre(e);
    }
    else{
        if(e < a->val){
            if(!a->fg){
                a->fg = creerArbre(e);
            }else{
                a->fg = insertABR(a->fg, e);
            }
        }else{
            if(!a->fd){
                a->fd = creerArbre(e);
            }else{
                a->fd = insertABR(a->fd, e);
            }
        }
        return a;
    }
}

// On suppose e pas dans a
pArbre NoRecinsertABR(pArbre a, int e){
    if(!a){
        return creerArbre(e);
    }
    if(recherche(a,e)){
        return a;
    }
    while(1){
        if(e < a->val){
            if(!a->fg){
                a->fg = creerArbre(e);
                return a;
            }else{
                a=a->fg;
            }
        }else{
            if(!a->fd){
                a->fd = creerArbre(e);
                return a;
            }else{
                a=a->fd;
            }
        }
    }
}



//retourne max(a,b) (logique)
int max(int a, int b){
    return a<b ? b : a;
}

//retourne min(a,b) (logique)
int min(int a, int b){
    return a<b ? a : b;
}


//Trouve le mayorant du sous arbre
int trouveMayorantGauche(pArbre a){
    if(!a){
        return INT_MIN;
    }
    int maxG = trouveMayorantGauche(a->fg);
    int maxD = trouveMayorantGauche(a->fd);

    return(max(max(a->val,maxD),maxG));
}

//Trouve le mayorant du sous arbre
int trouveMinorantDroit(pArbre a){
    if(!a){
        return INT_MAX;
    }
    int minG = trouveMinorantDroit(a->fg);
    int minD = trouveMinorantDroit(a->fd);

    return(max(max(a->val,minD),minG));
}

//Est-ce un ABR ?
int YN_ABR(pArbre a){
    if(!a){
        return 1;
    }
    if(existeFilsGauche(a) && trouveMayorantGauche(a->fg)>=a->val){
        return 0;
    }
    if(existeFilsDroit(a) && trouveMinorantDroit(a->fd)<=a->val){
        return 0;
    }
    if(YN_ABR(a->fd)==0 || YN_ABR(a->fg)==0){
        return 0;
    }
    return 1;
}

//transforme un AB en ABR
pArbre transfoABR_commencefin(pArbre a, pArbre b){
    if(!a){
        return a;
    }
    if(existeFilsDroit(a)){
        b= transfoABR_commencefin(a->fd,b);
    }
    if(existeFilsGauche(a)){
        b= transfoABR_commencefin(a->fg,b);
    }
    int temp = a->val;
    a = NULL;
    free(a);
    b = insertABR(b,temp);
    return b;
}

//transforme un AB en ABR
pArbre transfoABR_commencedebut(pArbre a, pArbre b){
    if(!a){
        return a;
    }
    else{
        b=insertABR(b,a->val);
        transfoABR_commencedebut(a->fg,b);
        transfoABR_commencedebut(a->fd,b);
    }
    return b;
}

//verifie si n est dans tab
int estdedans(int tab[],int n,int d){
    for(int i=0;i<d;i++){
        if(n==tab[i]){
            return 1;
        }
    }
    return 0;
}

//Prend les elements d'un tableau pour faire un ABR
pArbre tab_en_ABR(int tab[],pArbre a){
    for(int i=0;i<15;i++){
        a = insertABR(a,tab[i]);
    }
    return a;
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

pArbre insertionAVL(pArbre a, int e, int* h){
    if(!a){
        *h=1;
        return creerArbre(e);
    }
    else if(e<a->val){
        a->fg =  insertionAVL(a->fg,e,h);
        *h=-*h;
    }
    else if(e>a->val){
        a->fd = insertionAVL(a->fd,e,h);
    }
    else{
        *h=0;
        return a;
    }
    if(*h!=0){
        a->eq+=*h;
        if(a->eq==0){
            *h=0;
        }
        else{
            *h=1;
        }
    }
    return a;   
}

// rappel :
// équilibre = hauteur_d - hauteur_g

// Strat : trouver le max du sous-arbre gauche.
pArbre suppMaxGaucheAVL(pArbre arbre, int* h, pArbre racine) {
    if (arbre->fd) {
        arbre->fd = suppMaxGaucheAVL(arbre->fd, h, racine);
        // *h = -*h;

        if (*h != 0) {
            arbre->eq += *h;
            arbre = equilibrerAVL(arbre);
            if (arbre->eq == 0) {
                // La hauteur a été réduite car on a perdu un fils droit
                *h = -1;
            } else {
                // La hauteur n'a pas changé
                *h = 0;
            }
        }
        return arbre;
    }
    else {
        racine->val = arbre->val;
        pArbre successeur = arbre->fg;
        free(arbre);
        *h = -1;
        return successeur;
    }

}

pArbre suppAVL(pArbre arbre, int element, int *h){
    if (arbre == NULL)
    {
        *h = 1;
        return NULL;
    }
    else if (element > arbre->val)
    {
        arbre->fd = suppAVL(arbre->fd, element, h);
        // Fils droit supprimé, on laisse l'ajustement avec la valeur par défaut (-1)
        // car l'équilibre doit baisser dans ce cas.
    }
    else if (element < arbre->val)
    {
        arbre->fg = suppAVL(arbre->fg, element, h);
        // Fils gauche supprimé, donc la hauteur gauche a baissé de 1.
        // Alors on doit ajouter de l'équilibre au lieu de soustraire.
        *h = -*h; 
        
        // Explication algébrique (oui je m'ennuie) :
        // 
        // Définitions :    eq_prev = hd - hg ; 
        //
        //                  eq_nouv = hd - (hg - 1) 
        //                          = hd - hg + 1
        //
        // eq_prev + delta = eq_nouv
        // ==>  eq_nouv - eq_prev = delta
        // ==>  delta = 1 
    }
    else
    {
        // c'est notre élément !!
        if (arbre->fd == NULL && arbre->fg == NULL)
        {
            // feuille
            free(arbre);
            arbre = NULL;
            *h = -1;
        }
        else if (arbre->fg != NULL && arbre->fd == NULL)
        {
            // Remplacement par fils gauche
            pArbre tmp = arbre;
            arbre = tmp->fg;
            free(tmp);
            *h = -1; // Changement de l'hauteur pour le parent (-1 par défaut)
            return arbre; // On s'arrête là (ne pas ajuster l'équilibre d'un nœud déplacé)
        }
        else if (arbre->fg == NULL && arbre->fd != NULL)
        {
            // Remplacement par fils droit
            pArbre tmp = arbre;
            arbre = tmp->fd;
            free(tmp);
            *h = -1; // Changement de l'hauteur pour le parent (-1 par défaut)
            return arbre; // On s'arrête là (ne pas ajuster l'équilibre d'un nœud déplacé)
        } else {
            // Deux non nuls
            arbre->fg = suppMaxGaucheAVL(arbre->fg, h, arbre);
            *h = -*h; // Fils gauche retiré, donc équilibre += 1
        }
    }

    if (arbre && *h != 0)
    {
        arbre->eq += *h; // *h = -1 si fils droit, *h = 1 si fils gauche
        arbre = equilibrerAVL(arbre); // Équilibrage si nécessaire
        if (arbre->eq == 0)
        {
            // L'arbre (rééquilibré) a perdu ses deux fils, donc la hauteur diminue.
            *h = -1; // Reset
        }
        else
        {
            // L'arbre (rééquilibré) a perdu un fils, mais il en reste un autre, donc la hauteur ne change pas.
            // Alors, plus aucun équilibre ne change à partir de maintenant.
            *h = 0; // On stop
        }
    }

    return arbre;
}

int powerRec(int a, int b){
    if(b==0){
        return 1;
    }
    else{
        return a*powerRec(a,b-1);
    }
}

int main(int argc, char *argv[]){
    printf("Bonjour. Il a %d variables, %s",argc,*argv);

    return 0;
}