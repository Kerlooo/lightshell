#include <iostream>
#include <string>
// #include <unistd.h>  
// #include <sys/wait.h>

#define endl '\n'
constexpr size_t MAX_INPUT_LEN = 4096;

int main(){
    while(true){
        std::string command{};
        std::cout << "lsh> ";

        if (!std::getline(std::cin, command)) {
            break;
        }

        if(command.size() > MAX_INPUT_LEN){
            std::cerr << "The input is too big!" << endl;
            continue;
        }

        if(command.empty())
            continue;

        if (command == "exit") {
            break;
        }

        std::cout << command << "\n";
    }

    return 0;
}