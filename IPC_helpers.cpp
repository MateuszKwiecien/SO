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

int init_semaphores() {
    key_t sem_key = generate_ipc_key('M');
    int sem_id = semget(sem_key, 4, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }

    // Initialize semaphores
    semctl(sem_id, SEM_MUTEX, SETVAL, 1);
    semctl(sem_id, SEM_X, SETVAL, 0);
    semctl(sem_id, SEM_Y, SETVAL, 0);
    semctl(sem_id, SEM_Z, SETVAL, 0);

    return sem_id;
}