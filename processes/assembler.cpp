#include "../common.hpp"

using namespace std;

void assemble_product(Warehouse* warehouse, char designation){
    if(warehouse->X && warehouse->Y && warehouse->Z){   // Checks if all required components are available
        warehouse->X -= 1;
        warehouse->Y -= 1;
        warehouse->Z -= 1;
        cout << "Assembler \"" << designation << "\" assembled a product." << endl;
    }
    else{
        cout << "Assembler \"" << designation << "\" lacks resources to assemble a product." << endl;
    }
}

int main(){
    char designation;   // Designation of assembler process

    // Initialize shared memory
    int shm_id = init_shared_memory(); 
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);
    

    if(fork()){
        designation = 'A';
        while(true){
            assemble_product(warehouse, designation);
            sleep(5);
        }
    }
    else{
        designation = 'B';
        while(true){
            assemble_product(warehouse, designation);
            sleep(5);
        }
    }
}