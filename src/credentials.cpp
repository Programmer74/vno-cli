#include "credentials.h"

#include <vector>
#include <string>
#include <iostream>

#include "utils.h"
#include "paths.h"

using namespace std;

bool credentials::are_credentials_present() {
    return utils::exists_file(utils::home_dir + SETTINGS_FILE);
}

bool credentials::save_credentials(string username, string password) {
    cout << "Checking credentials for user " << username << endl;
    int id = 1337;
    cout << "The user has id " << id << endl;
    
    utils::make_dir(utils::home_dir + SETTINGS_DIR);
    utils::write_to_file(utils::home_dir + SETTINGS_FILE, to_string(id) + "\n" + username + "\n" + password + "\n");
    return true;
}
