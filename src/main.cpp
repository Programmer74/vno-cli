#include <vector>
#include <string>
#include <iostream>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "init.h"
#include "commit.h"
#include "checkout.h"
#include "status.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc == 1) {
        cerr << "RTFM" << endl;
        return 0;
    }
    if (strcmp(argv[1], "init") == 0) {
        init::do_init();
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc < 2) {
            cerr << "Commit message not specified." << endl;
            return 0;
        }
        commit::do_commit(argv[2]);
    } else if (strcmp(argv[1], "checkout") == 0) {
        if (argc < 2) {
            cerr << "Commit hash or branch name not specified." << endl;
            return 0;
        }
        checkout::do_checkout(argv[2]);
    } else if (strcmp(argv[1], "status") == 0) {
        status::do_status();
    } else {
        cerr << "RTFM" << endl;
        return 0;
    }
}
