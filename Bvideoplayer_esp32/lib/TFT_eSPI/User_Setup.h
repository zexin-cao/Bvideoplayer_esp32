// lib/TFT_eSPI/User_Setup.h
#define USER_SETUP_LOADED
#define DISABLE_TOUCH

// 驱动选择
#define ST7789_DRIVER

// 屏幕参数
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_CS   D18  // 片选引脚 (GPIO18)
#define TFT_DC   D5   // 数据/命令控制引脚 (GPIO5)
#define TFT_RST  D4   // 复位引脚 (GPIO4)
#define TFT_MOSI D2   // SPI数据线 (MOSI, GPIO2)
#define TFT_SCLK D15  // SPI时钟线 (SCLK, GPIO15)
// ST7789命令码（根据数据手册）
#define TFT_CASET   0x2A
#define TFT_PASET   0x2B
#define TFT_RAMWR   0x2C
#define TFT_RAMRD   0x2E
#define TFT_INVON   0x21
#define TFT_INVOFF  0x20
#define TFT_SWRST   0x01

#define TFT_MISO   -1

// 其他配置
#define LOAD_GLCD
#define SPI_FREQUENCY  27000000