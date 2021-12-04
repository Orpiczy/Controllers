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
#include "SimpleLogger.hpp"

enum ScanResult {
    Healthy = 1, Unrecognized = 0, Unhealthy = -1
};

std::string getResultName(ScanResult result);

class FileSystemController : private SimpleLogger {


public:
    FileSystemController(FileSystemController& other) = delete;

    void operator=(const FileSystemController&) = delete;

    static FileSystemController* GetInstance(bool isLogInfoEnable = true, bool isLogErrorEnable = true);

    std::string getTimeStamp();

    void addToTimeStamp(std::string& timeStamp, int value);

    int addScanToMainDataBase(ScanResult result = testResult, uint16_t out1 = testOut1, uint16_t out2 = testOut2,
                              uint16_t out3 = testOut3, uint16_t outA = testOutA,
                              std::vector<std::pair<uint32_t, uint32_t>> profileData = testProfileData) {
        auto id = getTimeStamp();
        std::string resultPath = "/" + getResultName(result) + "/";
        auto fullPath = savePath + resultPath + id + ".txt";

        if (std::filesystem::exists(fullPath)) {
            LG_INF("FAILURE - FILE EXISTS - cmd addScanToMainDataBase");
            return -1;
        }

        std::ofstream file;
        file.open(fullPath);
        if (file.is_open()) {
            LG_INF("SUCCESS - FILE WAS OPENED CORRECTLY - cmd addScanToMainDataBase");
        } else {
            LG_ERR("FAILURE - FILE WAS NOT OPENED CORRECTLY " + std::string(strerror(errno)) + " - " + fullPath +
                   " - " +
                   "cmd addScanToMainDataBase");
            return -1;
        }


        file << warningScanData;
        file << scanDataHeadline << std::endl;
        file << std::setw(initialPadding) << id << delimiter;
        file << std::setw(padding) << std::to_string(result) << delimiter;
        file << std::setw(padding) << std::to_string(out1) << delimiter;
        file << std::setw(padding) << std::to_string(out2) << delimiter;
        file << std::setw(padding) << std::to_string(out3) << delimiter;
        file << std::setw(padding) << std::to_string(outA) << delimiter;

        //      for (auto pairXY : profileData) {
        for (auto it = profileData.begin(); it < profileData.end(); it++) {
            file << std::setw(padding) << std::to_string(it->first) << delimiter;
            file << std::setw(padding) << std::to_string(it->second) << delimiter;
            if (it + 1 != profileData.end()) {
                file << std::endl;
                // delimiter.size() to te 1
                file << std::setw(initialPadding + padding * 5 + 6 * 1) << delimiter;
            }
        }
        file.close();

        addScanToDailyStatistic(getNStringBetween(0, initCharForMainStamp, endCharForMainStamp, id), result);
        return 0;
    }

    int addScanToDailyStatistic(std::string stamp, ScanResult result) {
        auto fullPath = savePath + "DailyStatistic.txt";

        std::ofstream wFile;
        std::ifstream rFile;

        if (not std::filesystem::exists(fullPath)) {
            LG_INF("CREATING DATA BASE - FILE DOES NOT EXIST - cmd addScanToMainDataBase");
            wFile.open(fullPath);
            if (wFile.is_open()) {
                LG_INF("SUCCESS - FILE wFile WAS OPENED CORRECTLY - cmd addScanToMainDataBase");
            } else {
                LG_ERR("FAILURE - FILE wFile WAS NOT OPENED CORRECTLY - " + std::string(strerror(errno)) + " - " +
                       fullPath + " - " +
                       "cmd addScanToMainDataBase");
                return -1;
            }
            wFile << warningScanData;
            wFile << statisticHeadline;
            wFile << getNewDailyStatisticLine(stamp, result);
            wFile.close();
            return 1;
        }


        rFile.open(fullPath);
        if (rFile.is_open()) {
            LG_INF("SUCCESS - FILE rFile WAS OPENED CORRECTLY - cmd addScanToMainDataBase");
        } else {
            LG_ERR("FAILURE - FILE rFile WAS NOT OPENED CORRECTLY - " + std::string(strerror(errno)) + " - " +
                   fullPath + " - " +
                   "cmd addScanToMainDataBase");
            return -1;
        }

        std::stringstream reWrittenDailyStatistic;
        std::string line;

        int nrLine = 0;
        int nrDataLine = 0;
        nrDataLine += std::count(warningStatisticData.cbegin(), warningStatisticData.cend(), '\n');
        nrDataLine += std::count(statisticHeadline.cbegin(), statisticHeadline.cend(), '\n');
        nrDataLine += 1;

        while (getline(rFile, line)) {
            nrLine++;
            if (nrLine == nrDataLine) {
                std::pair<int, std::string> modifiedLine = getNewOrUpdatedDailyStatisticLine(line, stamp, result);
                switch (modifiedLine.first) {
                    case (1): //ADD
                        reWrittenDailyStatistic << modifiedLine.second;
                        reWrittenDailyStatistic << std::endl;
                        break;
                    case (0): //UPDATE
                        reWrittenDailyStatistic << modifiedLine.second;
                        reWrittenDailyStatistic << std::endl;
                        continue;
                    case (-1): //ERROR OBVIOUSLY
                        LG_ERR("FAILURE - DURING " + fullPath + " READING - addScanToHistoryDataBase");
                       return -1;
                }
            }


            reWrittenDailyStatistic << line;
            reWrittenDailyStatistic << "\n"; // probably necessary
        }
        rFile.close();

        wFile.open(fullPath, std::ios::out | std::ios::trunc); //clears all previous content
        if (wFile.is_open()) {
            LG_INF("SUCCESS - FILE wFile WAS OPENED CORRECTLY - overwriting -cmd addScanToMainDataBase");
        } else {
            LG_ERR("FAILURE - FILE wFile WAS NOT OPENED CORRECTLY - overwriting - " + std::string(strerror(errno)) +
                   " - " +
                   fullPath + " - " +
                   "cmd addScanToMainDataBase");
            return -1;
        }
        wFile << reWrittenDailyStatistic.str();

        return 0;
    }

