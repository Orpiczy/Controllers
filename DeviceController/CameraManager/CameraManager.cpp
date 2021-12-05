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



