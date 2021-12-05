//
// Created by lukas on 12/5/2021.
//

#ifndef CONTROLLERS_CAMERAMANAGER_HPP
#define CONTROLLERS_CAMERAMANAGER_HPP

#include "../../BaseClasses/SimpleLogger.hpp"
#include "../../Common/CustomEnumTypes.hpp"
#include "../../Common/ControllersFlags.hpp"
#include "../../BaseClasses/DeviceManager.hpp"
#include "../../FileSystemController/FileSystem/FileSystemController.hpp"
#include <vector>

class CameraManager : private SimpleLogger, public DeviceManager {
////METHODS
public:

////INTEGRAL PARTS OF CLASS
    static CameraManager* GetInstance();

    CameraManager(CameraManager
                  & other) = delete;

    void operator=(const CameraManager&) = delete;

////BASIC CMD
    int addInfoToScannedData(ScannedData& data) override {
        if (not IS_CAMERA_AVAILABLE) {
            data.cameraImage = {};
            data.resultCamera = CAMERA_RESULT;
            data.finalResult = data.resultCamera == data.finalResult ? data.finalResult : ScanResult::Unrecognized;
            return 0;
        }
        /*
         *
         *
         *  IMPLEMENTATION
         *
         *
         */
        return -1;
    }

    int addInfoToScannedDataAndSaveItToDataBase(ScannedData& data) override {
        if (not IS_CAMERA_AVAILABLE) {
            data.cameraImage = {};
            data.resultCamera = CAMERA_RESULT;
            data.finalResult = data.resultCamera == data.finalResult ? data.finalResult : ScanResult::Unrecognized;
            FileSystemController::GetInstance()->addCameraImageToCategorizedDataBase(CAMERA_RESULT, {});
            return 0;
        }
        /*
         *
         *
         *  IMPLEMENTATION
         *
         *
         */
        return -1;
    }

protected:

////INTEGRAL PARTS OF CLASS
    explicit CameraManager(bool isLogInfoEnable = false, bool isLogErrorEnable = true)
            : SimpleLogger(isLogInfoEnable, isLogErrorEnable) {};

private:




////VARIABLES
public:
protected:
    static CameraManager* cam_;
private:
};


#endif //CONTROLLERS_CAMERAMANAGER_HPP
