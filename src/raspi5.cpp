#include "raspi5.h"

struct gpiod_chip *ControlPin::chip = nullptr;

ControlPin::ControlPin(int pinNumber, bool isOutput) : pinNumber(pinNumber), isOutput(isOutput) {
    if (!chip) {
        chip = gpiod_chip_open("/dev/gpiochip0");
        if (!chip) {
            std::cerr << "No se pudo abrir el chip GPIO" << std::endl;
            exit(1);
        }
    }

    line = gpiod_chip_get_line(chip, pinNumber);
    if (!line) {
        std::cerr << "No se pudo obtener la línea GPIO" << std::endl;
        exit(1);
    }

    setMode(isOutput);
}

ControlPin::~ControlPin() {
    if (line) {
        gpiod_line_release(line);
    }
}

void ControlPin::setMode(bool isOutput) {
    this->isOutput = isOutput;

    if (isOutput) {
        gpiod_line_request_output(line, "raspi5", 0);
    } else {
        gpiod_line_request_input(line, "raspi5");
    }
}

void ControlPin::setValue(int value) {
    if (isOutput) {
        gpiod_line_set_value(line, value);
    } else {
        std::cerr << "The GPIO is set as Input, is not possible to write a Value." << std::endl;
    }
}

int ControlPin::getValue() {
    if (!isOutput) {
        return gpiod_line_get_value(line);
    } else {
        std::cerr << "The GPIO is set as Output, is not possible to read a Value." << std::endl;
        return -1;
    }
}

void ControlPin::toggle() {
    if (isOutput) {
        int currentValue = gpiod_line_get_value(line);
        gpiod_line_set_value(line, !currentValue);
    } else {
        std::cerr << "The GPIO as Input not willing to do Toggle." << std::endl;
    }
}


// =========================================== SPI ====================================

SpiDevice::SpiDevice(const std::string& device, uint32_t speed)
    : speed(speed), mode(SPI_MODE_0), bitOrder(SPI_MSB_FIRST) {
    fd = open(device.c_str(), O_RDWR);
    if (fd < 0) {
        std::cerr << "No se pudo abrir el dispositivo SPI." << std::endl;
        exit(1);
    }

    // Configuración de velocidad
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
        std::cerr << "No se pudo configurar la velocidad del SPI." << std::endl;
        close(fd);
        exit(1);
    }

    // Configuración del modo SPI
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
        std::cerr << "No se pudo configurar el modo SPI." << std::endl;
        close(fd);
        exit(1);
    }

    // Configuración del orden de bits
    if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &bitOrder) == -1) {
        std::cerr << "No se pudo configurar el orden de bits." << std::endl;
        close(fd);
        exit(1);
    }
}

void SpiDevice::setBitOrder(int order) {
    if (ioctl(fd, SPI_IOC_WR_LSB_FIRST, &order) == -1) {
        std::cerr << "Error al establecer el orden de bits." << std::endl;
    } else {
        bitOrder = order;
    }
}

void SpiDevice::setDataMode(uint8_t mode) {
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
        std::cerr << "Error al establecer el modo SPI." << std::endl;
    } else {
        this->mode = mode;
    }
}

void SpiDevice::setClockDivider(uint32_t divider) {
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &divider) == -1) {
        std::cerr << "Error al establecer el divisor del reloj." << std::endl;
    } else {
        speed = divider;
    }
}

bool SpiDevice::transfer(uint8_t* tx, uint8_t* rx, size_t length) {
    struct spi_ioc_transfer tr = {};
    tr.tx_buf = reinterpret_cast<unsigned long>(tx);
    tr.rx_buf = reinterpret_cast<unsigned long>(rx);
    tr.len = length;
    tr.speed_hz = speed;
    tr.bits_per_word = 8;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
        std::cerr << "Error en la transferencia SPI." << std::endl;
        return false;
    }
    return true;
}

// Otros métodos como sendCommand, sendData, receiveData ya estaban implementados anteriormente.

SpiDevice::~SpiDevice() {
    if (fd >= 0) {
        close(fd);
    }
}