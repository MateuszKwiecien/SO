#include "common.hpp"
#include <errno.h>

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

int init_shared_memory_warehouse() {
    key_t key = generate_ipc_key('W');
    int shmid = shmget(key, sizeof(Warehouse), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Błąd przy tworzeniu pamięci współdzielonej");
        exit(1);
    }
    return shmid;
}

int init_shared_memory_pid() {
    key_t key = generate_ipc_key('P');
    int shmid = shmget(key, 5 * sizeof(pid_t), IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("Błąd przy tworzeniu pamięci współdzielonej");
        exit(1);
    }
    return shmid;
}

int init_semaphores() {
    // Generate key for semaphore
    key_t sem_key = generate_ipc_key('M');
    int sem_id = semget(sem_key, 5, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }

    // Use union semun to set values
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_attr;

    sem_attr.val = 1; // Set semaphores to 1 when neccesary

    // Initialize semaphores with proper semctl calls
    if (semctl(sem_id, SEM_MUTEX, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_MUTEX");
        exit(1);
    }
    sem_attr.val = 0; // Set values for other semaphores
    if (semctl(sem_id, SEM_X, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_X");
        exit(1);
    }
    if (semctl(sem_id, SEM_Y, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_Y");
        exit(1);
    }
    if (semctl(sem_id, SEM_Z, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_Z");
        exit(1);
    }
    sem_attr.val = 1;
    if (semctl(sem_id, SEM_PID, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_PID");
        exit(1);
    }

    // Debug print the semaphore values
    printf("Semaphore values: %d %d %d %d %d\n",
        semctl(sem_id, SEM_MUTEX, GETVAL, 0),
        semctl(sem_id, SEM_X, GETVAL, 0),
        semctl(sem_id, SEM_Y, GETVAL, 0),
        semctl(sem_id, SEM_Z, GETVAL, 0),
        semctl(sem_id, SEM_PID, GETVAL, 0));

    return sem_id;
}

int init_semaphores_2() {
    // Generate key for semaphore
    key_t sem_key = generate_ipc_key('M');
    int sem_id = semget(sem_key, 5, IPC_CREAT | 0666);
    if (sem_id == -1) {
        perror("semget failed");
        exit(1);
    }

    // Use union semun to set values
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_attr;

    sem_attr.val = 1; // Set semaphores to 1 when neccesary

    // Debug print the semaphore values
    printf("Semaphore values: %d %d %d %d %d\n",
        semctl(sem_id, SEM_MUTEX, GETVAL, 0),
        semctl(sem_id, SEM_X, GETVAL, 0),
        semctl(sem_id, SEM_Y, GETVAL, 0),
        semctl(sem_id, SEM_Z, GETVAL, 0),
        semctl(sem_id, SEM_PID, GETVAL, 0));

    return sem_id;
}

void semaphore_op(int sem_id, int sem_num, int op) {
    struct sembuf sb = {
        static_cast<short unsigned int>(sem_num), 
        static_cast<short int>(op), 
        0
    };

    while (semop(sem_id, &sb, 1) == -1) {
        if (errno == EINTR) {
            // Log interrupted calls
            std::cerr << "semop interrupted, retrying...\n";
            continue;
        } else {
            // Log other errors
            perror("semop failed");
            exit(1);
        }
    }
}

void remove_IPC_file(){
    remove(IPC_PATH);
}