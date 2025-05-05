// This code is for testing the SPI interface controlling a ST7735 display

#include "raspi5.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "[ SPI test ]" << std::endl;
          
        SpiDevice spi("/dev/spidev0.0");
        ControlPin dcPin(24, true);
        ControlPin rstPin(25, true);
       
        rstPin.setValue(0);
        usleep(100000); // 100 ms
        rstPin.setValue(1);
        usleep(100000);

        auto sendCommand = [&](uint8_t cmd) {
            dcPin.setValue(0); // Modo comando
            uint8_t dummyRx;
            spi.transfer(&cmd, &dummyRx, 1);
        };

        auto sendData = [&](const uint8_t* data, size_t len) {
            dcPin.setValue(1); // Modo datos
            std::vector<uint8_t> mutableRx(len);  // Buffer mutable para la respuesta
            spi.transfer(const_cast<uint8_t*>(data), mutableRx.data(), len);
        };

        // Secuencia de inicialización mínima
        sendCommand(0x01); // Software Reset
        usleep(150000);    // Esperar 150 ms

        sendCommand(0x11); // Sleep Out
        usleep(150000);    

        sendCommand(0x3A); // Colores 16-bit
        uint8_t mode = 0x05;
        sendData(&mode, 1);

        sendCommand(0x29); // Display ON
        usleep(100000);

        // Background color red
        sendCommand(0x2C); // Write Memory Start
        uint8_t color[2] = {0x00, 0x08}; // Color rojo
        for (int i = 0; i < 128 * 160; ++i) {
            sendData(color, sizeof(color));
        }
        usleep(100000); // Esperar 100 ms


        std::cout << "ST7735 Inicializado." << std::endl;
        return 0;
}   