#include "checkout.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"

using namespace std;

void checkout::do_checkout(string commit_hash) {
    cout << "Checkout to " << commit_hash << " called" << endl;
    
    utils::rm_rf_files();
    cout << "Working directory cleaned" << endl;
    
    if (!utils::exists_file(".vno/commits/" + commit_hash)) {
        cerr << "Cannot find " << commit_hash << endl;
        return;
    }
    
    ifstream commitfile;
    commitfile.open(".vno/commits/" + commit_hash);

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
        blob_path = ".vno/blobs/" + line;
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
    utils::write_to_file(".vno/head", commit_hash);
}
