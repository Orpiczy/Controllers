cmake_minimum_required(VERSION 3.16)
project(Controllers)

set(CMAKE_CXX_STANDARD 20)

add_executable(Controllers main.cpp
        DeviceController/ProfilometerManager/SerialPortManager.cpp   DeviceController/ProfilometerManager/SerialPortManager.hpp
        DeviceController/ProfilometerManager/ProfilometerManager.cpp DeviceController/ProfilometerManager/ProfilometerManager.hpp
        DeviceController/ProfilometerManager/MsgManager.cpp          DeviceController/ProfilometerManager/MsgManager.hpp
        DeviceController/ProfilometerManager/CommandProvider.hpp     DeviceController/ProfilometerManager/ErrorProvider.hpp
        	
        FileSystemController/FileSystem/FileSystemController.cpp  FileSystemController/FileSystem/FileSystemController.hpp

        BaseClasses/SimpleLogger.hpp BaseClasses/SimpleLogger.cpp
		Common/CustomEnumTypes.hpp Common/ControllersFlags.hpp DeviceController/DeviceController.cpp DeviceController/DeviceController.hpp DeviceController/CameraManager/CameraManager.cpp DeviceController/CameraManager/CameraManager.hpp BaseClasses/DeviceManager.hpp BaseClasses/ScannedData.hpp Common/CustomEnumTypes.cpp)