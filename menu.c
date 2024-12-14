#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <json-c/json.h>
#include <crypt.h>
#include "personne.h"
#include "ressource.h"
#include "menu.h"


const char *clef = "$1$fR$"; // Clé de chiffrage du mot de passe
char *mot_de_passe = NULL;

// Fonction pour le menu administrateur
void menu_admin(Personne **personnes, Ressource **ressources) {
    char mot_de_passe_saisi[100];

    printf("Entrez le mot de passe: ");
    scanf("%99s", mot_de_passe_saisi);
    char *mot_de_passe_hashe = crypt(mot_de_passe_saisi, clef);

    if (strcmp(mot_de_passe_hashe, mot_de_passe) != 0) {
        printf("Mot de passe incorrect.\n");
        return;
    }
    printf("Mot de passe haché : %s\n", mot_de_passe);
    while (1) {
        printf("\nMenu Admin:\n");
        printf("1. Ajouter une personne\n");
        printf("2. Ajouter une ressource\n");
        printf("3. Supprimer une personne\n");
        printf("4. Supprimer une ressource\n");
        printf("5. Changer le mot de passe\n");
        printf("6. Lister toutes les personnes\n");
        printf("7. Lister toutes les ressources\n");
        printf("8. Quitter\n");
        printf("Entrez votre choix: \n");
        int choix;
        scanf("%d", &choix);

        Personne *nouvelle_personne = NULL;
        Ressource *nouvelle_ressource = NULL;
        struct json_object *nouvelle_personne_obj = NULL;
        struct json_object *nouvelle_ressource_obj = NULL;

        switch (choix) {
            case 1: {
                Personne *personne = (Personne *)malloc(sizeof(Personne));
                printf("Entrez l'ID de la personne: ");
                scanf("%d", &(personne->id));
                Personne *personne_existante = trouver_personne_par_id(*personnes, personne->id);
                if (personne_existante != NULL) {
                    printf("Une personne avec cet ID existe déjà. Veuillez en choisir une autre.\n");
                    free(personne);
                    break;
                }
                printf("Entrez le prénom de la personne: ");
                scanf("%s", personne->prenom);
                printf("Entrez le nom de la personne: ");
                scanf("%s", personne->nom);
                personne->suivant = NULL;
                ajouter_personne(personnes, personne);
                struct json_object *jobj = json_object_from_file("data.json");
                struct json_object *personnes_array = json_object_object_get(jobj, "personnes");
                nouvelle_personne_obj = json_object_new_object();
                json_object_object_add(nouvelle_personne_obj, "id", json_object_new_int(personne->id));
                json_object_object_add(nouvelle_personne_obj, "prenom", json_object_new_string(personne->prenom));
                json_object_object_add(nouvelle_personne_obj, "nom", json_object_new_string(personne->nom));
                json_object_array_add(personnes_array, nouvelle_personne_obj);
                json_object_to_file("data.json", jobj);
                json_object_put(jobj);
                printf("La personne a été ajoutée avec succès.\n");
                break;
            }
            case 2: {
                int id;
                printf("Entrez l'ID de la ressource : ");
                scanf("%d", &id);
                Ressource *ressource_existante = trouver_ressource_par_id(*ressources, id);
                if (ressource_existante != NULL) {
                    printf("Une ressource avec cet ID existe déjà. Veuillez en choisir une autre.\n");
                    break;
                }
                Ressource *nouvelle_ressource = creer_ressource("Nouvelle ressource");
                nouvelle_ressource->id = id;
                printf("Entrez le nom de la ressource: ");
                scanf("%s", nouvelle_ressource->prenom);
                ajouter_ressource(ressources, nouvelle_ressource);
                struct json_object *jobj = json_object_from_file("data.json");
                struct json_object *ressources_array = json_object_object_get(jobj, "ressources");
                nouvelle_ressource_obj = json_object_new_object();
                json_object_object_add(nouvelle_ressource_obj, "id", json_object_new_int(nouvelle_ressource->id));
                json_object_object_add(nouvelle_ressource_obj, "prenom", json_object_new_string(nouvelle_ressource->prenom));
                json_object_array_add(ressources_array, nouvelle_ressource_obj);
                json_object_to_file("data.json", jobj);
                json_object_put(jobj);
                printf("La ressource a été ajoutée avec succès.\n");
                break;
            }
            case 3:
                supprimer_personne(personnes);
                break;
            case 4:
                supprimer_ressource(ressources);
                break;
            case 5:
                changer_mot_de_passe();
                break;
            case 6:
                afficher_personne(*personnes);
                break;
            case 7:
                afficher_res_dispo(*ressources);
                break;
            case 8:
                return;
            default:
                printf("Choix invalide. Réessayez.\n");
        }
    }
}

