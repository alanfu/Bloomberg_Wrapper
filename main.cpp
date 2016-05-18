//
//  main.cpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#include <iostream>
#include <string>
#include "Bloomberg.hpp"
#include "Security.hpp"
#include "ResponseHandler.hpp"

using namespace std;
using namespace BloombergLP;
using namespace blpapi;

int main(int argc, const char * argv[])
{
    
    try {
        vector<string> tickers = {"IBM US Equity" , "MSFT US Equity"};
        map<string, SecuritySP> securities;
        for (vector<string>::const_iterator it = tickers.begin(), e = tickers.end(); it != e; ++it)
            securities[*it] = SecuritySP(new Security(*it));
        
        typedef map<string, SecuritySP>::iterator SecIter;
        
        BBGDataManager dataMgr;
        RequestParams req;
        
        req.insert(ParamPair(BBG_HISTORICAL_DATA_REQUEST, "HistoricalDataRequest"));
        req.insert(ParamPair(BBG_REQUEST_FIELD, "PX_LAST"));
        req.insert(ParamPair(BBG_REQUEST_FIELD, "PX_OPEN"));
        req.insert(ParamPair(BBG_REQUEST_FIELD, "PX_CLOSE_1D"));
        req.insert(ParamPair(BBG_REQUEST_FIELD, "PX_VOLUME"));
        req.insert(ParamPair(BBG_REQUEST_PERIODICITY, "DAILY"));
        req.insert(ParamPair(BBG_REQUEST_START_DT, "20160501"));
        req.insert(ParamPair(BBG_REQUEST_END_DT, "20160512"));
        
        for (SecIter it = securities.begin(); it != securities.end(); ++it)
        {
            RequestParams::iterator reqIter = req.find(BBG_REQUEST_SECURITY);
            if (reqIter != req.end())
                req.erase(reqIter);
            
            req.insert(ParamPair(BBG_REQUEST_SECURITY, it->first));
            
            if (dataMgr.ProcessDataRequestH(req, *it->second))
            {
                (*it->second).printDataFrame();
                
                Date dt = boost::gregorian::from_string("2016-05-10");
                string req_field = "PX_LAST";
                cout << (*it->second).getData(dt, req_field);
            }
        }
        
    } catch (exception& e) {
        cerr << e.what() << endl;
    }
    
    int done;
    cin >> done;
    return 0;
}
