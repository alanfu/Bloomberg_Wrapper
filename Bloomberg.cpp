//
//  Bloomberg.cpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#include "Bloomberg.hpp"

bool BBGDataManager::ProcessDataRequestH(const RequestParams& currReq,
                                         Security& sec)
{
    setCurrentRequest(currReq);
    
    d_host = "localhost";
    d_port = 8194;
    bool bIsBarDataReq = false;
    
    SessionOptions sessionOptions;
    sessionOptions.setServerHost(d_host.c_str());
    sessionOptions.setServerPort(d_port);
    
    cout << "Connecting to " << d_host << " : " << d_port << endl;
    
    Session session(sessionOptions);
    
    if (!session.start())
    {
        cerr << "Failed to start session." << endl;
        return false;
    }
    
    if (!session.openService("//blp/refdata"))
    {
        cerr << "Failed to open //blp/refdata." << endl;
        return false;
    }
    
    Service refDataService = session.getService("//blp/refdata");
    
    RequestParams::const_iterator it = currReq.find(BBG_REF_DATA_REQUEST);
    if (it == currReq.end())
    {
        it = currReq.find(BBG_HISTORICAL_DATA_REQUEST);
        if (it == currReq.end())
        {
            it = currReq.find(BBG_INTRADAY_BAR_DATA_REQUEST);
            if (it == currReq.end())
            {
                cerr << " Request type missing in request." << endl;
                return false;
            }
            else
                bIsBarDataReq = true;
        }
    }
    Request request = refDataService.createRequest(it->second.c_str());
    
    pair<RequestParams::const_iterator, RequestParams::const_iterator> ret = currReq.equal_range(BBG_REQUEST_SECURITY);
    
    for (RequestParams::const_iterator it = ret.first; it != ret.second; ++it)
        request.getElement("securities").appendValue(it->second.c_str());
    
    ret = currReq.equal_range(BBG_REQUEST_FIELD);
    for (RequestParams::const_iterator it = ret.first; it != ret.second; ++it)
        request.getElement("field").appendValue(it->second.c_str());
    
    it = currReq.find(BBG_REQUEST_PERIODICITY);
    if (it != currReq.end())
        request.set("periodcitySelection", it->second.c_str());
    
    it = currReq.find(BBG_REQUEST_START_DT);
    if (it != currReq.end())
        request.set("startDate", it->second.c_str());
    
    it = currReq.find(BBG_REQUEST_END_DT);
    if (it != currReq.end())
        request.set("endDate", it->second.c_str());
    
    session.sendRequest(request);
    
    while (true) {
        Event event = session.nextEvent();
        MessageIterator msgIter(event);
        while (msgIter.next()) {
            Message msg = msgIter.message();
            
            if ((event.eventType() != Event::PARTIAL_RESPONSE) && (event.eventType() != Event::RESPONSE))
                continue;
            
            boost::shared_ptr<ResponseHandler> respHandler(ResponseHandler::create(currReq, msg, sec));
            
            if (!respHandler->processResponse())
                return false;
            
        }
        if (event.eventType() == Event::RESPONSE)
            break;
    }
    return true;
}

