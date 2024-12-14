#ifndef __RESSOURCE_H__
#define __RESSOURCE_H__

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "personne.h"


// Structure de données représentant une ressource
typedef struct Location {
    time_t date_debut;
    time_t date_fin;
    struct Location *suivant;
} Location;

typedef struct Ressource {
    int id;
    char prenom[50];
    struct Personne *emprunteur;
    struct Location *locations;
    struct Ressource *suivant;
} Ressource;

Ressource* creer_ressource(const char *prenom);
Ressource *selectionner_ressource(Ressource *ressources);
Ressource *trouver_ressource_par_id(Ressource *ressources, int id);
void afficher_res_dispo(Ressource *ressources);
void louer_ressource(Ressource *ressource, Personne *emprunteur, time_t date_debut, time_t date_fin);
int date_valide(int jour, int mois, int annee);
int verifier_conflit_dates(Location *locations, time_t debut, time_t fin);
void ajouter_ressource(Ressource** ressources, Ressource *nouvelle_ressource);
void supprimer_ressource(Ressource **ressources);
void liberer_ressource(Ressource *ressource);

#endif
