#include "../common.hpp"

using namespace std;

int main(){
    // Obtain keys for IPC communication
    key_t shared_memory_key = generate_ipc_key('S');

    // Create shared memory from Warehouse struct
    int shm_id = shmget(shared_memory_key, sizeof(Warehouse), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    while(true){
        warehouse->X += 1;
        warehouse->Y += 1;
        warehouse->Z += 1;
        sleep(3);
    }
}