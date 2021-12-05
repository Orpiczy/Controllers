//
// Created by lukas on 15/11/2021.
//

#ifndef PROFILOMETERLINUX_ERRORPROVIDER_HPP
#define PROFILOMETERLINUX_ERRORPROVIDER_HPP

#include <cstdint>
#include <string>
#include <map>

struct ErrorProvider {
    ErrorProvider() = delete;

    static const uint8_t errorFirsByte = 0xe0;

    static std::map<uint8_t, std::string> getErrorMap() {
        std::map<uint8_t, std::string> map{{0x01, "The corresponding command could not be found."},
                                           {0x02, "A parameter specified by the command was invalid."},
                                           {0x03, "STX, ETX, or the packet length was invalid or a reception time-out has occurred."},
                                           {0x04, "A BCC calculation mismatch has occurred."},
                                           {0x05, "The stop bit could not be detected."},
                                           {0x06, "A data mismatch has occurred during a flash writing inspection."},
                                           {0x07, "The half-duplex communication procedure was not followed."},
                                           {0x08, "A command that cannot be executed was sent during measurement."}};
        return map;
    }

};


#endif //PROFILOMETERLINUX_ERRORPROVIDER_HPP
