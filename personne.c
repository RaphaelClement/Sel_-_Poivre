#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "personne.h"


// Créer une nouvelle personne
Personne* creer_personne(int id, const char *prenom, const char *nom) {
    Personne *personne = malloc(sizeof(Personne));
    if (!personne) {
        return NULL;
    }
    personne->id = id;
    strncpy(personne->prenom, prenom, 50);
    strncpy(personne->nom, nom, 50);
    personne->res_possede = NULL;
    personne->res_emprunt = NULL;
    personne->suivant = NULL;
    return personne;
}

// Rechercher une personne par son identifiant
Personne *trouver_personne_par_id(Personne *personnes, int id) {
  Personne *actuel = personnes;
  while (actuel != NULL) {
    if (actuel->id == id) {
      return actuel;
    }
    actuel = actuel->suivant;
  }
  return NULL;
}

// Afficher les informations d'une personne
void afficher_personne(Personne *personnes) {
    Personne *actuel = personnes;

    printf("\nListe des personnes:\n");
    while (actuel != NULL) {
        printf("%d. %s %s\n", actuel->id, actuel->prenom, actuel->nom);
        actuel = actuel->suivant;
    }
}

// Ajouter une personne à la liste chaînée
void ajouter_personne(Personne** personnes, Personne* nouvelle_personne) {
	if (*personnes == NULL) {
    	*personnes = nouvelle_personne;
	} else {
    	Personne* actuel = *personnes;
    	while (actuel->suivant != NULL) {
        	actuel = actuel->suivant;
    	}
    	actuel->suivant = nouvelle_personne;
	}
}

// Supprimer une personne de la liste chaînée
void supprimer_personne(Personne **personnes) {
  int id;
  printf("Entrez l'ID de la personne à supprimer : ");
  scanf("%d", &id);
  Personne *precedent = NULL;
  Personne *actuel = *personnes;
  while (actuel != NULL) {
    if (actuel->id == id) {
      if (precedent == NULL) {
        *personnes = actuel->suivant;
      } else {
        precedent->suivant = actuel->suivant;
      }
      liberer_personne(actuel);
      printf("La personne a été supprimée avec succès.\n");
      return;
    }
    precedent = actuel;
    actuel = actuel->suivant;
  }
  printf("La personne n'a pas été trouvée.\n");
}

// Libérer la mémoire occupée par une personne et ses ressources
void liberer_personne(Personne *personne) {
  Personne *ressource = personne->res_possede;
  while (ressource != NULL) {
	Personne *suivant = ressource->suivant;
	liberer_personne(ressource);
	ressource = suivant;
  }
  ressource = personne->res_emprunt;
  while (ressource != NULL) {
	Personne *suivant = ressource->suivant;
	liberer_personne(ressource);
	ressource = suivant;
  }
  free(personne);
}
