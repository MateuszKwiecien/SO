#include "../common.hpp"
#include <thread>
#include <chrono>
#include <random>

using namespace std;

void supply_product(Warehouse* warehouse, char designation){
    switch(designation){
        case 'X':
            if (warehouse->X + 1 <= X_max){ // Checks if there is enough space for additional X component in the warehouse
                warehouse->X += 1;
                cout << "Component \"" << designation << "\" has been delivered to the warehouse." << endl;
            }
            else{
                cout << "There is no room for \"" << designation << "\" component in the warehouse." << endl;
            }
        break;
        
        case 'Y':
            if (warehouse->Y + 1 <= Y_max){ // Checks if there is enough space for additional X component in the warehouse
                warehouse->Y += 1;
                cout << "Component \"" << designation << "\" has been delivered to the warehouse." << endl;
            }
            else{
                cout << "There is no room for \"" << designation << "\" component in the warehouse." << endl;
            }
        break;

        case 'Z':
            if (warehouse->Z + 1 <= Z_max){ // Checks if there is enough space for additional X component in the warehouse
                warehouse->Z += 1;
                cout << "Component \"" << designation << "\" has been delivered to the warehouse." << endl;
            }
            else{
                cout << "There is no room for \"" << designation << "\" component in the warehouse." << endl;
            }
        break;

        default:
            cout << "ERROR: Wrong designation" << endl;
            exit(60);
    }
}

int main(){
    char designation;   // Designation of supplier process

    // Initialize shared memory
    int shm_id = init_shared_memory();
    Warehouse* warehouse = (Warehouse*)shmat(shm_id, nullptr, 0);

    if(fork()){
        designation = 'X';

        while(true){
            supply_product(warehouse, designation);

            std::mt19937_64 eng{std::random_device{}()};
            std::uniform_int_distribution<> dist{3, 7};
            std::this_thread::sleep_for(std::chrono::seconds{dist(eng)});
        }
    }
    else if(fork()){
        designation = 'Y';

        while(true){
            supply_product(warehouse, designation);

            std::mt19937_64 eng{std::random_device{}()};
            std::uniform_int_distribution<> dist{3, 7};
            std::this_thread::sleep_for(std::chrono::seconds{dist(eng)});
        }
    }
    else{
        designation = 'Z';

        while(true){
            supply_product(warehouse, designation);
            
            std::mt19937_64 eng{std::random_device{}()};
            std::uniform_int_distribution<> dist{3, 7};
            std::this_thread::sleep_for(std::chrono::seconds{dist(eng)});
        }
    }
}