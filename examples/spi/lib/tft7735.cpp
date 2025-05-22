//#include "tftscreen.h"
#include <tft7735.h>
#include <iostream>
#include <jpeglib.h> // Para convertir imagenes jpg
#include <png.h> // Para convertir imagenes png
#include <string>
#include <stdint.h>  // Para los tipos uint16_t y uint8_t
#include <bitset>  // Para trabajar con bits solo mostrar
#include <vector>
#include <cmath> // Para la funcion round
using namespace std;

TFTSCREEN::TFTSCREEN(int tftled, int tftrst, int tftdc, uint16_t anchop, uint16_t altop)
    : pinled(tftled), pinrst(tftrst), pindc(tftdc), ancho(anchop), alto(altop),
      tft_led(tftled,true),tft_dc(tftdc, true), tft_rst(tftrst, true), tft_spi("/dev/spidev0.0", sck_freq) {

    _widthTFT = ancho;
    _heightTFT = alto;

}

void TFTSCREEN::start(){
    TFTInit();
    TFTInitPCBType(TFT_ST7735S_Black); // Inicializa el tipo de PCB
    /*ready_to_paint(0,0, ancho-1, alto-1); // Prepara la pantalla para pintar
    uint8_t color[2] = {0xA1, 0xA1}; // Color rojo
    for (int i = 0; i < 128 * 160; ++i) {
        sendData(color, sizeof(color));
    }
    usleep(100000); // Esperar 100 ms*/
}

void TFTSCREEN::TFTInit(void) {
    tft_rst.setValue(0);
    usleep(100000); // 100 ms
    tft_rst.setValue(1);
    usleep(100000);

    sendCommand(ST7735_SWRESET); // Software Reset (0x01)
    usleep(150000);    // Esperar 150 ms
    sendCommand(ST7735_SLPOUT); // Sleep Out (0x11)
    usleep(150000);

    sendCommand(ST7735_COLMOD); // Colores 16-bit (0x3A)
    uint8_t mode = 0x05;
    sendData(&mode, 1);

    //tft_led.setValue(1); // Encender el LED de fondo
    //backLight(true); // Encender el LED de fondo
    sendCommand(ST7735_DISPON); // Display ON (0x29)
    usleep(100000);
}

void TFTSCREEN::TFTResetPIN(void) {
    tft_rst.setValue(1); // Set RST high
    usleep(100000);
    tft_rst.setValue(0); // Set RST low
    usleep(100000); // 100 ms
    tft_rst.setValue(1); // Set RST high
    usleep(100000);
}

uint8_t TFTSCREEN::TFTInitPCBType(TFT_PCBtype_e pcbType)
{
	//HighFreqDelaySet(CommDelay); // Esto modifica el HighFreqDelay
	switch(pcbType)
	{
		//case TFT_ST7735R_Red : TFTRedTabInitialize(); break;
		case TFT_ST7735R_Green: TFTGreenTabInitialize(); break;
		case TFT_ST7735S_Black: TFTBlackTabInitialize(); break;
		//case TFT_ST7735B : TFTST7735BInitialize(); break;
		default:
			std::cout << "Error:TFTInitPCBType: Wrong input pcb type:" << pcbType<<std::endl;
			return 2;
		break;
	}
	return 0;
}

uint8_t TFTSCREEN::TFTGreenTabInitialize() {
	TFTResetPIN();
    tft_dc.setValue(0); // Modo comando
	Rcmd1();
	Rcmd2green();
	Rcmd3();
	TFT_PCBtype = TFT_ST7735R_Green;
	return 0;
}

void TFTSCREEN::Rcmd2green() {
	uint8_t seq1[] {0x00, 0x02, 0x00, (0x7F + 0x02)};
	uint8_t seq2[] {0x00, 0x01, 0x00, (0x9F + 0x01)};
    sendCommand(ST7735_CASET);
    sendData(seq1, sizeof(seq1));
    sendCommand(ST7735_RASET);
    sendData(seq2, sizeof(seq2));
}


uint8_t TFTSCREEN:: TFTBlackTabInitialize(){
    TFTResetPIN();
    tft_dc.setValue(0); // Modo comando
    Rcmd1();
    Rcmd2red();
    Rcmd3();
    sendCommand(ST7735_MADCTL);
    uint8_t madctl = 0xC0; // Cambia el orden de los colores a RGB
    sendData(&madctl, 1);
    TFT_PCBtype = TFT_ST7735S_Black;
    return 0;
}

