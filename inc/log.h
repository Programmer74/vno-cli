#ifndef LOG_H
#define LOG_H

#include <vector>
#include <string>
#include <iostream>

class log
{
public:
    static void do_log();
    static std::string generate_message_for_commit(std::string commit_id, std::string* prev_commit_hash);
};

#endif // COMMIT_H
