#include "../common.hpp"

using namespace std;

int main(){
    // Initialize shared memory
    int shm_id = init_shared_memory(); 
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    while(true){
        warehouse->X += -1;
        warehouse->Y += -1;
        warehouse->Z += -1;
        sleep(4);
    }
}