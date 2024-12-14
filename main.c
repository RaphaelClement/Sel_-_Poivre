#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <json-c/json.h>
#include "personne.h"
#include "ressource.h"
#include "menu.h"


// Liste chaînée de personnes et de ressources
Personne *liste_personnes = NULL;
Ressource *liste_ressources = NULL;

int main() {
    // Lire le mot de passe administrateur depuis un fichier JSON
    mot_de_passe = lire_mot_de_passe_de_json();
    if (!mot_de_passe) {
        printf("Erreur lors de la lecture du mot de passe initial.\n");
        return 1;
    }

    // Charger les données depuis le fichier JSON
    struct json_object *jobj = json_object_from_file("data.json");
    if (!jobj) {
        printf("Erreur lors de la lecture du fichier JSON.\n");
        return 1;
    }

    // Récupérer les listes de personnes et de ressources depuis le fichier JSON
    struct json_object *personnes_array = json_object_object_get(jobj, "personnes");
    int longueur_personnes = json_object_array_length(personnes_array);
    for (int i = 0; i < longueur_personnes; i++) {
        struct json_object *personne_obj = json_object_array_get_idx(personnes_array, i);
        struct json_object *id_obj = json_object_object_get(personne_obj, "id");
        int id = json_object_get_int(id_obj);
        struct json_object *prenom_obj = json_object_object_get(personne_obj, "prenom");
        const char *prenom = json_object_get_string(prenom_obj);
        struct json_object *nom_obj = json_object_object_get(personne_obj, "nom");
        const char *nom = json_object_get_string(nom_obj);
        Personne *personne = creer_personne(id, prenom, nom);
        ajouter_personne(&liste_personnes, personne);
    }

    struct json_object *ressources_array = json_object_object_get(jobj, "ressources");
    int longueur_ressources = json_object_array_length(ressources_array);
    for (int i = 0; i < longueur_ressources; i++) {
        struct json_object *ressource_obj = json_object_array_get_idx(ressources_array, i);
        struct json_object *id_obj = json_object_object_get(ressource_obj, "id");
        int id = json_object_get_int(id_obj);
        struct json_object *prenom_obj = json_object_object_get(ressource_obj, "prenom");
        const char *prenom = json_object_get_string(prenom_obj);
        Ressource *ressource = creer_ressource(prenom);
        ressource->id = id;
        ajouter_ressource(&liste_ressources, ressource);
    }
    json_object_put(jobj);

    // Boucle principale du programme
    int choix;
    while (1) {
        printf("Choisissez : 1. Admin, 2. Utilisateur, 3. Quitter\n");
        scanf("%d", &choix);
        if (choix == 1) {
            menu_admin(&liste_personnes, &liste_ressources);
        } else if (choix == 2) {
            menu_utilisateur(liste_personnes, liste_ressources);
        } else if (choix == 3) {
            printf("Au revoir !\n");
            break;
        } else {
            printf("Choix invalide. Réessayez.\n");
        }
    }

    // Libérer la mémoire occupée par les personnes et les ressources
    liberer_personne(liste_personnes);
    liberer_ressource(liste_ressources);
    free(mot_de_passe);
    return 0;
}
