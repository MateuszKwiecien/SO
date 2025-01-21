#include "common.hpp"

const char* IPC_PATH = "./file";    // Path to the file containing IPC files

key_t generate_ipc_key(char id) {
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


int init_shared_memory() {
    key_t key = generate_ipc_key('S');
    int shmid = shmget(key, sizeof(Warehouse), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Błąd przy tworzeniu pamięci współdzielonej");
        exit(1);
    }
    return shmid;
}