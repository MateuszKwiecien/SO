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

// int init_semaphores() {
//     key_t sem_key = generate_ipc_key('M');
//     int sem_id = semget(sem_key, 5, IPC_CREAT | 0666);
//     if (sem_id == -1) {
//         perror("semget failed");
//         exit(1);
//     }
//     printf("%d %d %d\n", semctl(sem_id, GETVAL, 1), semctl(sem_id, GETVAL, 2), semctl(sem_id, GETVAL, 3));
//     printf("%d\n", errno);
//     switch(errno){
//         case EACCES:
//             printf("Blad: EACCES\n");
//             break;
//         case EEXIST:
//         printf("Blad: EACCES\n");
//             break;
//         case ENOENT:
//         printf("Blad: EACCES\n");
//             break;
//         case ENOMEM:
//         printf("Blad: EACCES\n");
//             break;
//         case ENOSPC:
//         printf("Blad: EACCES\n");
//             break;
//         case EFAULT:
//         printf("Blad: EACCES\n");
//             break;
//             case EIDRM:
//         printf("Blad: EACCES\n");
//             break;
//             case EINVAL:
//         printf("Blad: EACCES\n");
//             break;
//             case EPERM:
//         printf("Blad: EACCES\n");
//             break;
//             case ERANGE:
//         printf("Blad: EACCES\n");
//             break;
//         default:
//             printf("Inny blad: %d\n", errno);
//     }

//     // Initialize semaphores
//     semctl(sem_id, SEM_MUTEX, SETVAL, 1);
//     semctl(sem_id, SEM_X, SETVAL, 0);
//     semctl(sem_id, SEM_Y, SETVAL, 0);
//     semctl(sem_id, SEM_Z, SETVAL, 0);
//     semctl(sem_id, SEM_PID, SETVAL, 1);

//     printf("%d %d %d\n", semctl(sem_id, GETVAL, 1), semctl(sem_id, GETVAL, 2), semctl(sem_id, GETVAL, 3));


//     return sem_id;
// }

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

void semaphore_op(int sem_id, int sem_num, int op) {
    struct sembuf sb = {
        static_cast<short unsigned int>(sem_num), 
        static_cast<short int>(op), 
        0
    };
    if (semop(sem_id, &sb, 1) == -1) {
        perror("semop failed");
        exit(1);
    }
}

void remove_IPC_file(){
    remove(IPC_PATH);
}