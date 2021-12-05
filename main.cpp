
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "FileSystemController/FileSystem/FileSystemController.hpp"
#include "DeviceController/ProfilometerManager/ProfilometerManager.hpp"

int main() {

//    auto fs = FileSystemController::GetInstance();
//    auto pfm = ProfilometerManager::GetInstance();
//    fs->addScanToMainDataBase(ScanResult::Healthy,pfm->getOut1(),pfm->getOut2(),pfm->getOut3(),pfm->getOutA(),pfm->getProfile());
//    auto data = fs->getDailyStatisticsFromLastNDays(3);


}

//    std::string =
//    std::string healthyStr =  fs->getNStringBetween(0, delimiter, delimiter, line);
//    std::string unrecognizedStr =  fs->getNStringBetween(1, delimiter, delimiter, line);
//    std::string unhealthyStr =  fs->getNStringBetween(2, delimiter, delimiter, line);

// char a[] = "1234_5678.txt";
// std::cout<< std::strtok(a,".");
// std::string result;
// std::string a = "1234_5678.txt";
// auto it_begin = std::find_if(a.cbegin(),a.cend(),[](char c){ return not std::isdigit(c); });
// auto  it_end = std::find_if(it_begin + 1,a.cend(),[](char c){ return not std::isdigit(c); });
// std::cout<< std::string {it_begin + 1, it_end};
//
//    auto a = '_';
//    auto b = '.';
//    std::string text = "|       04122021|           1|           2|           3|";
//    "       04122021|           1|           0|           0|"
//    auto it_begin = std::find_if(text.cbegin(), text.cend(), [&a](char c) { return c == a; });
//    auto it_end = std::find_if(it_begin + 1, text.cend(), [&b](char c) { return c == b; });
//    std::cout <<  std::string{it_begin + 1, it_end };
//    fs->getNStringBetween(0,a,b,text);
//auto str = fs -> getNStringBetween(0,'|','|',text);
//std::cout << str;
//}
auto a = 2;