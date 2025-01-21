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
            semaphore_op(sem_id, 4, -1);
            for(int i = 0; i < 5; i++){
                kill(pid_array[i], SIGINT);
            }
            semaphore_op(sem_id, 4, 1);

            cout
                << warehouse->X << '\n'
                << warehouse->Y << '\n'
                << warehouse->Z << '\n';

            stock.clear();
            stock.open(STOCK_FILE_NAME, ios::trunc | ios::out); // Opens the file in write mode and truncates its content
            if(!stock.is_open()){                               // If the file could not be opened it discards the stock
                cout << "Could no open file to write\nDiscarding the stock\n";
                return;
            }
            stock                                               // Saved the content of the warehouse to the file
                << warehouse->X << '\n'
                << warehouse->Y << '\n'
                << warehouse->Z << '\n';
            stock.close();
        break;

        case '4':
            semaphore_op(sem_id, 4, -1);
            for(int i = 0; i < 5; i++){
                kill(pid_array[i], SIGINT);
            }
            semaphore_op(sem_id, 4, 1);

            remove(STOCK_FILE_NAME);
        break;

        case '5':
            cout << "Exiting the \'director\' process.\n";
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

void initialize_to_zero(){
    warehouse->X = 0;
    warehouse->Y = 0;
    warehouse->Z = 0;
}

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
    if(is_number(temporary)){
        warehouse->X = stoi(temporary);
    }
    else{
        cout << "Could not read a number.\n"
            << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }
    if(warehouse->X > X_max){
        cout << "Provided number of components is too large to fit into the warehouse.\n"
        << "Initializing warehouse stock to 0 instead\n";
        initialize_to_zero();
        return;
    }

    stock >> temporary;
    if(is_number(temporary)){
        warehouse->Y = stoi(temporary);
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
        warehouse->Z = stoi(temporary);
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

}

int main(){
    int shm_id_warehouse = init_shared_memory_warehouse();          // Initialize shared memory
    int shm_id_pid = init_shared_memory_pid();                      // Initialize shared memory for PID array
    sem_id = init_semaphores();                                     // Initialize semaphores

    warehouse = (Warehouse*)shmat(shm_id_warehouse, nullptr, 0);    // Assign Warehouse struct as shared memory
    pid_array = (pid_t *)shmat(shm_id_pid, nullptr, 0);             // Assign PID array as shared memory


    if(filesystem::exists(STOCK_FILE_NAME)){                        // Checks if stock file exists
        read_stock_from_file();
    }
    else{
        initialize_to_zero();
    }
    semaphore_op(sem_id, SEM_WH, -1);                                // After initializing the warehouse it allows the workers to start

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