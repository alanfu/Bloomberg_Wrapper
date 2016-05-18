//
//  ResponseHandler.cpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#include "ResponseHandler.hpp"

boost::shared_ptr<ResponseHandler> ResponseHandler::create(const RequestParams& req,
                                                           const Message& resp,
                                                           Security& sec)
{
    RequestParams::const_iterator it = req.find(BBG_HISTORICAL_DATA_REQUEST);
    if (it != req.end())
        return boost::shared_ptr<ResponseHandler>(new HistDataHandler(req, resp, sec));
    
    else if ((it = req.find(BBG_REF_DATA_REQUEST)) != req.end())
        return boost::shared_ptr<ResponseHandler>(new RefDataHandler(req, resp, sec));
    
    else if ((it = req.find(BBG_INTRADAY_BAR_DATA_REQUEST)) != req.end())
        return boost::shared_ptr<ResponseHandler>(new IntraDayBarDataHandler(req, resp, sec));
    
    else
        throw runtime_error("Unknown Request Type.");
}

//============================================================================//

bool HistDataHandler::processResponse()
{
    Element securityData = response.getElement("securityData");
    Element securityName = securityData.getElement("security");
    cout << securityName << "\n\n" ;
    if (!ProcessErrors() && !ProcessExceptions())
    {
        ProcessFields();
        return true;
    }
    return false;
}

//============================================================================//

bool ResponseHandler::ProcessErrors()
{
    Element securityData = response.getElement("securityData");
    if (securityData.hasElement("securityError"))
    {
        Element security_error = securityData.getElement("securityError");
        Element error_message = security_error.getElement("message");
        cout << error_message << endl;
        return true;
    }
    return false;
}

//============================================================================//

bool ResponseHandler::ProcessExceptions()
{
    Element securityData = response.getElement("securityData");
    Element field_exceptions = securityData.getElement("fieldExceptions");
    
    if (field_exceptions.numValues() > 0)
    {
        Element element = field_exceptions.getValueAsElement(0);
        Element field_id = element.getElement("fieldId");
        Element error_info = element.getElement("errorInfo");
        Element error_message = error_info.getElement("message");
        cout << field_id << endl;
        cout << error_message << endl;
        return true;
    }
    return false;
}

//============================================================================//

void ResponseHandler::ProcessFields()
{
    const char* delimiter = " ";
    cout << "Date" << *delimiter << *delimiter ;
    pair<RequestParams::const_iterator, RequestParams::const_iterator> ret = request.equal_range(BBG_REQUEST_FIELD);
    
    for (RequestParams::const_iterator it = ret.first; it != ret.second; ++it)
        security.d_fields.push_back(it->second);
    
    for (int k = 0; k < (int)security.d_fields.size(); k++)
        cout << security.d_fields[k].c_str() << *delimiter ;
    
    cout << "\n\n" ;
    
    Element securityData = response.getElement("securityData");
    Element fieldData = securityData.getElement("fieldData");
    
    if (fieldData.numValues() > 0)
    {
        vector<blpapi_Float64_t> my_fields;
        for (int j = 0; j < (int)fieldData.numValues(); j++) {
            my_fields.clear();
            int datatype;
            Element element = fieldData.getValueAsElement(j);
            Datetime date = element.getElementAsDatetime("date");
            
            for (int k = 0; k < (int)security.d_fields.size(); k++)
            {
                const char* temp_field_str = security.d_fields[k].c_str();
                if (element.hasElement(temp_field_str))
                {
                    Element temp_field = element.getElement(temp_field_str);
                    const Name TEMP_FIELD_STR(temp_field_str);
                    
                    datatype = temp_field.datatype();
                    
                    if (datatype == BLPAPI_DATATYPE_FLOAT64 || datatype == BLPAPI_DATATYPE_INT64)
                        my_fields.push_back(element.getElementAsFloat64(TEMP_FIELD_STR));
                }
            }
            security.dataFrame.insert(pair<Datetime, vector<blpapi_Float64_t>>(date, my_fields));
        }
    }
}

