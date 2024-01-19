// Cannot use OTA.  Set partition scheme to Huge Partition (3MB)
/* This is written to run on an TTGO T4 v1.3 w/TTF backpack (dimmable) ESP32.
   It should also run just fine on ESP8266 but there is none with the
   tft.  But, if you want to add your own display, then it should be
   easy to migrate it to that unit.

   NOTE: I use WiFiManager named "ESPWiFiMgr".  The first time you runas it, connect
   via WiFi to that AP ancod select the upstream WAP.  After that, as long as
   it sees that same SSID/Password, it will automatically connect.  So this
   WAP and it should run just fine.

   Now using WifiManager.

   Gets NTP time.  Creates offsets and determins TZ letters internally.

   Operation:

   While the display is off, press the top button to turn it on for 5 seconds.
   If you long press the top button, it will stay on until you do one of two
   things -- either press the bottom button or double press either button.

   While the display is on, press the bottom button to turn it off.  If will
   stay off until you press the top button or double press either button.

   See UserSettings.h tab for these variables:

   holdScreenOn    - this is how long, after a single press of the top button
                      the screen will remain on during Screen Saver (SS) period.
   showAMPM        - If you want AM/PM or 24 hours setting.  True for AM/PM

  Display:
  If you are compiling for the one with the volume knob, use this:
    #include <User_Setups/Setup23A_TTGO_TM_1.3.h> // Setup file for ESP32 and TTGO TM ST7789 SPI bus TFT
  If you are compiling for the Backlight Dimming one, use this:
    #include <User_Setups/Setup22_TTGO_T4_v1.3.h>      // Setup file for ESP32 and TTGO T4 version 1.3
  If you are compiling for thce 4 inch ILI9486, use this:
    #include <User_Setups/Setup6A_ESP_SPI_ILI9486.h>     // Setup file for SPI serial on ESP32/ESP8266
  Note: Comment out all drivers in the User_Setup.h file.  The define is done
         in the Setup6A_ESP_SPI_ILI9486.h file.

  And a bit about apilayer.com.  I fetch the exchange rate each 3 hours.  With
  apilayer.com, you get 250 free fetches per month.  8 per day is 248 per month
  but, if you restart more than twice, you can go over.  You will get back a
  code of 429.  The program catches this and tries to step down to a second
  api key.  So best to get 2 free accounts and list them both, here, so you
  can switch over as needed.  Just use any email address you have access to
  to create the second api key.
*/

//For stack size displaying.
//void * StackPtrAtStart;
//void * StackPtrEnd;
//UBaseType_t watermarkStart;

#include <ArduinoJson.h>
#include "Preferences.h"
#include "UserSettings.h"  // Do all customization here.
#include "Adafruit_GFX.h"
//#include "BluetoothSerial.h"
#include <WiFi.h>
#include "FS.h"
#include <SPI.h>

#include "OpenFontRender.h"
OpenFontRender ofr;
char sizingChars[] = "ABCabcjyWM";

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite sprite61 = TFT_eSprite(&tft);
TFT_eSprite spriteHH = TFT_eSprite(&tft);


//#include <ezTime.h>
#include "TimeLib.h"
#include "esp_sntp.h"

// If this is changed, also go to setup() around line 118 and coordinate.
const int  maxAPIkeys = 4;
const char * apiArray[] = {
  "k5MJFkvlen6ebpAvKRpUlbbBd7uPAzAC",
  "Livt8olpTHbdFKD1vY19vTfIy4IrolKy",
  "gs2IkNdDcZMjmlCsfFn3PFPkBBquhu82",
  "23TJj8EbyRWec1MPBECv5kLI7zAZNh3C",
};

#if defined CONFIG_FOR_JOE

