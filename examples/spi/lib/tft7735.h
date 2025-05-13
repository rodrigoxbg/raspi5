#ifndef TFT7735_H
#define TFT7735_H

// #include <bcm2835.h> // Deprecated, only used until Raspberry Pi 4
#include <gpiod.h> // Para Raspberry Pi 5
#include <raspi5.h>
//#include "st7735_tft/ST7735_TFT.h"
#include <iostream>

#include <cstdlib>  // Para malloc y free
#include <cstring>  // Para memset

// Colors could be selected from: https://rgbcolorpicker.com/565



// La fuente es generada con GLCDFontCreator 2
// Y deben ser incluidas en los archivos donde van a ser utilizados, aqui solo es un ejemplo
// si activamos estas ocuparán mucho espacion en memoria.

//#include "../utils/fonts/roboto12.h"
//#include "../utils/fonts/tekosemibold.h"
//#include "../utils/fonts/tekosemibold12.h"
//#include "../utils/fonts/tekosemibold24.h"

// ============================ Para control de los GPIO de la Rpi5 ===============================
/*struct GPIOPin {
    gpiod_chip* chip;
    gpiod_line* line;
};*/

// ============================= TFT Registers Declaration ===============================
#define ST7735_NOP     0x00 /**< Non operation */
#define ST7735_SWRESET 0x01 /**< Soft Reset */
#define ST7735_RDDID   0x04 /**< Read Display ID*/
#define ST7735_RDDST   0x09 /**< Read Display Status*/

// ST7735 Modes
#define ST7735_SLPIN   0x10 /**< Sleep ON */
#define ST7735_SLPOUT  0x11 /**< Sleep OFF */
#define ST7735_PTLON   0x12 /**< Partial mode */
#define ST7735_NORON   0x13 /**< Normal Display */
#define ST7735_INVOFF  0x20 /**< Display invert off */
#define ST7735_INVON   0x21 /**< Display Invert on */
#define ST7735_DISPOFF 0x28 /**< Display off */
#define ST7735_DISPON  0x29 /**< Display on */
#define ST7735_TFT_Idle_modeON 0x39 /**< Idle Mode ON */
#define ST7735_TFT_Idle_modeOF  0x38 /**< Idle Mode OFF */

// ST7735 Addressing
#define ST7735_CASET   0x2A /**< Column address set */
#define ST7735_RASET   0x2B /**<  Page address set */
#define ST7735_RAMWR   0x2C /**< Memory write */
#define ST7735_RAMRD   0x2E /**< Memory read */
#define ST7735_PTLAR   0x30 /**< Partial Area */
#define ST7735_VSCRDEF 0x33 /**< Vertical scroll def */
#define ST7735_COLMOD  0x3A /**< Interface Pixel Format */
#define ST7735_MADCTL  0x36 /**< Memory Access Control */
#define ST7735_VSCRSADD 0x37 /**< Vertical Access Control */

// Frame Rate Control
#define ST7735_FRMCTR1 0xB1 /**< Normal */
#define ST7735_FRMCTR2 0xB2 /**< idle */
#define ST7735_FRMCTR3 0xB3 /**< Partial */

#define ST7735_INVCTR  0xB4 /**< Display Inversion control */
#define ST7735_DISSET5 0xB6 /**< Display Function set */

// ST7735 Power_Control
#define ST7735_PWCTR1  0xC0 /**< Power_Control VRH: Set the GVDD voltage */
#define ST7735_PWCTR2  0xC1 /**< Power_Control BT: set VGH/ VGL voltage */
#define ST7735_PWCTR3  0xC2 /**< Power_Control APA: adjust op amp & DCA: boost Voltage */
#define ST7735_PWCTR4  0xC3 /**< Power_Control APB: adjust op amp & DCB: boost Voltage */
#define ST7735_PWCTR5  0xC4 /**< Power_Control APC: adjust op amp & DCC: boost Voltage */
#define ST7735_PWCTR6  0xFC /**< Power_Control In partial mode + Idle */

#define ST7735_VMCTR1  0xC5 /**< VCOM_Control 1 */

#define ST7735_RDID1   0xDA /**< read ID1 */
#define ST7735_RDID2   0xDB /**< read ID2  */
#define ST7735_RDID3   0xDC /**< read ID3  */
#define ST7735_RDID4   0xDD /**< read ID4  */

// ST7735 color control
#define ST7735_GMCTRP1 0xE0 /**< Positive Gamma Correction Setting */
#define ST7735_GMCTRN1 0xE1 /**< Negative Gamma Correction Setting */

// Memory Access Data Control  Register
#define ST7735_MADCTL_MY 0x80 /**< Row Address Order */
#define ST7735_MADCTL_MX 0x40 /**< Column Address Order */
#define ST7735_MADCTL_MV 0x20 /**< Row/Column Order (MV) */
#define ST7735_MADCTL_ML 0x10 /**< Vertical Refresh Order */
#define ST7735_MADCTL_RGB 0x00 /**< RGB order */
#define ST7735_MADCTL_BGR 0x08 /**< BGR order */
#define ST7735_MADCTL_MH 0x04  /**< Horizontal Refresh Order */

// Color definitions 16-Bit Color Values R5G6B5
#define   ST7735_BLACK   0x0000
#define   ST7735_BLUE    0x001F
#define   ST7735_RED     0xF800
#define   ST7735_GREEN   0x07E0
#define   ST7735_CYAN    0x07FF
#define   ST7735_MAGENTA 0xF81F
#define   ST7735_YELLOW  0xFFE0
#define   ST7735_WHITE   0xFFFF
#define   ST7735_TAN     0xED01
#define   ST7735_GREY    0x9CD1
#define   ST7735_BROWN   0x6201
#define   ST7735_DGREEN  0x01c0
#define   ST7735_ORANGE  0xFC00


