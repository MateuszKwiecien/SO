#include "../common.hpp"
#include <filesystem>
#include <fstream>

#define BUFFER_SIZE 2048

using namespace std;

const char* STOCK_FILE_NAME = "stock";
pid_t* pid_array;       // Stores the values of PIDs of other processes
Warehouse* warehouse;
int sem_id;             // Needs to be a global variable to be accessed in a function
fstream stock;          // Handler for the file
int shm_id_warehouse;
int shm_id_pid;

/*
    Is called to safely release IPC resources
*/
void cleanup_resources() {
    // Detach shared memory
    if (shmdt(warehouse) == -1) {
        perror("Failed to detach warehouse shared memory");
    }
    if (shmdt(pid_array) == -1) {
        perror("Failed to detach PID array shared memory");
    }

    // Remove shared memory segments
    if (shmctl(shm_id_warehouse, IPC_RMID, nullptr) == -1) {
        perror("Failed to remove warehouse shared memory segment");
    }
    if (shmctl(shm_id_pid, IPC_RMID, nullptr) == -1) {
        perror("Failed to remove PID array shared memory segment");
    }

    // Remove semaphore set
    if (semctl(sem_id, 0, IPC_RMID) == -1) {
        perror("Failed to remove semaphore set");
    }

    remove_IPC_file();

    cout << "IPC resources successfully cleaned up.\n";
}

void read_option(char &option, char* buffer){
    cin >> buffer;
    option = buffer[0];
}

/*
    Provides the functionality for the main task of this process
    Checks the user input and acts accoridingly
*/
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

        case '3':   // Kills both assembler and supplier processes and stores the state of warehouse into a file
            semaphore_op(sem_id, 4, -1);
            for(int i = 0; i < 5; i++){
                kill(pid_array[i], SIGINT);
            }
            semaphore_op(sem_id, 4, 1);

            cout
                << warehouse->X << '\n'
                << warehouse->Y << '\n'
                << warehouse->Z << '\n';

            // Save the contents of the warehouse to a file
            stock.open(STOCK_FILE_NAME, ios::trunc | ios::out); // Opens the file in write mode and truncates its content
            if(!stock.is_open()){                               // If the file could not be opened it discards the stock
                cout << "Could not open file to write\nDiscarding the stock\n";
                return;
            }
            stock                                               // Saved the content of the warehouse to a file
                << warehouse->X << '\n'
                << warehouse->Y << '\n'
                << warehouse->Z << '\n';
            stock.close();

            cleanup_resources();
            exit(0);
        break;

        case '4':   // Kills both assembler and supplier processes and discards the state of the warehouse
            semaphore_op(sem_id, 4, -1);
            for(int i = 0; i < 5; i++){
                kill(pid_array[i], SIGINT);
            }
            semaphore_op(sem_id, 4, 1);

            remove(STOCK_FILE_NAME);
            cleanup_resources();
            exit(0);
        break;

        default:
            cout << "You have picked an invalid option.\nPlease input one of the expected ones.";
    }
}

bool is_number(const string& str){
    for(const char& c : str){
        if (!isdigit(c)) {
            return false;
        }
    }
    return !str.empty();
}

/*
    Initializes the warehouse stock for all components to 0
*/
void initialize_to_zero(){
    semaphore_op(sem_id, SEM_MUTEX, -1);
    warehouse->X = 0;
    warehouse->Y = 0;
    warehouse->Z = 0;
    semaphore_op(sem_id, SEM_MUTEX, 1);
}

/*
    This function is called when a file expected to hold the values
    of saved warehouse state is present

    It attempts to read the warehouse stock date from a file and
    in case it encounters issues it switches to initializing the
    values to 0
*/
void read_stock_from_file(){
    cout << "Reading stock values form a file\n";
    stock.clear();
    stock.open(STOCK_FILE_NAME, ios::in);   // Opens the file to read the stock values
    if(!stock.is_open()){                   // File could not be opened, initializing stock values to 0
        cout << "Could not open file to read!\n"
            << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }

    string temporary;   // Temporarily holds the input from file to check validity

    stock >> temporary;
    if(is_number(temporary)){   // Checks if the read value is a number
        semaphore_op(sem_id, SEM_MUTEX, -1);
        warehouse->X = stoi(temporary);
        semaphore_op(sem_id, SEM_MUTEX, 1);
    }
    else{                       // In case of wrong input switches to initializing stock values to 0
        cout << "Could not read a number.\n"
            << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }
    if(warehouse->X > X_max){   // In case the read number is too large to fit in the warehouse it switches to initializing to 0
        cout << "Provided number of components is too large to fit into the warehouse.\n"
        << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }

    stock >> temporary;
    if(is_number(temporary)){
        semaphore_op(sem_id, SEM_MUTEX, -1);
        warehouse->Y = stoi(temporary);
        semaphore_op(sem_id, SEM_MUTEX, 1);
    }
    else{
        cout << "Could not read a number.\n"
            << "Initializing warehouse stock to 0 instead\n";         
        initialize_to_zero();
        return;
    }
    if(warehouse->Y > Y_max){
        cout << "Provided number of components is too large to fit into the warehouse.\n"
        << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }


    stock >> temporary;
    if(is_number(temporary)){
        semaphore_op(sem_id, SEM_MUTEX, -1);
        warehouse->Z = stoi(temporary);
        semaphore_op(sem_id, SEM_MUTEX, 1);
    }
    else{
        cout << "Could not read a number.\n"
            << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }
    if(warehouse->Z > Z_max){
        cout << "Provided number of components is too large to fit into the warehouse.\n"
        << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }
    stock.close();
    
    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
        struct seminfo *__buf;
    } sem_attr;

    // Set the semaphore values to those read from the file
    sem_attr.val = warehouse->X;
    if (semctl(sem_id, SEM_X, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_X");
        exit(1);
    }
    sem_attr.val = warehouse->Y;
    if (semctl(sem_id, SEM_Y, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_Y");
        exit(1);
    }
    sem_attr.val = warehouse->Z;
    if (semctl(sem_id, SEM_Z, SETVAL, sem_attr) == -1) {
        perror("semctl failed for SEM_Z");
        exit(1);
    }
}

int main(){
    shm_id_warehouse = init_shared_memory_warehouse();              // Initialize shared memory
    shm_id_pid = init_shared_memory_pid();                          // Initialize shared memory for PID array
    sem_id = init_semaphores();                                     // Initialize semaphores

    warehouse = (Warehouse*)shmat(shm_id_warehouse, nullptr, 0);    // Assign Warehouse struct as shared memory
    pid_array = (pid_t *)shmat(shm_id_pid, nullptr, 0);             // Assign PID array as shared memory


    if(filesystem::exists(STOCK_FILE_NAME)){                        // Checks if stock file exists
        read_stock_from_file();
    }
    else{
        initialize_to_zero();
    }

    cout << "\tDIRECTOR PROCESS\n========================================";
    char option;  // User chosen option

    char* buffer = new char[BUFFER_SIZE];   // Holds the input form the user, first character is the chosen option the rest is discarded

    while(true){
        cout << "\n\nChoose one of the following options:\n"
        << "1 - factory finishes work\n"
        << "2 - warehouse finishes work\n"
        << "3 - both factory and warehouse finish work, warehouse stock is saved to a file\n"
        << "4 - both factory and warehouse finish work, warehouse stock is discarded\n"
        << "Enter your choice: ";

        read_option(option, buffer);
        execute_command(option);
    }
    
}