void TFTSCREEN::Rcmd1() {
    uint8_t seq1[] = {0x01, 0x2C, 0x2D};
    uint8_t seq3[] = {0xA2, 0x02, 0x84};
    sendCommand(ST7735_SWRESET);
    usleep(150000); // Esperar 150 ms
    sendCommand(ST7735_SLPOUT);
    usleep(500000); // Esperar 500 ms
    sendCommand(ST7735_FRMCTR1);

    sendData(seq1, sizeof(seq1));
    sendCommand(ST7735_FRMCTR2);
    sendData(seq1, sizeof(seq1));
    sendCommand(ST7735_FRMCTR3);
    sendData(seq1, sizeof(seq1));
    sendData(seq1, sizeof(seq1));
    sendCommand(ST7735_INVCTR);
    uint8_t inv = 0x07;
    sendData(&inv, 1);

    sendCommand(ST7735_PWCTR1);
    sendData(seq3, sizeof(seq3));
    sendCommand(ST7735_PWCTR2);
    uint8_t pwctr2 = 0xC5;
    sendData(&pwctr2, 1);

    sendCommand(ST7735_PWCTR3);
    uint8_t pwctr3[] = {0x0A, 0x00};
    sendData(pwctr3, sizeof(pwctr3));
    sendCommand(ST7735_PWCTR4);
    uint8_t pwctr4[] = {0x8A, 0x2A};
    sendData(pwctr4, sizeof(pwctr4));
    sendCommand(ST7735_PWCTR5);
    uint8_t pwctr5[] = {0x8A, 0xEE};
    sendData(pwctr5, sizeof(pwctr5));
    sendCommand(ST7735_VMCTR1);
    uint8_t vmctr1 = 0x0E;
    sendData(&vmctr1, 1);

    sendCommand(ST7735_INVOFF);
    sendCommand(ST7735_MADCTL);
    uint8_t madctl = 0xC8;
    sendData(&madctl, 1);
    sendCommand(ST7735_COLMOD);
    uint8_t colmod = 0x05;
    sendData(&colmod, 1);
}

void TFTSCREEN::Rcmd2red() {
    uint8_t seq1[] = {0x00, 0x00, 0x00, 0x7F};
    uint8_t seq2[] = {0x00, 0x00, 0x00, 0x9F};
    sendCommand(ST7735_CASET);
    sendData(seq1, sizeof(seq1));
    sendCommand(ST7735_RASET);
    sendData(seq2, sizeof(seq2));
}

void TFTSCREEN::Rcmd3() {
    sendCommand(ST7735_GMCTRP1);
    uint8_t seq4[] = {0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D, 0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10};
    sendData(seq4, sizeof(seq4));
    sendCommand(ST7735_GMCTRN1);
    uint8_t seq5[] = {0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D, 0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10};
    sendData(seq5, sizeof(seq5));
    sendCommand(ST7735_NORON);
    usleep(10000); // Esperar 10 ms
    sendCommand(ST7735_DISPON);
    usleep(100000); // Esperar 100 ms
}


void TFTSCREEN::sendCommand(uint8_t cmd) {
    tft_dc.setValue(0); // Modo comando
    uint8_t dummyRx;
    tft_spi.transfer(&cmd, &dummyRx, 1);
}

void TFTSCREEN::sendData(const uint8_t* data, size_t length) {
    tft_dc.setValue(1); // Modo datos
    std::vector<uint8_t> mutableRx(length);  // Buffer mutable para la respuesta
    tft_spi.transfer(const_cast<uint8_t*>(data), mutableRx.data(), length);
}

void TFTSCREEN::ready_to_paint(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
    sendCommand(ST7735_CASET); // Set column address
    uint8_t col[] = {0x00, x0, 0x00, x1}; // 0–127
    sendData(col, sizeof(col));
    sendCommand(ST7735_RASET); // Set row address
    uint8_t row[] = {0x00, y0, 0x00, y1}; // 0–159
    sendData(row, sizeof(row));
    sendCommand(ST7735_RAMWR); // Write to RAM
}


void TFTSCREEN::backLight(bool state){
    tft_led.setValue(state ? 1 : 0);
}

void TFTSCREEN::colorInvert(bool invert) {
    sendCommand(invert ? ST7735_INVON : ST7735_INVOFF);
}




// ===========================================================================
// ---------------------------------------------------------------------------
// [ Funciones de la Pantalla ]
// ---------------------------------------------------------------------------
// oooooooooooooo GEOMETRIA
// oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
// oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo
// oooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooooo

void TFTSCREEN::fillScreen(uint16_t color) {
    ready_to_paint(0, 0, ancho - 1, alto - 1); // Prepara la pantalla para escribir

    uint8_t high = (color >> 8) & 0xFF;
    uint8_t low = color & 0xFF;
    /*
    uint8_t pixel[] = {high, low};
    for (int i = 0; i < ancho * alto; ++i) {
        sendData(pixel, 2);
    }*/
    size_t totalPixels = ancho * alto;
    size_t blockSize = 1024; // Size of the Chunks

    std::vector<uint8_t> block(blockSize * 2);
    for (size_t i = 0; i < blockSize; ++i) {
        block[2 * i] = high;
        block[2 * i + 1] = low;
    }

    while (totalPixels > 0) {
        size_t chunk = std::min(blockSize, totalPixels);
        sendData(block.data(), chunk * 2);
        totalPixels -= chunk;
    }
}

