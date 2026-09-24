#include "history.hpp"

#include <fstream>
#include <iostream>

#include "colors.hpp"

using namespace std;
using namespace color;

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

void clear_history(const string& path, vector<string>& history){
    if(path.empty())
        return;

    ofstream out(path, ios::trunc);
    if(!out){
        cerr << RED << "Error while clearing history: " << path << RESET << endl;
        return;
    }
    history.clear();
}
