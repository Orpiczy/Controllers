//
// Created by lukas on 12/3/2021.
//

#include "FileSystemController.hpp"


std::string FileSystemController::getTimeStamp() {
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    int day = ltm->tm_mday;
    int hour = ltm->tm_hour;
    int minute = ltm->tm_min;
    int second = ltm->tm_sec;
    std::string timeStamp;
    addToTimeStamp(timeStamp, hour);
    addToTimeStamp(timeStamp, minute);
    addToTimeStamp(timeStamp, second);
    timeStamp += "_";
    addToTimeStamp(timeStamp, day);
    addToTimeStamp(timeStamp, month);
    addToTimeStamp(timeStamp, year);
    return timeStamp;
}

void FileSystemController::addToTimeStamp(std::string& timeStamp, int value) {
    if (value < 10) {
        timeStamp += std::to_string(0);
    }
    timeStamp += std::to_string(value);
}

FileSystemController* FileSystemController::GetInstance(bool isLogInfoEnable, bool isLogErrorEnable) {
    if (fsc_ == nullptr) {
        fsc_ = new FileSystemController(isLogInfoEnable, isLogErrorEnable);
    }
    return fsc_;
}

FileSystemController* FileSystemController::fsc_ = nullptr;
const std::vector<std::pair<uint32_t, uint32_t>> FileSystemController::testProfileData = {{1001, 2001},
                                                                                          {1002, 2002},
                                                                                          {1003, 2003},
                                                                                          {1004, 2004},
                                                                                          {1005, 2005},
                                                                                          {1006, 2006},
                                                                                          {1007, 2007}};

std::string getResultName(ScanResult result){
    switch(result){
        case(ScanResult::Healthy):
            return "Healthy";
        case(ScanResult::Unrecognized):
            return "Unrecognized";
        case(ScanResult::Unhealthy):
            return "Unhealthy";
        default:
            return "Incorrect Result";
    }
}