//
// Created by lukas on 12/5/2021.
//

#pragma once
#ifndef CONTROLLERS_CUSTOMENUMTYPES_HPP
#define CONTROLLERS_CUSTOMENUMTYPES_HPP

#include <string>

namespace  ScanResult{
    enum Type{
        Healthy, Unrecognized, Unhealthy
    };

    //std::string getResultName(int result);
    std::string getResultName(ScanResult::Type result);
};



#endif //CONTROLLERS_CUSTOMENUMTYPES_HPP
