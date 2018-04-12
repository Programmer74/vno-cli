#include "rest.h"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "utils.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

#include "document.h"

using namespace std;
using namespace curlpp::options;
using namespace rapidjson;

void rest::do_rest(string url) {
    cout << "Rest called" << endl;
    
    try
    {
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
        myRequest.setOpt(new curlpp::options::UserPwd("user:pwd"));
        
        // Send request and get a result.
        // By default the result goes to standard output.
        myRequest.perform();
        
        std::cout << "Response code: " 
			<< curlpp::infos::ResponseCode::get(myRequest) 
			<< std::endl;
			
        std::cout << "=====================" << endl;
        Document document;
        document.Parse(result.str().c_str());
        
        if (document.HasMember("error")) {
            cerr << "Error occured: " << endl;
            
            assert(document.HasMember("status"));
            assert(document["status"].IsInt());
            cerr << "Status: " << document["status"].GetInt() << endl;
            
            assert(document.HasMember("message"));
            assert(document["message"].IsString());
            cerr << "Message: " << document["message"].GetString() << endl;
        } else {        
            cout << result.str() << endl;
        }
    }
    
    catch(curlpp::RuntimeError & e)
    {
        std::cout << e.what() << std::endl;
    }
    
    catch(curlpp::LogicError & e)
    {
        std::cout << e.what() << std::endl;
    }

    cout << "Rest done" << endl;
}
    
