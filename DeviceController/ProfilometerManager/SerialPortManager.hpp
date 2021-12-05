//
// Created by lukas on 06/11/2021.
//

#ifndef UNTITLED_SERIALPORTMANAGER_HPP
#define UNTITLED_SERIALPORTMANAGER_HPP

#include <memory>
#include <optional>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()
#include <vector>
#include <stdio.h>
#include <string.h>
#include "../../Common/ControllersFlags.hpp"


class SerialPortManager {

public:
    ////INTEGRAL PARTS OF CLASS
    SerialPortManager(uint8_t comPortNumber, int defaultBaudRate = B19200,int defaultVMin = 0,int defaultVTime = 10 );

    SerialPortManager(SerialPortManager&& spm) noexcept;

    SerialPortManager& operator=(SerialPortManager&& spm) noexcept;

    SerialPortManager(SerialPortManager const&) = delete;

    SerialPortManager& operator=(SerialPortManager const&) = delete;

    virtual ~SerialPortManager();

    ////MAIN FUNCTIONALITY
    int sendMessage(const std::vector<uint8_t>& cmd);

    std::vector<uint8_t> readMessage(const ssize_t& requiredSize = 0);

    std::vector<uint8_t> readMessagesUntilEndSign(const ssize_t& requiredSize = 0);

    void clearBuffer(uint8_t timeBeforeFlush = 0);
protected:
    int readBufferSize_;
    int defaultReadBufferSize_ = 2048;
    int defaultBaudRate_;
//    static constexpr auto defaultBaudRate_ = B57600;
    int defaultVTime_ = 10;
    int defaultVMin_ = 0;
    const int maxConsecutiveReadCmd = 30;
    uint8_t comPortNumber_;
    std::optional<int> serialPort_;
    std::vector<uint8_t> readBuffer_;

    ////HELPERS
    int setUpPort();

};


#endif //UNTITLED_SERIALPORTMANAGER_HPP
