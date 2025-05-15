#include "raspi5.h"
#include <vector>
#include <chrono>
void processCommand(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Unknown Command. Usa: raspi5 setgpio <pin> o raspi5 cleargpio <pin>" << std::endl;
        return;
    }

    std::string command = argv[1];

    // Verificar si el comando es 'setgpio' o 'cleargpio'
    if (command == "setgpio" && argc == 3) {
        int pin = std::stoi(argv[2]);
        ControlPin pinControl(pin, true);  // Configura el pin como salida
        pinControl.setValue(1);            // Establece el valor a HIGH (1)
        std::cout << "Pin " << pin << " configurado a HIGH." << std::endl;
    } 
    else if (command == "cleargpio" && argc == 3) {
        int pin = std::stoi(argv[2]);
        ControlPin pinControl(pin, true);  // Configura el pin como salida
        pinControl.setValue(0);            // Establece el valor a LOW (0)
        std::cout << "Pin " << pin << " configurado a LOW." << std::endl;
    } 
    else if (command == "pulsegpio" && argc == 3) {
        int pin = std::stoi(argv[2]);
        ControlPin pinControl(pin, true);  // Configura el pin como salida
        pinControl.setValue(1);            // Establece el valor a HIGH (1)
        std::cout << "Pin " << pin << ": --------[High]-------->";
        usleep(300000);
        std::cout << " (300 ms) ";
        pinControl.setValue(0);            // Establece el valor a LOW (0)
        std::cout << "--------[LOW]-------->"<< std::endl;
    } 
    else {
        std::cerr << "Comando no válido. Usa: raspi5 setgpio <pin> o raspi5 cleargpio <pin>" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "\033[1;33mRaspi :\033[0m Is a library based on gpiod created to access to the pheriperals \n"
        << "of the Raspberry Pi 5. This library provide access to control the internal chip (Rp1-C0) \n"
        << "with all capabilities such as \033[1;34m(GPIOs, PWM, SPI, I2C, Interruptions, etc)\033[0m \n"
        << "\033[1;32mRaspi :\033[0m -help -pinout -map" << std::endl;
        
        std::cout << "- - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
        std::cout << "\033[1;33mRaspi :\033[0m Version 1.0.2" << std::endl;
        std::cout << "\033[1;33mRaspi :\033[0m Author: Rodrigo Carita" << std::endl;
        std::cout << "- - - - - -- - - - - - - - - - - - - - - - - - - - - - - - - - - -" << std::endl;
        return 1;

        
    }
    else if (argc == 2 && std::string(argv[1]) == "-help") {
        std::cout << "These are the functions of the raspi5 check the Git for updates:\n";
        std::cout << "--------------------------------\n";
        std::cout << "Commands availables:\n";
        std::cout << "  raspi5 setgpio <pin>      - Set the GPIO #pin to HIGH\n";
        std::cout << "  raspi5 cleargpio <pin>    - Set the GPIO #pin to LOW\n";
        std::cout << "  raspi5 pulsegpio <pin>    - Set the GPIO #pin to HIGH and LOW (300ms)\n";
        std::cout << "  raspi5 -pinout            - Show the Raspberry Pi 5 PinOut\n";
        std::cout << "  raspi5 -map               - (Soon)\n";
        std::cout << "--------------------------------\n";
        return 0;
    }
    else if (argc == 2 && std::string(argv[1]) == "-pinout") {
        std::cout << "╔════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                     \033[1;36mRaspberry Pi 5 PINOUT\033[0m                      ║\n";
        std::cout << "╠════════════════════╦═════════╦═════════╦═══════════════════════╣\n";
        std::cout << "║      Function      ║ GPIO L  ║ GPIO R  ║       Function        ║\n";
        std::cout << "╠════════════════════╬═════════╬═════════╬═══════════════════════╣\n";
        std::cout << "║ \033[1;31mPower\033[0m              ║   \033[1;31m3.3V\033[0m  ║    \033[1;31m5V\033[0m   ║ \033[1;31mPower\033[0m                 ║\n";
        std::cout << "║ \033[0;34mI2C_SDA\033[0m            ║ \033[0;36mGPIO 2\033[0m  ║    \033[1;31m5V\033[0m   ║ \033[1;31mPower\033[0m                 ║\n";
        std::cout << "║ \033[0;34mI2C_SCL\033[0m            ║ \033[0;36mGPIO 3\033[0m  ║   GND   ║                       ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 4\033[0m  ║ \033[0;36mGPIO 14\033[0m ║ \033[0;32mUART_Tx\033[0m               ║\n";
        std::cout << "║                    ║   GND   ║ \033[0;36mGPIO 15\033[0m ║ \033[0;32mUART_Rx\033[0m               ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 17\033[0m ║ \033[0;36mGPIO 18\033[0m ║ \033[0;35mPCM_CLK\033[0m               ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 27\033[0m ║   GND   ║                       ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 22\033[0m ║ \033[0;36mGPIO 23\033[0m ║                       ║\n";
        std::cout << "║ \033[1;31mPower\033[0m              ║   \033[1;31m3.3V\033[0m  ║ \033[0;36mGPIO 24\033[0m ║                       ║\n";
        std::cout << "║ \033[0;33mSPI_MOSI\033[0m           ║ \033[0;36mGPIO 10\033[0m ║   GND   ║                       ║\n";
        std::cout << "║ \033[0;33mSPI_MISO\033[0m           ║ \033[0;36mGPIO 9\033[0m  ║ \033[0;36mGPIO 25\033[0m ║                       ║\n";
        std::cout << "║ \033[0;33mSPI_SCLK\033[0m           ║ \033[0;36mGPIO 11\033[0m ║ \033[0;36mGPIO 8\033[0m  ║ \033[0;33mSPI_CE0\033[0m               ║\n";
        std::cout << "║                    ║   GND   ║ \033[0;36mGPIO 7\033[0m  ║ \033[0;33mSPI_CE1\033[0m               ║\n";
        std::cout << "║ ID SD              ║    -    ║    -    ║ ID SC                 ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 5\033[0m  ║   GND   ║                       ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 6\033[0m  ║ \033[0;36mGPIO 12\033[0m ║                       ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 13\033[0m ║   GND   ║                       ║\n";
        std::cout << "║ \033[0;35mPCM_FS\033[0m             ║ \033[0;36mGPIO 19\033[0m ║ \033[0;36mGPIO 16\033[0m ║                       ║\n";
        std::cout << "║                    ║ \033[0;36mGPIO 26\033[0m ║ \033[0;36mGPIO 20\033[0m ║ \033[0;35mPCM_Din\033[0m               ║\n";
        std::cout << "║                    ║   GND   ║ \033[0;36mGPIO 21\033[0m ║ \033[0;35mPCM_Dout\033[0m              ║\n";
        std::cout << "╚════════════════════╩═════════╩═════════╩═══════════════════════╝\n";
        return 0;
    }
    else if (argc == 2 && std::string(argv[1]) == "-testspi") {
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
        uint8_t color[2] = {0xFF, 0x00}; // Color rojo
        for (int i = 0; i < 128 * 160; ++i) {
            sendData(color, sizeof(color));
        }
        usleep(100000); // Esperar 100 ms


        std::cout << "ST7735 Inicializado." << std::endl;
        return 0;
    }


    processCommand(argc, argv);  // Procesar los argumentos
    return 0;
}
