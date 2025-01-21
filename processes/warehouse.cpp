#include <iostream>
#include "../common.hpp"

using namespace std;

int main(){
    // Initialize shared memory
    int shm_id = init_shared_memory_warehouse();
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    // Just checking if any of it works
    while(true){
        cout << "X: " << warehouse->X << '\t'
        << "Y: " << warehouse->Y << '\t'
        << "Z: " << warehouse->Z << endl;
        sleep(1);
    }

}