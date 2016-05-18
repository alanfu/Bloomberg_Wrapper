//
//  ResponseHandler.hpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#ifndef ResponseHandler_hpp
#define ResponseHandler_hpp

#include <stdio.h>
#include "Security.hpp"
#include "Constants.h"

using namespace BloombergLP;
using namespace blpapi;
using namespace std;

typedef multimap<int, string> RequestParams;
typedef pair<int, string> ParamPair;

//============================================================================//

class ResponseHandler
{
protected:
    const RequestParams& request;
    const Message& response;
    Security& security;
    
    void ProcessFields();
    bool ProcessErrors();
    bool ProcessExceptions();
    
public:
    ResponseHandler(const RequestParams& req,
                    const Message& resp,
                    Security& sec)
    : request(req), response(resp), security(sec) {}
    virtual ~ResponseHandler() {}
    virtual bool processResponse() = 0;
    
    static boost::shared_ptr<ResponseHandler> create(const RequestParams& req,
                                                     const Message& resp,
                                                     Security& sec);
};

//============================================================================//

class HistDataHandler : public ResponseHandler
{
public:
    HistDataHandler(const RequestParams& req,
                    const Message& resp,
                    Security& sec)
    : ResponseHandler(req, resp, sec) {}
    virtual bool processResponse();
};

//============================================================================//

class RefDataHandler : public ResponseHandler
{
public:
    RefDataHandler(const RequestParams& req,
                   const Message& resp,
                   Security& sec)
    : ResponseHandler(req, resp, sec) {}
    
    virtual bool processResponse();
};

//============================================================================//

class IntraDayBarDataHandler : public ResponseHandler
{
public:
    IntraDayBarDataHandler(const RequestParams& req,
                           const Message& resp,
                           Security& sec)
    : ResponseHandler(req, resp, sec) {}
    
    virtual bool processResponse();
};

#endif /* ResponseHandler_hpp */