//The following strings must all be equal to the longest one.
const char * cZulu  = "UTC0                  ";  // DO NOT CHANGE THIS ONE!!!
const char * cTopTZ  = "PST8PDT,M3.2.0,M11.1.0";  // Top    city time zone
const char * cBotTZ  = "PHT-8                 ";  // Bottom city time zone
const char * cHomeTZ = "PST8PDT,M3.2.0,M11.1.0";  // Physical local time
//  >>> *** *** *** *** *** *** *** *** *** *** *** *** *** ***  <<<
const char * cTopCityname = "Benicia";               // Top    city name
const char * cBotCityname = "San Clemente, Tarlac";  // Bottom city name

#else  // for Mike

//The following strings must all be equal to the longest one.
const char * cZulu   = "UTC0                  ";  // DO NOT CHANGE THIS ONE!!!
const char * cTopTZ  = "PHT-8                 ";  // Top    city time zone
const char * cBotTZ  = "PST8PDT,M3.2.0,M11.1.0";  // Bottom city time zone
const char * cHomeTZ = "PHT-8                 ";
//  >>> *** *** *** *** *** *** *** *** *** *** *** *** *** ***  <<<
const char * cTopCityname = "Bangui";   // Bottom city name
const char * cBotCityname = "Benicia";  // Top    city name

#endif

int  iTopOffset, iBotOffset, iHomeOffset, iTempOffset;
//char cTopDST[10], cBotDST[10], cHomeDST[10];
struct tm * tm;
tmElements_t tmElements;

//Exchange rate variables
//bool  doneForThisHour = false;  // Rate fetch done for this hour.
bool  pktValidity = false;  // True if good exchange rate fetched
bool  firstXPass = true;  //
unsigned long XRateFetchInterval = 10800;  // 3 hours in seconds
// LONG time ago! Guarantees the time criteria is satisfied for first fetch.
unsigned long lastXRateFetchEpoch = 0;
char   readingTime[50];  // For current exchange rate reading.
float  PHP_Rate = 0.;
// Time, if exceeded, to zero out the old PHP_Rate.
unsigned long int PHP_Rate_Clear_millis = 0;  
// How long to keep the last good reading.
unsigned long int PHP_Rate_Valid_millis = 64800000;  
String serverPath;

char  fullDate[100], fullTime[100], workTime[100];
// For comparing to save some updating/flicker
char  prevTopDate[100], prevBottomDate[100];  
byte  bootDay = 6; // Saturday
int   spriteM = 15;  // Margins, left and right.
int   spriteW = 480 - (spriteM * 2), spriteH = 61;
int   spriteWC = spriteW / 2;

//int   sprite52M = 15;  // Margins, left and right.
//int   sprite52W = 480 - (sprite52M * 2), sprite52H = 52;
//int   sprite52WC = sprite52W / 2;  // Width Center, not Water Closet!
int *spriteHHOK = 0;  // If it stays 0, then there is no spriteHH, use tft drawer.

byte  R, G, B;
int   myTW, myTH;

#define ARRAY_ENTRY_LEN 60
#include "Time_Zones.h"

#define RGB565(r,g,b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))
//BluetoothSerial SerialBT;
//#include "WiFiMulti.h"    // https://github.com/tzapu/WiFiManager
//WiFiMulti wifiMulti;
#include "WiFiManager.h"    // https://github.com/tzapu/WiFiManager
const char* myPortalName = "BLCPortal";

#include <HTTPClient.h>
WiFiManager wifiManager;

#include "MyFonts.h"
#include "LEDs.h"  // Images of faked LEDs for the Binary LED Clock

//Width and Height are quoted in Portrait mode.  I need it in Landscape.
#define dispWidth  480
#define dispHeight 320

uint16_t calibrationData[5];
uint8_t calDataOK = 0;

//For landscape orientations (1 & 3), we have to swap the width and height
const int xCenter = dispWidth  / 2;
const int yCenter = dispHeight / 2;

//SpriteHH is only used for time on the 3-line display to eliminate flicker.
// Half Height (HH) Sprite for 3Line Display no flicker with TTF.
const int sprite_H_HH  = dispHeight / 2;
const int sprite_W_HH  = dispWidth;
const int spCenterW_HH = sprite_W_HH / 2;
const int spCenterH_HH = sprite_H_HH / 2;

