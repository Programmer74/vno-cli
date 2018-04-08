#include "init.h"

#include <vector>
#include <string>
#include <iostream>

#include "utils.h"

using namespace std;

void init::do_init() {
    cout << "Init called" << endl;
    
    utils::make_dir(".vno");
    utils::make_dir(".vno/commits");
    utils::make_dir(".vno/blobs");
    utils::make_dir(".vno/branches");
    
    utils::write_to_file(".vno/head", "null");
    utils::write_to_file(".vno/current_branch", "master");
    utils::write_to_file(".vno/branches/master", "null");
    
    cout << "Init done" << endl;
}
