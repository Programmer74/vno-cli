#include "commit.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>

#include "utils.h"
#include "paths.h"
#include "status.h"

#include "document.h"
#include "writer.h"
#include "stringbuffer.h"

#include "checkout.h"

using namespace std;
using namespace rapidjson;

void commit::do_commit(string commit_message, bool suppress_status) {
    cout << "Commit called" << endl;
    
    if ((!suppress_status) && (status::do_status() == 0)) {
        cerr << "Nothing to commit, working directory clean." << endl;
        return;
    }
    
    ofstream commitfile;
    string commitfile_path = TMP_COMMIT_FILE;
    commitfile.open(commitfile_path);
    
    // document is the root of a json message
	rapidjson::Document document;
 	// define the document as an object rather than an array
	document.SetObject();
	// must pass an allocator when the object may need to allocate memory
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    rapidjson::Value cobj(rapidjson::kObjectType);
        
    string parent;
    ifstream parentcfile(LAST_COMMIT_ID_FILE);
    getline(parentcfile, parent);
    
    //storing the new commit's parent commit hash
    commitfile << parent << endl;
    
    rapidjson::Value parent_ids_array(rapidjson::kArrayType);
    parent_ids_array.PushBack(rapidjson::Value(atoi(parent.c_str())), allocator);
    cobj.AddMember(rapidjson::Value("parentIds", allocator), parent_ids_array, allocator);   
    
    //storing the commit's message
    commitfile << commit_message << endl;
    cobj.AddMember(rapidjson::Value("message", allocator), rapidjson::Value(commit_message.c_str(), allocator), allocator);
    
    //storing the commit's author	
    string commit_author = utils::read_line_from_file(utils::home_dir + SETTINGS_FILE, 0);	
    commitfile << commit_author << endl;
    
    //storing the timestamp
    std::time_t result = std::time(nullptr);
    commitfile << result << endl;
    cobj.AddMember(rapidjson::Value("timestamp", allocator), rapidjson::Value(result), allocator);
    
    document.AddMember(rapidjson::Value("commit", allocator), cobj, allocator);
    
    //setup the array for json
    rapidjson::Value array(rapidjson::kArrayType);
    
    //copying files from tree to blob dir
    //storing working tree: 1st line for path, 2nd line for blob path
    //and also storing them to json lol
    vector<string> fnames = utils::parse_file_tree(WORKING_DIR);
    for (string fname : fnames) {
        std::string hash = utils::hashfile(fname);
        cout << fname << " : " << hash << endl;
        utils::copy_file(fname, BLOBS_DIR + hash);
        commitfile << fname << endl;
        commitfile << hash << endl;
        
        rapidjson::Value bobj(rapidjson::kObjectType);
        bobj.AddMember(rapidjson::Value("name", allocator), rapidjson::Value(fname.c_str(), allocator), allocator);
        bobj.AddMember(rapidjson::Value("mode", allocator), rapidjson::Value(0), allocator);
        string b64 = utils::base64_encode_file(fname);
        bobj.AddMember(rapidjson::Value("content", allocator), rapidjson::Value(b64.c_str(), allocator), allocator);
    
        array.PushBack(bobj, allocator);
    }
    
    document.AddMember(rapidjson::Value("blobs", allocator), array, allocator);
    
    commitfile.close();
    std::string commithash = utils::hashfile(commitfile_path);
    utils::move_file(commitfile_path, COMMITS_DIR + commithash);
    
    cout << "JSON:" << endl;
    StringBuffer strbuf;
	Writer<StringBuffer> writer(strbuf);
	document.Accept(writer);

	std::cout << strbuf.GetString() << std::endl;
	
	utils::repo_id = utils::read_line_from_file(REPO_ID_FILE, 0);
	utils::branch_id = utils::read_line_from_file(CUR_BRANCH_ID_FILE, 0);
	
	int errcode = -1;
	string ans = utils::do_put_request("/r/" + utils::repo_id + "/" + utils::branch_id + "/", strbuf.GetString(), &errcode);

	if (errcode == 200) {
	
        cout << "OK; answer : " << ans << endl;
	
        Document jans;
        jans.Parse(ans.c_str());
        
        assert(jans.HasMember("revision"));
        assert(jans["revision"].IsInt());
	
        int cid = jans["revision"].GetInt();
	
        //updating the last commit
        utils::write_to_file(LAST_COMMIT_ID_FILE, to_string(cid));
        
        //get current branch name
        string current_branch = utils::read_line_from_file(CUR_BRANCH_ID_FILE, 0);
        cout << "Current branch : " << current_branch << endl;
        //update the !!LOCAL!! branch to point to our last commit
        utils::write_to_file(BRANCHES_DIR + current_branch, to_string(cid));
        
        cout << "Commit done; id : " << cid << endl;
        
        checkout::do_checkout(to_string(cid));
	} else {   	
        cerr << "Error code : " << errcode << endl;
        cerr << "Message : " << ans << endl;
	}
}

void commit::do_dummy_commit_locally(int commit_id) {

    ofstream commitfile;
    string commitfile_path = TMP_COMMIT_FILE;
    commitfile.open(commitfile_path);
   
    string parent;
    ifstream parentcfile(LAST_COMMIT_ID_FILE);
    getline(parentcfile, parent);
    
    //storing the new commit's parent commit hash
    commitfile << "" << endl;
    
    //storing the commit's message
    commitfile << "" << endl;

    //storing the commit's author	
    commitfile << "" << endl;
    
    //storing the timestamp
    commitfile << "" << endl;
        
    vector<string> fnames = utils::parse_file_tree(WORKING_DIR);
    for (string fname : fnames) {
        std::string hash = utils::hashfile(fname);
        //cout << fname << " : " << hash << endl;
        utils::copy_file(fname, BLOBS_DIR + hash);
        commitfile << fname << endl;
        commitfile << hash << endl;
    }
        
    commitfile.close();
    utils::move_file(commitfile_path, COMMITS_DIR + to_string(commit_id));
}
