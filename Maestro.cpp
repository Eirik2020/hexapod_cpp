#include "Maestro.h"
#include <boost/asio.hpp>
#include <vector>
#include <cstdint>
#include <cmath>  // For bit-shifting and operations

// Constructor to initialize the serial port
Maestro::Maestro(const std::string& portName, unsigned int baudRate)
    : io(), serial(io, portName) {
    serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate));
}

// Destructor to close the serial port
Maestro::~Maestro() {
    if (serial.is_open()) {
        serial.close();
    }
}

// Convert 0–255 to target range 4000–8000 with 128 as midpoint (0 radians)
uint16_t Maestro::bitToTarget(uint8_t bitValue) const {
    const uint16_t minTarget = 4000;
    const uint16_t maxTarget = 8000;
    const int16_t midPoint = 128;

    // Optimized scaling using a single formula
    return minTarget + ((bitValue - midPoint) * (maxTarget - minTarget) / 255) + (maxTarget - minTarget) / 2;
}

// Set multiple targets using 0–255 values
void Maestro::setMultipleTargets(uint8_t firstChannel, const std::vector<uint8_t>& bitValues) {
    std::vector<uint16_t> targets;
    targets.reserve(bitValues.size());  // Reserve memory for efficiency
    for (const auto& bitValue : bitValues) {
        targets.push_back(bitToTarget(bitValue));  // Convert 0-255 to target
    }
    std::vector<uint8_t> command = generateSetMultipleTargetsCommand(firstChannel, targets);
    sendCommand(command);
}

// Generate byte sequence for "Set Multiple Targets" command
std::vector<uint8_t> Maestro::generateSetMultipleTargetsCommand(uint8_t firstChannel, const std::vector<uint16_t>& targets) const {
    std::vector<uint8_t> command;

    // Compact protocol: 0x9F, number of targets, first channel number
    command.reserve(3 + targets.size() * 2); // Reserve memory for efficiency
    command.push_back(0x9F);
    command.push_back(static_cast<uint8_t>(targets.size()));  // Number of targets
    command.push_back(firstChannel);                          // First channel number

    // Append each target (low bits first, then high bits)
    for (const auto& target : targets) {
        command.push_back(target & 0x7F);       // Lower 7 bits
        command.push_back((target >> 7) & 0x7F); // Upper 7 bits
    }

    return command;
}

// Send the command over the serial port
void Maestro::sendCommand(const std::vector<uint8_t>& command) {
    boost::asio::write(serial, boost::asio::buffer(command));
}

