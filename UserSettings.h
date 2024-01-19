#define CONFIG_FOR_MIKE
//#define CONFIG_FOR_JOE
//#define LOCK_TO_DTZ

// Cannot use OTA.  Set partition scheme to Huge Partition (3MB)

//Social Security pays on different dates depending on when you were born.
#if defined CONFIG_FOR_MIKE
int myBirthdate = 2;
#elif defined CONFIG_FOR_JOE
int myBirthdate = 20;
#else
int myBirthdate = 0;  // Not on Social Security now.
#endif

// If uncommented, Long Press won't return until finger lifted.
//#define LONG_PRESS_DELAYED_RETURN
// If uncommented, don't return from swipe until finger lifted.
#define SWIPE_DELAYED_RETURN
const boolean showAMPM = true;  // If you like AM and PM. False gives 24 hour clock.

#if defined CONFIG_FOR_JOE
//                          0    1    2    3    4    5       Hours
int hourlyBrilliance[] = { 80,  80,  80,  80,  80,  80,   //  0- 5
                           //6   7    8    9   10   11
                           80,  80,  80,  80,  80,  80,   //  6-11
                           //12  13   14   15   16   17
                           80,  80,  80,  80,  80,  80,  // 12-17
                           //18  19   20   21   22   23
                           80,  80,  80,  80,  80,  80   // 18-23
                         };
#else
//                         0    1    2    3    4    5       Hours
int hourlyBrilliance[] = {60,  50,  30,  30,  20,  20,   //  0- 5
                          //6   7    8    9   10   11
                          30,  50,  70,  80,  80, 100,   //  6-11
                          //12  13   14   15   16   17
                          160, 160, 160, 160, 160, 160,  // 12-17
                          //18  19   20   21   22   23
                          160,  160,  160,  160,  120, 80   // 18-23
                         };
#endif

//Text string alignment and colors now.
//Normally strings are printed relative to the top left corner but this can be
//changed with the setTextDatum() function. The library has #defines for:
//TL_DATUM 0 // Top left (default)
//TC_DATUM 1 // Upper center
//TR_DATUM 2 // Top right
//ML_DATUM 3 // Left midfielder
//CL_DATUM 3 // Center left, as above
//MC_DATUM 4 // Central center
//CC_DATUM 4 // Center to center, as above
//MR_DATUM 5 // Right midfielder
//CR_DATUM 5 // Center right, as above
//BL_DATUM 6 // Bottom left
//BC_DATUM 7 // Bottom center
//BR_DATUM 8 // Bottom right
//L_BASELINE 9 // Base line of the left character (line on which the character ‘A’ would sit)
//C_BASELINE 10 // Base line of the central character
//R_BASELINE 11 // Base line of the right character

// Standard colors also available without #define needed.
//TFT_BLACK
//TFT_NAVY (#0000800)
//TFT_DARKGREEN (#008000)
//TFT_DARKCYAN (#008080)
//TFT_MAROON (#80000)
//TFT_PURPLE #800080)
//TFT_OLIVE (#808000)
//TFT_LIGHTGREY (#D3D3D3)
//TFT_DARKGREY (#808080)
//TFT_BLUE (#0000FF)
//TFT_GREEN (#00FF00)
//TFT_CYAN (#00FFFF)
//TFT_RED (#FF0000)
//TFT_MAGENTA (#FF00FF)
//TFT_YELLOW (#FFFF00)
//TFT_WHITE (#FFFFFF)
//TFT_ORANGE (#FFB400)
//TFT_GREENYELLOW (#B4FF00)
//TFT_PINK (#FFC0CB)
//TFT_BROWN (#964B00)
//TFT_GOLD (#FFD700)
//TFT_SILVER (#C0C0C0)
//TFT_DarkerRed (#87CEEB)
//TFT_VIOLET (#B42EE2)
//#define Black RGB565(0,0,0)
//#define Red  RGB565(255,0,0)
//#define Green RGB565(0,255,0)
//#define Blue RGB565(0,0,255)
//#define Gray RGB565(128,128,128)
#define LighterRed RGB565(255,150,150)
//#define LighterGreen RGB565(150,255,150)
//#define LighterBlue RGB565(150,150,255)
#define DarkerRed RGB565(150,0,0)
#define DarkerGreen RGB565(0,100,0)
#define DarkerBlue RGB565(0,0,100)
//#define Cyan RGB565(0,255,255)
//#define Magenta RGB565(255,0,255)
//#define Yellow RGB565(255,255,0)
//#define ReddishYellow RGB565(255,192,0)
//#define White RGB565(255,255,255)
//#define DarkYellow RGB565(128,128,0)
#define SkyBlue RGB565(0x00, 0xe8, 0xff)

