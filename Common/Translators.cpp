//
// Created by lukas on 12/5/2021.
//

#include "Translators.hpp"

std::string getResultName(ScanResult result) {

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