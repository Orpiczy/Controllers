//
// Created by lukas on 12/5/2021.
//

#ifndef CONTROLLERS_DEVICECONTROLLER_HPP
#define CONTROLLERS_DEVICECONTROLLER_HPP

#include "../BaseClasses/SimpleLogger.hpp"
#include "../Common/CustomEnumTypes.hpp"
#include "CameraManager/CameraManager.hpp"
#include "ProfilometerManager/ProfilometerManager.hpp"

class DeviceController : private SimpleLogger {
////METHODS
public:

    ////INTEGRAL PARTS OF CLASS
    static DeviceController* GetInstance();

    DeviceController(DeviceController& other) = delete;

    void operator=(const DeviceController&) = delete;

    ////CMD
    ScannedData getAllData(){
        ScannedData data;
        for(auto device: deviceList){
            device->addInfoToScannedDataAndSaveItToDataBase(data);
        }
        FileSystemController::GetInstance() -> addScanToDailyStatistic(data.finalResult);
        return data;
    }
protected:
    ////INTEGRAL PARTS OF CLASS
    explicit DeviceController(bool isLogInfoEnable = false, bool isLogErrorEnable = true)
            : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {};

private:




    ////VARIABLES
public:
protected:
    static DeviceController* dvc_;
    std::vector<DeviceManager*> deviceList = {CameraManager::GetInstance(),ProfilometerManager::GetInstance()};
private:


};


#endif //CONTROLLERS_DEVICECONTROLLER_HPP
