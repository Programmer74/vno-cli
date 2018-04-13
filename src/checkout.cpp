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

using namespace std;
using namespace rapidjson;

void checkout_to_commit(string commit_hash) {
    cout << "Checking out to " << commit_hash << "..." << endl;

    utils::rm_rf_files();
    cout << "Working directory cleaned" << endl;

    string orig_path;
    string parent_commit;

    int errcode = -1;
    Document d = utils::do_get_request("/r/full/1/1/" + commit_hash + "/", &errcode);
    string s = "";
    
    if (errcode != 200) {
        cerr << "Error code " << errcode << " while trying to checkout to commit " << commit_hash << endl;
    }
    assert(d.HasMember("commit"));
    assert(d["commit"].IsObject());
    
    const Value& c = d["commit"];
    
    assert(c.HasMember("revision"));
    assert(c["revision"].IsInt());
    int this_commit_id = c["revision"].GetInt();
    
    assert(c.HasMember("parentIds"));
    const Value& a = c["parentIds"];
    assert(a.IsArray());
    int prev_commit_id = a[0].GetInt();
    parent_commit = to_string(prev_commit_id);
   
    assert(c.HasMember("authorId"));
    assert(c["authorId"].IsInt());
    
    int author_id = c["authorId"].GetInt();
    string author = utils::get_userstuff_by_user_id(author_id);
    
    assert(c.HasMember("message"));
    assert(c["message"].IsString());
    string commit_message = c["message"].GetString();
    
    assert(c.HasMember("timestamp"));
    assert(c["timestamp"].IsInt64());
    time_t commit_time = c["timestamp"].GetInt64();

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
    
    cout << "Checkout done." << endl;
    
    /*return ss.str();
    
    getline(commitfile, parent_commit);
    getline(commitfile, commit_message);
    getline(commitfile, author);
    getline(commitfile, timestamp);
    while (getline(commitfile, line))
    {
        orig_path = line;
        getline(commitfile, line);
        blob_path = BLOBS_DIR + line;
        //generate dir if required
        for (int i = orig_path.size() - 1; i > 2; i--) {
            if (orig_path[i] == '/') {
                string dir_path = orig_path.substr(0, i);
                cout << "Making directory " << dir_path << endl;
                utils::make_dir(dir_path);
            }
        }
        //copy file from blobs to working dir
        std::cout << "Copying " << blob_path << " to " << orig_path << endl;
        utils::copy_file(blob_path, orig_path);
    }
    
    commitfile.close();
    cout << "Checkout to " << commit_hash << " done." << endl;
    cout << "Message: " << commit_message << endl;
    cout << "Parent commit: " << parent_commit << endl;
    
    //updating to this commit
    utils::write_to_file(LAST_COMMIT_ID_FILE, commit_hash);*/
}

void checkout::do_checkout(string checkout_to) {
    cout << "Checkout to " << checkout_to << " called" << endl;
    
    /*if (status::do_status() != 0) {
        cerr << "Cannot do checkout: working directory not clean!" << endl;
        return;
    }*/
    
    //if (utils::exists_file(COMMITS_DIR + checkout_to)) {
        checkout_to_commit(checkout_to);
        return;
    //}
    
    /*if (utils::exists_file(BRANCHES_DIR + checkout_to)) {
        string commit_hash = utils::read_line_from_file(BRANCHES_DIR + checkout_to, 0);
        checkout_to_commit(commit_hash);
        return;
    }*/
    
    //cerr << "Cannot find " << checkout_to << endl;
}
