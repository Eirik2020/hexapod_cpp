#ifndef MAESTRO_H
#define MAESTRO_H

#include <boost/asio.hpp>
#include <vector>
#include <cstdint>
#include <string>

class Maestro {
public:
    // Constructor to initialize the serial port
    Maestro(const std::string& portName, unsigned int baudRate);

    // Destructor to close the serial port
    ~Maestro();

    // Function to set multiple targets using 0–255 values
    void setMultipleTargets(uint8_t firstChannel, const std::vector<uint8_t>& bitValues);

private:
    boost::asio::io_service io;
    boost::asio::serial_port serial;

    // Convert 0–255 bit value to target range with 128 as the midpoint
    uint16_t bitToTarget(uint8_t bitValue) const;

    // Generate the byte sequence for the "Set Multiple Targets" command
    std::vector<uint8_t> generateSetMultipleTargetsCommand(uint8_t firstChannel, const std::vector<uint16_t>& targets) const;

    // Send the command over the serial port
    void sendCommand(const std::vector<uint8_t>& command); 

};

#endif // MAESTRO_H
