#include "client.h"
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
//--------decrypte moov --------------------------------------------------------------
// Fonction pour retirer le dernier caractère d'une chaîne
void retirer_a_la_fin(char* chaine) {
    int len = strlen(chaine);
    if (len > 0) {
        chaine[len - 1] = '\0';  // Retire le dernier caractère
    }
}

// Fonction pour déplacer les n derniers caractères au début
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

// Fonction de décryptage
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
//-----------------------FIN decrypte moov --------------------------------------------------------------

//------------------------ecrypte Seq----------------------------------------------------

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
//------------------------FIN ecrypte Seq----------------------------------------------------

//------------------------decrypte Seq----------------------------------------------------

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

//---------------------------fin decrypte seq------------------------------------
// Fonction principale
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

    envoyer("load crypteSeq");
    envoyer("aide");
    envoyer_recevoir("start", reponse);

    decrypte(reponse, DEC);
    encrypte(DEC, ENC);
    envoyer_recevoir(ENC, reponse);

    char DEC_2[MAXREP] = "";
    decrypte_seq(reponse, DEC_2);
    //envoyer(DEC_2);

    // Déconnexion du serveur
    deconnexion();

    return 0;
}
