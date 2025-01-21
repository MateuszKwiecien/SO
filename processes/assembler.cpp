#include "../common.hpp"

using namespace std;

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

void sigint_handler(int sig){
    cout << "Assembler \'" << designation << "\' received SIGINT and stopped working.";
    exit(0);
}

int main(){
    int shm_id = init_shared_memory();  // Initialize shared memory
    int sem_id = init_semaphores();     // Initialize semaphores

    signal(SIGINT, sigint_handler);

    warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);   // Assign Warehouse struct as shared memory

    switch(fork()){
        case -1:    // fork() error
            cout << "ERROR: fork() function returned an error." << endl;
            exit(1);
        break;

        case 0:     // child process
            designation = 'A';
            while(true){
                assemble_product(warehouse, designation, sem_id);
                sleep(5);
            }
        break;
        
        default:    // parent process
            designation = 'B';
            while(true){
                assemble_product(warehouse, designation, sem_id);
                sleep(5);
            }
        
    }
}