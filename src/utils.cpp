#include "utils.h"

#include <glob.h>
#include <vector>
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdint>
#include <iomanip>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <ctime>
#include <list>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>
#include <curlpp/Types.hpp>

#include "document.h"
#include "paths.h"

using namespace std;
using namespace curlpp::options;
using namespace rapidjson;

std::string utils::home_dir = "";

int utils::make_dir(string path) {
    int res = mkdir(path.c_str(), ACCESSPERMS);
    if (res < 0) {
        perror(("make_dir " + path + " failed").c_str());
    }
    return res;
}

int utils::copy_file(string source, string dest)
{
    int childExitStatus;
    pid_t pid;

    pid = fork();

    if (pid == 0) { /* child */
        execl("/bin/cp", "/bin/cp", source.c_str(), dest.c_str(), (char *)0);
    }
    else if (pid < 0) {
        /* error - couldn't start process - you decide how to handle */
    }
    else {
        /* parent - wait for child - this has all error handling, you
         * could just call wait() as long as you are only expecting to
         * have one child process at a time.
         */
        pid_t ws = waitpid( pid, &childExitStatus, WNOHANG);
        if (ws == -1)
        { /* error - handle as you wish */
        }

        if( WIFEXITED(childExitStatus)) /* exit code in childExitStatus */
        {
            //status = WEXITSTATUS(childExitStatus); /* zero is normal exit */
            /* handle non-zero as you wish */
        }
        else if (WIFSIGNALED(childExitStatus)) /* killed */
        {
        }
        else if (WIFSTOPPED(childExitStatus)) /* stopped */
        {
        }
    }
    return 0;
}

int utils::rm_rf_files()
{
    system("rm -rf ./*");
    return 0;
}

string utils::hashfile(string filename)
{ 
    std::ifstream fp(filename);
    std::stringstream ss;
    
    // Unable to hash file, return an empty hash.
    if (!fp.is_open()) {
        return "";
    }
    
    // Hashing
    uint64_t magic = 0x8899aabbccddeeff;
    char c;
    while (fp.get(c)) {
    magic = ((magic << 5) + magic) + c; // magic * 33 + c
    }
    
    ss << std::hex << std::setw(16) << std::setfill('0') << magic;
    
    //i tried to add date to hash. lol.
    /*time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];
    
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    
    strftime(buffer,sizeof(buffer),"%d%m%y%H%M%S",timeinfo);
    ss << buffer;*/

    return ss.str();
}

int utils::move_file(string source, string dest) {
    int res = rename(source.c_str(), dest.c_str());
    if (res < 0) {
        perror(("move_file " + source + " -> " + dest + " failed").c_str());
    }
    return res;
}

int utils::remove_file(string path) {
    int res = remove(path.c_str());
    if (res < 0) {
        perror(("remove_file " + path + " failed").c_str());
    }
    return res;
}

bool utils::exists_file (string name) {
    struct stat buffer;   
    return (stat (name.c_str(), &buffer) == 0); 
}

int utils::write_to_file(string filename, string to_write) {
    ofstream out(filename);
    out << to_write << endl;
    out.close();
    return 0;
}

string utils::read_line_from_file(string filename, int line_no) {
    ifstream file(filename);
    string line;
    for (int i = 0; i <= line_no; i++) {
        getline(file, line);
    }
    return line;
}

void utils::print(string to_print) {
    if ((to_print[0] != '.') && (to_print[1] != '/')) {
        to_print = "./" + to_print;
    }
    cout << to_print << endl;
}

vector<string> utils::parse_file_tree(string pat){
    glob_t glob_result;
    glob(pat.c_str(),GLOB_TILDE | GLOB_MARK,NULL,&glob_result);
    //std::cout << "glob at pattern " << pat << endl;
    vector<string> ret;
    string res;
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
        res = string(glob_result.gl_pathv[i]);
        if (res[res.size() - 1] == '/') {
            for (string s : parse_file_tree(res + "*")) {
                ret.push_back(s);
            }
        } else {
            ret.push_back(res);
        }
    }
    globfree(&glob_result);
    return ret;
}

Document utils::do_initial_get_request(string url, string username, string password, int* response_code) {
    try
    {
        string addr = SRV_ADDRESS;
        url = addr + url;
    
        // That's all that is needed to do cleanup of used resources (RAII style).
        curlpp::Cleanup myCleanup;
        
        // Our request to be sent.
        curlpp::Easy myRequest;
        
        // Set the URL.
        myRequest.setOpt<Url>(url);
        
        // Set where to write answer to
        std::stringstream result;
        myRequest.setOpt(cURLpp::Options::WriteStream(&result));
        
        // Set username/password
        myRequest.setOpt(new curlpp::options::UserPwd(username + ":" + password));
        
        // Send request and get a result.
        // By default the result goes to standard output.
        myRequest.perform();
        
        *response_code = curlpp::infos::ResponseCode::get(myRequest);
        
        Document document;
        document.Parse(result.str().c_str());
        return document;
    }
    
    catch(curlpp::RuntimeError & e)
    {
        std::cout << e.what() << std::endl;
        *response_code = -2;
        return nullptr;
    }
    
    catch(curlpp::LogicError & e)
    {
        std::cout << e.what() << std::endl;
        *response_code = -3;
        return nullptr;
    }

}

string gusername = "";
string gpassword = "";
Document utils::do_get_request(string url, int* response_code) {
    if (gusername == "") {
        gusername = read_line_from_file(home_dir + SETTINGS_FILE, 1);
        gpassword = read_line_from_file(home_dir + SETTINGS_FILE, 2);
    }
    return do_initial_get_request(url, gusername, gpassword, response_code);
}

string utils::do_put_request(string url, string body, int* response_code) {
    //std::list<std::string> header;
    //header.push_back("Content-Type: application/json");
    
    if (gusername == "") {
        gusername = read_line_from_file(home_dir + SETTINGS_FILE, 1);
        gpassword = read_line_from_file(home_dir + SETTINGS_FILE, 2);
    }
    
    curlpp::Cleanup clean;
    curlpp::Easy r;
    r.setOpt(new curlpp::options::Url(url));
    //r.setOpt(new curlpp::options::HttpHeader(header));
    r.setOpt(new curlpp::options::PostFields(body));
    r.setOpt(new curlpp::options::PostFieldSize(body.length()));
    
    r.setOpt(new curlpp::options::UserPwd(gusername + ":" + gpassword));
    
    std::ostringstream response;
    r.setOpt(new curlpp::options::WriteStream(&response));
    
    r.perform();
    
    *response_code = curlpp::infos::ResponseCode::get(r);
    
    return std::string(response.str());
}

std::map<int, std::string> usernames_by_id;
std::map<int, std::string>::iterator usernames_by_id_it;
string utils::get_userstuff_by_user_id(int id) {
    usernames_by_id_it = usernames_by_id.find(id);
    if (usernames_by_id_it != usernames_by_id.end()) {
        return usernames_by_id_it->second;
    }
    
    int errcode = -1;
    Document d = do_get_request("/user/get?id=" + to_string(id), &errcode);
    
    assert(d.HasMember("firstname"));
    assert(d["firstname"].IsString());
    
    assert(d.HasMember("lastname"));
    assert(d["lastname"].IsString());
    
    assert(d.HasMember("email"));
    assert(d["email"].IsString());
    
    string s = string(d["firstname"].GetString())
             + " " + string(d["lastname"].GetString())
             + " <" + string(d["email"].GetString()) + ">";
    usernames_by_id.insert(pair<int, string>(id, s));
    return s;
}
