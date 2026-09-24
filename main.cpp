#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>

#include "colors.hpp"
#include "history.hpp"

using namespace std;
using namespace color;

constexpr size_t MAX_INPUT_LEN = 4096;
string get_home(){
    const char* home = getenv("HOME");

    if(home == nullptr){
        cerr << RED << "Error while getting home path" << RESET;
        return "";
    }
    return home;
}

string get_lsh_dir(){
    namespace fs = std::filesystem;

    string home = get_home();
    if(home.empty())
        return "";
    
    fs::path dir = fs::path(home) / ".lsh";
    error_code ec;

    fs::create_directories(dir, ec);
    if(ec){
        cerr << RED << "Error while creating " << dir << ": " << ec.message() << endl;
        return "";
    }

    return dir.string();
}

vector<string> tokenize(string& command){
    vector<string> tokens;
    istringstream iss(command);
    string tok;

    while (iss >> tok) {
        tokens.push_back(tok);
    }
    return tokens;
}

void print_help(){
    cout << BOLDCYAN << "lightshell (lsh)" << RESET << " - a minimal shell written in C++\n"
         << "Made by " << BOLDGREEN << "Kerlo" << RESET << "\n"
         << "GitHub: " << BOLDGREEN << "https://github.com/Kerlooo/lightshell" << RESET << "\n\n"
         << BOLDYELLOW << "Builtin commands:" << RESET << "\n"
         << "  " << GREEN << "help" << RESET << "                          show this message\n"
         << "  " << GREEN << "history" << RESET << "                       show command history\n"
         << "  " << GREEN << "clear-history" << RESET << ", " << GREEN << "history -c" << RESET << "     clear command history\n"
         << "  " << GREEN << "exit" << RESET << "                          exit the shell\n\n"
         << "Any other input is executed as an external command." << endl;
}

void execute(const vector<string>& args){
    vector<char*> argv;
    for (const auto& s : args) {
        argv.push_back(const_cast<char*>(s.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if(pid < 0){
        cerr << RED << "Error while creating the fork" << endl;
        return;
    }

    if(pid == 0){
        execvp(argv[0], argv.data());
        
        // if execvp returns it means that failed.
        cerr <<  "lsh> command not found: " << RED << argv[0] << RESET << endl;
        _exit(127);
    }

    else{
        int stat{};
        waitpid(pid, &stat, 0);
    }
}

int main(){
    string history_dir = get_lsh_dir();
    string history_path = history_dir.empty() ? "" : history_dir + "/history.txt";

    vector<string> history = load_history(history_path);

    while(true){
        string command{};
        cout << RESET << "lsh> ";

        if (!getline(cin, command)) {
            break;
        }

        if(command.size() > MAX_INPUT_LEN){
            cerr << RED << "The input is too big!" << endl;
            continue;
        }

        if(command.empty())
            continue;

        if (command == "exit") {
            break;
        }

        if(command == "help"){
            print_help();
            continue;
        }

        if(command == "history"){
            for(auto c : history)
                cout << c << endl;
            continue;
        }

        if(command == "clear-history" || command == "history -c"){
            clear_history(history_path, history);
            continue;
        }

        history.push_back(command);
        append_history(history_path, command);

        auto args = tokenize(command);
        if (args.empty())
            continue;

        execute(args);
    }

    return 0;
}
