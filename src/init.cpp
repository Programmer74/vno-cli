#include "init.h"

#include <vector>
#include <string>
#include <iostream>

#include "utils.h"
#include "paths.h"

using namespace std;

void init::do_init() {
    cout << "Init called" << endl;
    
    utils::make_dir(REPO_DIR);
    utils::make_dir(COMMITS_DIR);
    utils::make_dir(BLOBS_DIR);
    utils::make_dir(BRANCHES_DIR);
    
    utils::write_to_file(LAST_COMMIT_ID_FILE, "null");
    utils::write_to_file(CUR_BRANCH_ID_FILE, "master");
    utils::write_to_file(BRANCHES_DIR "master", "null");
    utils::write_to_file(REPO_ID_FILE, "1");
    
    cout << "Init done" << endl;
}
