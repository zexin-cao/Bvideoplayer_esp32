// Support for 1.47" 320x172 Round Rectangle Color IPS TFT Display
#define USER_SETUP_ID 71

#define ST7789_DRIVER  // Full configuration option, define additional parameters below for this display

#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

#define TFT_WIDTH 172  // ST7789 172 x 320
#define TFT_HEIGHT 320  // ST7789 240 x 320

#define TFT_BACKLIGHT_ON HIGH  // Level to turn ON back-light (HIGH or LOW)

#define TFT_MISO -1
#define TFT_BL  45 // LED back-light

#define TFT_CS   D18  // 片选引脚 (GPIO18)
#define TFT_DC   D5   // 数据/命令控制引脚 (GPIO5)
#define TFT_RST  D4   // 复位引脚 (GPIO4)
#define TFT_MOSI D2   // SPI数据线 (MOSI, GPIO2)
#define TFT_SCLK D15  // SPI时钟线 (SCLK, GPIO15)

#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font needs ~1820 bytes in FLASH
#define LOAD_FONT2  // Font 2. Small 16 pixel high font, needs ~3534 bytes in FLASH, 96 characters
#define LOAD_FONT4  // Font 4. Medium 26 pixel high font, needs ~5848 bytes in FLASH, 96 characters
#define LOAD_FONT6  // Font 6. Large 48 pixel font, needs ~2666 bytes in FLASH, only characters 1234567890:-.apm
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel font, needs ~2438 bytes in FLASH, only characters 1234567890:-.
#define LOAD_FONT8  // Font 8. Large 75 pixel font needs ~3256 bytes in FLASH, only characters 1234567890:-.
//#define LOAD_FONT8N // Font 8. Alternative to Font 8 above, slightly narrower, so 3 digits fit a 160 pixel TFT
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts FF1 to FF48 and custom fonts

#define SMOOTH_FONT

#define SPI_FREQUENCY 27000000
