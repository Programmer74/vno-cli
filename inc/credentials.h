#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <vector>
#include <string>
#include <iostream>

class credentials
{
public:
    static bool are_credentials_present();
    static bool save_credentials(std::string username, std::string password);
};

#endif // INIT_H
