#include <iostream>
#include "../common.hpp"
using namespace std;

int main(){
    // Obtain keys for IPC communication
    key_t shared_memory_key = generate_ipc_key('S');

    // Create shared memory from Warehouse struct
    int shm_id = shmget(shared_memory_key, sizeof(Warehouse), IPC_CREAT | 0666);
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    // Just checking if any of it works
    while(true){
        cout << "X: " << warehouse->X << '\t'
        << "Y: " << warehouse->Y << '\t'
        << "Z: " << warehouse->Z << endl;
        sleep(2);
    }

}