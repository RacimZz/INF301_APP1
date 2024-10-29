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

char decalage_car(char a) {
   int a_ascii = (int)a - 5;
   return (char)a_ascii;
}

// Fonction pour gérer le dépassement de bornes
char depassage(char a) {
    int a_ascii = (int)a + 26;
    return (char)a_ascii;
}

// Fonction pour vérifier si un caractère est alphabétique
bool est_alpha(char a) {
    return ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z'));
}

// Fonction de chiffrement César
void cesar(char message[],char final[]) {

    int i;
    int len =strlen(message);
    for (i = 0; i < len; i++) {
        // Si le caractère est alphabétique
        if (est_alpha(message[i])) {
            message[i] = decalage_car(message[i]);

            // Gestion du dépassement des bornes ASCII
            if ((message[i] < 'A') || (message[i] > 'Z' && message[i] < 'a')) {
                message[i] = depassage(message[i]);
            }
        }

        // Transfert dans la chaîne finale
        final[i] = message[i];
    }

    final[i] = '\0'; // Fin de chaîne


}

int main() {
    char reponse[MAXREP];
    //char DEC[MAXREP];
    // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer("login 12315200 \"ZAIDI\"");
    envoyer("load projetX");

    envoyer_recevoir("help",reponse);
    //cesar(reponse,DEC);
    //envoyer(DEC);
    envoyer("start");
    envoyer("veni vidi vici");
    deconnexion();
    printf ("Fin de la connection au serveur\n");
    return 0;
}