    std::pair<int, std::string>
    getNewOrUpdatedDailyStatisticLine(const std::string& line, const std::string& stamp, const ScanResult& result) {
        std::string lineStamp = getNStringBetween(0, line[0], delimiter, line);
        //cleaning
        lineStamp.erase(
                std::remove_if(lineStamp.begin(), lineStamp.end(),
                               [](unsigned char x) { return std::isspace(x); }),
                lineStamp.end());

        //Inverse yyyy-mm-dd is easier to compare in context of stamp age

        std::string lineStampInverted = std::string{lineStamp}.substr(4) + std::string{lineStamp}.substr(2, 2) +
                                        std::string{lineStamp}.substr(0, 2);
        std::string measureStampInverted = std::string{stamp}.substr(4) + std::string{stamp}.substr(2, 2) +
                                           std::string{stamp}.substr(0, 2);
        if (stoi(measureStampInverted) > stoi(lineStampInverted)) {
            std::string newLine = getNewDailyStatisticLine(stamp, result);
            if (newLine.empty())
                return {-1, newLine};
            else {
                return {1, newLine};
            }
        } else if (lineStamp == stamp) {
            std::string updatedLine = updateDailyStatisticLine(line, stamp, result);
            if (updatedLine.empty())
                return {-1, updatedLine};
            else {
                return {0, updatedLine};
            }
        }

        LG_ERR("FAILURE - TOO EARLY DATE - YOU CAN'T MODIFY STATISTIC FROM PREVIOUS DAYS - cmd addOrUpdateStatisticLine ");
        return {-1, ""};

    }

    std::string
    getNewDailyStatisticLine(const std::string& stamp, const ScanResult& result) {
        std::stringstream newStatisticLine;
        std::string healthyStr;
        std::string unrecognizedStr;
        std::string unhealthyStr;

//        int lineStampDay = stoi(std::string{lineStamp}.substr(0, 2));
//        int lineStampMonth = stoi(std::string{lineStamp}.substr(2, 4));
//        int lineStampYear = stoi(std::string{lineStamp}.substr(4));
//
//        int measureStampDay = stoi(std::string{stamp}.substr(0, 2));
//        int measureStampMonth = stoi(std::string{stamp}.substr(2, 4));
//        int measureStampYear = stoi(std::string{stamp}.substr(4));


        switch (result) {
            case (ScanResult::Healthy):
                healthyStr = "1";
                unrecognizedStr = "0";
                unhealthyStr = "0";
                break;

            case (ScanResult::Unrecognized):
                healthyStr = "0";
                unrecognizedStr = "1";
                unhealthyStr = "0";
                break;

            case (ScanResult::Unhealthy):
                healthyStr = "0";
                unrecognizedStr = "0";
                unhealthyStr = "1";
                break;

            default:
                LG_ERR("FAILURE - UNRECOGNIZED RESULT - cmd getNewDailyStatisticLine");
                return "";
        }
        newStatisticLine<< delimiter;
        newStatisticLine << std::setw(initialPadding) << stamp << delimiter;
        newStatisticLine << std::setw(padding) << healthyStr << delimiter;
        newStatisticLine << std::setw(padding) << unrecognizedStr << delimiter;
        newStatisticLine << std::setw(padding) << unhealthyStr << delimiter;
        auto str = newStatisticLine.str();
        return newStatisticLine.str();
    }


