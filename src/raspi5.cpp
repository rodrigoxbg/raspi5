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

/*SpiDevice::SpiDevice(const std::string& device, uint32_t speed) : speed(speed) {
    fd = open(device.c_str(), O_RDWR);
    if (fd < 0) {
        std::cerr << "Is not possible to Access to the SPI Port" << std::endl;
        exit(1);
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
}

SpiDevice::~SpiDevice() {
    if (fd >= 0) close(fd);
}

bool SpiDevice::transfer(uint8_t* tx, uint8_t* rx, size_t length) {
    struct spi_ioc_transfer tr{};
    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = length;
    tr.speed_hz = speed;
    tr.bits_per_word = 8;

    return ioctl(fd, SPI_IOC_MESSAGE(1), &tr) >= 0;
}*/
SpiDevice::SpiDevice(const std::string& device, uint32_t speed)
    : speed(speed) {
    // Abrir el dispositivo SPI
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
}

// Método para enviar un comando
bool SpiDevice::sendCommand(uint8_t cmd) {
    uint8_t dummyRx;
    return transfer(&cmd, &dummyRx, 1); // Enviar solo el comando
}

// Método para enviar datos
bool SpiDevice::sendData(const uint8_t* data, size_t length) {
    uint8_t dummyRx[length]; // Buffer para la recepción
    return transfer(const_cast<uint8_t*>(data), dummyRx, length);
}

// Método para recibir datos
bool SpiDevice::receiveData(uint8_t* rx, size_t length) {
    uint8_t dummyTx[length]; // Buffer vacío para el envío
    return transfer(dummyTx, rx, length); // Recibir datos
}

// Transferencia de datos
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

// Destructor
SpiDevice::~SpiDevice() {
    if (fd >= 0) {
        close(fd);
    }
}