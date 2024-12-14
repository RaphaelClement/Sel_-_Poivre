#ifndef _MENU_H_
#define _MENU_H_

#include "personne.h"
#include "ressource.h"


extern char *mot_de_passe;

void menu_admin(Personne **personnes, Ressource **ressources);
void menu_utilisateur(Personne *personnes, Ressource *ressources);
void changer_mot_de_passe();
Personne *afficher_liste_utilisateurs(Personne *personnes);
char* lire_mot_de_passe_de_json();
void sauvegarder_mot_de_passe_json(const char *nouveau_mot_de_passe);

#endif
