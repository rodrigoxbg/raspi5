// This code is for testing the SPI interface controlling a ST7735 display

#include "raspi5.h"
#include <iostream>
#include <vector>
#include "tft7735.h"

int main() {
    std::cout << "[ SPI test ]" << std::endl;

    TFTSCREEN tft(12, 25, 24, 128, 160); // (bgLED, pinRST, pinDC, width, height)
    tft.start();
    tft.backLight(true); // Encender el LED de fondo

    std::cout << "ST7735 Inicializado." << std::endl;
    while(true){
        sleep(3); // wait 1 second
        std::cout << "ST7735 Poniendo background color plomo oscuro." << std::endl;
        tft.fillScreen(ST7735_GREY);
        tft.drawPixel(64, 50, ST7735_WHITE);
        tft.drawPixel(64, 100, ST7735_WHITE);
        tft.drawPixel(64, 130, ST7735_WHITE);
        sleep(3); // wait 1 second
        std::cout << "ST7735 Poniendo background color rojo." << std::endl;
        tft.fillScreen(ST7735_RED);
        sleep(3); // wait 1 second
        tft.fillScreen(ST7735_BLUE);
        std::cout << "ST7735 Poniendo background color azul." << std::endl;
        sleep(3); // wait 1 second
        tft.fillScreen(ST7735_GREEN);
        std::cout << "ST7735 Poniendo background color verde." << std::endl;
    }
    sleep(3); // wait 1 second
    std::cout << "Test de SPI terminado." << std::endl;

    return 0;
}   