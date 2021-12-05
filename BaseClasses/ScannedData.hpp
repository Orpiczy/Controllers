//
// Created by lukas on 12/5/2021.
//

#ifndef CONTROLLERS_SCANNEDDATA_HPP
#define CONTROLLERS_SCANNEDDATA_HPP


#include <cstdint>
#include <utility>
#include <vector>
#include "../Common/CustomEnumTypes.hpp"

struct ScannedData {
    //profilometer
    uint16_t out1 {0};
    uint16_t out2 {0};
    uint16_t out3 {0};
    uint16_t outA {0};
    std::vector<std::pair<uint16_t,uint16_t>> profileData {};
    ScanResult resultProfilometer {ScanResult::Unrecognized};

    //camera
    std::vector<std::vector<std::tuple<uint8_t, uint8_t,uint8_t>>> cameraImage {};
    ScanResult resultCamera {ScanResult::Unrecognized};


    //Overall Result
    ScanResult finalResult {ScanResult::Unrecognized};
};


#endif //CONTROLLERS_SCANNEDDATA_HPP
