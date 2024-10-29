#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
void retirer_a_la_fin(char* chaine) {
    int len = strlen(chaine);
    if (len > 0) {
        chaine[len - 1] = '\0';  // Retire le dernier caractère
    }
}


void deplacer_n_car_au_debut(char* TXT, int n) {
    int len = strlen(TXT);
    if (n > len) {
        n = len;
    }

    char temp[1000];
    strncpy(temp, TXT+len-n, n);
    temp[n] = '\0';

    memmove(&TXT[n], TXT, len - n);
    strncpy(TXT, temp,n);
}

void decrypte(char* ENC, char* DEC) {
    while (ENC[0] != '\0') {
        // Récupère le dernier caractère du message crypté
        char dernier_char = ENC[strlen(ENC) - 1];

        // Retire ce dernier caractère de la chaîne cryptée
        retirer_a_la_fin(ENC);

        // Insère ce caractère au début de la chaîne décryptée
        memmove(&DEC[1], DEC, strlen(DEC) + 1);
        DEC[0] = dernier_char;

        // Déplace un nombre de caractères modulo 8 depuis la fin vers le début
        int nb_car_a_deplacer = dernier_char % 8;
        deplacer_n_car_au_debut(&DEC[1], nb_car_a_deplacer);
    }
}


int main() {
        // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);




    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer("login 12315200 ZAIDI");
    //envoyer("login 12315199 ZENATI");

    envoyer("load BayOfPigs");
    char reponse[MAXREP];
    char DEC[MAXREP] = "";

    envoyer("aide");
    envoyer_recevoir("start",reponse);
    envoyer_recevoir("Par otuam eriet",reponse);
    decrypte(reponse, DEC);

    envoyer(DEC);




    //deconnexion();

    return 0;
}