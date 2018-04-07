#include "commit.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"

using namespace std;

void commit::do_commit(string commit_message) {
    cout << "Commit called" << endl;
    
    ofstream commitfile;
    string commitfile_path = ".vno/commits/tmpcommit";
    commitfile.open(commitfile_path);
        
    string parent;
    ifstream parentcfile(".vno/head");
    getline(parentcfile, parent);
    
    //storing the new commit's parent commit hash
    commitfile << parent << endl;
    
    //storing the commit's message
    commitfile << commit_message << endl;
    
    //copying files from tree to blob dir
    //storing working tree: 1st line for path, 2nd line for blob path
    vector<string> fnames = utils::parse_file_tree(".");
    for (string fname : fnames) {
        std::string hash = utils::hashfile(fname);
        cout << fname << " : " << hash << endl;
        utils::copy_file(fname, ".vno/blobs/" + hash);
        commitfile << fname << endl;
        commitfile << hash << endl;
    }
    commitfile.close();
    std::string commithash = utils::hashfile(commitfile_path);
    utils::move_file(commitfile_path, ".vno/commits/" + commithash);
    
    //updating the last commit
    utils::write_to_file(".vno/head", commithash);
    
    cout << "Commit done; hash : " << commithash << endl;
}
