#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace constants
{
    // Baudrate of desk communication. Shouldn't change.
    constexpr int BAUDRATE = 9600;

    // GPIO of RX port for SoftwareSerial
    constexpr int SOFTWARE_SERIAL_RX = 14;

    // GPIO of TX port for SoftwareSerial
    constexpr int SOFTWARE_SERIAL_TX = 4;

    // Input buffer size for SoftwareSerial
    // Increase if the Controller fills the buffer and causes packet loss 
    constexpr int SOFTWARE_SERIAL_INPUT_BUFFER_SIZE = 64;

    // Time in ms to wait between sending keep alive messages to the handset
    constexpr int ALWAYS_ON_TIMER_DELAY = 110;

    // Time in ms to wait after showing the desk height and show the custom value
    constexpr int DISPLAY_VALUE_TIMER_DELAY = 1000;

    // Time in ms to wait between sending settings requests to the controller 
    constexpr int INITIAL_STATE_REQUEST_DELAY = 500;

    // Value to send to the handset to keep the screen awake
    // range: 1801-65535 or 0
    constexpr uint16_t KEEP_ALIVE_VALUE = 0xFFFFU;
}

#endif