Preferences preferences;

//I have to create my own, fake, enum since enum is useless in C++.
int analogHandsShow = 0;
int four_LineShow   = 1;
int three_LineShow  = 2;
int BCD_Show        = 3;
int dtz_Show        = 4;
int TooHigh         = 5;
int whichToShow, prevWhichToShow;
const char displayName[][25] = {"Analog Hands", "Four Line Show", "Three Line Show",
                                "Binary Coded Decimal", "Dual Time Zones"
                               };
time_t t, UTC, workTime_t;
char workDate[100];
char fourLineScreenTime[100];  // Just the time part of the date-time string.
int tempHour, tempSec = 0;
int prev_month = -1, prev_DOM = -1, cornerPrev_DOW = -1, rebootPrev_DOM = -1;
int SS_Pay_Wed_DOM;
int myYear, prev_myYear = -1;
int myMonth, myDay, iYear;
unsigned int iStartMillis;

//Day names (Note trailing space)
char daysOfTheWeekS[7][12] = {"Sun ", "Mon ", "Tue ", "Wed ",
                              "Thu ", "Fri ", "Sat "
                             };
//Day names (Note NO trailing space)
char daysOfTheWeekL[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday",
                              "Thursday", "Friday", "Saturday"
                             };
// To get first Wednesday, if DOW is x, add/subtract y:
// 0 add 3, 1 add 2, 2 add 1, 3 add 0, 4 subtract 1, 5 subtract 2, 6 subtract 3

//Month names (Note trailing space)
String monthsS[12] = {"Jan ", "Feb ", "Mar ", "Apr ", "May ", "Jun ", "Jul ",
                      "Aug ", "Sep ", "Oct ", "Nov ", "Dec "
                     };

//Month names (Note NO trailing space)
String monthsL[12] = {"January", "February", "March", "April",
                      "May", "June", "July", "August", "September",
                      "October", "November", "December"
                     };

