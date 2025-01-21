#include "common.h"

key_t generate_key(char id) {
    // Ensure the key file exists
    FILE *file = fopen(IPC_PATH, "w");
    if (file == NULL) {
        perror("Nie można utworzyć pliku klucza");
        exit(1);
    }
    fclose(file);

    // Generate the key using ftok
    key_t key = ftok(IPC_PATH, id);
    if (key == -1) {
        perror("Błąd podczas generowania klucza przy użyciu ftok");
        exit(1);
    }

    return key;
}