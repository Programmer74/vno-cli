#include "clone.h"

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
#include "init.h"
#include "checkout.h"

using namespace std;
using namespace rapidjson;

void clone::do_clone(int repo_id) {
    cout << "Trying to retrieve info about repo " << repo_id << "..." << endl;

    int errcode = -1;
    Document d = utils::do_get_request("/repo/" + to_string(repo_id), &errcode);
    
    string repo_name = "";
    string repo_desc = "";
    int branch_id = 0;
    
    if (errcode == 200) {
        assert(d.HasMember("name"));
        assert(d["name"].IsString());
        repo_name = d["name"].GetString();
        
        assert(d.HasMember("description"));
        assert(d["description"].IsString());
        repo_desc = d["description"].GetString();
        
        assert(d.HasMember("branchIds"));
        const Value& a = d["branchIds"];
        assert(a.IsArray());
        branch_id = a[0].GetInt();
    } else {
        cout << "Error " << errcode << " while trying to clone repo " << repo_id << endl;
    }
    
    cout << "Trying to clone " << repo_name << ": " << repo_desc << " ... " << endl;
    utils::make_dir(repo_name);
    utils::change_dir(repo_name);
    init::do_init();
    
    utils::write_to_file(REPO_ID_FILE, to_string(repo_id));
    utils::write_to_file(CUR_BRANCH_ID_FILE, to_string(branch_id));
    
    string branch_name;
    int commit_id = utils::get_head_by_branch_id(repo_id, branch_id, branch_name);
    
    utils::write_to_file(CUR_BRANCH_NAME_FILE, branch_name);
    
    cout << "Trying to checkout to branch " << branch_name << ", commit id " << commit_id << "..." << endl;
    
    d = utils::do_get_request("/r/full/" + to_string(repo_id) + "/" + to_string(branch_id) + "/" + to_string(commit_id) + "/", &errcode);
     
    if (errcode == 404) {
        cerr << "Seems that the commit information is wrong. Trying to recreate the branch..." << endl;
        utils::write_to_file("./README.md", "# " + repo_name + "\n\n" +repo_desc);
        commit::do_commit("Initial commit", true);
        
        commit_id = utils::get_head_by_branch_id(repo_id, branch_id, branch_name);
        cout << "Trying to checkout to branch " << branch_name << ", commit id " << commit_id << "..." << endl;
    }
    
    checkout::do_checkout(to_string(commit_id));
}
