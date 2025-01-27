#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <csignal>
#include <iostream>
#include <unistd.h>

#define M       90      // Maximum capacity of the Warehouse
#define X_max   (M / 2) // Maximum capacity for component X
#define Y_max   (M / 3) // Maximum capacity for component Y
#define Z_max   (M / 6) // Maximum capacity for component Z

// Semaphore macros
#define SEM_MUTEX   0
#define SEM_X       1
#define SEM_Y       2
#define SEM_Z       3
#define SEM_PID     4
#define SEM_WH      5

struct Warehouse{
    int X;
    int Y;
    int Z;
};

key_t generate_ipc_key(char proj_id);
int init_shared_memory_warehouse();
int init_shared_memory_pid();
int init_semaphores();
int init_semaphores_2();
void semaphore_op(int sem_id, int sem_num, int op);
void remove_IPC_file();
