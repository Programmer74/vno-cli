#include "checkout.h"

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

using namespace std;
using namespace rapidjson;

void checkout_to_commit(string commit_hash) {
    cout << "Checking out to " << commit_hash << "..." << endl;

    utils::rm_rf_files();
    cout << "Working directory cleaned" << endl;

    string orig_path;
    string parent_commit;

    utils::repo_id = utils::read_line_from_file(REPO_ID_FILE, 0);
    utils::branch_id = utils::read_line_from_file(CUR_BRANCH_ID_FILE, 0);
    
    int errcode = -1;
    Document d = utils::do_get_request("/r/" + utils::repo_id + "/" + utils::branch_id + "/" + commit_hash + "/", &errcode);
    string s = "";
    
    if (errcode != 200) {
        cerr << "Error code " << errcode << " while trying to checkout to commit " << commit_hash << endl;
    }

    /*assert(d.HasMember("commit"));
    assert(d["commit"].IsObject());

    const Value& c = d["commit"];*/
    
    assert(d.HasMember("revision"));
    assert(d["revision"].IsInt());
    int this_commit_id = d["revision"].GetInt();

    assert(d.HasMember("parents"));
    const Value& a = d["parents"];
    assert(a.IsArray());
    if (a.GetArray().Size() == 0) {
        cout << "No parent commit for this one." << endl;
        parent_commit = "0";
    } else {
        const Value& ad = a[0];
        int prev_commit_id = ad["revision"].GetInt();
        parent_commit = to_string(prev_commit_id);
    }

    //assert(d.HasMember("authorId"));
    string author = "<someone>";
    //if (c["authorId"].IsInt()) {
        int author_id = 1;//c["authorId"].GetInt();
        author = utils::get_userstuff_by_user_id(author_id);
    //}

    assert(d.HasMember("message"));
    assert(d["message"].IsString());
    string commit_message = d["message"].GetString();
    
    assert(d.HasMember("timestamp"));
    assert(d["timestamp"].IsInt64());
    time_t commit_time = d["timestamp"].GetInt64();

    stringstream ss;

    ss << "commit " << this_commit_id << endl;
    ss << "Author: " << author << endl;
    ss << "Date:   " << std::asctime(std::localtime(&commit_time)) << endl;
    
    ss << "\t" << commit_message << endl << endl;
    cout << ss.str() << endl;
    
    assert(d.HasMember("blobs"));
    const Value& b = d["blobs"];
    
    assert(b.IsArray());
    
    string based64_file = "";
    int blobid = 0;
    for (SizeType i = 0; i < b.Size(); i++) {
        //f*cking awesome
        orig_path = d["blobs"][i]["name"].GetString();
        based64_file = d["blobs"][i]["content"].GetString();
        blobid = d["blobs"][i]["id"].GetInt();
        cout << "Inflating blob with id=" << blobid << " to " << orig_path << endl;
        utils::base64_decode_file(orig_path, based64_file);
    }
    
    //do dummy checkout for vno-cli status being fine
    commit::do_dummy_commit_locally(this_commit_id);
    
    cout << "Checkout done." << endl;
    
    //updating to this commit
    utils::write_to_file(LAST_COMMIT_ID_FILE, commit_hash);
}

void checkout::do_checkout(string checkout_to) {
    cout << "Checkout to " << checkout_to << " called" << endl;
    
    utils::repo_id = utils::read_line_from_file(REPO_ID_FILE, 0);
    
    /*if (status::do_status() != 0) {
        cerr << "Cannot do checkout: working directory not clean!" << endl;
        return;
    }*/
    
    if ((checkout_to[0] < '0') || (checkout_to[0] > '9')) {
        //checking out to branch

        int required_branch_id = -1;
        int required_commit_id = utils::get_head_by_branch_name(checkout_to, &required_branch_id);
        
        cout << "Required commit id is " << required_commit_id << endl;
        
        if (required_commit_id <= 0) return;
        
        utils::write_to_file(CUR_BRANCH_ID_FILE, to_string(required_branch_id));
        utils::write_to_file(CUR_BRANCH_NAME_FILE, checkout_to);
        checkout_to_commit(to_string(required_commit_id));
    } else {
        checkout_to_commit(checkout_to);
        return;
    }
    
    /*if (utils::exists_file(BRANCHES_DIR + checkout_to)) {
        string commit_hash = utils::read_line_from_file(BRANCHES_DIR + checkout_to, 0);
        checkout_to_commit(commit_hash);
        return;
    }*/
    
    //cerr << "Cannot find " << checkout_to << endl;
}
