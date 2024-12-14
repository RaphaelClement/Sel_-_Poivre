#ifndef __PERSONNE_H__
#define __PERSONNE_H__


// Structure de données représentant une personne
typedef struct Personne {
  int id;
  char prenom[50];
  char nom[50];
  struct Personne *res_possede;
  struct Personne *res_emprunt;
  struct Personne *suivant;
} Personne;

Personne* creer_personne(int id, const char *prenom, const char *nom);
Personne *trouver_personne_par_id(Personne *personnes, int id);
void afficher_personne(Personne *personne);
void ajouter_personne(Personne** personnes, Personne *nouvelle_personne);
void supprimer_personne(Personne **personnes);
void liberer_personne(Personne *personne);

#endif