const unsigned char month_length[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int currentHour, prevHour = 100, prevDay = 100;

int16_t myX1      = 10000, myY1      = 10000;
int16_t mySX      = 10000, mySY      = 10000;
//Hour hand definitions
int16_t myHX      = 10000, myHY      = 10000;
int16_t prevHX    = 10000, prevHY    = 10000;
int16_t myHXb1    = 10000, myHYb1    = 10000;  // b for back side
int16_t prevHXb1  = 10000, prevHYb1  = 10000;  // b for back side
int16_t myHXb2    = 10000, myHYb2    = 10000;  // b for back side
int16_t prevHXb2  = 10000, prevHYb2  = 10000;  // b for back side
//Minute hand definitions
int16_t myMX      = 10000, myMY      = 10000;
int16_t prevMX    = 10000, prevMY    = 10000;
int16_t myMXb1    = 10000, myMYb1    = 10000;  // b for back side
int16_t prevMXb1  = 10000, prevMYb1  = 10000;  // b for back side
int16_t myMXb2    = 10000, myMYb2    = 10000;  // b for back side
int16_t prevMXb2  = 10000, prevMYb2  = 10000;  // b for back side
unsigned int eraseColor, myColor;

#define aModeDraw 1
#define aModeErase 0

bool myIsPM, initHands = true;
String myBuildString, prev_Line1, prev_Line2, prev_Line3;

// This sets the display rotation.
#if defined T_Gallery
int requestedOrientation = 3;
// Sets portrait rotation with power on the left(T14) or top (T4)
#else
int requestedOrientation = 1;
// Sets portrait rotation with power on the right(T14) or bottom (T4)
#endif

// Setting PWM properties, do not change this!
const int pwmFreq = 5000;
const int pwmResolution = 8;
const int pwmLedChannelTFT = 4;  // Backlight PWM channel number

int      yPos;
int      mySec, myMin, myHr;
int      handLength;
int16_t  cX, cY;  // c for centering (text)
uint16_t cW, cH;
uint16_t myW, myH;

// For BCD Clock show
bool hourDecade[2];
bool hourUnit[4];
bool minuteDecade[3];
bool minuteUnit[4];
bool secondDecade[3];
bool secondUnit[4];
int i, n, segWidth, segHeight, BCDBreak[10];

int touchType;
int myiX, myiY, myeX, myeY, mydX, mydY;
int initialX, initialY;
int endingX = 0, endingY = 0;
int distanceX, distanceY;

#define TOUCH_EVENT_WINDOW  400  // ms.  All clicking has to be done in this time.
#define SLONG_TOUCH_TIME    300  // ms.  Long Touch Time must be less than Double Click time.

#define SWIPE_MIN_X        10  // Pixels
#define SWIPE_MIN_Y        10  // Pixels
// Swipe Directions
//     17 / 10 \ 11
//     16 |  T | 12
//     15 \ 14 / 13
// Where T can be 0, 1, 2 or 3.

#define NO_TOUCH            0
#define SHORT_TOUCH         1
#define DOUBLE_TOUCH        2
#define TRIPLE_TOUCH        3
#define LONG_TOUCH          4

#define SWIPE_OFFSET       10  // Subtract this much to pick from the char array.
#define SWIPE_UP           10
#define SWIPE_UP_RIGHT     11
#define SWIPE_RIGHT        12
#define SWIPE_DOWN_RIGHT   13
#define SWIPE_DOWN         14
#define SWIPE_DOWN_LEFT    15
#define SWIPE_LEFT         16
#define SWIPE_UP_LEFT      17

#define KEY_TEXTSIZE 1   // Font size multiplier
char noPtr[] = " ";

//uint8_t whatFontLoaded;
unsigned long loopWait, last_sec = -1;
unsigned long loopStartMS, lastLoopTime;
long SleepTimer, SleepDuration;

const int CalHighlightCt = 20;  // But, only 8 fit on screen for legend showing.
struct sCalHighlights {
  int  Month;      // if 0, repeat every month.  If 1-12, show only on that month.
  int  DOM;        // Day of Month to highlight. If zero, ignore.
  int  TextColor;  // Color of the number part in RGB565
  int  RectColor;  // Color of the background in RGB565
  char Event[15];  // Not used for now, but at least documentary of event.
};
struct sCalHighlights HLEvents[CalHighlightCt];
#define LED_BUILIN 2

// Touch setups
// Calibration data is stored in SPIFFS so we need to include it
#define CALIBRATION_FILE "/TouchCalData3"
uint16_t tX, tY;  // Touch input variables
// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// Pins I used for this, screen and touch from the left as labeled
//    by the card.
//    01 - VCC   - 3.3v   (Guess)
//    02 - GND   - Ground (One more guess)
//    03 - CS    - D5     (Display chip select)
//    04 - Reset - D22    (Display reset)
//    05 - DC    - D33    (Data/Command switchover)
//    06 - MOSI  - D23(D) (Display data from the ESP board)
//    07 - SCK   - D18(D) (SPI Clock)
//    08 - LED   - D4     (Backlight control PWM pin)
//    09 - MISO  - xx     (This is display MISO which is not used)
//    10 - TCLK  - D18    (Parallel with display SCK)
//    11 - TCS   - D21    (Touch Clip Select aka, TOUCH_CS)
//    12 - TDIN  - D23    (Touch MOSI. Parallel with display MOSI)
//    13 - TD0   - D19    (This is touch MISO)
//
//  Used:
//  5 22 33 23 18 4 2 23 19
//              2      2


/*
  /draw_Clock_Face()   240 Ratio         320
  indiciaNear           98 0.408333333   130.6666667
  indiciaFar           115 0.479166667   153.3333333

  draw_Hands()
  hourHandLength        48  0.2          64
  HourHandBackLength    15  0.0625       20
  minuteHandLength      62  0.258333333  82.66666667
  minuteHandBackLength  19  0.079166667  25.33333333

  draw_Seconds_Dot()
  handLength            86  0.358333333  114.6666667
*/
