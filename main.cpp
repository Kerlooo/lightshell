#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>
#include <fstream> // For files
#include <filesystem>

#define endl  '\n'

// Colors
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      
#define RED     "\033[31m"      
#define GREEN   "\033[32m"      
#define YELLOW  "\033[33m"      
#define BLUE    "\033[34m"      
#define MAGENTA "\033[35m"      
#define CYAN    "\033[36m"      
#define WHITE   "\033[37m"      
#define BOLDBLACK   "\033[1m\033[30m"      
#define BOLDRED     "\033[1m\033[31m"      
#define BOLDGREEN   "\033[1m\033[32m"      
#define BOLDYELLOW  "\033[1m\033[33m"      
#define BOLDBLUE    "\033[1m\033[34m"      
#define BOLDMAGENTA "\033[1m\033[35m"      
#define BOLDCYAN    "\033[1m\033[36m"      
#define BOLDWHITE   "\033[1m\033[37m"      

using namespace std;

constexpr size_t MAX_INPUT_LEN = 4096;
string get_home(){
    const char* home = getenv("HOME");

    if(home == nullptr){
        cerr << RED << "Error while getting home path";
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

vector<string> load_history(const string& path){
    vector<string> history;
    if(path.empty())
        return history;

    ifstream in(path);
    if(!in)
        return history;

    string line;

    while(getline(in, line)){
        if(!line.empty())
            history.push_back(line);
    }
    return history;
}

void append_history(const string& path, const string& command){
    if(path.empty())
        return;
    
    ofstream out(path, ios::app);
    if(!out){
        cerr << RED << "Error while writing to history: " << path << endl;
        return;
    }
    out << command << '\n';
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
        cerr <<  "lsh> command not found: " RED << argv[0] << WHITE << endl;
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
        cout << WHITE << "lsh> ";

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

        if(command == "history"){
            for(auto c : history)
                cout << c << endl;
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