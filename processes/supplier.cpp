#include "../common.hpp"

using namespace std;

void supply_product(Warehouse* warehouse){
    warehouse->X += 1;
    warehouse->Y += 1;
    warehouse->Z += 1;
}

int main(){
    // Initialize shared memory
    int shm_id = init_shared_memory();
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    while(true){
        supply_product(warehouse);
        sleep(3);
    }
}