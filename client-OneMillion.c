#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define MAXMSG MAXREP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Nouvelle fonction pour décrypter en utilisant un mouvement similaire
void decrypt_move_fast(char * encrypted, char * output) {
    // Allocation d'un tampon pour stocker les données
    char * buffer = (char *)malloc(8000000);
    if (buffer == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire\n");
        return;  // Sortir si l'allocation échoue
    }

    int start_idx = 7999999; // Index de départ dans le tampon
    int end_idx = 7999999;   // Index de fin dans le tampon
    int buffer_length = 0;   // Longueur actuelle du tampon
    int modulo_8, j;         // Variable pour stocker le modulo 8
    int id_encrypted = strlen(encrypted) - 1; // Index pour parcourir le message chiffré
    unsigned long length_output = strlen(output); // Longueur de la chaîne de sortie

    // Copier le texte de sortie dans le tampon
    if (length_output > 0) {
        strcpy(buffer + start_idx - length_output, output);
        start_idx -= length_output; // Mettre à jour l'index de départ
        buffer_length = length_output; // Mettre à jour la longueur du tampon
    }

    // Boucle pour traiter le message chiffré
    while (id_encrypted >= 0) {
        modulo_8 = encrypted[id_encrypted] % 8; // Calculer le modulo 8
        // Déplacer les caractères en fonction du modulo
        if (buffer_length > modulo_8) {
            for (j = end_idx - modulo_8; j < end_idx; j++) {
                buffer[j - buffer_length] = buffer[j];
            }
            start_idx -= modulo_8; // Mettre à jour l'index de départ
            end_idx -= modulo_8;   // Mettre à jour l'index de fin
        }
        // Ajouter le caractère chiffré au tampon
        buffer[--start_idx] = encrypted[id_encrypted--];
        buffer_length++; // Augmenter la longueur du tampon
    }

    buffer[end_idx] = '\0'; // Terminer la chaîne
    strcpy(output, buffer + start_idx); // Copier le résultat dans output
    free(buffer); // Libérer la mémoire allouée
}


int main() {
    char server_response[MAXREP]; // Pour stocker la réponse du serveur

    // Affiche les échanges avec le serveur (false pour désactiver)
    show_messages(true);

    // Connexion au serveur AppoLab
    connexion("im2ag-appolab.u-ga.fr");

    char decoded_text[2000000] = "";
    char encrypted_data[2000000] = "";
    char help_string[101];
    int switch_values[100];
    int value_indices[100];
    int switch_count = 0;
    char password[101];

    // Remplacez <identifiant> et <mot de passe> ci-dessous.
    envoyer_recevoir("login 12315199 ZENATI", server_response);
    //envoyer("login 12315200 ZAIDI");


    envoyer_recevoir("load OneMillion", server_response);
    decrypt_move_fast(server_response, decoded_text);
    decoded_text[0] = '\0'; // Réinitialise 'decoded_text'

    envoyer_recevoir("aide", help_string);

    int index;
    // Concatène la chaîne d'aide 9999 fois dans 'encrypted_data'
    for (index = 0; index < 9999; index++) {
        strcat(encrypted_data, help_string);
    }
    // Ajoute la chaîne "0123456789" à la fin de 'encrypted_data'
    strcat(encrypted_data, "0123456789");

    // Remplit le tableau 'decoded_text' avec des valeurs de -100 à -1
    for (index = 0; index < 100; index++) {
        decoded_text[index] = -100 + index;
    }
    decoded_text[100] = '\0'; // Termine la chaîne 'decoded_text'
    encrypted_data[999900] = '\0'; // Termine la chaîne 'encrypted_data'

    // Déchiffre la chaîne 'encrypted_data' et stocke le résultat dans 'decoded_text'
    decrypt_move_fast(encrypted_data, decoded_text);

    // Traite les 100 premiers caractères de 'decoded_text'
    for (index = 0; index < 100; index++) {
        if (decoded_text[index] >= 0) {
            password[index] = decoded_text[index]; // Copie le caractère dans 'password' si >= 0
        } else {
            password[index] = ' '; // Remplace par un espace si < 0
            value_indices[switch_count] = index; // Stocke l'index dans 'value_indices'
            switch_values[switch_count] = decoded_text[index] + 100; // Ajoute 100 à la valeur et la stocke dans 'switch_values'
            switch_count++; // Incrémente le compteur de 'switch_count'
        }
    }
    password[100] = '\0'; // Termine la chaîne 'password'
    decoded_text[0] = '\0'; // Réinitialise 'decoded_text'

    // Envoie la commande "start" au serveur et reçoit la réponse
    envoyer_recevoir("start", server_response);
    // Envoie le mot de passe au serveur et reçoit la réponse
    envoyer_recevoir(password, server_response);

    // Déchiffre la réponse du serveur et stocke le résultat dans 'decoded_text'
    decrypt_move_fast(server_response, decoded_text);
    printf("\n%s\n", decoded_text); // Affiche le texte déchiffré
    decoded_text[0] = '\0'; // Réinitialise 'decoded_text'

    // Envoie une demande au serveur pour savoir si c'est terminé
    envoyer_recevoir("terminé?", server_response);

    return 0;
}