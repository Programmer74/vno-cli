#include "merge.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include "utils.h"
#include "status.h"
#include "paths.h"

#include "document.h"
#include "commit.h"
#include "checkout.h"
#include "diff.h"

using namespace std;

void merge::do_merge(string their_branch_name) {

    string our_branch_name = utils::read_line_from_file(CUR_BRANCH_NAME_FILE, 0);

    cout << "Applying changes from '" << their_branch_name << "' called" << endl;
    cout << "Our branch is '" << our_branch_name << "'" << endl;
    
    cout << "Step 1: Checkout to '" << their_branch_name << "'" << endl;
    checkout::do_checkout(their_branch_name);
    
    cout << "Step 2: Generating diff file on branch '" << their_branch_name 
        << "' against '" << our_branch_name << "'" << endl;
    string diff_content = "";
    diff::do_diff(our_branch_name, diff_content);
    string filename = REPO_DIR;
    filename += "/diff_" + their_branch_name + "_" + our_branch_name;
    utils::write_to_file(filename, diff_content);
    cout << "Diff saved to " << filename << endl;
    
    cout << "Step 3: Checkout back to '" << our_branch_name << "'" << endl;
    checkout::do_checkout(our_branch_name);
    
    cout << "Step 4: Applying patch" << endl;
    string command = "patch -p0 <" + filename;
    system(command.c_str());
}
