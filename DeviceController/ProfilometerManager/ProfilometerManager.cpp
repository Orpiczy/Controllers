//
// Created by lukas on 06/11/2021.
//

#include "ProfilometerManager.hpp"

////INTEGRAL PARTS OF CLASS

ProfilometerManager*
ProfilometerManager::GetInstance(bool isLogInfoEnable, bool isLogErrorEnable, const int comPortNumber) {
    if (pfm_ == nullptr) {
        pfm_ = new ProfilometerManager(isLogInfoEnable, isLogErrorEnable, comPortNumber);
    }
    return pfm_;
}


////TESTED
int ProfilometerManager::getOut(const std::vector<uint8_t>& cmd, const std::string& sourceName, int attemptNumber) {
    clearBuffer(1);
    sendMessage(cmd);
    auto msgBuffer = readMessage();

    auto isMsgInvalidData = MsgManager::isMessageInvalid(msgBuffer, 10);

    if (not isMsgInvalidData.has_value()) {
        return MsgManager::translateMsgToOutNValue(msgBuffer);
    } else {

        LG_INF("INCORRECT READ VALUE FROM " + sourceName + ", ATTEMPT NR ", attemptNumber);
        LG_ERR(isMsgInvalidData.value());
        if (attemptNumber < maxReAttempts) {
            attemptNumber++;
            return getOut(cmd, sourceName, attemptNumber);
        }
        std::cout << "\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd " << sourceName << " FAILED\n";
        return 0;
    }
}

ProfilometerManager* ProfilometerManager::pfm_ = nullptr;

int ProfilometerManager::getOut1() {
    if (not IS_PROFILOMETER_AVAILABLE) {
        return testOut1;
    }
    return getOut(MsgManager::cmdOut1(), "getOut1");
}

int ProfilometerManager::getOut2() {
    if (not IS_PROFILOMETER_AVAILABLE) {
        return testOut2;
    }
    return getOut(MsgManager::cmdOut2(), "getOut2");
}

int ProfilometerManager::getOut3() {
    if (not IS_PROFILOMETER_AVAILABLE) {
        return testOut3;
    }
    return getOut(MsgManager::cmdOut3(), "getOut3");
}

int ProfilometerManager::getOutA() {
    if (not IS_PROFILOMETER_AVAILABLE) {
        return testOutA;
    }
    return getOut(MsgManager::cmdOutA(), "getOutA");
}

std::vector<std::pair<uint16_t, uint16_t>> ProfilometerManager::getProfile(int attemptNumber) {
    if (not IS_PROFILOMETER_AVAILABLE) {
        return testProfileData;
    }
    clearBuffer(2);
    auto sizeTimeAddressData = getProfileSizeTimeInfoAndAddress();
    std::optional<std::vector<uint8_t>> rawPoints{};

    if (sizeTimeAddressData.has_value()) {
        auto[sizeInProfiles, timeInfo, address] = sizeTimeAddressData.value();
        //one profile, two points, 4 bytes, profile header has 4 bytes -> adding 4 bytes to address
        rawPoints = getRequestedSizeValueFromMemoryAddress(address + 0x0004, sizeInProfiles * 4);
    }

    if (sizeTimeAddressData.has_value() and rawPoints.has_value()) {
        return MsgManager::translateRawDataPointIntoPairXY(rawPoints.value());
    } else {
        LG_INF("INCORRECT READ VALUE PROFILE, ATTEMPT NR ", attemptNumber);;
        if (attemptNumber < maxReAttempts) {
            attemptNumber++;
            return getProfile(attemptNumber);
        }
        LG_INF("\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd getProfile FAILED\n");
        return {};
    }
}

std::optional<std::tuple<uint16_t, float, uint32_t>>
ProfilometerManager::getProfileSizeTimeInfoAndAddress(int attemptNumber) {
    clearBuffer(1);
    auto profileDataAdd = getProfileDataAddress();
    std::optional<std::tuple<uint16_t, float>> profileSizeTimeInfo{};
    if (profileDataAdd.has_value()) {
        profileSizeTimeInfo = getProfileSizeTimeInfo(profileDataAdd.value());
    }

    if (profileSizeTimeInfo.has_value() and profileDataAdd.has_value()) {
        auto[sizeInProfiles, timeInfo] = profileSizeTimeInfo.value();
        return std::make_tuple(sizeInProfiles, timeInfo, profileDataAdd.value());
    } else {
        LG_INF("INCORRECT READ VALUE FROM PROFILE SIZE, TIME INFO AND ADDRESS, ATTEMPT NR ", attemptNumber);
        if (attemptNumber < maxReAttempts) {
            attemptNumber++;
            return getProfileSizeTimeInfoAndAddress(attemptNumber);
        }
        LG_INF("\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd getProfileSizeTimeInfoAndAddress FAILED\n");
        return std::nullopt;
    }
}

