#include "../common.hpp"
#include <thread>
#include <chrono>
#include <random>

using namespace std;
/*
    supply_product() handles the main task of the supplier process

    Upon being called it checks the designation of the process and
    increases the stock value of the corresponding component

    When adding an item to the Warehouse it raises the value of the
    corresponding semaphore to signal availability to the assembler
    processes
*/
void supply_product(Warehouse* warehouse, char designation, int sem_id){
    // Lock the access to the warehouse
    semaphore_op(sem_id, SEM_MUTEX, -1);

    switch(designation){
        case 'X':                               // Handles the 'X' supplier process
            if (warehouse->X + 1 <= X_max){     // Checks if there is enough space for additional X component in the warehouse
                warehouse->X += 1;
                cout << "Component \"" << designation << "\" has been delivered to the warehouse." << endl;
                semaphore_op(sem_id, SEM_X, 1); // Signal availability of X
            }
            else{
                cout << "There is no room for \"" << designation << "\" component in the warehouse." << endl;
            }
        break;
        
        case 'Y':                               // Handles the 'Y' supplier process
            if (warehouse->Y + 1 <= Y_max){     // Checks if there is enough space for additional Y component in the warehouse
                warehouse->Y += 1;
                cout << "Component \"" << designation << "\" has been delivered to the warehouse." << endl;
                semaphore_op(sem_id, SEM_Y, 1); // Signal availability of Y
            }
            else{
                cout << "There is no room for \"" << designation << "\" component in the warehouse." << endl;
            }
        break;

        case 'Z':                               // Handles the 'Z' supplier process
            if (warehouse->Z + 1 <= Z_max){     // Checks if there is enough space for additional Z component in the warehouse
                warehouse->Z += 1;
                cout << "Component \"" << designation << "\" has been delivered to the warehouse." << endl;
                semaphore_op(sem_id, SEM_Z, 1); // Signal availability of Z
            }
            else{
                cout << "There is no room for \"" << designation << "\" component in the warehouse." << endl;
            }
        break;

        default:
            cout << "ERROR: Wrong designation" << endl;
            exit(60);
    }

    // Unlock access to the warehouse
    semaphore_op(sem_id, SEM_MUTEX, 1);
}

static char designation;    // Designation of supplier process
Warehouse* warehouse;       // Initialize warehouse struct
pid_t* pid_array;

/*
    sigint_handler() is called when process receives a SIGINT
    signal from another process. It termines the execution of
    the process
*/
void sigint_handler(int sig){
    // Detach shared memory
    if (shmdt(warehouse) == -1) {
        perror("Failed to detach warehouse shared memory");
    }
    if (shmdt(pid_array) == -1) {
        perror("Failed to detach PID array shared memory");
    }

    cout << "Supplier \'" << designation << "\' received SIGINT and stopped working." << endl;
    exit(0);
}

/*
    delay() function is used to slow the execution of the calling process
    in order to mimic performing an activity
*/
void delay(){
    std::mt19937_64 eng{std::random_device{}()};
    std::uniform_int_distribution<> dist{3000, 7000};
    std::this_thread::sleep_for(std::chrono::milliseconds{dist(eng)});
}

int main(){
    int shm_id_warehouse = init_shared_memory_warehouse();          // Initialize shared memory for Warehouse struct
    int shm_id_pid = init_shared_memory_pid();                      // Initialize shared memory for PID array
    int sem_id = init_semaphores_2();                               // Initialize semaphores

    signal(SIGINT, sigint_handler);

    warehouse = (Warehouse*)shmat(shm_id_warehouse, nullptr, 0);    // Assign Warehouse struct as shared memory
    pid_array = (pid_t *)shmat(shm_id_pid, nullptr, 0);             // Assign PID array as shared memory

    switch(fork()){ // fork() error
        case -1:
            cout << "ERROR: fork() function returned an error." << endl;
            exit(1);
        break;

        case 0:     // child process
            designation = 'X';

            // Place process PID in the array
            semaphore_op(sem_id, SEM_PID, -1);
            pid_array[2] = getpid();
            semaphore_op(sem_id, SEM_PID, 1);

            while(true){
                supply_product(warehouse, designation, sem_id);

                delay();
            }
        break;

        default:            // parent process
        switch (fork()){    // fork() error
            case -1:
                cout << "ERROR: fork() function returned an error." << endl;
                exit(1);
            break;

            case 0:         // child process
                designation = 'Y';

                // Place process PID in the array
                semaphore_op(sem_id, SEM_PID, -1);
                pid_array[3] = getpid();
                semaphore_op(sem_id, SEM_PID, 1);

                while(true){
                    supply_product(warehouse, designation, sem_id);

                    delay();
                }
            break;

            default:        // parent process
                designation = 'Z';

                // Place process PID in the array
                semaphore_op(sem_id, SEM_PID, -1);
                pid_array[4] = getpid();
                semaphore_op(sem_id, SEM_PID, 1);

                while(true){
                    supply_product(warehouse, designation, sem_id);
                    
                    delay();
                }
        }
    }

}