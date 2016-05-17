//
//  Security.hpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#ifndef Security_hpp
#define Security_hpp

#include <stdio.h>
#include <blpapi_session.h>
#include <blpapi_event.h>
#include <blpapi_message.h>
#include <blpapi_element.h>
#include <blpapi_name.h>
#include <blpapi_request.h>
#include <blpapi_exception.h>

#include <string>
#include <vector>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/shared_ptr.hpp>
#include <map>
#include <algorithm>
#include <iterator>
#include <exception>

using namespace BloombergLP;
using namespace blpapi;
using namespace std;

typedef boost::gregorian::date Date;

class Security
{
    string strBBGTicker;
    vector<string> d_fields;
    map<Datetime, vector<blpapi_Float64_t> > dataFrame;
    
public:
    friend class ResponseHandler;
    
    Security(const string& ticker) : strBBGTicker(ticker) {}
    const string& getTicker() {return strBBGTicker;}
    void printDataFrame() const;
    blpapi_Float64_t getData(Date& date, string& field);
};

#endif /* Security_hpp */
