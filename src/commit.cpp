#include "commit.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "utils.h"
#include "paths.h"

using namespace std;

void commit::do_commit(string commit_message) {
    cout << "Commit called" << endl;
    
    ofstream commitfile;
    string commitfile_path = TMP_COMMIT_FILE;
    commitfile.open(commitfile_path);
        
    string parent;
    ifstream parentcfile(LAST_COMMIT_ID_FILE);
    getline(parentcfile, parent);
    
    //storing the new commit's parent commit hash
    commitfile << parent << endl;
    
    //storing the commit's message
    commitfile << commit_message << endl;
    
    //storing the commit's author
    commitfile << utils::read_line_from_file(utils::home_dir + SETTINGS_FILE, 0) << endl;
    
    //storing the timestamp
    std::time_t result = std::time(nullptr);
    commitfile << result << endl;
    
    //copying files from tree to blob dir
    //storing working tree: 1st line for path, 2nd line for blob path
    vector<string> fnames = utils::parse_file_tree(WORKING_DIR);
    for (string fname : fnames) {
        std::string hash = utils::hashfile(fname);
        cout << fname << " : " << hash << endl;
        utils::copy_file(fname, BLOBS_DIR + hash);
        commitfile << fname << endl;
        commitfile << hash << endl;
    }
    commitfile.close();
    std::string commithash = utils::hashfile(commitfile_path);
    utils::move_file(commitfile_path, COMMITS_DIR + commithash);
    
    //updating the last commit
    utils::write_to_file(LAST_COMMIT_ID_FILE, commithash);
    
    //get current branch name
    string current_branch = utils::read_line_from_file(CUR_BRANCH_ID_FILE, 0);
    cout << "Current branch : " << current_branch << endl;
    //update the branch to point to our last commit
    utils::write_to_file(BRANCHES_DIR + current_branch, commithash);
    
    cout << "Commit done; hash : " << commithash << endl;
}
