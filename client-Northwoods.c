#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
int trouver_indice(char c, char seq[]) {
    int len = strlen(seq);
    for (int i = 0; i < len; i++) {
        if (seq[i] == c) {
            return i;
        }
    }
    return -1;
}

// Fonction d'encryption selon l'algorithme CrypteSeq
void encrypte(char* TXT, char* ENC) {
    char seq[MAXREP] = "";  // Séquence des caractères rencontrés
    int i = 0;  // Indice dans le texte

    // Parcours du texte à crypter
    while (TXT[i] != '\0') {
        char c = TXT[i];  // Caractère courant
        int indice = trouver_indice(c, seq);  // Cherche le caractère dans la séquence

        if (indice == -1) {
            // Si c'est la première fois que le caractère apparaît
            strncat(seq, &c, 1);  // Ajoute le caractère à la séquence
            ENC[i] = c;  // Laisse le caractère inchangé dans le message crypté
        } else {
            // Si le caractère a déjà été rencontré
            if (indice == 0) {
                ENC[i] = seq[strlen(seq) - 1];  // Remplace par le dernier caractère de la séquence
            } else {
                ENC[i] = seq[indice - 1];  // Remplace par le caractère précédent dans la séquence
            }
            size_t len = strlen(seq);  // Mettre à jour la longueur de la séquence
            // Déplace le caractère à la fin de la séquence
            for (size_t j = indice; j < len - 1; j++) {
                seq[j] = seq[j + 1];
            }
            seq[len - 1] = c;
        }

        i++;
    }
    ENC[i] = '\0';  // Terminer la chaîne cryptée
}

void decrypte_seq(char* ENC, char* DEC) {
    char seq[MAXREP] = "";  // Séquence des caractères rencontrés
    int i = 0;  // Indice dans le texte crypté

    // Parcours du texte crypté
    while (ENC[i] != '\0') {
        char c = ENC[i];  // Caractère courant
        int indice = trouver_indice(c, seq);  // Cherche le caractère dans la séquence

        if (indice == -1) {
            // Si c'est la première fois que le caractère apparaît
            char temp[2] = {c, '\0'};  // Crée une chaîne temporaire avec le caractère
            strncat(seq, temp, 1);  // Ajoute le caractère à la séquence
            DEC[i] = c;  // Laisse le caractère inchangé dans le message décrypté
        } else {
            int len_r = strlen(seq);  // Longueur actuelle de la séquence
            // Si le caractère a déjà été rencontré
            if (indice == len_r - 1) {
                DEC[i] = seq[0];
                indice = 0;  // Remplace par le premier caractère de la séquence
            } else {
                DEC[i] = seq[indice + 1];
                indice++; // Remplace par le caractère suivant dans la séquence
            }

            // Mise à jour de la séquence : déplacer le caractère à la fin
            char caractere_a_deplacer = DEC[i];  // Sauvegarde du caractère à déplacer

            // Gestion du déplacement
            if (indice == 0) {
                // Si c'est le premier caractère, il suffit de décaler les caractères
                memmove(seq, seq + 1, len_r - 1);  // Décale à gauche
            } else {
                // Sinon, on décale tous les caractères entre indice et la fin
                for (int j = indice; j < len_r - 1; j++) {
                    seq[j] = seq[j + 1];  // Décale à gauche
                }
            }

            seq[len_r - 1] = caractere_a_deplacer;  // Met le caractère à la fin
        }

        i++;
    }
    DEC[i] = '\0';  // Terminer la chaîne décryptée
}

void extraire_mot_de_passe(char* message, char* password) {
    char c[2] = " \'";
    int len = strlen(message);
    int i = 0;
    for (i=0; i < len; i++){
        if (message[i] == c[0] && message[i+1] == c[1]){
            int j = i + 2;
            int k = 0;
            while (message[j]!= c[1] && message[j]!= '\0'){
                password[k] = message[j];
                k++;
                j++;
            }
            password[k] = '\0';
            break;
        }

    }
    }

int main() {
        // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);
    char DEC[MAXREP] = "";
    char reponse[MAXREP];
    char ENC[MAXREP] = "";



    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer("login 12315200 ZAIDI");
    //envoyer("login 12315199 ZENATI");

    envoyer("load Northwoods");
    envoyer("aide");
    envoyer_recevoir("start",reponse);
    //decrypte(reponse, DEC);
    envoyer_recevoir("hasta la victoria siempre",reponse);
    //encrypte(DEC, ENC);
    decrypte_seq(reponse, DEC);

    //envoyer(DEC);

    char password[100];
    extraire_mot_de_passe(DEC,password);

    envoyer_recevoir(password, reponse);
    decrypte_seq(reponse, DEC);
    //envoyer(DEC);
    extraire_mot_de_passe(DEC, password);
    encrypte(password, ENC);
    envoyer_recevoir(ENC,reponse);
    decrypte_seq(&reponse[24], DEC);//pour retirer le access........
    envoyer(DEC);

    deconnexion();

    return 0;
}




