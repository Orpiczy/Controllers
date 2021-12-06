//
// Created by lukas on 12/5/2021.
//

#include "CameraManager.hpp"

////INTEGRAL PARTS OF CLASS

CameraManager* CameraManager::GetInstance() {
    if (cam_ == nullptr) {
        cam_= new CameraManager();
    }
    return cam_;
}

////VARIABLES
CameraManager* CameraManager::cam_ = nullptr;
const std::vector<std::vector<std::tuple<uint8_t,uint8_t,uint8_t>>> CameraManager::testImage = {{{12,24,5},{3,4,6}},{{7,8,9},{12,15,18}}};


