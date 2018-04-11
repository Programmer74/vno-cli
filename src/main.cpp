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
#include "log.h"
#include "branch.h"
#include "rest.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc == 1) {
        cerr << "RTFM" << endl;
        return 0;
    }
    if (strcmp(argv[1], "init") == 0) {
        init::do_init();
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc < 3) {
            cerr << "Commit message not specified." << endl;
            return 0;
        }
        commit::do_commit(argv[2]);
    } else if (strcmp(argv[1], "checkout") == 0) {
        if (argc < 3) {
            cerr << "Commit hash or branch name not specified." << endl;
            return 0;
        }
        checkout::do_checkout(argv[2]);
    } else if (strcmp(argv[1], "status") == 0) {
        status::do_status();
    } else if (strcmp(argv[1], "log") == 0) {
        log::do_log();
    } else if (strcmp(argv[1], "branch") == 0) {
        if (argc < 3) {
            cerr << "Branch name not specified." << endl;
            return 0;
        }
        branch::do_branch(argv[2]);
    } else if (strcmp(argv[1], "rest") == 0) {
        if (argc < 3) {
            cerr << "URL for rest not specified." << endl;
            return 0;
        }
        rest::do_rest(argv[2]);
    }else {
        cerr << "RTFM" << endl;
        return 0;
    }
}
