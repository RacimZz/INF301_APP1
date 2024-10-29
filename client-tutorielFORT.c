#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
void convertir_maj(char *rep) {
    for (unsigned long int i = 0; i < strlen(rep); i++) {
        rep[i] = toupper((unsigned char)rep[i]);
    }
}


int main() {
    // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer("login 12315200 \"ZAIDI\"");
    envoyer("load tutorielFORT");
    /*envoyer("start");
    envoyer("oui");
    envoyer("4");
    envoyer("blanc");
    envoyer("Pincemoi");
    envoyer("tutoriel");*/

    envoyer("help");
    envoyer("start");
    char reponse[MAXREP]; // à placer au début de la fonction 'main' car
                           // servira à stocker la réponse

    envoyer_recevoir("message à envoyer au serveur", reponse);
    printf("Réponse du serveur: %s", reponse);
    envoyer("OK");

    envoyer_recevoir("OUI", reponse);
    int i=0;
    while(i < 6){
        convertir_maj(reponse);
        envoyer_recevoir(reponse,reponse);
        i++;
    }


    printf ("Fin d'envoi des messages.\n");
    printf ("Pour envoyer d'autres lignes, ajouter des appels à la fonction `envoyer`\n");
    deconnexion();
    printf ("Fin de la connection au serveur\n");
    return 0;
}