void TFTSCREEN::drawPixel(int16_t x, int16_t y, uint16_t color) {
    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
        return; // Fuera de los límites de la pantalla
    }

    ready_to_paint(x,y,x+1,y+1); // Prepara la pantalla para escribir
    uint8_t high = (color >> 8) & 0xFF;
    uint8_t low = color & 0xFF;
    uint8_t pixel[] = {high, low};
    sendData(pixel, sizeof(pixel));
}

void TFTSCREEN::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
    // Implementación del algoritmo de Bresenham para dibujar líneas
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        drawPixel(x0, y0, color); // Dibuja el pixel

        if (x0 == x1 && y0 == y1) break; // Si hemos llegado al final de la línea

        int err2 = err * 2;
        if (err2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (err2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
void TFTSCREEN::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
    // Dibuja una línea vertical rápida
    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
        return; // Fuera de los límites de la pantalla
    }
    ready_to_paint(x, y, x, y + h - 1); // Prepara la pantalla para escribir
    uint8_t high = (color >> 8) & 0xFF;
    uint8_t low = color & 0xFF;
    uint8_t pixel[] = {high, low};
    for (int i = 0; i < h; ++i) {
        sendData(pixel, sizeof(pixel));
    }
}
void TFTSCREEN::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
    // Dibuja una línea horizontal rápida
    if (x < 0 || x >= ancho || y < 0 || y >= alto) {
        return; // Fuera de los límites de la pantalla
    }
    ready_to_paint(x, y, x + w - 1, y); // Prepara la pantalla para escribir
    uint8_t high = (color >> 8) & 0xFF;
    uint8_t low = color & 0xFF;
    uint8_t pixel[] = {high, low};
    for (int i = 0; i < w; ++i) {
        sendData(pixel, sizeof(pixel));
    }
}

void TFTSCREEN::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    // Dibuja un rectángulo
    drawFastHLine(x, y, w, color); // Línea superior
    drawFastHLine(x, y + h - 1, w, color); // Línea inferior
    drawFastVLine(x, y, h, color); // Línea izquierda
    drawFastVLine(x + w - 1, y, h, color); // Línea derecha
}

void TFTSCREEN::fillRectangle(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    ready_to_paint(x, y, x + w - 1, y + h - 1); // Prepara la pantalla para escribir
    uint8_t high = (color >> 8) & 0xFF;
    uint8_t low = color & 0xFF;
    uint8_t pixel[] = {high, low};
    for (int i = 0; i < w * h; ++i) {
        sendData(pixel, sizeof(pixel));
    }
}

void TFTSCREEN::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    // Dibuja un rectángulo con esquinas redondeadas
    drawFastHLine(x + r, y, w - 2 * r, color); // Línea superior
    drawFastHLine(x + r, y + h - 1, w - 2 * r, color); // Línea inferior
    drawFastVLine(x, y + r, h - 2 * r, color); // Línea izquierda
    drawFastVLine(x + w - 1, y + r, h - 2 * r, color); // Línea derecha
    drawCircleHelper(x + r, y + r, r, 1, color);
    drawCircleHelper(x + w - r - 1, y + r, r, 2, color);
    drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4, color);
    drawCircleHelper(x + r, y + h - r - 1, r, 8, color);
}

void TFTSCREEN::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r, uint16_t color) {
    // Rellena un rectángulo con esquinas redondeadas
    fillRectangle(x + r, y, w - 2 * r, h, color); // Rellena el rectángulo central
    fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1, color);
    fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1, color);
}

void TFTSCREEN::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    // Dibuja un círculo
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r;
    ddF_x = 1;
    ddF_y = -2 * r;
    x = 0;
    y = r;

    while (x < y) {
        drawPixel(x0 + x, y0 + y, color);
        drawPixel(x0 - x, y0 + y, color);
        drawPixel(x0 + x, y0 - y, color);
        drawPixel(x0 - x, y0 - y, color);
        drawPixel(x0 + y, y0 + x, color);
        drawPixel(x0 - y, y0 + x, color);
        drawPixel(x0 + y, y0 - x, color);
        drawPixel(x0 - y, y0 - x, color);

        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;
    }
}
void TFTSCREEN::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
    // Rellena un círculo
    drawFastVLine(x0, y0 - r, 2 * r + 1, color);
    fillCircleHelper(x0, y0, r, 1, 0, color);
}

void TFTSCREEN::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {
    // Dibuja un triángulo
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void TFTSCREEN::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color) {

    int16_t minX = std::min(std::min(x0, x1), x2);
    int16_t maxX = std::max(std::max(x0, x1), x2);
    int16_t minY = std::min(std::min(y0, y1), y2);
    int16_t maxY = std::max(std::max(y0, y1), y2);

    for (int16_t y = minY; y <= maxY; y++) {
        for (int16_t x = minX; x <= maxX; x++) {
            if (pointInTriangle(x, y, x0, y0, x1, y1, x2, y2)) {
                drawPixel(x, y, color);
            }
        }
    }
}

// oooooooooooooooooooooooooo
// oooooooooooooooooooooooooo  TEXTO
// oooooooooooooooooooooooooo  TEXTO
// oooooooooooooooooooooooooo

