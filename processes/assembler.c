#include "../common.h"

int main(){
    // Obtain keys for IPC communication
    key_t shared_memory_key = generate_ipc_key('S');

    // Create shared memory from Warehouse struct
    int shm_id = shmget(shared_memory_key, sizeof(struct Warehouse), IPC_CREAT | 0666);
    if (shm_id == -1) {
        perror("shmget failed");
        exit(1);
    }
    struct Warehouse* warehouse = (struct Warehouse*)shmat(shm_id, NULL, 0);

    while(1){
        warehouse->X += -1;
        warehouse->Y += -1;
        warehouse->Z += -1;
        sleep(4);
    }
}