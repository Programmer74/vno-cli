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

using namespace std;
using namespace curlpp::options;

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
        
        // Send request and get a result.
        // By default the result goes to standard output.
        myRequest.perform();
        
        //cout << result.str() << endl;
        
        std::cout << "Response code: " 
			<< curlpp::infos::ResponseCode::get(myRequest) 
			<< std::endl;
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
