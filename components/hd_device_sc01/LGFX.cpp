#include "LGFX.h"

namespace esphome {
namespace hd_device {

#define TFT_MOSI    GPIO_NUM_13 
#define TFT_MISO    -1  // Set this PIN for using shared SPI option
#define TFT_SCLK    GPIO_NUM_14
#define TFT_DC      GPIO_NUM_21 
#define TFT_CS      GPIO_NUM_15
#define TFT_RST     GPIO_NUM_22  

#define TOUCH_INT   GPIO_NUM_39
#define TOUCH_SDA   GPIO_NUM_18
#define TOUCH_SCL   GPIO_NUM_19

LGFX::LGFX(void)
{
  {
      // set up bus control.
      auto cfg = _bus_instance.config(); // gets the structure for bus settings.

      // SPI bus settings
      cfg.spi_host = VSPI_HOST; // Select SPI to use ESP32-S2,C3: SPI2_HOST or SPI3_HOST / ESP32: VSPI_HOST or HSPI_HOST

      //* Due to the ESP-IDF upgrade, the description of VSPI_HOST , HSPI_HOST will be deprecated, so if you get an error, use SPI2_HOST , SPI3_HOST instead.
      cfg.spi_mode = 0;          // Set SPI communication mode (0-3) 
      cfg.freq_write = 80000000; // SPI clock on transmission (up to 80MHz, rounded to 80MHz divided by integer)
      cfg.freq_read = 16000000;  // SPI clock on reception
      cfg.spi_3wire = false;      // Set true when receiving on the MOSI pin
      cfg.use_lock = true;       // set true if transaction lock is used
 
      //  * With the ESP-IDF version upgrade, SPI_DMA_CH_AUTO (automatic setting) of DMA channels is recommended. 
      cfg.dma_channel = SPI_DMA_CH_AUTO; // Set DMA channel to be used (0=DMA not used / 1=1ch / 2=ch / SPI_DMA_CH_AUTO=Auto setting)

      cfg.pin_sclk = TFT_SCLK; // Set SCLK pin number for SPI
      cfg.pin_mosi = TFT_MOSI; // Set SPI MOSI pin number

      // When using the SPI bus, which is common to the SD card, be sure to set MISO without omitting it.
      cfg.pin_miso = TFT_MISO; // Set THE MSO pin number of spi (-1 = disable)
      cfg.pin_dc = TFT_DC;    // Set THE D/C pin number of SPI (-1 = disable)

      _bus_instance.config(cfg);              // reflects the setting value on the bus.
      _panel_instance.setBus(&_bus_instance); // Set the bus to the panel.
  }

  {
      // set the display panel control.
      auto cfg = _panel_instance.config(); // gets the structure for display panel settings.

      cfg.pin_cs = TFT_CS;    // Pin number to which CS is connected (-1 = disable)
      cfg.pin_rst = TFT_RST;  // Pin number to which RST is connected (-1 = disable)
      cfg.pin_busy = -1;      // Pin number to which BUSY is connected (-1 = disable)

      // the following setting values are set to a general initial value for each panel,
      cfg.panel_width = TFT_WIDTH;    // actual visible width
      cfg.panel_height = TFT_HEIGHT;   // actually visible height
      cfg.offset_x = 0;         // Panel X-direction offset amount
      cfg.offset_y = 0;         // Panel Y offset amount
      cfg.offset_rotation = 0;  // offset of rotational values from 0 to 7 (4 to 7 upside down)
      cfg.dummy_read_pixel = 8; // number of bits in dummy leads before pixel read
      cfg.dummy_read_bits = 1;  // number of bits in dummy leads before reading non-pixel data
      cfg.readable = false;      // set to true if data can be read
      cfg.invert = false;       // set to true if the light and dark of the panel is reversed
      cfg.rgb_order = false;    // set to true if the red and blue of the panel are swapped
      cfg.dlen_16bit = false;   // Set to true for panels that transmit data lengths in 16-bit increments in 16-bit parallel or SPI
      cfg.bus_shared = true;    // Set to true when sharing the bus with sd card (bus control is performed with drawJpgFile, etc.)

      // The following should only be set if the display is misalized by a driver with a variable number of pixels, such as st7735 or ILI9163.
      // cfg.memory_width = 240; //Maximum width supported by driver ICs
      // cfg.memory_height = 320; //Maximum height supported by driver ICs

      _panel_instance.config(cfg);
  }

  {
      auto cfg = _light_instance.config();    

      cfg.pin_bl = 23;//45;              
      cfg.invert = false;           
      cfg.freq   = 44100;           
      cfg.pwm_channel = 7;          

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  
  }

  { 
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;
      cfg.x_max      = 319;
      cfg.y_min      = 0;  
      cfg.y_max      = 479;
      cfg.pin_int    = TOUCH_INT;  
      cfg.bus_shared = false; 
      cfg.offset_rotation = 0;

      cfg.i2c_port = 1;//I2C_NUM_1;
      cfg.i2c_addr = 0x38;
      cfg.pin_sda  = TOUCH_SDA;   
      cfg.pin_scl  = TOUCH_SCL;   
      cfg.freq = 400000;  

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  
    }

    setPanel(&_panel_instance); // Set the panel to be used.
  }
  setPanel(&_panel_instance);
}

}  // namespace ha_deck
}  // namespace esphome
