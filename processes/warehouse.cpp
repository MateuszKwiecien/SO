#include <iostream>
#include "../common.hpp"

using namespace std;

int main(){
    // Initialize shared memory
    int shm_id = init_shared_memory();
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    // Inicjalizacja magazynu
    warehouse->X= 0;
    warehouse->Y= 0;
    warehouse->Z= 0;

    // Just checking if any of it works
    while(true){
        cout << "X: " << warehouse->X << '\t'
        << "Y: " << warehouse->Y << '\t'
        << "Z: " << warehouse->Z << endl;
        sleep(2);
    }

}