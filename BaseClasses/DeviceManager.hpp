//
// Created by lukas on 12/5/2021.
//

#ifndef CONTROLLERS_DEVICEMANAGER_HPP
#define CONTROLLERS_DEVICEMANAGER_HPP

#include "../Common/CustomEnumTypes.hpp"
#include "ScannedData.hpp"

class DeviceManager{
public:

    virtual int addInfoToScannedData(ScannedData& data) = 0;
    virtual int addInfoToScannedDataAndSaveItToDataBase(ScannedData& data) = 0;
};
#endif //CONTROLLERS_DEVICEMANAGER_HPP
