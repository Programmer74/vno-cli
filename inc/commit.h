#ifndef COMMIT_H
#define COMMIT_H

#include <vector>
#include <string>
#include <iostream>

class commit
{
public:
    static void do_commit(std::string commit_message, bool suppress_status);
    static void do_dummy_commit_locally(int commit_id);
};

#endif // COMMIT_H
