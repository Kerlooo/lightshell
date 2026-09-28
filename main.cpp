#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <filesystem>
#include <csignal>
#include <cerrno>
#include <cstring>

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
        cerr << RED << "Error while creating " << dir << ": " << ec.message() << RESET << endl;
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
         << "  " << GREEN << "cd" << RESET << " [dir]                      change directory (default: $HOME)\n"
         << "  " << GREEN << "help" << RESET << "                          show this message\n"
         << "  " << GREEN << "history" << RESET << "                       show command history\n"
         << "  " << GREEN << "clear-history" << RESET << ", " << GREEN << "history -c" << RESET << "     clear command history\n"
         << "  " << GREEN << "exit" << RESET << "                          exit the shell\n\n"
         << "Any other input is executed as an external command." << endl;
}

volatile sig_atomic_t child_running = 0;

// Ctrl+C must kill the running command, not the shell.
void sigint_handler(int){
    if(child_running)
        write(STDOUT_FILENO, "\n", 1);
    else
        write(STDOUT_FILENO, "\nlsh> ", 6);
}

void change_dir(const vector<string>& args){
    string target = args.size() > 1 ? args[1] : get_home();
    if(target.empty())
        return;

    if(chdir(target.c_str()) != 0)
        cerr << RED << "cd: " << target << ": " << strerror(errno) << RESET << endl;
}

int execute(const vector<string>& args){
    vector<char*> argv;
    for (const auto& s : args) {
        argv.push_back(const_cast<char*>(s.c_str()));
    }
    argv.push_back(nullptr);

    pid_t pid = fork();
    if(pid < 0){
        cerr << RED << "Error while creating the fork" << RESET << endl;
        return 1;
    }

    if(pid == 0){
        signal(SIGINT, SIG_DFL);
        execvp(argv[0], argv.data());

        // if execvp returns it means that failed.
        if(errno == ENOENT){
            cerr <<  "lsh> command not found: " << RED << argv[0] << RESET << endl;
            _exit(127);
        }
        cerr << "lsh> " << RED << argv[0] << ": " << strerror(errno) << RESET << endl;
        _exit(126);
    }

    child_running = 1;
    int stat{};
    while(waitpid(pid, &stat, 0) < 0 && errno == EINTR);
    child_running = 0;

    if(WIFEXITED(stat))
        return WEXITSTATUS(stat);
    if(WIFSIGNALED(stat))
        return 128 + WTERMSIG(stat);
    return 1;
}

int main(){
    string history_dir = get_lsh_dir();
    string history_path = history_dir.empty() ? "" : history_dir + "/history.txt";

    vector<string> history = load_history(history_path);
    int last_status = 0;

    struct sigaction sa{};
    sa.sa_handler = sigint_handler;
    sa.sa_flags = SA_RESTART;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, nullptr);

    while(true){
        string command{};
        cout << RESET << "lsh> ";

        if (!getline(cin, command)) {
            break;
        }

        if(command.size() > MAX_INPUT_LEN){
            cerr << RED << "The input is too big!" << RESET << endl;
            continue;
        }

        auto args = tokenize(command);
        if (args.empty())
            continue;

        if (args[0] == "exit") {
            break;
        }

        if(args[0] == "help"){
            print_help();
            continue;
        }

        if(args[0] == "history" && args.size() == 1){
            for(auto c : history)
                cout << c << endl;
            continue;
        }

        if(args[0] == "clear-history" || (args[0] == "history" && args.size() > 1 && args[1] == "-c")){
            clear_history(history_path, history);
            continue;
        }

        history.push_back(command);
        append_history(history_path, command);

        if(args[0] == "cd"){
            change_dir(args);
            continue;
        }

        last_status = execute(args);
    }

    return last_status;
}