#define ILI9341_BLACK       0x0000      /*   0,   0,   0 */
#define ILI9341_NAVY        0x000F      /*   0,   0, 128 */
#define ILI9341_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define ILI9341_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define ILI9341_MAROON      0x7800      /* 128,   0,   0 */
#define ILI9341_PURPLE      0x780F      /* 128,   0, 128 */
#define ILI9341_OLIVE       0x7BE0      /* 128, 128,   0 */
#define ILI9341_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define ILI9341_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define ILI9341_BLUE        0x001F      /*   0,   0, 255 */
#define ILI9341_GREEN       0x07E0      /*   0, 255,   0 */
#define ILI9341_CYAN        0x07FF      /*   0, 255, 255 */
#define ILI9341_RED         0xF800      /* 255,   0,   0 */
#define ILI9341_MAGENTA     0xF81F      /* 255,   0, 255 */
#define ILI9341_YELLOW      0xFFE0      /* 255, 255,   0 */
#define ILI9341_WHITE       0xFFFF      /* 255, 255, 255 */
#define ILI9341_ORANGE      0xFD20      /* 255, 165,   0 */
#define ILI9341_GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define ILI9341_PINK        0xF81F
#define Red  RGB565(255,0,0)
#define White RGB565(255,255,255)

//And, now, for the colors I use.
#define HHLighter ReddishYellow
#define HHDarker  RGB565(0,255,224)
#define MHLighter ILI9341_YELLOW
#define MHDarker  ILI9341_YELLOW
#define DarkYellow   RGB565(128, 128,   0)
#define DarkerYellow RGB565( 80,  80,   0)
//#define DarkerRed    RGB565(  0,   0, 125)
//#define Yellow       RGB565(  0, 255, 255)
//#define HHDarker     RGB565(224, 255,   0)
//#define MHLighter    Yellow
//#define MHDarker     Yellow

//SSH Colors
#define sshNC          "\033[0m"     // No Color
#define sshRed         "\033[0;31m"  // Red
#define sshRedL        "\033[1;31m"  // Light Red
#define sshBlack       "\033[0;30m"  // Black    
#define sshGrayD       "\033[1;30m"  // Dark Gray
#define sshGreen       "\033[0;32m"  // Green
#define sshGreenL      "\033[1;32m"  // Light Green
#define sshBrownOrange "\033[0;33m"  // Brown/Orange
#define sshYellow      "\033[1;33m"  // Yellow
#define sshBlue        "\033[0;34m"  // Blue
#define sshBlueL       "\033[1;34m"  // Light Blue
#define sshPurple      "\033[0;35m"  // Purple
#define sshPurpleL     "\033[1;35m"  // Light Purple
#define sshCyan        "\033[0;36m"  // Cyan
#define sshCyanL       "\033[1;36m"  // Light Cyan
#define sshGrayL       "\033[0;37m"  // Light Gray
#define sshWhite       "\033[1;37m"  // White
//SSH Background Colors
#define sshBC_Black            "\033[40m"
#define sshBC_Red              "\033[41m"
#define sshBC_Green            "\033[42m"
#define sshBC_Orange           "\033[43m"
#define sshBC_Blue             "\033[44m"
#define sshBC_Magenta          "\033[45m"
#define sshBC_Cyan             "\033[46m"
#define sshBC_White            "\033[47m"
#define sshBC_GrayD            "\033[100m"
#define sshBC_RedL             "\033[101m"
#define sshBC_GreenL           "\033[102m"
#define sshBC_Yellow           "\033[103m"
#define sshBC_BlueL            "\033[104m"
#define sshBC_PurpleL          "\033[105m"
#define sshBC_Teal             "\033[106m"

