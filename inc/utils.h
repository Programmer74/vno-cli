#ifndef UTILS_H
#define UTILS_H

#include <glob.h>
#include <vector>
#include <string>
#include <iostream>
#include "document.h"

class utils
{
public:
    static std::string home_dir;
    static std::string repo_id;
    static std::string branch_id;
    
    static std::string exec(std::string cmd);
    static int change_dir(std::string path);
    static int make_dir(std::string path);
    static int copy_file(std::string source, std::string dest);
    static int move_file(std::string source, std::string dest);
    static int remove_file(std::string path);
    static int rm_rf_files();
    static bool exists_file (std::string name);
    static int write_to_file(std::string filename, std::string to_write);
    static std::string read_line_from_file(std::string filename, int line_no);
    
    static std::string hashfile(std::string filename);
    static std::string replace_substring(std::string original, std::string to_find, 
            std::string to_replace, int iterations);
    static std::string base64_encode_file(std::string filename);
    static bool base64_decode_file(std::string filename, std::string decoded_string);
    
    static void print(std::string to_print);

    static std::vector<std::string> parse_file_tree(std::string pat);
    
    static rapidjson::Document do_initial_get_request(std::string url, std::string username, std::string password, int* response_code);
    static rapidjson::Document do_get_request(std::string url, int* response_code);
    static std::string do_put_request(std::string url, std::string body, int* response_code);
    
    static std::string get_userstuff_by_user_id(int id);
    static int get_head_by_branch_id(int repo_id, int branch_id, std::string & branch_name);
    static int get_head_by_branch_name(std::string requiredBranchName, int* required_branch_id);
};

#endif // UTILS_H
