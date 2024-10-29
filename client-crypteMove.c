#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

#include <stddef.h> // Pour size_t




void rajouter_a_la_fin(char* chaine, char c) {
    int len = strlen(chaine);
    chaine[len] = c;
    chaine[len + 1] = '\0';
}

void extraire_n_car_et_deplacer(char* TXT, int n) {
    int len = strlen(TXT);
    if (n > len) {
        n = len;
    }

    char temp[1000];
    strncpy(temp, TXT, n);
    temp[n] = '\0';

    memmove(TXT, &TXT[n], len - n + 1);
    strcat(TXT, temp);
}

void crypte_message(char* TXT, char* ENC) {

    while (TXT[0] != '\0') {
        char premier_caractere = TXT[0];
        rajouter_a_la_fin(ENC, premier_caractere);

        int nb_car_a_extraire = premier_caractere % 8;
        extraire_n_car_et_deplacer(&TXT[1], nb_car_a_extraire);
        //printf("TXT : %s\n", TXT);
        memmove(TXT, TXT + 1, strlen(TXT));
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

    envoyer("load crypteMove");
    char reponse[MAXREP];
    char ENC[MAXREP] = "";

    //envoyer("aide");
    envoyer_recevoir("aide",reponse);
    envoyer("start");

    crypte_message(reponse, ENC);
    envoyer(ENC);

    deconnexion();

    return 0;
}