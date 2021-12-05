//
// Created by lukas on 07/11/2021.
//

#ifndef UNTITLED_MSGMANAGER_HPP
#define UNTITLED_MSGMANAGER_HPP

#include <cstdint>
#include <vector>
#include "CommandProvider.hpp"
#include "ErrorProvider.hpp"
#include <optional>


class MsgManager{

public:

    MsgManager() = delete;

    //Cmd
    static std::vector<uint8_t> cmdOut1();
    static std::vector<uint8_t> cmdOut2();
    static std::vector<uint8_t> cmdOut3();
    static std::vector<uint8_t> cmdOutA();
    static std::vector<uint8_t> cmdProfileMemoryAddress();
    static std::vector<uint8_t> cmdProfileSizeAndTimeInfo(const uint32_t& memoryAddress, const uint8_t rlen = 0x02);
    static std::vector<uint8_t> cmdValueFromMemoryAddress(const uint32_t& memoryAddress, const uint8_t rlen = 0x7e);
    std::vector<uint8_t> cmdProfileAndMeasuredValueAddress();

    //Translators
    static int translateMsgToOutNValue(const std::vector<uint8_t>& message) ;
    static uint32_t translateMsgToProfileMemoryAddress(const std::vector<uint8_t>& message) ;
    static std::pair<uint16_t, uint16_t> translateUnknownSizedMsgToRawSizeAndTimeInfo(const std::vector<uint8_t>& message);
    static std::pair<uint16_t, uint16_t> translateMsgToRawSizeAndTimeInfo(const std::vector<uint8_t>& message,bool isTimeInfoInMessage) ;
    static std::pair<uint16_t, float> translateRawSizeAndTimeInfo(std::pair<uint16_t, uint16_t>& pair);
    static std::vector<std::pair<uint16_t,uint16_t>> translateRawDataPointIntoPairXY(std::vector<uint8_t> ungroupDataPoints);


    //Message Checkers
    static std::optional<std::string>
    isMessageInvalid(const std::vector<uint8_t>& message, const size_t& requiredSize, int sizeComparisonType = 0);
    static bool isMessageCrcValueCorrect(const std::vector<uint8_t>& message);
    static bool isMessageSizeCorrect(const int& messageSize, const int& requiredMessageSize,int typeOfComparison = 0);
    static std::string isItErrorMessage(const std::vector<uint8_t>& message);

    //Message Modifiers
    static std::vector<uint8_t> getMessageFromStartSignToEndSign(std::vector<uint8_t> message, uint8_t startSign, uint8_t endSign, bool areStartAndStopSignsLeftInResultStream = true);
    static std::vector<uint8_t> getClearedMessageFromStream(std::vector<uint8_t> message);






public:

private:
    //helpers
    static uint8_t calculateCheckSum(const std::vector<uint8_t>& message,const uint8_t startBit, const uint8_t endBit);
    static uint8_t calculateCheckSumForCommand(const std::vector<uint8_t>& message);
    static uint8_t calculateCheckSumForReceivedMessage(const std::vector<uint8_t>& message);


};


#endif //UNTITLED_MSGMANAGER_HPP
