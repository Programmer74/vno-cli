#ifndef CHECKOUT_H
#define CHECKOUT_H

#include <vector>
#include <string>
#include <iostream>

class checkout
{
public:
    static void do_checkout(std::string commit_hash);
};

#endif // CHECKOUT_H
