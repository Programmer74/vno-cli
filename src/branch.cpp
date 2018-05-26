#include "branch.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "utils.h"
#include "paths.h"

#include "document.h"
#include "commit.h"
#include "init.h"
#include "checkout.h"

using namespace std;
using namespace rapidjson;

void branch::do_branch(string branch_name) {

    utils::repo_id = utils::read_line_from_file(REPO_ID_FILE, 0);
    
    vector < pair<string, int> > branches;

    if (branch_name == "") {
        int errcode = -1;
        Document d = utils::do_get_request("/repo/" + utils::repo_id, &errcode);
        
        if (errcode == 200) {
            
            assert(d.HasMember("branches"));
            const Value& a = d["branches"];
            assert(a.IsArray());
            
            for (uint i = 0; i < a.GetArray().Size(); i++) {
                string branchName = "";
                utils::get_head_by_branch_id(atoi(utils::repo_id.c_str()), a[i]["branch"].GetInt(), branchName);
                branches.push_back(make_pair(branchName, a[i]["branch"].GetInt()));
            }
            
        } else {
            cout << "Error " << errcode << " while trying to get branch ids." << endl;
        }
    
        cout << "Available branches:" << endl;
        cout << "Id Name" << endl;
        for (pair<string, int> branch : branches) {
            cout << branch.second << " " << branch.first << endl;
        }
    } else {
        cout << "Creating branch " << branch_name << "..." << endl;
        
        //get last commit hash
        string last_commit_hash = utils::read_line_from_file(LAST_COMMIT_ID_FILE, 0);
        
        cout << "Last commit: " << last_commit_hash << endl;
            
        int errcode;
        string ans = utils::do_put_request("/ref/" + utils::repo_id + "/" + last_commit_hash + "/" + branch_name, "", &errcode);
         
        if (errcode != 200) {
            cerr << "Error code " << errcode << ":" << endl;
            cerr << ans << endl;
            return;
        }
        
        //update the new branch to point to our last commit
        utils::write_to_file(BRANCHES_DIR + branch_name, last_commit_hash);
        
        cout << "Creating branch " << branch_name << " done." << endl;
        cout << "Remember to checkout to branch to develop there!" << endl;
    }
}
