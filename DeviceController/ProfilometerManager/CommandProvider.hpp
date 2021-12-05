//
// Created by lukas on 07/11/2021.
//

#ifndef UNTITLED_COMMANDPROVIDER_HPP
#define UNTITLED_COMMANDPROVIDER_HPP


#include <cstdint>
#include <vector>

struct CommandProvider {

    CommandProvider() = delete;

    static std::vector <uint8_t> cmdOut1(){
        return {stx, 1, outXAcquisitionCmd[0], outXAcquisitionCmd[1], 0x00, fromOut1, etx};
    }

    static std::vector <uint8_t> cmdOut2() {
        return {stx, 1, outXAcquisitionCmd[0], outXAcquisitionCmd[1], 0x00, fromOut2, etx};
    }

    static std::vector <uint8_t> cmdOut3() {
        return {stx, 1, outXAcquisitionCmd[0], outXAcquisitionCmd[1], 0x00, fromOut3, etx};
    }

    static std::vector <uint8_t> cmdOutA() {
        return {stx, 1, outXAcquisitionCmd[0], outXAcquisitionCmd[1], 0x00, fromOutA, etx};
    }

    static std::vector <uint8_t> cmdProfileMemoryAddress(){
        return {stx,0,profileMemoryAddressAcquisitionCmd[0],profileMemoryAddressAcquisitionCmd[1],etx};
    }

    static std::vector <uint8_t> cmdProfileSizeAndTimeInfo(const uint32_t& memoryAddress, const uint8_t rlen = 0x02){
        auto* byteMemoryAddress = (uint8_t*) &memoryAddress;
        std::vector <uint8_t> cmd = {stx, 3, ValueFromMemoryAddressAcquisitionCmd[0], ValueFromMemoryAddressAcquisitionCmd[1],
                                     byteMemoryAddress[3], byteMemoryAddress[2], byteMemoryAddress[1], byteMemoryAddress[0],
                                     rlen, 0x11, etx};
        return cmd;
    }

    static std::vector <uint8_t> cmdValueFromMemoryAddress(const uint32_t& memoryAddress, const uint8_t rlen = 0x7e){
        auto* byteMemoryAddress = (uint8_t*) &memoryAddress;
        std::vector <uint8_t> cmd = {stx, 3, ValueFromMemoryAddressAcquisitionCmd[0], ValueFromMemoryAddressAcquisitionCmd[1],
                                     byteMemoryAddress[3], byteMemoryAddress[2], byteMemoryAddress[1], (byteMemoryAddress[0]),
                                     rlen, 0x21, etx};
        return cmd;
    }

    static std::vector<uint8_t> cmdProfileAndMeasuredValueAddress(){
        return {stx,0x00,profileAndMeasuredValueAddressAcquisitionCmd[0],profileAndMeasuredValueAddressAcquisitionCmd[1],etx};
    }

    static std::vector<uint8_t> cmdNumberOfDataItems(){
        return {stx,0x03,numberOfDataItemsAcquisitionCmd[0],numberOfDataItemsAcquisitionCmd[1],etx};
    }

    static std::vector<uint8_t> cmdStorageData(const uint32_t& memoryAddress,const uint8_t rlen = 0xfd){
        auto* byteMemoryAddress = (uint8_t*) &memoryAddress;
        const uint8_t fixedValue = 0x11;
        std::vector <uint8_t> cmd = {stx, 0x03, storageDataAcquisitionCmd[0], storageDataAcquisitionCmd[1],
                                     byteMemoryAddress[3], byteMemoryAddress[2], byteMemoryAddress[1], (byteMemoryAddress[0]),
                                     rlen, fixedValue, etx};
        return cmd;
    }
    

private:

    static const uint8_t stx = 0x02;
    static const uint8_t etx = 0x03;

    //Acquisition commands
    static constexpr uint8_t outXAcquisitionCmd[2] = {0xa0, 0x17};
    static constexpr uint8_t profileMemoryAddressAcquisitionCmd[2] = {0x40,0x0b};
    static constexpr uint8_t ValueFromMemoryAddressAcquisitionCmd[2] = {0x00, 0x02};
    static constexpr uint8_t profileAndMeasuredValueAddressAcquisitionCmd[2] = {0xc0,0x0d};
    static constexpr uint8_t numberOfDataItemsAcquisitionCmd[2] = {0xc0,0x10};
    static constexpr uint8_t storageDataAcquisitionCmd[2] = {0x00,0x02};

    //Communication/ChangingSetting commands


    //Commands to be implemented
    static constexpr uint8_t shutterSpeedChangeSettingCmd [2] = {0x20,0x0e};
    static constexpr uint8_t writeSettingsToEepromChangeSettingCmd [2] = {0x00,0x05};
    static constexpr uint8_t cameraModeAcquisitionCmd[2] = {0x20,0x01};

    //Data for commands
    static const uint8_t fromOut1 = 0x00;
    static const uint8_t fromOut2 = 0X01;
    static const uint8_t fromOut3 = 0X02;
    static const uint8_t fromOutA = 0X03;
};


#endif //UNTITLED_COMMANDPROVIDER_HPP
