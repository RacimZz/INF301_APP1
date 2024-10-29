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

// Structure pour représenter un caractère et son association
typedef struct {
    char caractere;
    char associe;
} Association;

int find_index(char c, Association seq[], int taille_seq) {
    for (int i = 0; i < taille_seq; i++) {
        if (seq[i].caractere == c) {
            return i;
        }
    }
    return -1;
}

// Fonction d'encryption selon la nouvelle logique CrypteAssoc
void crypteassoc(char* TXT, char* ENC) {
    Association seq[MAXREP];  // Séquence des caractères rencontrés et leurs associations
    int taille_seq = 0;       // Taille actuelle de la séquence
    int i = 0;                // Indice dans le texte

    // Parcours du texte à crypter
    while (TXT[i] != '\0') {
        char c = TXT[i];  // Caractère courant
        int indice = find_index(c, seq, taille_seq);  // Cherche le caractère dans la séquence

        if (indice == -1) {
            // Si c'est la première fois que le caractère apparaît
            seq[taille_seq].caractere = c;
            seq[taille_seq].associe = c;  // S'associe à lui-même
            ENC[i] = c;  // Le caractère reste inchangé dans le message crypté
            taille_seq++;
        } else {
            // Si le caractère a déjà été rencontré
            int indice_prec;
            if (indice == 0) {
                indice_prec = taille_seq - 1;  // Si l'indice est 0, on prend le dernier caractère de la séquence
            } else {
                indice_prec = indice - 1;  // Sinon, on prend le caractère précédent dans la séquence
            }
            // Échange des associations
            char temp_associe = seq[indice].associe;
            seq[indice].associe = seq[indice_prec].associe;
            seq[indice_prec].associe = temp_associe;

            // Le caractère de sortie est celui associé au caractère d'entrée
            ENC[i] = seq[indice].associe;

            // Déplace le caractère actuel à la fin de la séquence
            Association temp = seq[indice];
            for (int j = indice; j < taille_seq - 1; j++) {
                seq[j] = seq[j + 1];
            }
            seq[taille_seq - 1] = temp;
        }

        i++;
    }

    ENC[i] = '\0';  // Terminer la chaîne cryptée
}

int trouver_indice_associe(char c, Association seq[], int taille_seq) {
    for (int i = 0; i < taille_seq; i++) {
        if (seq[i].associe == c) {
            return i;  // Retourner l'indice du caractère associé
        }
    }
    return -1;  // Retourne -1 si le caractère associé n'est pas trouvé
}

void decrypteassoc(char* ENC, char* DEC) {
    Association seq[MAXREP];  // Séquence des caractères rencontrés et leurs associations
    int taille_seq = 0;       // Taille actuelle de la séquence
    int i = 0;                // Indice dans le texte crypté

    // Parcours du texte crypté
    while (ENC[i] != '\0') {
        char c = ENC[i];  // Caractère courant dans le texte crypté
        int indice = trouver_indice_associe(c, seq, taille_seq);  // Cherche le caractère associé dans la séquence

        if (indice == -1) {
            // Si c'est la première fois que le caractère associé apparaît
            seq[taille_seq].caractere = c;
            seq[taille_seq].associe = c;  // S'associe à lui-même
            DEC[i] = c;  // Le caractère reste inchangé dans le message décrypté
            taille_seq++;
        } else {
            // Si le caractère associé a déjà été rencontré
            int indice_prec;

            if (indice == taille_seq - 1) {
                indice_prec = 0 ;  // Si l'indice est 0, on prend le dernier caractère de la séquence
            } else {
                indice_prec = indice + 1;  // Sinon, on prend le caractère précédent dans la séquence
            }

            // Échange des associations
            char temp_associe = seq[indice].associe;
            seq[indice].associe = seq[indice_prec].associe;
            seq[indice_prec].associe = temp_associe;

            // Le caractère de sortie est celui correspondant à l'association inverse
            DEC[i] = seq[indice_prec].caractere;

            // Déplace le caractère actuel à la fin de la séquence
            Association temp = seq[indice_prec];
            for (int j = indice_prec; j < taille_seq - 1; j++) {
                seq[j] = seq[j + 1];
            }
            seq[taille_seq - 1] = temp;
        }

        i++;
    }

    DEC[i] = '\0';  // Terminer la chaîne décryptée
}


int main() {
    char DEC[MAXREP] = "";
    char reponse[MAXREP];
    //char ENC[MAXREP] = "";

    // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);
    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");
    // Remplacez <identifiant> et <mot de passe> ci dessous.
    envoyer("login 12315200 ZAIDI");
    //envoyer("login 12315199 ZENATI");

    envoyer("load LostCause");

    envoyer_recevoir("aide",reponse);
    envoyer_recevoir("start",reponse);
    decrypteassoc(reponse,DEC);
    //envoyer(DEC);
    envoyer("tout va bien");
    //deconnexion();

    return 0;
}
