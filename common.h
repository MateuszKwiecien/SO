#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <unistd.h>

const int X_value = 1;  // The amount of space X takes up in storage
const int Y_value = 2;  // The amount of space Y takes up in storage
const int Z_value = 3;  // The amount of space Z takes up in storage
const int M = 1000;     // Maximum capacity of the Warehouse

struct Warehouse{
    int X;
    int Y;
    int Z;
};

const char* IPC_PATH = ".";    // Path to the directory containing IPC files

key_t generate_ipc_key(char id);

#endif // COMMON_H