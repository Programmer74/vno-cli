#include "checkout.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "status.h"
#include "paths.h"

using namespace std;

void checkout_to_commit(string commit_hash) {
    cout << "Checking out to " << commit_hash << "..." << endl;

    utils::rm_rf_files();
    cout << "Working directory cleaned" << endl;
    
    ifstream commitfile;
    commitfile.open(COMMITS_DIR + commit_hash);

    string line;
    string orig_path;
    string blob_path;
    string parent_commit;
    string commit_message;
    
    getline(commitfile, parent_commit);
    getline(commitfile, commit_message);
    while (getline(commitfile, line))
    {
        orig_path = line;
        getline(commitfile, line);
        blob_path = BLOBS_DIR + line;
        //generate dir if required
        for (int i = orig_path.size() - 1; i > 2; i--) {
            if (orig_path[i] == '/') {
                string dir_path = orig_path.substr(0, i);
                cout << "Making directory " << dir_path << endl;
                utils::make_dir(dir_path);
            }
        }
        //copy file from blobs to working dir
        std::cout << "Copying " << blob_path << " to " << orig_path << endl;
        utils::copy_file(blob_path, orig_path);
    }
    
    commitfile.close();
    cout << "Checkout to " << commit_hash << " done." << endl;
    cout << "Message: " << commit_message << endl;
    cout << "Parent commit: " << parent_commit << endl;
    
    //updating to this commit
    utils::write_to_file(LAST_COMMIT_ID_FILE, commit_hash);
}

void checkout::do_checkout(string checkout_to) {
    cout << "Checkout to " << checkout_to << " called" << endl;
    
    if (status::do_status() != 0) {
        cerr << "Cannot do checkout: working directory not clean!" << endl;
        return;
    }
    
    if (utils::exists_file(COMMITS_DIR + checkout_to)) {
        checkout_to_commit(checkout_to);
        return;
    }
    
    if (utils::exists_file(BRANCHES_DIR + checkout_to)) {
        string commit_hash = utils::read_line_from_file(BRANCHES_DIR + checkout_to, 0);
        checkout_to_commit(commit_hash);
        return;
    }
    
    cerr << "Cannot find " << checkout_to << endl;
}
