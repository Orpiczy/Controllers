#include "CustomEnumTypes.hpp"


std::string getResultName(ScanResult::Enum result) {

    switch (result) {
        case (ScanResult::Healthy):
            return "Healthy";
        case (ScanResult::Unrecognized):
            return "Unrecognized";
        case (ScanResult::Unhealthy):
            return "Unhealthy";
        default:
            return "Incorrect Result";
    }

}

//std::string getResultName(int result) {
//    switch (result) {
//        case (1):
//            return "Healthy";
//        case (0):
//            return "Unrecognized";
//        case (-1):
//            return "Unhealthy";
//        default:
//            return "Incorrect Result";
//    }
//}