// Fonction pour le menu utilisateur
void menu_utilisateur(Personne *personnes, Ressource *ressources) {
    int choix;
    Personne *utilisateur = afficher_liste_utilisateurs(personnes);

    while (1) {
        printf("\nMenu Utilisateur:\n");
        printf("1. Louer une ressource\n");
        printf("2. Retourner une ressource\n");
        printf("3. Lister toutes les ressources\n");
        printf("4. Quitter\n");
        printf("Entrez votre choix: \n");
        scanf("%d", &choix);

        switch (choix) {
            case 1: {
                Ressource *ressource = selectionner_ressource(ressources);
                if (ressource == NULL) {
                    printf("Ressource inexistante.\n");
                    break;
                }
                int jour_debut, mois_debut, annee_debut;
                int jour_fin, mois_fin, annee_fin;
                printf("Entrez la date de début de location (format jj/mm/aaaa): ");
                scanf("%d/%d/%d", &jour_debut, &mois_debut, &annee_debut);
                if (!date_valide(jour_debut, mois_debut, annee_debut)) {
                    printf("Date de début invalide.\n");
                    break;
                }
                printf("Entrez la date de fin de location (format jj/mm/aaaa): ");
                scanf("%d/%d/%d", &jour_fin, &mois_fin, &annee_fin);
                if (!date_valide(jour_fin, mois_fin, annee_fin)) {
                    printf("Date de fin invalide.\n");
                    break;
                }
                struct tm timeinfo_debut = {0}, timeinfo_fin = {0};
                timeinfo_debut.tm_mday = jour_debut;
                timeinfo_debut.tm_mon = mois_debut - 1;
                timeinfo_debut.tm_year = annee_debut - 1900;
                time_t date_debut = mktime(&timeinfo_debut);

                timeinfo_fin.tm_mday = jour_fin;
                timeinfo_fin.tm_mon = mois_fin - 1;
                timeinfo_fin.tm_year = annee_fin - 1900;
                time_t date_fin = mktime(&timeinfo_fin);

                louer_ressource(ressource, utilisateur, date_debut, date_fin);
                break;
            }
            case 2: {
                Ressource *ressource = selectionner_ressource(ressources);
                if (ressource == NULL) {
                    printf("Ressource inexistante.\n");
                    break;
                }
                if (ressource->emprunteur != utilisateur) {
                    printf("Cette ressource n'est pas empruntée par vous.\n");
                    break;
                }
                ressource->emprunteur = NULL;
                ressource->locations = NULL;
                printf("Ressource retournée avec succès.\n");
                break;
            }
            case 3:
                afficher_res_dispo(ressources);
                break;
            case 4:
                return;
            default:
                printf("Choix invalide. Réessayez.\n");
        }
    }
}

// Fonction pour changer le mot de passe administrateur
void changer_mot_de_passe() {
    char nouveau_mot_de_passe[100];
    printf("Entrez le nouveau mot de passe: ");
    scanf("%99s", nouveau_mot_de_passe);
    char *mot_de_passe_hashe = crypt(nouveau_mot_de_passe, clef);
    free(mot_de_passe);
    mot_de_passe = strdup(mot_de_passe_hashe);
    sauvegarder_mot_de_passe_json(mot_de_passe);
    printf("Mot de passe changé avec succès.\n");
}

// Fonction pour afficher la liste des utilisateurs
Personne *afficher_liste_utilisateurs(Personne *personnes) {
    int choix;
    Personne *actuel = personnes;
    printf("\nListe des utilisateurs:\n");
    while (actuel != NULL) {
        printf("ID: %d, %s %s\n", actuel->id, actuel->prenom, actuel->nom);
        actuel = actuel->suivant;
    }
    printf("Entrez votre choix : ");
    scanf("%d", &choix);
    while (trouver_personne_par_id(personnes, choix) == NULL) {
        printf("ID invalide. Veuillez réessayer : ");
        scanf("%d", &choix);
    }
    return trouver_personne_par_id(personnes, choix);
}

char* lire_mot_de_passe_de_json() {
    struct json_object *jobj = json_object_from_file("data.json");
    if (!jobj) {
        printf("Impossible de lire le fichier JSON.\n");
        return NULL;
    }

    struct json_object *password_obj;
    if (!json_object_object_get_ex(jobj, "password", &password_obj)) {
        const char *password_par_defaut = "admin";
        password_obj = json_object_new_string(crypt(password_par_defaut, clef));
        json_object_object_add(jobj, "password", password_obj);
        json_object_to_file("data.json", jobj);
        json_object_put(jobj);
        return strdup(json_object_get_string(password_obj));
    }

    const char *mot_de_passe = json_object_get_string(password_obj);
    char *copie_mot_de_passe = strdup(mot_de_passe);
    json_object_put(jobj);
    return copie_mot_de_passe;
}

void sauvegarder_mot_de_passe_json(const char *nouveau_mot_de_passe) {
    struct json_object *jobj = json_object_from_file("data.json");
    if (!jobj) {
        printf("Impossible de lire le fichier JSON.\n");
        return;
    }

    struct json_object *password_obj = json_object_new_string(nouveau_mot_de_passe);
    json_object_object_add(jobj, "password", password_obj);
    json_object_to_file("data.json", jobj);
    json_object_put(jobj);
}
