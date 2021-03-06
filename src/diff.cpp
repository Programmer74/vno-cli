#include "diff.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include <sstream>

#include "utils.h"
#include "paths.h"
#include "log.h"

using namespace std;

int diff::do_diff(string diff_to, string & out_data) {
    
    std::stringstream sout;

    sout << "### Diff against '" << diff_to << "' is called" << endl;

    string diff_to_commit_hash;
    
    if (diff_to == "") {
        ifstream headfile(LAST_COMMIT_ID_FILE);
        getline(headfile, diff_to_commit_hash);
    } else {
        if ((diff_to[0] < '0') || (diff_to[0] > '9')) {
            //branch name instead of commit id
            
            sout << "### Getting id of '" << diff_to << "'" << endl;
            
            int required_branch_id = -1;
            int required_commit_id = utils::get_head_by_branch_name(diff_to, &required_branch_id);
            
            if (required_commit_id <= 0) return -1;
            
            diff_to = to_string(required_commit_id);
        }
        diff_to_commit_hash = diff_to;
    }
    
    sout << "### Generating diff against " << diff_to_commit_hash << endl;
    
    string tmp;
    
    ifstream last_commit_file(COMMITS_DIR + diff_to_commit_hash);
    //skip first four lines
    for (int i = 0; i < 4; i++) {
        getline(last_commit_file, tmp);
    }
    
    vector< pair<string, string> > orig_state;
    vector< pair<string, string> > current_state;
    vector< pair<string, string> > changed_files;
       
    string orig_path;
    string file_hash;
   
    while (getline(last_commit_file, orig_path)) {
        getline(last_commit_file, file_hash);
        orig_state.push_back(make_pair(orig_path, file_hash));
    }
    
    vector<string> current_paths = utils::parse_file_tree(".");
    string current_hash = "";
    for (string current_path : current_paths) {
        current_hash = utils::hashfile(current_path);
        current_state.push_back(make_pair(current_path, current_hash));
    }
    
    auto it = begin(orig_state);
    while (it != end(orig_state)) {
        pair<string, string> orig_file = *it;
        bool found = false;
        bool changed = true;
        pair<string, string> found_file;
        int j = 0;
        for (pair<string, string> current_file : current_state) {
            if (orig_file.first == current_file.first) {
                found = true;
                found_file = current_file;
                if (orig_file.second == current_file.second) {
                    changed = false;
                }
                break;
            }
            j++;
        }
        if (found) {
            if (changed) {
                changed_files.push_back(orig_file);
            }
            it = orig_state.erase(it);
            current_state.erase(current_state.begin() + j);
            continue;
        } else {
            it++;
        }
    }
    
    if ((current_state.size() == 0) && 
        (orig_state.size() == 0) &&
        (changed_files.size() == 0)) {
        sout << endl;
        out_data = sout.str();
        return 0;
    } else {
        for (pair<string, string> s : changed_files) {
            sout << "### Changed " << s.first << ": was " << s.second << ", now " << utils::hashfile(s.first) << endl;
            sout << utils::exec("diff -u " + string(BLOBS_DIR) + s.second + " " + s.first) << endl;
        }
        
        for (pair<string, string> current_file : current_state) {
             sout << "### Added " << current_file.first << ": now " << current_file.second << endl;
             sout << utils::exec("diff -u /dev/null " + current_file.first) << endl;
        }
        
        for (pair<string, string> orig_file : orig_state) {
            sout << "### Removed " << orig_file.first << ": was " << orig_file.second << endl;
            string rm_diff = utils::exec("diff -u " + string(BLOBS_DIR) + orig_file.second + " /dev/null");
            //replacing path ./vno/blobs/XXX to real path in diff output
            rm_diff = utils::replace_substring(rm_diff, string(BLOBS_DIR) + orig_file.second, orig_file.first, 0);
            sout << rm_diff << endl;
        }
        out_data = sout.str();
        return changed_files.size() + orig_state.size() + current_state.size();
    }
}
