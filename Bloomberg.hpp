//
//  Bloomberg.hpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#ifndef Bloomberg_hpp
#define Bloomberg_hpp

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include "Security.hpp"
#include "ResponseHandler.hpp"

using namespace BloombergLP;
using namespace blpapi;
using namespace std;

typedef multimap<int, string> RequestParams;
typedef pair<int, string> ParamPair;

//============================================================================//

class BBGDataManager
{
    int d_port;
    vector<string> d_securities;
    vector<string> d_fields;
    string d_host;
    string d_startDate;
    string d_endDate;
    RequestParams currentReq;
    
    inline void setCurrentRequest(const RequestParams& request)
    {
        currentReq.clear();
        currentReq = request;
    }
    
public:
    
    bool ProcessDataRequestH(const RequestParams& currReq,
                             Security& sec);
    
};


#endif /* Bloomberg_hpp */
