/********************************************************************
*
* ILI9341_T4 library example. vsync and screen tearing demo.
*
* This example demonstrates the effect of vsync on screen tearing.
* At each frame, a disk is drawn, alternatingly in red and blue.
*
* When vsync is activated and the framerate becomes high enough, the
* disk appear to be solid violet ! When vsync is disabled, screen
* tearing becomes apparent...
********************************************************************/

#include <Arduino.h>
#include <ILI9341_T4.h>



//
// DEFAULT WIRING USING SPI 0 ON TEENSY 4/4.1
//
#define PIN_SCK     13      // mandatory
#define PIN_MISO    12      // mandatory  (if the display has no MISO line, set this to 255 but then VSync will be disabled...)
#define PIN_MOSI    11      // mandatory
#define PIN_DC      10      // mandatory, can be any pin but using pin 10 (or 36 or 37 on T4.1) provides greater performance

#define PIN_CS      9       // optional (but recommended), can be any pin.  
#define PIN_RESET   6       // optional (but recommended), can be any pin. 
#define PIN_BACKLIGHT 255   // optional, set this only if the screen LED pin is connected directly to the Teensy.
#define PIN_TOUCH_IRQ 255   // optional. set this only if the touchscreen is connected on the same SPI bus
#define PIN_TOUCH_CS  255   // optional. set this only if the touchscreen is connected on the same spi bus


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
//#define PIN_TOUCH_IRQ 255   // optional. set this only if the touchscreen is connected on the same SPI bus
//#define PIN_TOUCH_CS  255   // optional. set this only if the touchscreen is connected on the same spi bus



// 30MHz SPI. Can do much better with short wires
#define SPI_SPEED       30000000


// the screen driver object
ILI9341_T4::ILI9341Driver tft(PIN_CS, PIN_DC, PIN_SCK, PIN_MOSI, PIN_MISO, PIN_RESET, PIN_TOUCH_CS, PIN_TOUCH_IRQ);


// 2 diff buffers with about 6K memory each
// (in this simple case, only 1K memory buffer would be enough). 
ILI9341_T4::DiffBuffStatic<6000> diff1;
ILI9341_T4::DiffBuffStatic<6000> diff2;

// screen dimension 
const int LX = 320; 
const int LY = 240;

// framebuffers
uint16_t internal_fb[LX*LY];     // used for buffering
uint16_t fb[LX*LY];              // main framebuffer we draw onto.





/********************************************************************
********************************************************************/


/** fill a framebuffer with a given color */
void clear(uint16_t* fb, uint16_t color = 0)
    {
    for (int i = 0; i < LX * LY; i++) fb[i] = color;
    }


/** draw a disk centered at (x,y) with radius r and color col on the framebuffer fb */
void drawDisk(uint16_t* fb, double x, double y, double r, uint16_t col)
    {
    int xmin = (int)(x - r);
    int xmax = (int)(x + r);
    int ymin = (int)(y - r);
    int ymax = (int)(y + r);
    if (xmin < 0) xmin = 0;
    if (xmax >= LX) xmax = LX - 1;
    if (ymin < 0) ymin = 0;
    if (ymax >= LY) ymax = LY - 1;
    const double r2 = r * r;
    for (int j = ymin; j <= ymax; j++)
        {
        double dy2 = (y - j) * (y - j);
        for (int i = xmin; i <= xmax; i++)
            {
            const double dx2 = (x - i) * (x - i);
            if (dx2 + dy2 <= r2) fb[i + (j * LX)] = col;
            }
        }
    }



/********************************************************************
********************************************************************/





elapsedMillis em;
bool vsync;
bool c = 0; 

bool can_do_vsync; 

void setup()
    {
    Serial.begin(9600);

    tft.output(&Serial);                // output debug infos to serial port.  
    while (!tft.begin(SPI_SPEED));      // init the display
    
    tft.setRotation(3);                 // start in portrait mode 240x320
    tft.setFramebuffer(internal_fb);    // set the internal framebuffer (enables double buffering)
    tft.setDiffBuffers(&diff1, &diff2); // set 2 diff buffers -> enables diffenrential updates.
        
    if (PIN_BACKLIGHT != 255)
        { // make sure backlight is on
        pinMode(PIN_BACKLIGHT, OUTPUT);
        digitalWrite(PIN_BACKLIGHT, HIGH);
        }

    tft.setRefreshRate(90);  // start with a screen refresh rate around 40hz

    tft.setVSyncSpacing(1);  // enable vsync
    can_do_vsync = (tft.getVSyncSpacing() > 0); // check that if it is really enabled.
    
    tft.setVSyncSpacing(0);  // disable vsync => create screen tearing :-(

    em = 0; 
    c = false; 
    vsync= false; 
    }



void loop()
    {
    clear(fb, ILI9341_T4_COLOR_WHITE); // draw a white background 

    if (em > 4000)
      { // turn ON/OFF VSync
      em = 0; 
      vsync = !vsync;
      tft.setVSyncSpacing(vsync ? 1 : 0);
      }

    const float a = (em * M_PI) / 2000.0f;
    int x = LX / 2 + 50 * cos(a);  // the center of the disk 
    int y = LY / 2 + 50 * sin(a);  // move around a bit
    c = !c; // alternate color RED/BLUE
    
    drawDisk(fb, x, y, 60, c ? ILI9341_T4_COLOR_RED : ILI9341_T4_COLOR_BLUE); // alternate blue and red color at each frame. 
    tft.overlayFPS(fb); // draw the FPS counter on the top left, in red on a semi-transparent white background
    tft.overlayText(fb, "Color melting demo\nDrawing red / blue circle alternatively...", 2, 0, 14, ILI9341_T4_COLOR_BLACK, 1.0f, ILI9341_T4_COLOR_BLUE, 0.4f, 1);

    if (!can_do_vsync)
        {
        tft.overlayText(fb, "*** VSYNC DISABLED ***", 3, 0, 16, ILI9341_T4_COLOR_RED, 1.0f, ILI9341_T4_COLOR_BLACK, 0.5f);                
        tft.overlayText(fb, "Connect the MISO pin to activate this feature...", 3, 2, 12, ILI9341_T4_COLOR_RED, 1.0f, ILI9341_T4_COLOR_BLACK, 0.5f);                
        }
    else 
        {
        if (tft.getVSyncSpacing() > 0)
            {
            tft.overlayText(fb, "VSync ON", 3, 0, 16, ILI9341_T4_COLOR_GREEN, 1.0f, ILI9341_T4_COLOR_BLACK, 0.5f);    
            }
        else 
            {
            tft.overlayText(fb, "VSync OFF", 3, 0, 16, ILI9341_T4_COLOR_RED, 1.0f, ILI9341_T4_COLOR_BLACK, 0.5f);    
            }
        }
    
    tft.update(fb); // push the framebuffer to be displayed
    }





/** end of file */