void TFTSCREEN::loadFont(const mFont& fuente){
    currentFont = fuente;
    int charCount = currentFont.data[5];
    char_widthts = new uint16_t[charCount];

    // El array de anchos empieza desde la posición 6 hasya charCount+6
    for (int i = 0; i < charCount; i++) {
        char_widthts[i] = currentFont.data[i + 6];
    }
    font_group_size = round(currentFont.data[3]/8.0);
    text_spacing = font_group_size;
}

void TFTSCREEN::printText(int x, int y, const std::string& text, uint16_t color, uint16_t bgcolor, bool transparent) {
    drawCustomText(x, y, text, color, bgcolor,transparent);
}

void TFTSCREEN::drawCustomText(uint8_t x, uint8_t y, const std::string& text, uint16_t color, uint16_t bgcolor, bool transparent) {

    int textWidth = 0;
    uint8_t * array_anchos = new uint8_t[text.length()];
    int acum_lenghts = 0;

    for (size_t i = 0; i < text.length(); i++) {
        int e_charWidth = getCharWidth(text[i]);
        textWidth += e_charWidth  +1;
        //(debug) cout<<"El ancho de "<<text[i]<<" es: "<<e_charWidth<<endl;
        array_anchos[i] =e_charWidth;
        acum_lenghts += array_anchos[i]* +1;
    }
    /*
    for(int i=0; i<text.length(); i++){
        cout<<"El ancho de "<<text[i]<<" es: "<<getCharWidth(text[i])<<endl;
        array_anchos[i] = getCharWidth(text[i]);
        acum_lenghts += array_anchos[i]* +1;
    }*/
    int total_data_with_spaces = acum_lenghts + (text.length()-1);
    textBuffer = new uint8_t[total_data_with_spaces];

    uint8_t* charData = getCharArray(text[0]); // Por alguna razón esto no se debe borrar, porque se pierde la referencia
    for(size_t p=0; p<text.length(); p++){
        uint8_t* charData = getCharArray(text[p]);
        //delete[] charData; // Así no filtras memoria       
    }
    //OJO no borrar el loop de arriba -------------------------
    /*
    int indexion = 0;
    for(size_t p = 0; p < text.length(); p++){
        uint8_t* charData = getCharArray(text[p]);
        for(int i = 0; i < font_group_size * getCharWidth(text[p]); i++){
            textBuffer[indexion++] = charData[i];
        }
        delete[] charData; // ✅ Esto evita el double free
    }
    */

    
    int index = 0;
    for(size_t p=0; p<text.length(); p++){
        uint8_t* charData = getCharArray(text[p]);
        for(int i=0; i<font_group_size * getCharWidth(text[p]); i++){
            textBuffer[index] = charData[i];
            index++;
        }
        delete[] charData;
    }
    
    int startx = x;
    int starty = y;
    int gstartx = 0;
    int gstarty = 0;
    int indexc = 0;
    for(size_t p=0;p<text.length();p++){
        for(int gr = 0; gr<font_group_size; gr++){
            for(int col = 0; col < array_anchos[p]; col++){
                for (int bit = 7; bit >= 0 ; bit--){
                    bool bitval = (textBuffer[indexc] >> (7 - bit)) & 0x01;
                    if(bitval){
                        this->drawPixel(startx+gstartx,int(gstarty) + (starty+(7-bit)), color);
                    }else{
                        if (!transparent){
                            this->drawPixel(startx+gstartx,int(gstarty) + (starty+(7-bit)), bgcolor);
                        }
                        
                    }
                }
                gstartx += 1;
                indexc++;
            }
            gstarty = gstarty + 8;
            gstartx = 0;
        }
        gstarty = 0;
        startx += array_anchos[p] + text_spacing;
    }
    delete[] textBuffer;
    delete[] array_anchos;
}


uint8_t TFTSCREEN::getCharWidth(uint8_t c){
    //cout<<"ME pediste el ancho de "<<c<<endl;
    //cout<<"El ancho es: "<<char_widthts[c - 32]<<endl;
    return char_widthts[c - 32];    
}

uint8_t* TFTSCREEN::getCharArray(uint8_t c){
    int charIndex = c - 32;
    int charWidth = char_widthts[charIndex];
    int charInitIndex = 102; // Inicio de los datos de la fuente
    for(int r=0; r<charIndex;r++){
        charInitIndex += char_widthts[r] * font_group_size;
    }
    int charDataSize = font_group_size * charWidth;
    uint8_t* charData = new uint8_t[charDataSize]; // Array para los datos del carácter
    for (int i = 0; i < charDataSize; i++) {
        charData[i] = currentFont.data[charInitIndex + i];
    }
    int indexc = 0;
    int cant_bit_per_group = font_group_size * 8;
    int bit_sobrantes = cant_bit_per_group - currentFont.netHeight;
    for(int gr = 0; gr<font_group_size; gr++){
        for(int col = 0; col < charWidth; col++){
            if(gr == font_group_size-1){
                charData[indexc] = (charData[indexc] >> bit_sobrantes) | (charData[indexc] << (8 - bit_sobrantes));
            }
            indexc++;
        }
    }
    return charData;
}

