#include "FileSystem/FileSystemController.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

//int getBasicId(std::string id){
//    bool isDigit = true;
//    for()
//}
int main() {

//    std::ofstream file;
//    std::string filename = "testFile.txt";
//
//    //seeks the end of the stream before each writing ios::app
//
//    file.open(savePath + filename);
//    std::string line = "Something different happened 2";
//    std::cout << "Error: " << strerror(errno) << std::endl;
//    if(file.is_open()){
//        std::cout<<"File was opened correctly"<<std::endl;
//    }
//    file << line << std::endl;
//    file.close();
//
//    std::ifstream rFile;
//    filename = "testFile.txt";
//    rFile.open(savePath + filename);
//    std::string word;
//    while (getline(rFile, word)) {
//        std::cout << word << std::endl;
//    }
//    rFile.close();
//    time_t now = time(0);
//    tm *ltm = localtime(&now);
//    int year = 1900 + ltm->tm_year;
//    int month = 1 + ltm->tm_mon;
//    int day = ltm->tm_mday;
//    int hour = ltm->tm_hour;
//    int minute = ltm->tm_min;
//    int second = ltm->tm_sec;
//
//    std::cout << "Year:" << year <<std::endl;
//    std::cout << "Month: "<< month <<std::endl;
//    std::cout << "Day: "<< day <<std::endl;
//    std::cout << "Time: "<< hour << ":";
//    std::cout << minute << ":";
//    std::cout << second <<std::endl;
//
//    std::cout<<FileSystemController::getTimeStamp();
//
//    return std::filesystem::exists(savePath + filename);;
    auto fs = FileSystemController::GetInstance();
     fs->addScanToMainDataBase();
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

    auto a = '_';
    auto b = '.';
    std::string text = "|       04122021|           1|           2|           3|";
//    "       04122021|           1|           0|           0|"
    auto it_begin = std::find_if(text.cbegin(), text.cend(), [&a](char c) { return c == a; });
//    auto it_end = std::find_if(it_begin + 1, text.cend(), [&b](char c) { return c == b; });
//    std::cout <<  std::string{it_begin + 1, it_end };
//    fs->getNStringBetween(0,a,b,text);
//auto str = fs -> getNStringBetween(0,'|','|',text);
//std::cout << str;
}



