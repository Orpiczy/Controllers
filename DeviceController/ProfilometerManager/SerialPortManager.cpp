//
// Created by lukas on 06/11/2021.
//

#include <iostream>
#include "SerialPortManager.hpp"

////INTEGRAL PARTS OF CLASS
SerialPortManager::SerialPortManager(uint8_t comPortNumber, int defaultBaudRate, int defaultVMin, int defaultVTime)
        : comPortNumber_(comPortNumber), defaultBaudRate_(defaultBaudRate), defaultVMin_(defaultVMin),
          defaultVTime_(defaultVTime), serialPort_(std::nullopt) {

    if (IS_PROFILOMETER_AVAILABLE) {
        //Settings
        setUpPort();
    }

    //BUFFER
    readBufferSize_ = defaultReadBufferSize_;
    readBuffer_.resize(readBufferSize_);
    std::fill(readBuffer_.begin(), readBuffer_.end(), 0);
}


SerialPortManager& SerialPortManager::operator=(SerialPortManager&& spm) noexcept {
    if (this != &spm) {
        comPortNumber_ = spm.comPortNumber_;
        defaultBaudRate_ = spm.defaultBaudRate_;
        defaultVMin_ = spm.defaultVMin_;
        defaultVTime_ = spm.defaultVTime_;
        close(spm.serialPort_.value());
        spm.serialPort_ = std::nullopt;
        spm.comPortNumber_ = -1;

        setUpPort();
    }
    return *this;
}

SerialPortManager::SerialPortManager(SerialPortManager&& spm) noexcept {
    comPortNumber_ = spm.comPortNumber_;
    defaultBaudRate_ = spm.defaultBaudRate_;
    defaultVMin_ = spm.defaultVMin_;
    defaultVTime_ = spm.defaultVTime_;
    close(spm.serialPort_.value());
    spm.serialPort_ = std::nullopt;
    spm.comPortNumber_ = -1;

    setUpPort();
}

SerialPortManager::~SerialPortManager() {
    if (serialPort_.has_value()) {
        close(serialPort_.value());
    }
}

//// MAIN FUNCTIONALITY
int SerialPortManager::sendMessage(const std::vector<uint8_t>& cmd) {
    //preparation
    if (not serialPort_.has_value()) {
        setUpPort();
    }
    if (not serialPort_.has_value()) {
        return -1;
    }
    return write(serialPort_.value(), &cmd[0], cmd.size());

}

std::vector<uint8_t> SerialPortManager::readMessage(const ssize_t& requiredSize) {
    //preparation
    if (not serialPort_.has_value()) {
        setUpPort();
    }
    if (not serialPort_.has_value()) {
        return {};
    }
    std::fill(readBuffer_.begin(), readBuffer_.end(), 0);

    //reading
    ssize_t n = read(serialPort_.value(), &readBuffer_[0], readBufferSize_);
    return {readBuffer_.begin(), readBuffer_.begin() + n};
}

std::vector<uint8_t> SerialPortManager::readMessagesUntilEndSign(const ssize_t& requiredSize) {
    //preparation
    if (not serialPort_.has_value()) {
        setUpPort();
    }
    if (not serialPort_.has_value()) {
        return {};
    }
    std::fill(readBuffer_.begin(), readBuffer_.end(), 0);

    //reading
    ssize_t n{0};
    int i = 0;
    uint8_t endSign{0};
    do {
        n += read(serialPort_.value(), &readBuffer_[n], readBufferSize_ - n);
        i++;
        endSign = readBuffer_[abs(n - 2)];
    } while (endSign != 0x03 and i < maxConsecutiveReadCmd and n < readBufferSize_);
    return {readBuffer_.begin(), readBuffer_.begin() + n};
};

////HELPER
int SerialPortManager::setUpPort() {

    //PORT
    std::string path = "/dev/ttyS";
    path += std::to_string(comPortNumber_);
    serialPort_ = open(path.c_str(), O_RDWR);

    //Check for errors
    if (serialPort_.value() < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        serialPort_ = std::nullopt;
        return -1;
    }

    ////most important settings
    auto baudRate = defaultBaudRate_;
    auto vTime = defaultVTime_;
    auto vMin = defaultVMin_;
    /////


    struct termios tty;

    if (tcgetattr(serialPort_.value(), &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        serialPort_ = std::nullopt;
        return -1;
    }

    //configure termios
    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR |
                     ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)
    tty.c_cc[VTIME] = vTime;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = vMin;
    cfsetispeed(&tty, baudRate); // in theory you can pass custom baud rates as int
    cfsetospeed(&tty, baudRate);
    //cfsetspeed(&tty, B9600); two at the same time

    // Save tty settings, also checking for error
    if (tcsetattr(serialPort_.value(), TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        serialPort_ = std::nullopt;
        return -1;
    }
    return 0;
}

void SerialPortManager::clearBuffer(uint8_t timeBeforeFlush) {
    if (timeBeforeFlush > 0) {
        sleep(timeBeforeFlush);
    }
    tcflush(serialPort_.value(), TCSAFLUSH);
}















