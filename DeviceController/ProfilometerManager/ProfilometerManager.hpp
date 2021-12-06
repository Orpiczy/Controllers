//
// Created by lukas on 06/11/2021.
//

#ifndef UNTITLED_PROFILOMETERMANAGER_HPP
#define UNTITLED_PROFILOMETERMANAGER_HPP

#include <iostream>
#include "SerialPortManager.hpp"
#include "MsgManager.hpp"
#include "../../BaseClasses/SimpleLogger.hpp"
#include "../../Common/CustomEnumTypes.hpp"
#include "../../Common/ControllersFlags.hpp"
#include "../../BaseClasses/DeviceManager.hpp"
#include "../../FileSystemController/FileSystem/FileSystemController.hpp"

class ProfilometerManager : public SerialPortManager, private SimpleLogger, public DeviceManager {

public:

    ////INTEGRAL PARTS OF CLASS
    static ProfilometerManager*
    GetInstance(bool isLogInfoEnable = false, bool isLogErrorEnable = true, int comPortNumber = 9);

    explicit ProfilometerManager(bool isLogInfoEnable, bool isLogErrorEnable, const int comPortNumber)
            : SimpleLogger(isLogInfoEnable, isLogErrorEnable), SerialPortManager(comPortNumber, B230400) {}


    ProfilometerManager(ProfilometerManager& other) = delete; //can not be cloneable
    void operator=(const ProfilometerManager&) = delete; // can not be assignable

    ////BASIC CMD
    int addInfoToScannedData(ScannedData& data) override {
        if (not IS_PROFILOMETER_AVAILABLE) {
            data.out1 = getOut1();
            data.out2 = getOut2();
            data.out3 = getOut3();
            data.outA = getOutA();
            data.profileData = getProfile();
            data.resultProfilometer = PROFILOMETER_RESULT;
            data.finalResult =
                    data.resultProfilometer == data.finalResult ? data.finalResult : ScanResult::Unrecognized;
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
        if (not IS_PROFILOMETER_AVAILABLE) {
            data.out1 = getOut1();
            data.out2 = getOut2();
            data.out3 = getOut3();
            data.outA = getOutA();
            data.profileData = getProfile();
            data.resultProfilometer = PROFILOMETER_RESULT;
            data.finalResult =
                    data.resultProfilometer == data.finalResult ? data.finalResult : ScanResult::Unrecognized;
            FileSystemController::GetInstance()->addProfilometerScanDataToCategorizedDataBase(data.resultProfilometer,
                                                                                              data.out1, data.out2,
                                                                                              data.out3, data.outA,
                                                                                              data.profileData);
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

    ////CMD
    int getOut1();

    int getOut2();

    int getOut3();

    int getOutA();


    ////PROFILE CMD
    std::vector<std::pair<uint16_t, uint16_t>> getProfile(int attemptNumber = 0);

    std::optional<uint32_t> getProfileDataAddress(int attemptNumber = 0);

    std::optional<std::tuple<uint16_t, float>> getProfileSizeTimeInfo(uint32_t memoryAddress, int attemptNumber = 0);

    std::optional<std::tuple<uint16_t, float, uint32_t>> getProfileSizeTimeInfoAndAddress(int attemptNumber = 0);

    std::optional<std::vector<uint8_t>> getValueFromMemoryAddress(uint32_t memoryAddress, int attemptNumber = 0);

    std::optional<std::vector<uint8_t>>
    getRequestedSizeValueFromMemoryAddress(uint32_t memoryAddress, size_t requestedDataLength, int attemptNumber = 0);


    ////HELPERS
    bool isConnectedAndWorking();

protected:
    static ProfilometerManager* pfm_;
private:
    int getOut(const std::vector<uint8_t>& cmd, const std::string& sourceName, int attemptNumber = 0);

    ///// TEST DATA, CAN BE DELETED IN PRODUCTION CODE
    const static std::vector<std::pair<uint16_t, uint16_t>> testProfileData;
    const static uint16_t testOut1 = 4123;
    const static uint16_t testOut2 = 235;
    const static uint16_t testOut3 = -2314;
    const static uint16_t testOutA = -13;
    const static ScanResult testResult = ScanResult::Unrecognized;
    int maxReAttempts = 2;
    int maxReadMemoryReAttempts = 5;

};


#endif //UNTITLED_PROFILOMETERMANAGER_HPP
