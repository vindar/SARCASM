/********************************************************************
*
* ILI9341_T4 library example. Interfacing with the LVGL library
*
*
* music player demo from lvgl adapted to Teensy 4 with ILI9341_T4.
*
*
* This example demonstrates how to use the ILI9341_T4 driver with the
* LVGL library. Here, allocate an internal framebuffer of size 320x240
* for the ILI9341_T4 driver but only a small 320x40 buffer for LVGL
* to draw onto. Then we use the updateRegion() method from the library
* to update the internal framebuffer and sync with the screen using
* differential updates for maximum performance.
*
* The total memory consumption for the 'graphic part' is 191KB:
*
*   - 150KB for the internal framebuffer
*   - 25KB for LVGL draw buffer
*   - 16KB for 2 diffs buffer with 8Kb each.
* 
* -----------------------------------
* BUILDING THE EXAMPLE (FOR ARDUINO)
* -----------------------------------
*
* (1) Install the 'lvgl' libraries in Arduino's library folder.
*     from the github repo: https://github.com/lvgl/lvgl/ directly
*     into Arduino's library folder
*     !!! This example requires LVGL version 9.2 or later. !!!
*
* (2) Copy and rename the file 'libraries/lvgl/lv_conf_template.h' to
*     'libraries/lv_conf.h' (i.e. put this file directly in Arduino's
*     libraries root folder).
*
* (3) Edit the file 'lv_conf.h' such that:
*
*     -> Replace '#if 0' by '#if 1'               (at the begining of the file)
*     -> set #define LV_COLOR_DEPTH 16            (should be already set to the correct value)
*     -> set #define LV_DEF_REFR_PERIOD  16       (33FPS, this is to increase it to 60FPS, come on !!!)
*     -> set #define LV_FONT_MONTSERRAT_12  1
*     -> set #define LV_FONT_MONTSERRAT_14  1     (should be already set to the correct value)
*     -> set #define LV_FONT_MONTSERRAT_16  1
*
********************************************************************/


#include <ILI9341_T4.h> // the screen driver library

#include <lvgl.h> // see the comment above for infos about installing and configuring LVGL. 

#include "lv_demo_music.h" // the src/music/ folder contain the source code for LVGL music demo. 


//
// DEFAULT WIRING USING SPI 0 ON TEENSY 4/4.1
//
#define PIN_SCK     13      // mandatory
#define PIN_MISO    12      // mandatory  (if the display has no MISO line, set this to 255 but then VSync will be disabled)
#define PIN_MOSI    11      // mandatory
#define PIN_DC      10      // mandatory, can be any pin but using pin 10 (or 36 or 37 on T4.1) provides greater performance

#define PIN_CS      9       // optional (but recommended), can be any pin.  
#define PIN_RESET   6       // optional (but recommended), can be any pin. 
#define PIN_BACKLIGHT 255   // optional, set this only if the screen LED pin is connected directly to the Teensy.
#define PIN_TOUCH_IRQ 255   // optional, set this only if the touchscreen is connected on the same SPI bus
#define PIN_TOUCH_CS  255   // optional, set this only if the touchscreen is connected on the same spi bus


//
// ALTERNATE WIRING USING SPI 1 ON TEENSY 4/4.1 
//
//#define PIN_SCK     27      // mandatory 
//#define PIN_MISO    1       // mandatory  (if the display has no MISO line, set this to 255 but then VSync will be disabled)
//#define PIN_MOSI    26      // mandatory
//#define PIN_DC      0       // mandatory, can be any pin but using pin 0 (or 38 on T4.1) provides greater performance

//#define PIN_CS      30      // optional (but recommended), can be any pin.  
//#define PIN_RESET   29      // optional (but recommended), can be any pin.  
//#define PIN_BACKLIGHT 255   // optional, set this only if the screen LED pin is connected directly to the Teensy. 
//#define PIN_TOUCH_IRQ 255   // optional, set this only if the touchscreen is connected on the same SPI bus
//#define PIN_TOUCH_CS  255   // optional, set this only if the touchscreen is connected on the same spi bus


