#include "branch.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "paths.h"

using namespace std;

void branch::do_branch(string branch_name) {
    cout << "Branch called" << endl;
    cout << "Creating branch " << branch_name << "..." << endl;
    
    //get last commit hash
    string last_commit_hash = utils::read_line_from_file(LAST_COMMIT_ID_FILE, 0);

    //update the new branch to point to our last commit
    utils::write_to_file(BRANCHES_DIR + branch_name, last_commit_hash);
    
    cout << "Creating branch " << branch_name << " done." << endl;
    cout << "Remember to checkout to branch to develop there!" << endl;
}
