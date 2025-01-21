#pragma once
#include <fstream>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <csignal>
#include <iostream>
#include <unistd.h>

#define M       90      // Maximum capacity of the Warehouse
#define X_max   (M / 2) // Maximum capacity for component X
#define Y_max   (M / 3) // Maximum capacity for component Y
#define Z_max   (M / 6) // Maximum capacity for component Z


struct Warehouse{
    int X;
    int Y;
    int Z;
};

key_t generate_ipc_key(char proj_id);
int init_shared_memory();
int init_semaphore();
int init_message_queue();
void lock_semaphore(int semid);
void unlock_semaphore(int semid);
