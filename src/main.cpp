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
#include "credentials.h"
#include "diff.h"
#include "clone.h"
#include "paths.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc == 1) {
        cerr << "RTFM" << endl;
        return 0;
    }
    
    utils::home_dir = getenv("HOME");
    utils::home_dir += "/";
    
    if (strcmp(argv[1], "credentials") == 0) {
        if (argc < 4) {
            cerr << "Please specify <username> <password>" << endl;
            return 0;
        }
        credentials::save_credentials(argv[2], argv[3]);
        return 0;
    } else if (!credentials::are_credentials_present()) {
        cerr << "I don't know who you are. Please, do vno-cli credentials <username> <password>" << endl;
        return 0;
    }
    
    if (strcmp(argv[1], "init") != 0) {
        //assume that there is a repo
        utils::repo_id = utils::read_line_from_file(REPO_ID_FILE, 0);
        utils::branch_id = utils::read_line_from_file(CUR_BRANCH_ID_FILE, 0);
    }
    
    if (strcmp(argv[1], "init") == 0) {
        init::do_init();
    } else if (strcmp(argv[1], "commit") == 0) {
        if (argc < 3) {
            cerr << "Commit message not specified." << endl;
            return 0;
        }
        commit::do_commit(argv[2], false);
    } else if (strcmp(argv[1], "checkout") == 0) {
        if (argc < 3) {
            cerr << "Commit hash or branch name not specified." << endl;
            return 0;
        }
        checkout::do_checkout(argv[2]);
    } else if (strcmp(argv[1], "status") == 0) {
        status::do_status();
    } else if (strcmp(argv[1], "diff") == 0) {
        if (argc < 3) {
            diff::do_diff("");
        } else {
            diff::do_diff(argv[2]);
        }
    } else if (strcmp(argv[1], "log") == 0) {
        log::do_log();
    } else if (strcmp(argv[1], "branch") == 0) {
        if (argc < 3) {
            branch::do_branch("");
        } else {
            branch::do_branch(argv[2]);
        }
    } else if (strcmp(argv[1], "rest") == 0) {
        if (argc < 3) {
            cerr << "URL for rest not specified." << endl;
            return 0;
        }
        rest::do_rest(argv[2]);
    } else if (strcmp(argv[1], "clone") == 0) {
        if (argc < 3) {
            cerr << "RepoID not specified." << endl;
            return 0;
        }
        clone::do_clone(atoi(argv[2]));
    }else {
        cerr << "RTFM" << endl;
        return 0;
    }
}
