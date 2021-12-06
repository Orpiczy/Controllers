//
// Created by lukas on 12/3/2021.
//

#ifndef FILESYSTEM_FILESYSTEM_CONTROLLER_HPP
#define FILESYSTEM_FILESYSTEM_CONTROLLER_HPP

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <vector>
#include "../../BaseClasses/SimpleLogger.hpp"
#include "../../Common/CustomEnumTypes.hpp"
#include "../../Common/Translators.hpp"


class FileSystemController : private SimpleLogger {
////METHODS
public:

    ////INTEGRAL PARTS OF CLASS
    static FileSystemController* GetInstance(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

    FileSystemController(FileSystemController& other) = delete;

    void operator=(const FileSystemController&) = delete;

    ////BASIC CMD

    int addProfilometerScanDataToCategorizedDataBase(ScanResult result, uint16_t out1, uint16_t out2,
                                                     uint16_t out3, uint16_t outA,
                                                     const std::vector<std::pair<uint16_t, uint16_t>>& profileData);

    int addCameraImageToCategorizedDataBase(ScanResult result, std::vector<std::vector<std::tuple<uint8_t, uint8_t, uint8_t>>>);;

    int addScanToDailyStatistic(ScanResult result);

    std::vector<std::tuple<std::string, int, int, int>> getDailyStatisticsFromLastNDays(int n = 3);


protected:
    ////INTEGRAL PARTS OF CLASS
    explicit FileSystemController(bool isLogInfoEnable = false, bool isLogErrorEnable = true);

private:
    ////BASIC CMD INTERNAL HELPERS

    std::pair<int, std::string>
    getNewOrUpdatedDailyStatisticLine(const std::string& line, const std::string& stamp, const ScanResult& result);

    std::string
    getNewDailyStatisticLine(const std::string& stamp, const ScanResult& result);


    std::string
    updateDailyStatisticLine(const std::string& line, const std::string& stamp,
                             const ScanResult& result);


    ////HELPERS
    std::string getFullTimeStamp();

    void addToTimeStamp(std::string& timeStamp, int value);

    std::string getNStringBetween(int n, char a, char b, std::string text);




////VARIABLES

protected:
    static FileSystemController* fsc_;
private:
    const char delimiter = '|';
    std::string statisticHeadline;
    std::string scanDataHeadline;
    const std::string savePath = "/mnt/e/Dokumenty/AiR_rok_4/S7/EngineeringThesis/Controllers/FileSystemController/Database/";
    const std::string warningScanData = "\n\n///// WARNING THIS IS READONLY MEASURE DATA /////\n\n\n";
    const std::string warningStatisticData = "\n\n///// WARNING THIS IS READONLY STATISTIC DATA /////\n\n\n";
    const std::string dailyStatisticFileName = "DailyStatistic.txt";
    const static char initCharForMainStamp = '_';
    const static char endCharForMainStamp = '.';
    const int padding = 12;
    const int initialPadding = 16;

    std::string getDailyTimeStamp();
};

#endif //FILESYSTEM_FILESYSTEM_CONTROLLER_HPP
