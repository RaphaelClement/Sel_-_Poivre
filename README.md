# Sel_-_Poivre
Un programme en langage C permettant de gérer un parc de ressources physiques (PC, livres, capteurs, outils, formation ...) ainsi que les prêts de ces ressources.

La première action à effectuer est de compiler l’ensemble des fichiers à l’aide du Makefile.
Pour ce faire, il suffit de taper ‘make’, puis d’exécuter le fichier main. Il ne reste alors plus
qu’à suivre les indications qui s’affichent à l’écran, d’abord pour choisir entre le menu
administrateur et le menu utilisateur. Si vous sélectionnez le menu administrateur, il vous
sera demandé le mot de passe, celui n’est pas très compliqué (il s’agit de admin) mais est
crypté par une clé unique. Si vous sélectionnez le menu utilisateur, vous pourrez ainsi vous
identifier parmi la liste de personne pour savoir qui a emprunté telle ressource, mais aussi
pour que vous puissiez rendre seulement les ressources empruntées par vous.
Afin de supprimer tous les fichiers .o ainsi que le fichier exécutable, vous pouvez taper la
commande ‘make clean’.
