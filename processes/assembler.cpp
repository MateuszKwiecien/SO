#include "../common.hpp"

using namespace std;

void assemble_product(Warehouse* warehouse){
    if(warehouse->X && warehouse->Y && warehouse->Z){   // Checks if all required components are available
        warehouse->X -= 1;
        warehouse->Y -= 1;
        warehouse->Z -= 1;
    }
}

int main(){
    // Initialize shared memory
    int shm_id = init_shared_memory(); 
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    while(true){
        assemble_product(warehouse);
        sleep(4);
    }
}