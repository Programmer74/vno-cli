#include "log.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

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
        string author;
        string timestamp;
        
        getline(commitfile, parent_commit);
        getline(commitfile, commit_message);
        getline(commitfile, author);
        getline(commitfile, timestamp);
        std::time_t commit_time = atol(timestamp.c_str());
        int author_id = atoi(author.c_str());
        
        author = utils::get_userstuff_by_user_id(author_id);
        
        cout << "commit " << commit_hash << endl;
        cout << "Author: " << author << endl;
        cout << "Date:   " << std::asctime(std::localtime(&commit_time)) << endl;
        
        cout << "\t" << commit_message << endl << endl;
    
        commit_hash = parent_commit;
    }
    
}