// oooooooooooooooooooooooooo
// oooooooooooooooooooooooooo  Imagenes
// oooooooooooooooooooooooooo  Imagenes
// oooooooooooooooooooooooooo

uint8_t TFTSCREEN::drawIcon(uint8_t x, uint8_t y, uint8_t w, uint16_t color, uint16_t backcolor, const unsigned char character[]){
    // Dibuja un icono en la pantalla
    // Out of screen bounds
	if ((x >= _widthTFT) || (y >= _heightTFT))
	{
		std::cout << "Error TFTdrawIcon 2: Out of screen bounds" << std::endl;
		return 2;
	}
	// Check for null pointer
	if(character == nullptr)
	{
		std::cout << "Error TFTdrawIcon 3: Character array is not valid pointer object" << std::endl;
		return 3;
	}
	uint8_t value;
	for (uint8_t byte = 0; byte < w; byte++)
	{
		for (uint8_t mybit = 0; mybit < 8; mybit++)
		{
			value = !!(character[byte]& (1 << mybit));
			if (value)
			{
				drawPixel(x + byte, y + mybit, backcolor);
			} else
			{
				drawPixel(x + byte, y + mybit, color);
			}
			value = 0;
		}
	}
    return 0;
}

uint8_t TFTSCREEN::drawBitmap(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, uint16_t bgcolor, uint8_t* pBmp) {
    int16_t byteWidth = (w + 7) / 8;
	uint8_t byte = 0;
	uint16_t mycolor = 0;
	uint32_t ptr;

	// 1. Check for null pointer
	if( pBmp == nullptr)
	{
		std::cout << "Error TFTdrawBitmap 1: Bitmap array is nullptr" << std::endl;
		return 1;
	}
	// 2. Check bounds
	if ((x >= _widthTFT) || (y >= _heightTFT))
	{
		std::cout << "Error TFTdrawBitmap 2: Out of screen bounds, check x & y" << std::endl;
		return 2;
	}

	if ((x + w - 1) >= _widthTFT) w = _widthTFT - x;
	if ((y + h - 1) >= _heightTFT) h = _heightTFT - y;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w * h * 2);
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error TFTdrawBitmap 3: MALLOC could not assign memory " << std::endl;
		return 3;
	}
	ptr = 0;

	for (int16_t j = 0; j < h; j++)
	{
		for (int16_t i = 0; i < w; i++)
		{
			if (i & 7)
				byte <<= 1;
			else
				byte = (pBmp[j * byteWidth + i / 8]);
			mycolor = (byte & 0x80) ? color : bgcolor;
			buffer[ptr++] = mycolor >> 8;
			buffer[ptr++] = mycolor;
		}
	}
	// Set window and write buffer
	//TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);
	//spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));
    ready_to_paint(x, y, x + w - 1, y + h - 1); // Prepara la pantalla para escribir
    size_t totalPixels = w * h;
    size_t blockSize = 1024; // Puedes ajustar esto

    std::vector<uint8_t> block(blockSize * 2);
    for (size_t i = 0; i < blockSize; ++i) {
        block[2 * i] = buffer[2 * i];
        block[2 * i + 1] = buffer[2 * i + 1];
    }

    while (totalPixels > 0) {
        size_t chunk = std::min(blockSize, totalPixels);
        sendData(block.data(), chunk * 2);
        totalPixels -= chunk;
    }

	free(buffer);
    return 0;
}