// ============================ Para control de los GPIO de la Rpi5 ===============================




/*
struct GPIOPin {
    gpiod_chip* chip = nullptr;
    gpiod_line* line = nullptr;
};
*/

// ============================ Para control de las fuentes de texto ===============================
struct mFont {
    const unsigned char* data;  // Puntero a los datos de la fuente
    int netWidth;              // Ancho de Malla para alojar un caracter
    int netHeight;             // Alto de Malla para alojar un caracter
};

/*class TFTSCREEN: public ST7735_TFT{*/
class TFTSCREEN {
    public:

        enum  TFT_rotate_e : uint8_t
        {
            TFT_Degrees_0 = 0, /**< No rotation 0 degrees*/
            TFT_Degrees_90,    /**< Rotation 90 degrees*/
            TFT_Degrees_180,   /**< Rotation 180 degrees*/
            TFT_Degrees_270   /**< Rotation 270 degrees*/
        };

        /*! TFT type type of PCB */
        enum TFT_PCBtype_e: uint8_t
        {
            TFT_ST7735R_Red = 0, /**<  ST7735R Red Tab  */
            TFT_ST7735R_Green,   /**<  ST7735R Green Tab */
            TFT_ST7735S_Black,   /**<  ST7735S Black Tab */
            TFT_ST7735B,         /**<  ST7735B controller */
        };
        TFT_rotate_e TFT_rotate = TFT_Degrees_0; /**< Enum to hold rotation */
        TFT_PCBtype_e TFT_PCBtype;               /**< Enum to hold TFT type  */


        TFTSCREEN(int tftled, int tftrst, int tftdc, uint16_t anchop, uint16_t altop);
        void start();
        //void drawText(int x, int y, const char* text, uint16_t color, uint16_t bgcolor, uint8_t size, bool wrap = false);
        void backLight(bool state);
        uint8_t TFTInitPCBType(TFT_PCBtype_e pcbType, uint16_t CommDelay); // SW SPI
	    //uint8_t TFTInitPCBType(TFT_PCBtype_e pcbType, uint32_t hertz = 0, uint8_t SPICE_Pin = 0 ); // HW SPI
        
        // ============================= [Funciones de la pantalla] ==============================
        void fillScreen(uint16_t color);
        void drawPixel(int16_t x, int16_t y, uint16_t color);
        // ======================================================================================

        
        void drawText(int x, int y, const std::string& text, uint16_t color, uint16_t bgcolor, uint8_t size);
        uint16_t RGBtoBGR(uint16_t color);

        void showImageJGP(const char *filename, int x, int y);
        void popup_loading();

        // -----------------------------------------------------------------------------------------------------------------
        // Funciones para texto y fuentes
        // -----------------------------------------------------------------------------------------------------------------
        void loadFont(const mFont& fuente);
        void printText(int x, int y, const std::string& text, uint16_t color, uint16_t bgcolor=0xFF, bool transparent=true);
        //------------------------------------------------------------------------------------------------------------------
    private:
        // .............................................................................................................
        void TFTInit(void);
        void TFTResetPIN(void);
        void sendCommand(uint8_t cmd);
        void sendData(const uint8_t* data, size_t length);

        // ------------------ INTERNAL FUNCTIONS (Don't Touch) ------------------
        
        uint8_t TFTGreenTabInitialize(void);
        uint8_t TFTBlackTabInitialize(void);

        void Rcmd1(void);
	    void Rcmd2red(void);
	    void Rcmd3(void);
        void Rcmd2green(void);

        void ready_to_paint(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
        // -----------------------------------------------------------------------


        // Para texto y fuentes
        mFont currentFont;
        uint8_t* textBuffer;           // Buffer para almacenar el texto
        int bufferWidth, bufferHeight;  // Dimensiones del buffer
        uint16_t* char_widthts;         // Ancho de cada caracter
        uint16_t* font_data;            // Datos de la fuente
        int font_group_size;                 // Tamaño de grupo de caracteres a obtener de la matriz general (multiplos)
        int text_spacing;               // Espacio entre caracteres
        uint8_t getCharWidth(uint8_t c);
        uint8_t *getCharArray(uint8_t c);
        void drawCustomChar(uint8_t x, uint8_t y, char c, uint16_t color, uint16_t bgcolor=0xFF, bool transparent=true);
        void drawCustomText(uint8_t x, uint8_t y, const std::string& text, uint16_t color, uint16_t bgcolor=0xFF, bool transparent=true);
        // .............................................................................................................

        int pinled;
        int pinrst;
        int pindc;
        int ancho;
        int alto;

        int sck_freq = 8000000;
        int spi_ce = 0;

        uint32_t _hertz;        /**< Spi freq in Hertz , MAX 125 Mhz MIN 30Khz */
	    uint8_t  _SPICEX_pin;  /**< value = X , which SPI_CE pin to use */

        ControlPin tft_led;
        ControlPin tft_dc;
        ControlPin tft_rst;
        SpiDevice tft_spi;

        
        
        // GPIO para los pines
        /*GPIOPin gpio_led; // Para el led de la pantalla
        GPIOPin gpio_rst; // Para el reset de la pantalla
        GPIOPin gpio_dc;  // Para el pin de datos/comando de la pantalla*/
};


#endif