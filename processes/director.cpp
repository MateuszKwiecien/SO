#include "../common.hpp"
#define BUFFER_SIZE 2048

using namespace std;

void read_option(char &option, char* buffer){
    cin >> buffer;
    option = buffer[0];
}

void execute_command(const char option){
    switch(option){
        case '1':
            
        break;

        case '2':
        break;

        case '3':
        break;

        case '4':
        break;

        case '5':
            cout << "Exiting the \'director\' process.\n";
            exit(0);
        break;

        default:
            cout << "You have picked an invalid option.\nPlease input one of the expected ones.";
    }
}

int main(){
    cout << "\tDIRECTOR PROCESS\n========================================";
    char option;  // User chosen option

    char* buffer = new char[BUFFER_SIZE];   // Holds the input form the user, first character is the chosen option the rest is discarded

    while(true){
        cout << "\n\nChoose one of the following options:\n"
        << "1 - warehouse finishes work\n"
        << "2 - factory finishes work\n"
        << "3 - both factory and warehouse finish work, warehouse stock is saved to a file\n"
        << "4 - both factory and warehouse finish work, warehouse stock is discarded\n"
        << "5 - exits director process\n"
        << "Provide an option: ";

        read_option(option, buffer);
        execute_command(option);
    }
    
}