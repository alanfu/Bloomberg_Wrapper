//
//  Security.cpp
//  Bloomberg_Wrapper
//
//  Created by FuYongrui on 5/16/16.
//  Copyright © 2016 FuYongrui. All rights reserved.
//

#include "Security.hpp"

void Security::printDataFrame() const
{
    for (auto it = dataFrame.begin(), e = dataFrame.end(); it != e; ++it)
    {
        cout << it->first << " " ;
        for (vector<blpapi_Float64_t>::const_iterator j = (it->second).begin(); j != (it->second).end(); ++j)
            cout << *j << " " ;
        cout << endl ;
    }
}

blpapi_Float64_t Security::getData(Date& date, string& field)
{
    Datetime d(date.year(), date.month(), date.day());
    auto e = dataFrame.end();
    
    if (dataFrame.find(d) == e)
        throw runtime_error("date not found.");
    
    auto my_fields = dataFrame[d];
    vector<string>::iterator it;
    it = find(d_fields.begin(), d_fields.end(), field);
    
    if (it == d_fields.end())
        throw runtime_error("field not found.");
    
    int loc = distance(d_fields.begin(), it);
    return my_fields.at(loc);
}