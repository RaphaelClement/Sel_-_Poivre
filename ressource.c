#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ressource.h"
#include "personne.h"


// Créer une nouvelle ressource
Ressource* creer_ressource(const char *prenom) {
    Ressource *ressource = (Ressource*) malloc(sizeof(Ressource));
    static int identifiant_unique = 1;
    ressource->id = identifiant_unique++;
    strncpy(ressource->prenom, prenom, 50);
    ressource->emprunteur = NULL;
    ressource->locations = NULL;
    ressource->suivant = NULL;
    return ressource;
}

Ressource *selectionner_ressource(Ressource *ressources) {
    Ressource *actuel = ressources;
    Ressource *selectionne = NULL;
    int choix = 0;
    if (actuel == NULL) {
        printf("Aucune ressource disponible.\n");
        return NULL;
    }
    printf("Entrez l'ID de la ressource à sélectionner : ");
    scanf("%d", &choix);
    actuel = ressources;
    while (actuel != NULL) {
        if (actuel->id == choix) {
            selectionne = actuel;
            break;
        }
        actuel = actuel->suivant;
    }
    if (selectionne == NULL) {
        printf("Ressource non trouvée.\n");
    }
    return selectionne;
}

// Rechercher une ressource par son identifiant
Ressource *trouver_ressource_par_id(Ressource *ressources, int id) {
    while (ressources) {
        if (ressources->id == id) {
            return ressources;
        }
        ressources = ressources->suivant;
    }
    return NULL;
}

// Afficher les informations et disponibilité d'une ressource
void afficher_res_dispo(Ressource *ressources) {
    Ressource *current = ressources;
    while (current != NULL) {
        printf("Ressource ID: %d\n", current->id);
        printf("Nom: %s\n", current->prenom);
        if (current->emprunteur != NULL) {
            printf("Emprunteur: %s %s\n", current->emprunteur->prenom, current->emprunteur->nom);
            Location *location = current->locations;
            while (location != NULL) {
                struct tm *debut = localtime(&location->date_debut);
                struct tm *fin = localtime(&location->date_fin);
                printf("Du %02d/%02d/%04d au %02d/%02d/%04d\n", debut->tm_mday, debut->tm_mon + 1, debut->tm_year + 1900, fin->tm_mday, fin->tm_mon + 1, fin->tm_year + 1900);
                location = location->suivant;
            }
        } else {
            printf("Pas d'emprunteur.\n");
        }
        printf("\n");
        current = current->suivant;
    }
}

// Louer une ressource
void louer_ressource(Ressource *ressource, Personne *emprunteur, time_t date_debut, time_t date_fin) {
    if (verifier_conflit_dates(ressource->locations, date_debut, date_fin)) {
        printf("Les dates sélectionnées sont déjà prises.\n");
        return;
    }
    Location *nouvelle_location = malloc(sizeof(Location));
    nouvelle_location->date_debut = date_debut;
    nouvelle_location->date_fin = date_fin;
    nouvelle_location->suivant = ressource->locations;
    ressource->locations = nouvelle_location;
    ressource->emprunteur = emprunteur;
    printf("Ressource empruntée.\n");
}

// Vérifier la validité d'une date
int date_valide(int jour, int mois, int annee) {
    struct tm timeinfo = {0};
    timeinfo.tm_mday = jour;
    timeinfo.tm_mon = mois - 1;
    timeinfo.tm_year = annee - 1900;
    time_t t = mktime(&timeinfo);
    struct tm *res = localtime(&t);
    return res->tm_mday == jour && res->tm_mon == mois - 1 && res->tm_year == annee - 1900;
}

// Vérifier les conflits de dates pour une location
int verifier_conflit_dates(Location *locations, time_t debut, time_t fin) {
    Location *current = locations;
    while (current != NULL) {
        if ((debut >= current->date_debut && debut <= current->date_fin) ||
            (fin >= current->date_debut && fin <= current->date_fin) ||
            (debut <= current->date_debut && fin >= current->date_fin)) {
            return 1; // Conflit de dates
        }
        current = current->suivant;
    }
    return 0; // Pas de conflit
}

// Ajouter une ressource à la liste chaînée
void ajouter_ressource(Ressource** ressources, Ressource* nouvelle_ressource) {
	if (*ressources == NULL) {
    	*ressources = nouvelle_ressource;
	} else {
    	Ressource* actuel = *ressources;
    	while (actuel->suivant != NULL) {
        	actuel = actuel->suivant;
    	}
    	actuel->suivant = nouvelle_ressource;
	}
}

// Supprimer une ressource de la liste chaînée
void supprimer_ressource(Ressource **ressources) {
  int id;
  printf("Entrez l'ID de la ressource à supprimer : ");
  scanf("%d", &id);
  Ressource *precedent = NULL;
  Ressource *actuel = *ressources;
  while (actuel != NULL) {
    if (actuel->id == id) {
      if (precedent == NULL) {
        *ressources = actuel->suivant;
      } else {
        precedent->suivant = actuel->suivant;
      }
      liberer_ressource(actuel);
      printf("La ressource a été supprimée avec succès.\n");
      return;
    }
    precedent = actuel;
    actuel = actuel->suivant;
  }
  printf("La ressource n'a pas été trouvée.\n");
}

// Libérer la mémoire occupée par une ressource
void liberer_ressource(Ressource *ressource) {
  free(ressource);
}
