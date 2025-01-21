#include "../common.hpp"
#define BUFFER_SIZE 2048

using namespace std;

pid_t* pid_array;       // Stores the values of PIDs of other processes
Warehouse* warehouse;
int sem_id;

void read_option(char &option, char* buffer){
    cin >> buffer;
    option = buffer[0];
}

void execute_command(const char option){
    switch(option){
        case '1':   // Kills all assembler processes
            semaphore_op(sem_id, 4, -1);
            kill(pid_array[0], SIGINT);
            kill(pid_array[1], SIGINT);
            semaphore_op(sem_id, 4, 1);
        break;

        case '2':   // Kills all supplier processes
            semaphore_op(sem_id, 4, -1);
            kill(pid_array[2], SIGINT);
            kill(pid_array[3], SIGINT);
            kill(pid_array[4], SIGINT);
            semaphore_op(sem_id, 4, 1);
        break;

        case '3':
            
        break;

        case '4':
        break;

        case '5':
            cout << "Exiting the \'director\' process.\n";
            exit(0);
        break;

        default:
            cout << "You have picked an invalid option.\nPlease input one of the expected ones.";
    }
}

int main(){
    int shm_id_warehouse = init_shared_memory_warehouse();          // Initialize shared memory
    int shm_id_pid = init_shared_memory_pid();                      // Initialize shared memory for PID array
    sem_id = init_semaphores();     // Initialize semaphores

    warehouse = (Warehouse*)shmat(shm_id_warehouse, nullptr, 0);    // Assign Warehouse struct as shared memory
    pid_array = (pid_t *)shmat(shm_id_pid, nullptr, 0);             // Assign PID array as shared memory

    cout << "\tDIRECTOR PROCESS\n========================================";
    char option;  // User chosen option

    char* buffer = new char[BUFFER_SIZE];   // Holds the input form the user, first character is the chosen option the rest is discarded

    while(true){
        cout << "\n\nChoose one of the following options:\n"
        << "1 - warehouse finishes work\n"
        << "2 - factory finishes work\n"
        << "3 - both factory and warehouse finish work, warehouse stock is saved to a file\n"
        << "4 - both factory and warehouse finish work, warehouse stock is discarded\n"
        << "5 - exits director process\n"
        << "Provide an option: ";

        read_option(option, buffer);
        execute_command(option);
    }
    
}