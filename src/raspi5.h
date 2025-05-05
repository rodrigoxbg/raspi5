#ifndef RASPI5_H
#define RASPI5_H

// Library requested to control GPIO pins 
#include <gpiod.h>
#include <iostream>

// Library requested to control SPI

#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#ifndef SPI_MSB_FIRST
#define SPI_MSB_FIRST 0
#endif

// Clase ControlPin para controlar un GPIO
class ControlPin {
public:
    ControlPin(int pinNumber, bool isOutput = true);
    ~ControlPin();

    void setMode(bool isOutput);  // Antes llamada setDirection
    void setValue(int value);
    int getValue();
    void toggle();

private:
    int pinNumber;
    bool isOutput;
    struct gpiod_line *line;
    static struct gpiod_chip *chip;
};

// Clase para manejar SPI
class SpiDevice {
    public:
        SpiDevice(const std::string& device, uint32_t speed = 500000);
        ~SpiDevice();

        // Config the order of the PINs (MSB by default)
        void setBitOrder(int order = SPI_MSB_FIRST);

        // Config the SPI mode (0 by default)
        void setDataMode(uint8_t mode = SPI_MODE_0);

        // Config the SPI speed (500000 by default)
        void setClockDivider(uint32_t divider = 500000);

        bool transfer(uint8_t* tx, uint8_t* rx, size_t length);
        bool sendCommand(uint8_t cmd);
        bool sendData(const uint8_t* data, size_t length);
        bool receiveData(uint8_t* rx, size_t length);
    
    private:
        int baudrate = 500000;
        int rstPin = 25;
        int dcPin = 24;
        int fd;
        uint32_t speed;
        uint8_t mode;
        int bitOrder;
    };
#endif