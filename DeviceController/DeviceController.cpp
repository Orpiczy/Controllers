//
// Created by lukas on 12/5/2021.
//

#include "DeviceController.hpp"

////INTEGRAL PARTS OF CLASS

DeviceController* DeviceController::GetInstance() {
    if (dvc_ == nullptr) {
        dvc_= new DeviceController();
    }
    return dvc_;
}

////VARIABLES
DeviceController* DeviceController::dvc_ = nullptr;



