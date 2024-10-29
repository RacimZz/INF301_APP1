#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
char decalage_car(char a, int decalage) {
    if (a >= 'a' && a <= 'z') {
        return (a - decalage < 'a') ? (a - decalage + 26) : (a - decalage);
    } else if (a >= 'A' && a <= 'Z') {
        return (a - decalage < 'A') ? (a - decalage + 26) : (a - decalage);
    }
    return a; // Si ce n'est pas une lettre, on ne fait rien
}

// Vérifier si un caractère est une lettre
bool est_alpha(char a) {
    return (a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z');
}

// Fonction principale pour décaler une chaîne de caractères
void decaler_chaine(const char *chaine, int decalage, char *resultat) {
    int i;
    int len = strlen(chaine);
    for (i = 0; i < len; i++) {
        if (est_alpha(chaine[i])) {
            resultat[i] = decalage_car(chaine[i], decalage);
        } else {
            resultat[i] = chaine[i]; // Conserver les autres caractères tels quels
        }
    }
    resultat[i] = '\0'; // Ajouter le caractère nul de fin de chaîne
}

int main() {
    // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);
    char reponse[10000];
    char message[MAXREP];
    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer("login 12315200 ZAIDI");
    envoyer("load planB");
    envoyer_recevoir("aide",message);

    int decalage =  message[0]-'C';

    decaler_chaine("hasta la revolucion",decalage,reponse);
    envoyer("start");

    envoyer_recevoir(reponse,reponse);
    char message_2[MAXREP];

    decaler_chaine(reponse, 5, message_2);
    envoyer(message_2);
    //printf ("Fin d'envoi des messages.\n");
    deconnexion();
    //printf ("Fin de la connection au serveur\n");
    return 0;
}