std::optional<std::tuple<uint16_t, float>>
ProfilometerManager::getProfileSizeTimeInfo(uint32_t memoryAddress, int attemptNumber) {
    clearBuffer(1);
    uint8_t rlen = 0x02; //0x01 - only size
    auto cmd = MsgManager::cmdProfileSizeAndTimeInfo(memoryAddress, rlen);
    sendMessage(cmd);
    auto msgBuffer = readMessage();
    auto isMsgInvalidData = MsgManager::isMessageInvalid(msgBuffer, 10 + 2 * rlen);

    if (not isMsgInvalidData.has_value()) {
        auto[sizeInProfiles, timeInfo] = MsgManager::translateUnknownSizedMsgToRawSizeAndTimeInfo(msgBuffer);
        return std::make_tuple(sizeInProfiles, timeInfo);
    } else {
        LG_INF("INCORRECT READ VALUE FROM PROFILE SIZE AND TIME INFO, ATTEMPT NR ", attemptNumber);
        LG_ERR(isMsgInvalidData.value());
        if (attemptNumber < maxReAttempts) {
            attemptNumber++;
            return getProfileSizeTimeInfo(memoryAddress, attemptNumber);
        }

        LG_INF("\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd getProfileSizeTimeInfo FAILED\n");
        return std::nullopt;
    }

}

std::optional<uint32_t> ProfilometerManager::getProfileDataAddress(int attemptNumber) {
    clearBuffer(1);

    auto cmd = MsgManager::cmdProfileMemoryAddress();
    sendMessage(cmd);
    auto msgBuffer = readMessage();

    auto isMsgInvalidData = MsgManager::isMessageInvalid(msgBuffer, 10);

    if (not isMsgInvalidData.has_value()) {
        return MsgManager::translateMsgToProfileMemoryAddress(msgBuffer);
    } else {

        LG_INF("INCORRECT READ VALUE FROM PROFILE ADDRESS, ATTEMPT NR ", attemptNumber);
        LG_ERR(isMsgInvalidData.value());
        if (attemptNumber < maxReAttempts) {
            attemptNumber++;
            return getProfileDataAddress(attemptNumber);
        }
        LG_INF("\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd getProfileDataAddress FAILED\n");
        return std::nullopt;
    }
}


std::optional<std::vector<uint8_t>>
ProfilometerManager::getRequestedSizeValueFromMemoryAddress(uint32_t memoryAddress, size_t requestedDataLength,
                                                            int attemptNumber) {
    clearBuffer(2);
    std::vector<uint8_t> rawData{};

    while (rawData.size() < requestedDataLength) {

        auto data = getValueFromMemoryAddress(memoryAddress + static_cast<uint32_t>(rawData.size()));

        if (data.has_value()) {
            rawData.insert(rawData.end(), data.value().begin() + 8, data.value().end() - 2);

        } else {
            LG_INF("INCORRECT READ VALUE FROM REQUESTED SIZE MEMORY ADDRESS, ATTEMPT NR ", attemptNumber);;
            if (attemptNumber < maxReAttempts) {
                attemptNumber++;
                return getRequestedSizeValueFromMemoryAddress(memoryAddress, requestedDataLength, attemptNumber);
            }
            LG_INF("\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd getRequestedSizeValueFromMemoryAddress FAILED\n");
            return std::nullopt;
        }
    }
    std::vector result(rawData.begin(), rawData.begin() + requestedDataLength);
    return result;
}


std::optional<std::vector<uint8_t>>
ProfilometerManager::getValueFromMemoryAddress(uint32_t memoryAddress, int attemptNumber) {
    clearBuffer(1);
    auto cmd = MsgManager::cmdValueFromMemoryAddress(memoryAddress);
    sendMessage(cmd);
    auto msgBuffer = readMessagesUntilEndSign();

    auto isMsgInvalidData = MsgManager::isMessageInvalid(msgBuffer, 10, 1);

    if (not isMsgInvalidData.has_value()) {
        return msgBuffer;
    } else {
        LG_INF("INCORRECT READ VALUE FROM MEMORY ADDRESS, ATTEMPT NR ", attemptNumber);
        LG_ERR(isMsgInvalidData.value());
        if (attemptNumber < maxReadMemoryReAttempts) {
            attemptNumber++;
            return getValueFromMemoryAddress(memoryAddress, attemptNumber);
        }
        LG_INF("\n\n TRANSMISSION FAILURE - TOO MANY REATTEMPTS - cmd getValueFromMemoryAddress FAILED\n");
        return std::nullopt;
    }
}





bool ProfilometerManager::isConnectedAndWorking() {
    if (not serialPort_.has_value()) {
        setUpPort();
    }
    if (not serialPort_.has_value()) {
        return false;
    }
    auto data = getProfile();
    bool wasConnectionSuccessful = not data.empty();
    return wasConnectionSuccessful;
}


const std::vector<std::pair<uint16_t, uint16_t>> ProfilometerManager::testProfileData = {{1001, 2001},
                                                                                         {1002, 2002},
                                                                                         {1003, 2003},
                                                                                         {1004, 2004},
                                                                                         {1005, 2005},
                                                                                         {1006, 2006},
                                                                                         {1007, 2007}};

////NOT TESTED


