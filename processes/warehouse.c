#include "../common.h"

int main(){
    // Obtain keys for IPC communication
    key_t shared_memory_key = generate_ipc_key('S');

    // Create shared memory from Warehouse struct
    int shm_id = shmget(shared_memory_key, sizeof(struct Warehouse), IPC_CREAT | 0666);
    struct Warehouse* warehouse = (struct Warehouse*)shmat(shm_id, NULL, 0);

    warehouse->X = 0;
    warehouse->Y = 0;
    warehouse->Z = 0;

    // Just checking if any of it works
    while(1){
        sleep(2);
    }

}