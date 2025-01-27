#include "../common.hpp"

using namespace std;

/*
    assemble_product() handles the main task of the assembler process

    It forces the assembler process to await for the availability of all components
    neccesary through the use of semaphores and when possible allows it to assemble
    a product with exclusive access to the shared memory struct Warehouse
*/
void assemble_product(Warehouse* warehouse, char designation, int sem_id){
    // Wait for the required components
    semaphore_op(sem_id, SEM_X, -1); // Wait for X
    semaphore_op(sem_id, SEM_Y, -1); // Wait for Y
    semaphore_op(sem_id, SEM_Z, -1); // Wait for Z
    
    // Lock access to the warehouse
    semaphore_op(sem_id, SEM_MUTEX, -1);

    if(warehouse->X && warehouse->Y && warehouse->Z){   // Checks if all required components are available
        warehouse->X -= 1;
        warehouse->Y -= 1;
        warehouse->Z -= 1;
        cout << "Assembler \"" << designation << "\" assembled a product." << endl;
    }
    else{
        cout << "Assembler \"" << designation << "\" lacks resources to assemble a product." << endl;
    }

    // Unlock access to the warehouse
    semaphore_op(sem_id, SEM_MUTEX, 1);
}

static char designation;    // Designation of assembler process
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

    cout << "Assembler \'" << designation << "\' received SIGINT and stopped working." << endl;
    exit(0);
}

int main(){
    int shm_id_warehouse = init_shared_memory_warehouse();          // Initialize shared memory
    int shm_id_pid = init_shared_memory_pid();                      // Initialize shared memory for PID array
    int sem_id = init_semaphores_2();                                 // Initialize semaphores

    signal(SIGINT, sigint_handler);

    warehouse = (Warehouse*)shmat(shm_id_warehouse, nullptr, 0);    // Assign Warehouse struct as shared memory
    pid_array = (pid_t *)shmat(shm_id_pid, nullptr, 0);             // Assign PID array as shared memory

    switch(fork()){
        case -1:    // fork() error
            cout << "ERROR: fork() function returned an error." << endl;
            exit(1);
        break;

        case 0:     // child process
            designation = 'A';

            // Place process PID in the array
            semaphore_op(sem_id, SEM_PID, -1);
            pid_array[0] = getpid();
            semaphore_op(sem_id, SEM_PID, 1);

            while(true){
                assemble_product(warehouse, designation, sem_id);
                sleep(5);
            }
        break;
        
        default:    // parent process
            designation = 'B';

            // Place process PID in the array
            semaphore_op(sem_id, SEM_PID, -1);
            pid_array[1] = getpid();
            semaphore_op(sem_id, SEM_PID, 1);

            while(true){
                assemble_product(warehouse, designation, sem_id);
                sleep(5);
            }
    }
}