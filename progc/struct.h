#ifndef STRUCT_H
#define STRUCT_H

typedef struct Ville{
    int trajets;
    char nom[30];
    int premier;
} Ville;

typedef struct Arbre_t{
    Ville ville;
    int eq;
    struct Arbre_t *fg;
    struct Arbre_t *fd;
} Arbre_t;

typedef Arbre_t *pArbre_t;

typedef struct Trajet{
    int IDTrajet;
    float max;
    float min;
    float total;
    int nb_step;
    pArbre_t villes[60];
} Trajet;

typedef struct Arbre_s{
    int eq;
    Trajet trajet;
    struct Arbre_s *fg;
    struct Arbre_s *fd;
} Arbre_s;

typedef Arbre_s *pArbre_s;

#endif // STRUCT_H