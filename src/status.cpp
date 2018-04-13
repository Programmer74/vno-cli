#include "status.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "utils.h"
#include "paths.h"
#include "log.h"

using namespace std;

int status::do_status() {
    cout << "Status called" << endl;

    string last_commit_hash;
    ifstream headfile(LAST_COMMIT_ID_FILE);
    getline(headfile, last_commit_hash);

    string tmp;
    
    ifstream last_commit_file(COMMITS_DIR + last_commit_hash);
    //skip first four lines
    for (int i = 0; i < 4; i++) {
        getline(last_commit_file, tmp);
    }
    
    //output fancy status
    cout << "Previous " << log::generate_message_for_commit(last_commit_hash, &tmp);

    vector< pair<string, string> > orig_state;
    vector< pair<string, string> > current_state;
    vector<string> changed_paths;
    
    cout << "File-to-hash at the last commit:" << endl;
    
    string orig_path;
    string file_hash;
   
    while (getline(last_commit_file, orig_path)) {
        getline(last_commit_file, file_hash);
        std::cout << "\t" << orig_path << " : " << file_hash << endl;
        orig_state.push_back(make_pair(orig_path, file_hash));
    }
    
    cout << "File-to-hash now:" << endl;
    
    vector<string> current_paths = utils::parse_file_tree(".");
    string current_hash = "";
    for (string current_path : current_paths) {
        current_hash = utils::hashfile(current_path);
        std::cout << "\t" << current_path << " : " << current_hash << endl;
        current_state.push_back(make_pair(current_path, current_hash));
    }
    
    auto it = begin(orig_state);
    while (it != end(orig_state)) {
        pair<string, string> orig_file = *it;
        bool found = false;
        bool changed = true;
        pair<string, string> found_file;
        int j = 0;
        for (pair<string, string> current_file : current_state) {
            if (orig_file.first == current_file.first) {
                found = true;
                found_file = current_file;
                if (orig_file.second == current_file.second) {
                    changed = false;
                }
                break;
            }
            j++;
        }
        if (found) {
            if (changed) {
                changed_paths.push_back(orig_file.first);
            }
            it = orig_state.erase(it);
            current_state.erase(current_state.begin() + j);
            continue;
        } else {
            it++;
        }
    }
    
    if ((current_state.size() == 0) && 
        (orig_state.size() == 0) &&
        (changed_paths.size() == 0)) {
        cout << "Working directory is clean, nothing changed." << endl;
        return 0;
    } else {
        cout << "Working directory status:" << endl;
        for (string s : changed_paths) {
            cout << "\tChanged: " << s << endl;
        }
        
        for (pair<string, string> current_file : current_state) {
             cout << "\tAdded  : " << current_file.first << endl;
        }
        
        for (pair<string, string> orig_file : orig_state) {
            cout << "\tRemoved: " << orig_file.first << endl;
        }
        
        return changed_paths.size() + orig_state.size() + current_state.size();
    }
}
