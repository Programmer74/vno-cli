#include "log.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "paths.h"

using namespace std;

void log::do_log() {
    cout << "Log called" << endl;
    
    string commit_hash;
    ifstream current_commit_file(LAST_COMMIT_ID_FILE);
    getline(current_commit_file, commit_hash);
    
    
    while (commit_hash != "null") {
        ifstream commitfile;
        commitfile.open(COMMITS_DIR + commit_hash);
    
        string parent_commit;
        string commit_message;
        
        getline(commitfile, parent_commit);
        getline(commitfile, commit_message);
        
        cout << "\t" << commit_hash << " : " << commit_message << endl;
    
        commit_hash = parent_commit;
    }
    
}
