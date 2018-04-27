#include "log.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#include "utils.h"
#include "paths.h"

#include "document.h"

using namespace std;
using namespace rapidjson;

string log::generate_message_for_commit(string commit_id, string* prev_commit_hash) {
    
    if ((commit_id == "0") || (commit_id == "null")) {
        return "No such commit\n";
    }
   
    utils::repo_id = utils::read_line_from_file(REPO_ID_FILE, 0);
   
    int errcode = -1;
    Document d = utils::do_get_request("/r/" + utils::repo_id + "/1/" + commit_id + "/", &errcode);
    string s = "";
    
    if (errcode == 200) {
    
        assert(d.HasMember("revision"));
        assert(d["revision"].IsInt());
        
        assert(d.HasMember("parentIds"));
        
        const Value& a = d["parentIds"];
        assert(a.IsArray());
        int prev_commit_id = a[0].GetInt();
        
        assert(d.HasMember("authorId"));
        assert(d["authorId"].IsInt());
        int author_id = d["authorId"].GetInt();
        
        assert(d.HasMember("message"));
        assert(d["message"].IsString());
        string commit_message = d["message"].GetString();
        
        assert(d.HasMember("timestamp"));
        assert(d["timestamp"].IsInt64());
        time_t commit_time = d["timestamp"].GetInt64();
        
        string author;
    
        *prev_commit_hash = to_string(prev_commit_id);
        author = utils::get_userstuff_by_user_id(author_id);
        
        stringstream ss;
        
        ss << "commit " << commit_id << endl;
        ss << "Author: " << author << endl;
        ss << "Date:   " << std::asctime(std::localtime(&commit_time)) << endl;
        
        ss << "\t" << commit_message << endl << endl;
        
        return ss.str();
    } else {
        s = "<some commit with id = " + commit_id + ">";
        cerr << "Error code " << errcode << " while retreiving info about commit " << commit_id << endl;
        return s;
    }
}

void log::do_log() {
    cout << "Log called" << endl;
    
    string commit_hash;
    ifstream current_commit_file(LAST_COMMIT_ID_FILE);
    getline(current_commit_file, commit_hash);
    
    int i = 0;
    
    while ((commit_hash != "null") && (commit_hash != "0")) {

        string parent_commit;
        string info;
        info = generate_message_for_commit(commit_hash, &parent_commit);
        cout << info;
        commit_hash = parent_commit;
        
        i++;
        
        if (i >= 10) {
            cerr << "Exceeded max commit number" << endl;
            break;
        }
    }
    
}