//const float Vers = 2.04;  // Using font address pointers instead of fake number.
//const float Vers = 2.05;  // Add in time zone offsets.  All 37 of them!
//const float Vers = 2.06;  // Can now change time zone by double click lower button
//const float Vers = 2.07;  // DST and various cleanups
//const float Vers = 2.08;  // Various cleanups
//const float Vers = 2.09;  // Various setting saved in SPIFFS or LittleFS
//const float Vers = 3.0;   // T14 version
//const float Vers = 3.01;  // Added calendar and Payday routines.
//const float Vers = 3.011; // Minor fixups, mostly to Calendar and Payday and exiting them.
//const float Vers = 3.02;  // Added the first of (possibly) many special dates in colors
// This needs to be expanded to an array of structures
//  with date & color in each one, then a routine to
//  comb that array for each DOM (calDate) to put the
//  correct color on the correct day.
// Fixed the reverse color in calendar to not overwrite lines
//const float Vers = 3.21; // Finished up Calendar Highlighting
// DOM == 0 in an entry inactivates it.
//const float Vers = 3.03;   // Updated to T4 v1.3, changed dimming scheme for sleep
//const float Vers = 3.041;  // Changed to WiFiMulti. Adding Alarm capability.
//const float Vers = 3.05;   // Changed to 4 inch display with touch. No legacy support.
//const float Vers = 3.06;   // Canvas is gone.  Sprite is in but only used one place.
//const float Vers = 3.061;  // Removed all of the 7b fonts that canvas required.
//const float Vers = 3.07;   // Moved to TimeLib time keeping
//const float Vers = 3.075;  // Fixes to TimeLib time keeping
//const float Vers = 3.076;  // Slight changes in presentation (hour flash, et. al.)
//const float Vers = 3.077;  // Just a safety for minor logic edits
//const float Vers = 3.078;  // Just a safety for minor logic edits
//const float Vers = 3.079;  // Just a safety for minor logic edits
//const float Vers = 3.080;  // Adding delayed swipe return
//const float Vers = 3.081;  // Oops, did not init all static variables in myGetTouch
//const float Vers = 3.082;  // Double Oops.  The Return button fell off the screen!
//                             Also fix the "th" in PayDaySearch.  I am an idiot, frequently!
//const float Vers = 3.09;   // Add in BCD Clock show
//const float Vers = 3.10;  // Add in Dual Time Zones
//const float Vers = 3.11;  // Fixups for exTime conversion
//const float Vers = 3.12;  // Added DTZ screen lock for the touch challenged displays
//const float Vers = 3.13;  // I forgot
//const float Vers = 3.14;  // Modify Dual Display to show DOW and no year per user request.
//const float Vers = 3.15;  // showCalendar, not changing month on swipe. Fixed!
//const float Vers = 3.16;  // Change 3Line background to DarkerRed, foreground to Yellow
//                             Add "th" to 3Line display line 3.
//const float Vers = 3.17;  // Add PHP/USD to the DTZ clock screen.
//const float Vers = 3.18;  // Add screen message during XRate fetch.
//const float Vers = 3.19;  // Change \n to \r\n to make Putty happy.
//const float Vers = 3.20;  // Change in the way X Rate fetch is timed.

//The following is not fully implemented:
//const float Vers = 3.21;  // Trying to verify that the obtained time is probably correct.

//const float Vers = 3.22;  // Changed to WiFiManager
//const float Vers = 3.23;  // Begin using apiKey fallback
//const float Vers = 3.24;  // Fixups for apiKey fallback
//const float Vers = 3.30;  // Trying to make TTF work for all tft screens and sprites
//const float Vers = 3.40;  // Continuing to make TTF work for all screens and sprites
//const float Vers = 3.41;  // Add conditional use of spriteHH if PSRAM is usable.
//const float Vers = 3.60;  // Lots of updates to get it current except for non-sprite wedge hands.
//const float Vers = 3.62;  // Put in the long timeout for XRate display.  18 hours.
//const float Vers = 3.70;  // Change back to NTP.  ezTime is GONE, dude!!!  HOORAY!!!!!
// Also trying out Hotspot to Windows 11.
const float Vers = 3.76;  // Fixup for weekly boot without costing extra xRate calls.