    std::string
    updateDailyStatisticLine(const std::string& line,const std::string& stamp,
                             const ScanResult& result) {
        std::stringstream updatedStatisticLine;
        std::string healthyStr;
        std::string unrecognizedStr;
        std::string unhealthyStr;


            healthyStr = getNStringBetween(1, delimiter, delimiter, line);
            unrecognizedStr = getNStringBetween(2, delimiter, delimiter, line);
            unhealthyStr = getNStringBetween(3, delimiter, delimiter, line);

            //cleaning
            healthyStr.erase(
                    std::remove_if(healthyStr.begin(), healthyStr.end(),
                                   [](unsigned char x) { return std::isspace(x); }),
                    healthyStr.end());
            unrecognizedStr.erase(std::remove_if(unrecognizedStr.begin(), unrecognizedStr.end(),
                                                 [](unsigned char x) { return std::isspace(x); }),
                                  unrecognizedStr.end());
            unhealthyStr.erase(std::remove_if(unhealthyStr.begin(), unhealthyStr.end(),
                                              [](unsigned char x) { return std::isspace(x); }), unhealthyStr.end());
            switch (result) {
                case (ScanResult::Healthy):
                    healthyStr = std::to_string(stoi(healthyStr) + 1);
                    break;

                case (ScanResult::Unrecognized):
                    unrecognizedStr = std::to_string(stoi(unrecognizedStr) + 1);
                    break;

                case (ScanResult::Unhealthy):
                    unhealthyStr = std::to_string(stoi(unhealthyStr) + 1);
                    break;
                default:
                    LG_ERR("FAILURE - UNRECOGNIZED RESULT - updateDailyStatisticLine");
                    return "";
            }
            updatedStatisticLine << delimiter;
            updatedStatisticLine << std::setw(initialPadding ) << stamp << delimiter;
            updatedStatisticLine << std::setw(padding) << healthyStr << delimiter;
            updatedStatisticLine << std::setw(padding) << unrecognizedStr << delimiter;
            updatedStatisticLine << std::setw(padding) << unhealthyStr << delimiter;
            return updatedStatisticLine.str();


    }


public:
    FileSystemController(bool isLogInfoEnable = false, bool isLogErrorEnable = true) : SimpleLogger(isLogInfoEnable,
                                                                                                    isLogErrorEnable) {
        std::stringstream statisticHeadlineSS;
        statisticHeadlineSS << delimiter;
        statisticHeadlineSS << std::setw(initialPadding ) << "id" << delimiter;
        statisticHeadlineSS << std::setw(padding) << "Healthy" << delimiter;
        statisticHeadlineSS << std::setw(padding) << "Unrecognized" << delimiter;
        statisticHeadlineSS << std::setw(padding) << "Unhealthy" << delimiter;
        statisticHeadlineSS << std::endl;
        statisticHeadline = statisticHeadlineSS.str();

        std::stringstream scanDataHeadlineSS;
        scanDataHeadlineSS << std::setw(initialPadding) << " " << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "Result" << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "Out1" << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "Out2" << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "Out3" << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "OutA" << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "XCoord" << delimiter;
        scanDataHeadlineSS << std::setw(padding) << "YCoord" << delimiter;
        scanDataHeadline = scanDataHeadlineSS.str();

    };

    std::string getNStringBetween(int n, char a, char b, std::string text) {
//        auto it_begin = std::find_if(text.cbegin(), text.cend(), [](char c) { return not std::isdigit(c); }
        auto it_begin = std::find_if(text.cbegin(), text.cend(), [&a](char c) { return c == a; });
        auto it_end = std::find_if(it_begin + 1, text.cend(), [&b](char c) { return c == b; });

        if (n > 0 && it_begin != text.end() && it_end != text.end()) {
            return getNStringBetween(n - 1, a, b, std::string{it_end, text.cend()});
        }
        if (it_begin != text.end()) {
            return std::string{it_begin + 1, it_end};

        } else {
            return "";
        }

    }

    static FileSystemController* fsc_;
private:
    const char delimiter = '|';
    std::string statisticHeadline;
    std::string scanDataHeadline;
    const std::string savePath = "/mnt/e/Dokumenty/AiR_rok_4/S7/EngineeringThesis/Profilometr/FileSystemController/Database/";
    const std::string warningScanData = "\n\n///// WARNING THIS IS READONLY MEASURE DATA /////\n\n\n";
    const std::string warningStatisticData = "\n\n///// WARNING THIS IS READONLY STATISTIC DATA /////\n\n\n";
    const int padding = 12;
    const int initialPadding = 15;

    ///// TEST DATA, CAN BE DELETED IN PRODUCTION CODE
    const static std::vector<std::pair<uint32_t, uint32_t>> testProfileData;
    const static uint16_t testOut1 = 4123;
    const static uint16_t testOut2 = 235;
    const static uint16_t testOut3 = -2314;
    const static uint16_t testOutA = -13;
    const static ScanResult testResult = ScanResult::Unrecognized;
    const static char initCharForMainStamp = '_';
    const static char endCharForMainStamp = '.';

};


#endif //FILESYSTEM_FILESYSTEM_CONTROLLER_HPP
