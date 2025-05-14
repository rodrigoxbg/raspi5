// This code is for testing the SPI interface controlling a ST7735 display

#include "raspi5.h"
#include <iostream>
#include <vector>
#include "tft7735.h"
#include "lib/utils/fonts/roboto12.h"
//#include "lib/utils/fonts/tekosemibold12.h"
//#include "lib/utils/fonts/roboto10.h"
void screen_boot();
void screen_tour();
TFTSCREEN tft(12, 25, 24, 128, 160); // (bgLED, pinRST, pinDC, width, height)

int main() {
    std::cout << "[ SPI test ]" << std::endl;
    tft.start();
    std::cout << "ST7735 Inicializado." << std::endl;
    screen_boot();
    while(true){
        screen_tour();
        tft.fillScreen(ST7735_RED);
        tft.drawLine(0, 30, 80, 70, ST7735_WHITE);
        tft.drawFastHLine(0, 50, 80, ST7735_WHITE);
        tft.drawFastVLine(90, 100, 60, ST7735_WHITE);
        sleep(3); // wait 1 second
        tft.fillScreen(ST7735_BLUE);
        tft.drawRect(10, 10, 50, 50, ST7735_BLACK);
        tft.drawRect(60, 70, 50, 50, ST7735_BLACK);
        std::cout << "ST7735 Poniendo background color azul." << std::endl;
        sleep(3); // wait 1 second
        tft.fillScreen(ST7735_GREEN);
        tft.drawRoundRect(10, 10, 50, 50, 5, ST7735_BLACK);
        tft.fillRoundRect(60, 70, 20, 20, 5, ST7735_BLACK);
        tft.drawCircle(100, 140, 20, ST7735_BLACK);
        tft.fillCircle(20,100, 10, ST7735_BLACK);
        std::cout << "ST7735 Poniendo background color verde." << std::endl;
        sleep(3); // wait 1 second
        tft.fillScreen(ST7735_BROWN);
        tft.drawTriangle(10, 50, 60, 2, 120, 50, ST7735_WHITE);
        tft.fillTriangle(10, 100, 60, 150, 120, 100, ST7735_WHITE);
        sleep(3); // wait 1 second
    }
    sleep(3); // wait 1 second
    std::cout << "Test de SPI terminado." << std::endl;

    return 0;
}

void screen_boot(){
    //mFont teXTFontaux = {Roboto10, 10, 10}; // Widht, height. Normalmente height es el tamaño de la fuente
    mFont teXTFont = {Roboto12, 10, 13}; // Widht, height. Normalmente height es el tamaño de la fuente
    //mFont teXTFont2 = {TekoSemiBold12, 10, 13}; // Widht, height. Normalmente height es el tamaño de la fuente
    tft.fillScreen(0x2925); // Color gris oscuro
    tft.loadFont(teXTFont);
    tft.printText(2, 5, "Hola Mundo !", ST7735_WHITE, ST7735_GREY);
    tft.backLight(true); // Turn on the backlight LED
    sleep(4); // wait 4 second stability
    
    tft.showImageJPG("spi/lib/utils/bg/raspberrypi1.jpg", 0, 0);
    tft.printText(40, 145, "Loading ...", ST7735_WHITE, ST7735_GREY);
    sleep(4); // wait 4 second stability
}

void screen_tour(){
    //----------------- [ Pantalla de Tour ] --------------------
    tft.showImageJPG("spi/lib/utils/bg/earth.jpg", 0, 0);
    tft.printText(2, 145, "Welcome on Board!", ST7735_WHITE, ST7735_GREY);
    sleep(3); // wait 1 second
    //-----------------------------------------------------------
    //_
    //_
    //----------------- [ Introduce a PIXEL ] -------------------
    tft.fillScreen(ST7735_BLACK);
    tft.printText(24, 10, "This is a Pixel", ST7735_WHITE, ST7735_GREY, true);
    tft.drawFastVLine(64, 30, 50, ST7735_GREY);
    tft.drawLine(64, 85, 54, 75, ST7735_GREY);
    tft.drawLine(64, 85, 74, 75, ST7735_GREY);
    
    tft.drawPixel(64, 96, ST7735_YELLOW);
    sleep(4); // wait 1 second
    tft.colorInvert(true);
    sleep(4); // wait 1 second
    tft.colorInvert(false);
    sleep(4); // wait 1 second
    //-----------------------------------------------------------
}