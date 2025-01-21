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

#define X_value = 1;  // The amount of space X takes up in storage
#define Y_value = 2;  // The amount of space Y takes up in storage
#define Z_value = 3;  // The amount of space Z takes up in storage
#define M = 1000;     // Maximum capacity of the Warehouse


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