uint8_t TFTSCREEN::drawBitmap24(uint8_t x, uint8_t y, uint8_t *pBmp, char w, char h) {
    uint8_t i, j;
	uint16_t color;
	uint32_t rgb, ptr;
	// 1. Check for null pointer
	if( pBmp == nullptr)
	{
		std::cout << "Error TFTdrawBitmap24 1: Bitmap array is nullptr" << std::endl;
		return 1;
	}
	// Check bounds
	if ((x >= _widthTFT) || (y >= _heightTFT))
	{
		std::cout << "Error TFTdrawBitmap24 2: Out of screen bounds" << std::endl;
		return 2;
	}
	if ((x + w - 1) >= _widthTFT) w = _widthTFT - x;
	if ((y + h - 1) >= _heightTFT) h = _heightTFT - y;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w * h * 2);
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error TFTdrawBitmap24 3: MALLOC could not assign memory " << std::endl;
		return 3;
	}
	ptr = 0;
	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w ; i ++)
		{
			// Translate RBG24 to RGB565 bitmap
			rgb = *(unsigned int*)(pBmp + i * 3 + (h-1-j) * 3 * w);
			color = Color565(((rgb >> 16) & 0xFF), ((rgb >> 8) & 0xFF), (rgb & 0xFF));
			buffer[ptr++] = color >> 8;
			buffer[ptr++] = color;
		}
	}

	// Set window and write buffer
	//TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);
	//spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));
    ready_to_paint(x, y, x + w - 1, y + h - 1); // Prepara la pantalla para escribir
    size_t totalPixels = w * h;
    size_t blockSize = 1024; // Puedes ajustar esto

    std::vector<uint8_t> block(blockSize * 2);
    for (size_t i = 0; i < blockSize; ++i) {
        block[2 * i] = buffer[2 * i];
        block[2 * i + 1] = buffer[2 * i + 1];
    }

    while (totalPixels > 0) {
        size_t chunk = std::min(blockSize, totalPixels);
        sendData(block.data(), chunk * 2);
        totalPixels -= chunk;
    }

	free(buffer);
	return 0;
}
uint8_t TFTSCREEN::drawBitmap16(uint8_t x, uint8_t y, uint8_t *pBmp, char w, char h) {
    uint8_t i, j;
	uint16_t color;
	uint32_t ptr;
	// 1. Check for null pointer
	if( pBmp == nullptr)
	{
		std::cout << "Error TFTdrawBitmap24 1: Bitmap array is nullptr" << std::endl;
		return 1;
	}
	// Check bounds
	if ((x >= _widthTFT) || (y >= _heightTFT))
	{
		std::cout << "Error TFTdrawBitmap16 2: Out of screen bounds" << std::endl;
		return 2;
	}
	if ((x + w - 1) >= _widthTFT) w = _widthTFT - x;
	if ((y + h - 1) >= _heightTFT) h = _heightTFT - y;

	// Create bitmap buffer
	uint8_t* buffer = (uint8_t*)malloc(w * h * 2);
	if (buffer == nullptr) // check malloc
	{
		std::cout << "Error TFTdrawBitmap16 3 :MALLOC could not assign memory " << std::endl;
		return 3;
	}
	ptr = 0;

	for(j = 0; j < h; j++)
	{
		for(i = 0; i < w; i ++)
		{
			color = *(unsigned int*)(pBmp + i * 2 + (h-1-j) * 2 * w);
			buffer[ptr++] = color >> 8;
			buffer[ptr++] = color;
		}
	}

	// Set window and write buffer
	//TFTsetAddrWindow(x, y, x + w - 1, y + h - 1);
	//spiWriteDataBuffer(buffer, h*w*sizeof(uint16_t));
    ready_to_paint(x, y, x + w - 1, y + h - 1); // Prepara la pantalla para escribir

    /*
    size_t totalPixels = w * h;
    size_t blockSize = 1024; // Puedes ajustar esto

    std::vector<uint8_t> block(blockSize * 2);
    for (size_t i = 0; i < blockSize; ++i) {
        block[2 * i] = buffer[2 * i];
        block[2 * i + 1] = buffer[2 * i + 1];
    }

    while (totalPixels > 0) {
        size_t chunk = std::min(blockSize, totalPixels);
        sendData(block.data(), chunk * 2);
        totalPixels -= chunk;
    }
    */

    size_t totalPixels = w * h;
    size_t sent = 0;
    size_t blockSize = 1024; // Ajustable

    while (sent < totalPixels) {
        size_t chunk = std::min(blockSize, totalPixels - sent);
        sendData(buffer + sent * 2, chunk * 2);
        sent += chunk;
    }
    
	free(buffer);
    return 0;
}

void TFTSCREEN::showImageJPG(const char *filename, int x, int y){
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;
    JSAMPARRAY buffer;
    int row_stride;

    if ((infile = fopen(filename, "rb")) == NULL) {
        std::cout << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    uint16_t *image_buffer = new uint16_t[cinfo.output_width * cinfo.output_height];   
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (JDIMENSION i = 0; i < cinfo.output_width; i++) {
            uint8_t r = buffer[0][i * 3];
            uint8_t g = buffer[0][i * 3 + 1];
            uint8_t b = buffer[0][i * 3 + 2];
            uint16_t color = (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);  // Formato RGB565
            int inverted_index = (cinfo.output_height - cinfo.output_scanline) * cinfo.output_width + i;
            image_buffer[inverted_index] = color;
        }
    }

    uint8_t* image_buffer8 = reinterpret_cast<uint8_t*>(image_buffer);
    drawBitmap16(x, y, image_buffer8, cinfo.output_width, cinfo.output_height);
    delete[] image_buffer;

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}


//................................................................................
// .................................. [ HELPERS ]...............................
void TFTSCREEN::drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r;
    ddF_x = 1;
    ddF_y = -2 * r;
    x = 0;
    y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x4) {
            drawPixel(x0 + x, y0 + y, color);
            drawPixel(x0 + y, y0 + x, color);
        }
        if (cornername & 0x2) {
            drawPixel(x0 + x, y0 - y, color);
            drawPixel(x0 + y, y0 - x, color);
        }
        if (cornername & 0x8) {
            drawPixel(x0 - y, y0 + x, color);
            drawPixel(x0 - x, y0 + y, color);
        }
        if (cornername & 0x1) {
            drawPixel(x0 - y, y0 - x, color);
            drawPixel(x0 - x, y0 - y, color);
        }
    }
}
void TFTSCREEN::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) {
    int16_t f, ddF_x, ddF_y, x, y;
    f = 1 - r;
    ddF_x = 1;
    ddF_y = -2 * r;
    x = 0;
    y = r;

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        if (cornername & 0x1) {
            drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta, color);
        }
        if (cornername & 0x2) {
            drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta, color);
            drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta, color);
        }
    }
}

