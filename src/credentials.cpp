#include "credentials.h"

#include <vector>
#include <string>
#include <iostream>

#include "utils.h"
#include "paths.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

#include "document.h"

using namespace std;
using namespace curlpp::options;
using namespace rapidjson;

bool credentials::are_credentials_present() {
    return utils::exists_file(utils::home_dir + SETTINGS_FILE);
}

bool credentials::save_credentials(string username, string password) {
    cout << "Checking credentials for user " << username << endl;
    int id = -1;
    int errcode = -1;
    
    Document d = utils::do_initial_get_request("/user/get", username, password, &errcode);
    if (errcode != 200) {
        if (d.HasMember("error")) {
            cerr << "Error occured: " << errcode << endl;
            
            assert(d.HasMember("status"));
            assert(d["status"].IsInt());
            cerr << "Status: " << d["status"].GetInt() << endl;
            
            assert(d.HasMember("message"));
            assert(d["message"].IsString());
            cerr << "Message: " << d["message"].GetString() << endl;
        } else {
            cerr << "Strange errcode occured : " << errcode << endl;
        }
        return false;
    }
    
    assert(d.HasMember("id"));
    assert(d["id"].IsInt());
    id = d["id"].GetInt();
    
    cout << "The user has id " << id << endl;
    
    assert(d.HasMember("firstname"));
    assert(d["firstname"].IsString());
    
    assert(d.HasMember("lastname"));
    assert(d["lastname"].IsString());
    
    assert(d.HasMember("email"));
    assert(d["email"].IsString());
    
    cout << "Your name is " << d["firstname"].GetString() << " " << d["lastname"].GetString() << " <" << d["email"].GetString() << ">" << endl;
    
    utils::make_dir(utils::home_dir + SETTINGS_DIR);
    utils::write_to_file(utils::home_dir + SETTINGS_FILE, to_string(id) + "\n" + username + "\n" + password + "\n");
    return true;
}
