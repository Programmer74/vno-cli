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
    
    static int make_dir(std::string path);
    static int copy_file(std::string source, std::string dest);
    static int move_file(std::string source, std::string dest);
    static int remove_file(std::string path);
    static int rm_rf_files();
    static bool exists_file (std::string name);
    static int write_to_file(std::string filename, std::string to_write);
    static std::string read_line_from_file(std::string filename, int line_no);
    
    static std::string hashfile(std::string filename);
    
    static void print(std::string to_print);

    static std::vector<std::string> parse_file_tree(std::string pat);
    
    static rapidjson::Document do_initial_get_request(std::string url, std::string username, std::string password, int* response_code);
    static rapidjson::Document do_get_request(std::string url, int* response_code);
    
    static std::string get_userstuff_by_user_id(int id);
};

#endif // UTILS_H