bool TFTSCREEN::pointInTriangle(int16_t x, int16_t y, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
    // Verifica si un punto está dentro de un triángulo
    float area = 0.5 * (-y1 * x2 + y0 * (-x1 + x2) + x1 * (y2 - y0) + x2 * (y1 - y0));
    float s = 1 / (2 * area) * (y0 * x2 - x0 * y2 + (y2 - y0) * x);
    float t = 1 / (2 * area) * (x0 * y1 - y0 * x1 + (x0 - x1) * y);
    return s >= 0 && t >= 0 && (s + t <= 1);
}

int16_t TFTSCREEN::Color565(int16_t r, int16_t g, int16_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}





/*
void TFTSCREEN::drawText(int x, int y, const std::string& text, uint16_t color, uint16_t bgcolor, uint8_t size) {
    // Convierte std::string a const char* y llama a la otra función
    //drawText(x, y, text.c_str(), color, bgcolor, size, wrap);
    char* texto = const_cast<char*>(text.c_str());  // Convierte el std::string a char* sin advertencias
    TFTdrawText(x, y, texto, color, bgcolor, size);
}


uint16_t TFTSCREEN::RGBtoBGR(uint16_t color){
    return ((color & 0x1F) << 11) | (color & 0x7E0) | ((color & 0xF800) >> 11);
}

void TFTSCREEN::showImageJGP(const char *filename, int x, int y){
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE *infile;
    JSAMPARRAY buffer;
    int row_stride;

    if ((infile = fopen(filename, "rb")) == NULL) {
        std::cout << "Error al abrir el archivo " << filename << std::endl;
        return;
    }

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);
    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_start_decompress(&cinfo);

    row_stride = cinfo.output_width * cinfo.output_components;
    buffer = (*cinfo.mem->alloc_sarray)((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
    uint16_t *image_buffer = new uint16_t[cinfo.output_width * cinfo.output_height];   
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);
        for (JDIMENSION i = 0; i < cinfo.output_width; i++) {
            uint8_t r = buffer[0][i * 3];
            uint8_t g = buffer[0][i * 3 + 1];
            uint8_t b = buffer[0][i * 3 + 2];
            uint16_t color = (r >> 3) << 11 | (g >> 2) << 5 | (b >> 3);  // Formato RGB565
            int inverted_index = (cinfo.output_height - cinfo.output_scanline) * cinfo.output_width + i;
            image_buffer[inverted_index] = color;
        }
    }

    uint8_t* image_buffer8 = reinterpret_cast<uint8_t*>(image_buffer);
    TFTdrawBitmap16(x, y, image_buffer8, cinfo.output_width, cinfo.output_height);
    delete[] image_buffer;

    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
    fclose(infile);
}

void TFTSCREEN::popup_loading(){
    int y = 60;
    TFTfillRoundRect(6, y+1, 117, 50, 5, ST7735_GREY);
    TFTfillRoundRect(5, y, 117, 50, 5, ST7735_WHITE);
    TFTdrawRoundRect(5, y, 117, 50, 5, ST7735_GREY);
    showImageJGP("utils/icons/sandclock.jpg", 54, y+6);
    drawText(38, y+38, "LOADING...", ST7735_BLACK,ST7735_WHITE, 1);
}

// ______________________________________________________________________

void TFTSCREEN::loadFont(const mFont& fuente){
    currentFont = fuente;
    int charCount = currentFont.data[5];
    char_widthts = new uint16_t[charCount];

    // El array de anchos empieza desde la posición 6 hasya charCount+6
    for (int i = 0; i < charCount; i++) {
        char_widthts[i] = currentFont.data[i + 6];
    }
    font_group_size = round(currentFont.data[3]/8.0);
    text_spacing = font_group_size;
}

void TFTSCREEN::drawCustomText(uint8_t x, uint8_t y, const std::string& text, uint16_t color, uint16_t bgcolor, bool transparent) {

    int textWidth = 0;
    uint8_t * array_anchos = new uint8_t[text.length()];
    int acum_lenghts = 0;

    for (int i = 0; i < text.length(); i++) {
        int e_charWidth = getCharWidth(text[i]);
        textWidth += e_charWidth  +1;
        cout<<"El ancho de "<<text[i]<<" es: "<<e_charWidth<<endl;
        array_anchos[i] =e_charWidth;
        acum_lenghts += array_anchos[i]* +1;
    }
    
    //for(int i=0; i<text.length(); i++){
    //    cout<<"El ancho de "<<text[i]<<" es: "<<getCharWidth(text[i])<<endl;
    //    array_anchos[i] = getCharWidth(text[i]);
    //    acum_lenghts += array_anchos[i]* +1;
    //}
    int total_data_with_spaces = acum_lenghts + (text.length()-1);
    textBuffer = new uint8_t[total_data_with_spaces];

    //uint8_t* charData = getCharArray(text[0]); // Por alguna razón esto no se debe borrar, porque se pierde la referencia
    for(int p=0; p<text.length(); p++){
        uint8_t* charData = getCharArray(text[p]);       
    }
    int index = 0;
    for(int p=0; p<text.length(); p++){
        uint8_t* charData = getCharArray(text[p]);
        for(int i=0; i<font_group_size * getCharWidth(text[p]); i++){
            textBuffer[index] = charData[i];
            index++;
        }
        delete[] charData;
    }
    int startx = x;
    int starty = y;
    int gstartx = 0;
    int gstarty = 0;
    int indexc = 0;
    for(int p=0;p<text.length();p++){
        for(int gr = 0; gr<font_group_size; gr++){
            for(int col = 0; col < array_anchos[p]; col++){
                for (int bit = 7; bit >= 0 ; bit--){
                    bool bitval = (textBuffer[indexc] >> (7 - bit)) & 0x01;
                    if(bitval){
                        this->TFTdrawPixel(startx+gstartx,int(gstarty) + (starty+(7-bit)), color);
                    }else{
                        if (!transparent){
                            this->TFTdrawPixel(startx+gstartx,int(gstarty) + (starty+(7-bit)), bgcolor);
                        }
                        
                    }
                }
                gstartx += 1;
                indexc++;
            }
            gstarty = gstarty + 8;
            gstartx = 0;
        }
        gstarty = 0;
        startx += array_anchos[p] + text_spacing;
    }
    delete[] textBuffer;
    delete[] array_anchos;
}

void TFTSCREEN::drawCustomChar(uint8_t x, uint8_t y, char c, uint16_t color, uint16_t bgcolor, bool transparent) {
    //int charValue = c;
    int charIndex = c - 32;
    int charWidth = char_widthts[charIndex];
    int charInitIndex = 102; // Inicio de los datos de la fuente
    for(int r=0; r<charIndex;r++){
        charInitIndex += char_widthts[r] * font_group_size;
    }
    int charDataSize = font_group_size * charWidth;
    uint8_t* charData = new uint8_t[charDataSize]; // Array para los datos del carácter
    for (int i = 0; i < charDataSize; i++) {
        charData[i] = currentFont.data[charInitIndex + i];
    }

    uint8_t gstartx = 0;
    uint8_t gstarty = 0;
    int indexc = 0;
    int cant_bit_per_group = font_group_size * 8;
    int bit_sobrantes = cant_bit_per_group - currentFont.netHeight;
    for(int gr = 0; gr<font_group_size; gr++){
        for(int col = 0; col < charWidth; col++){
            if(gr == font_group_size-1){
                charData[indexc] = (charData[indexc] >> bit_sobrantes) | (charData[indexc] << (8 - bit_sobrantes));
            }
            for (int bit = 7; bit >= 0 ; bit--){
                bool bitval = (charData[indexc] >> (7 - bit)) & 0x01;
                if(bitval){
                    this->TFTdrawPixel(x+gstartx,int(gstarty) + (y+(7-bit)), color);
                }else{
                    if(!transparent){
                        this->TFTdrawPixel(x+gstartx,int(gstarty) + (y+(7-bit)), bgcolor);
                    }
                }
            }
            gstartx += 1;
            indexc++;
        }
        gstarty = gstarty + 8;
        gstartx = 0;
    }
    delete[] charData;  // Liberamos la memoria del array temporal
}

uint8_t TFTSCREEN::getCharWidth(uint8_t c){
    //cout<<"ME pediste el ancho de "<<c<<endl;
    //cout<<"El ancho es: "<<char_widthts[c - 32]<<endl;
    return char_widthts[c - 32];    
}

uint8_t* TFTSCREEN::getCharArray(uint8_t c){
    int charIndex = c - 32;
    int charWidth = char_widthts[charIndex];
    int charInitIndex = 102; // Inicio de los datos de la fuente
    for(int r=0; r<charIndex;r++){
        charInitIndex += char_widthts[r] * font_group_size;
    }
    int charDataSize = font_group_size * charWidth;
    uint8_t* charData = new uint8_t[charDataSize]; // Array para los datos del carácter
    for (int i = 0; i < charDataSize; i++) {
        charData[i] = currentFont.data[charInitIndex + i];
    }
    int indexc = 0;
    int cant_bit_per_group = font_group_size * 8;
    int bit_sobrantes = cant_bit_per_group - currentFont.netHeight;
    for(int gr = 0; gr<font_group_size; gr++){
        for(int col = 0; col < charWidth; col++){
            if(gr == font_group_size-1){
                charData[indexc] = (charData[indexc] >> bit_sobrantes) | (charData[indexc] << (8 - bit_sobrantes));
            }
            indexc++;
        }
    }
    return charData;
}

void TFTSCREEN::printText(int x, int y, const std::string& text, uint16_t color, uint16_t bgcolor, bool transparent) {
    drawCustomText(x, y, text, color, bgcolor,transparent);
}
*/