// 40MHz SPI. Can do much better with short wires
#define SPI_SPEED       40000000

// screen size in landscape mode
#define LX  320
#define LY  240

// 2 diff buffers with about 8K memory each
ILI9341_T4::DiffBuffStatic<8000> diff1;
ILI9341_T4::DiffBuffStatic<8000> diff2;

// the internal framebuffer for the ILI9341_T4 driver (150KB) 
// in DMAMEM to save space in the lower (faster) part of RAM. 
DMAMEM uint16_t internal_fb[LX * LY];

// the screen driver object
ILI9341_T4::ILI9341Driver tft(PIN_CS, PIN_DC, PIN_SCK, PIN_MOSI, PIN_MISO, PIN_RESET, PIN_TOUCH_CS, PIN_TOUCH_IRQ);


// number of lines in lvgl's internal draw buffer 
#define BUF_LY 40

lv_color_t lvgl_buf[LX * BUF_LY]; // memory for lvgl draw buffer (25KB) 

lv_display_t* disp; // pointer to lvgl display object
lv_indev_t* indev;  // pointer to lvgl input device object


/** LVGL Callback to draw on the screen */
void my_disp_flush(lv_display_t* disp, const lv_area_t* area, uint8_t* px_map)
    {
    const bool redraw_now = lv_disp_flush_is_last(disp);  // check if when should update the screen (or just buffer the changes). 
    tft.updateRegion(redraw_now, (uint16_t*)px_map, area->x1, area->x2, area->y1, area->y2); // update the interval framebuffer and then redraw the screen if requested
    lv_disp_flush_ready(disp); // tell lvgl that we are done and that the lvgl draw buffer can be reused immediately  
    }


/** LVGL Callback to read the touchpad */
void my_touchpad_read(lv_indev_t* indev, lv_indev_data_t* data)
    {
    int touchX, touchY, touchZ;
    bool touched = tft.readTouch(touchX, touchY, touchZ); // read the touchpad
    if (!touched)
        { // nothing
        data->state = LV_INDEV_STATE_REL;
        } else
        { // pressed
        data->state = LV_INDEV_STATE_PR;
        data->point.x = touchX;
        data->point.y = touchY;
        }
    }


/*use Arduinos millis() as tick source*/
static uint32_t my_tick(void)
    {
    return millis();
    }



void setup()
    {
    Serial.begin(9600);

    // ------------------------------
    // Init the ILI9341_T4 driver. 
    // ------------------------------
    tft.output(&Serial);                // send debug info to serial port.
    while (!tft.begin(SPI_SPEED));      // init
    tft.setFramebuffer(internal_fb);    // set the internal framebuffer
    tft.setDiffBuffers(&diff1, &diff2); // set the diff buffers
    tft.setRotation(1);                 // landscape mode 1 : 320x240
    tft.setDiffGap(4);                  // with have large 8K diff buffers so we can use a small gap. 
    tft.setVSyncSpacing(1);             // lvgl is already controlling framerate: we just set this to 1 to minimize screen tearing. 
    tft.setRefreshRate(100);            // 100Hz refresh, why not...
    tft.clear(0);                       // black screen to start. 


    // ------------------------------
    // Init LVGL
    // ------------------------------
    lv_init();

    lv_tick_set_cb(my_tick);

    disp = lv_display_create(LX, LY);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, lvgl_buf, nullptr, LX * BUF_LY * sizeof(int16_t), LV_DISPLAY_RENDER_MODE_PARTIAL);

    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);


    // ------------------------------
    // Run the demo
    // ------------------------------
    lv_demo_music();
    }



void loop()
    {

    lv_task_handler(); // lvgl gui handler    
    }


/** end of file */

