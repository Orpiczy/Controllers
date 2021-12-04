//
// Created by lukas on 14/11/2021.
//

#ifndef PROFILOMETERLINUX_SIMPLELOGGER_HPP
#define PROFILOMETERLINUX_SIMPLELOGGER_HPP

#include <string>

class SimpleLogger {
private:
    bool isLogInfoEnable_;
    const std::string infoString  = " >> INFO  - ";
    bool isLogErrorEnable_;
    const std::string errorString = " >> ERROR - ";

public:
    explicit SimpleLogger(bool isLogInfoEnable, bool isLogErrorEnable)
            : isLogInfoEnable_(isLogInfoEnable), isLogErrorEnable_(isLogErrorEnable){}

    void LG_INF(const std::string& info) const;
    void LG_INF(const std::string& info, const int& intInfo) const;

    void LG_ERR(const std::string& info) const;
    void LG_ERR(const std::string& info, const int& intInfo) const;


};

#endif //PROFILOMETERLINUX_SIMPLELOGGER_HPP
