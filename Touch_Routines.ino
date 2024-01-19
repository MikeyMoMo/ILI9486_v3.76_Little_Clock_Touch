/* A little chat about buttons' creation and use:

    First, they are very strange.  There are two places to set the button
    text but the one (required) overrides the first one so the first one is
    totally useless and is left blank.  So why is is there?  The first one,
    to be left blank, is in the button definition (.initButton).

    Secondly, the button background and the button text locations are totally
    separate.  Why?  Can't say.  I did not write this fairly horrible code.
    So, you have to set two groups of things.  First, the size (H & W) of
    the button and, then the location of the text that will go on top of the
    button (rounded edged rectangle).  If you get it wrong, the text will
    be located outside of the button rectangle.  It takes quite some time
    to get it all right.  At least, it did for me for the first few hours.
    I was following a horrible example and it lead me down the rabbit hole.

    Then, when you find a button has been pressed, you have to set the .press
    variable and then check it, later, to set the button showing as pressed.
    This is just, plain ludicrous!

    The button label location should be an offset from the button X and Y.
    The button selected state should be ONE operation.  Going through setting
    the .press and, then, later checking it with the .justPressed is just
    stupid!

    Yes, I came from a GOOD visual environment, VB6.  When you asked for a
    button, it did the work for you.  Here, you have to do it all, piecemeal,
    and can easily get it wrong.  AND, there is almost no documentation on
    use of the buttons.  The example code bits are atrocious.  This code is
    a better example than the demos in the TFT_eSPI library!

*/
#define NUM_BUTTONS 4
/***************************************************************************/
void showOptionsScreen()
/***************************************************************************/
{
  int last_sec = -1;

  time(&UTC); workTime_t = UTC + iHomeOffset;
  tm = localtime(&workTime_t);

  TFT_eSPI_Button buttons[NUM_BUTTONS];

  bool pressed;
  String s, tzString;
  int b, otd;

  struct buttonData {
    int X;
    int Y;
    int W;
    int H;
    int outlineColor;
    int fillColor;
    int textColor;
    int textSize;
    int labelDatumX;
    int labelDatumY;
    char buttonLabel[20];
  };
  buttonData myButtons[NUM_BUTTONS];

  // Button definition settings.
  // adding to datumx and datumy moves the text down and right
  // subtracting from datumx and datumy moves the text up and left

  s = "Return"; int returnButton = 0;
  strcpy(myButtons[returnButton].buttonLabel, s.c_str());
  myButtons[returnButton].W = 120;
  myButtons[returnButton].H = 35;
  myButtons[returnButton].X = xCenter - myButtons[returnButton].W / 2;
  myButtons[returnButton].Y = dispHeight - myButtons[returnButton].H - 10;
  myButtons[returnButton].outlineColor = TFT_RED;
  myButtons[returnButton].fillColor = DarkerRed;
  myButtons[returnButton].textColor = TFT_WHITE;
  myButtons[returnButton].labelDatumX = -45;
  myButtons[returnButton].labelDatumY = -6;

  s = "Show Legend"; int showLegendButton = 1;
  strcpy(myButtons[showLegendButton].buttonLabel, s.c_str());
  myButtons[showLegendButton].W = 225;
  myButtons[showLegendButton].H = 70;
  myButtons[showLegendButton].X = 10;
  myButtons[showLegendButton].Y = 30;
  myButtons[showLegendButton].outlineColor = TFT_BLUE;
  myButtons[showLegendButton].fillColor = DarkerBlue;
  myButtons[showLegendButton].textColor = TFT_WHITE;
  myButtons[showLegendButton].labelDatumX = -90;
  myButtons[showLegendButton].labelDatumY = -8;

  s = "Show Payday"; int showPDButton = 2;
  strcpy(myButtons[showPDButton].buttonLabel, s.c_str());
  myButtons[showPDButton].W = 225;
  myButtons[showPDButton].H = 70;
  myButtons[showPDButton].X = 10;
  myButtons[showPDButton].Y = 120;
  myButtons[showPDButton].outlineColor = TFT_BLUE;
  myButtons[showPDButton].fillColor = DarkerBlue;
  myButtons[showPDButton].textColor = TFT_WHITE;
  myButtons[showPDButton].labelDatumX = -88;
  myButtons[showPDButton].labelDatumY = -8;

  s = "Reboot"; int showRebootButton = 3;
  strcpy(myButtons[showRebootButton].buttonLabel, s.c_str());
  myButtons[showRebootButton].W = 225;
  myButtons[showRebootButton].H = 70;
  myButtons[showRebootButton].X = 250;
  myButtons[showRebootButton].Y = 30;
  myButtons[showRebootButton].outlineColor = TFT_BLUE;
  myButtons[showRebootButton].fillColor = DarkerBlue;
  myButtons[showRebootButton].textColor = TFT_WHITE;
  myButtons[showRebootButton].labelDatumX = -90;
  myButtons[showRebootButton].labelDatumY = -8;

  tft.fillScreen(TFT_GREEN);
  tft.unloadFont(); tft.loadFont(BritanicBold32);  // Load a different font

  // Wait for finger off the screen before creating menu buttons.
  while (tft.getTouch(&tX, &tY)) { }

  //X,Y,W,H,Outline, Fill, Text colors, label (useless),
  // text size for ugly fonts.
  for (b = 0; b < NUM_BUTTONS; b++) {
    buttons[b].initButtonUL(&tft, myButtons[b].X, myButtons[b].Y,
                            myButtons[b].W, myButtons[b].H,
                            myButtons[b].outlineColor, myButtons[b].fillColor,
                            myButtons[b].textColor, // Outline, Fill color, Text
                            noPtr, KEY_TEXTSIZE); // Label (9 char max), text size
    // Button text 20 character max (I think).
    buttons[b].setLabelDatum(myButtons[b].labelDatumX,
                             myButtons[b].labelDatumY, TL_DATUM);
    buttons[b].drawButton(false, myButtons[b].buttonLabel);
  }
  unsigned long endMillis = millis() + 60000;

  // Start the wait loop.
  //  Serial.println("Starting button wait loop.");
  //  Serial.printf("t %i, last_sec %i\r\n", t, last_sec);

  while (1) {  // Wait for an event
    time(&UTC); workTime_t = UTC + iHomeOffset;
    tm = localtime(&workTime_t);
    t = tm->tm_sec;
    if (t != last_sec) {  // Second changed.  Need action.
      last_sec = t;
      otd = tft.getTextDatum();
      tft.setTextDatum(TL_DATUM);
      tft.setTextColor(TFT_BLACK, TFT_GREEN);
      tft.setTextPadding(tft.width());
      tft.drawString(ctime(&workTime_t), 18, 240);
      tft.drawString("Big Little Clock version " + String(Vers), 18, 200);
      tft.setTextPadding(0);
      tft.setTextDatum(otd);
    }

    if (millis() > endMillis ) return;  // Get out after a minute idle

    pressed = tft.getTouch(&tX, &tY);
    if (pressed) endMillis = millis() + 60000;  // Restart the minute idle wait to exit.

    // ----> Exit Button
    if (pressed && buttons[returnButton].contains(tX, tY))
    {
      buttons[returnButton].drawButton(true, myButtons[returnButton].buttonLabel);
      while (tft.getTouch(&tX, &tY)) delay(250);  // Wait for untouch.
      break;  // Return button selected.  Exit the while loop (returns to the clock face selected).
    }

    // ----> Show Show Legend Button
    if (pressed && buttons[showLegendButton].contains(tX, tY))
    {
      buttons[showLegendButton].press(true);  // tell the button it is pressed
      buttons[showLegendButton].drawButton(true, myButtons[showLegendButton].buttonLabel);
      while (tft.getTouch(&tX, &tY)) // Wait for untouch.
        delay(250);
      showLegend();  // Return button selected.
      while (tft.getTouch(&tX, &tY)) delay(250);  // Wait for untouch.
      break;  // Return button selected.
    }

    // ----> Show Next Payday Button
    if (pressed && buttons[showPDButton].contains(tX, tY))
    {
      buttons[showPDButton].press(true);  // tell the button it is pressed
      buttons[showPDButton].drawButton(true, myButtons[showPDButton].buttonLabel);
      while (tft.getTouch(&tX, &tY))   // Wait for untouch.
        delay(250);
      PayDaySearch();  // Show Payday button selected.
      while (tft.getTouch(&tX, &tY)) delay(250);  // Wait for untouch.
      break;
    }

    // ----> Show Reboot Button
    if (pressed && buttons[showRebootButton].contains(tX, tY))
    {
      buttons[showRebootButton].press(true);  // tell the button it is pressed
      buttons[showRebootButton].drawButton(true, myButtons[showRebootButton].buttonLabel);
      while (tft.getTouch(&tX, &tY)) // Wait for untouch.
        delay(250);
      ESP.restart();  // Return button selected.
      //      while (tft.getTouch(&tX, &tY)) delay(250);  // Wait for untouch.
      break;  // Return button selected.
    }
  }
}

/***************************************************************************/
void touch_calibrate()
/***************************************************************************/
{
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }
  // For the ILI9486 with Touch, select Setup6A_ESP_SPI_ILI948
  tft.unloadFont(); tft.loadFont(BritanicBold32);  // Load a different font
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.fillScreen(TFT_BLACK);

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    tft.drawString("Touch corners as indicated", xCenter, yCenter - 80);
    // data not valid so recalibrate
    Serial.println("Calibrating the display.  Set REPEAT_CAL to true to repeat.");
    if (REPEAT_CAL) {
      tft.drawString("Set REPEAT_CAL to false", xCenter, yCenter);
      tft.drawString("to stop this running again", xCenter, yCenter + 40);
      //      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

// 5 returns from here.  First, is the type of touch:
//  0:No Touch, 1:Short, 2:Double 3:Triple 4:Long
// Then the starting and ending values for X and Y and type of swipe.
//     17 / 10 \ 11  17:Up-Left,   10:Up,    11:Up-Right,
//     16 |  T | 12  16:Left,       T:Touch, 12:Right
//     15 \ 14 / 13  15:Down-Left, 14:Down,  13:Down-Right
/***************************************************************************/
int myGetTouch (int *iX, int *iY, int *eX, int *eY, int *dX, int *dY)
/***myGetTouch**************************************************************/
{
  /* Notes:
      - If there are multiple clicks (1, 2 or 3) only the position of the
         last one will be reported.)
      - If there is a single click that is not a swipe and a later click
         (within TOUCH_EVENT_WINDOW) only the swipe event will be reported.
  */
  static uint16_t x, y, Swipe_Type_Return;
  static int touchCt;
  static unsigned long touchStart;
  static unsigned long eventStart;
  static int eventTime;
  static bool long_touch_by_time;

  //  Serial.println("Entering myGetTouch.");

  // Initialize the static variables that need it.
  touchStart = 0;
  eventStart = 0;
  touchCt = NO_TOUCH;
  long_touch_by_time = false;

  do
  {
    //  Serial.printf("%lu : Top of loop - eS %lu, tS %lu\r\n", millis(),
    //                eventStart, touchStart);
    if (tft.getTouch(&x, &y)) {  // Someone touched the screen?
      if (eventStart == 0) eventStart = millis();
      //  Serial.printf("%lu : Setting eventStart - eventStart %lu, touchStart %lu\r\n",
      //                millis(), eventStart, touchStart);
      if (touchStart == 0) {
        touchStart = millis();  // Start the current touch timer
        //  Serial.printf("%lu ; Setting touchStart - eventStart %lu, touchStart %lu\r\n",
        //                millis(), eventStart, touchStart);
        initialX = x; initialY = y;
        //        Serial.printf("%lu : Touched - eS %lu, tS %lu\r\n", millis(),
        //                      eventStart, touchStart);
      }
      eventTime = millis() - touchStart;
      if (eventTime >= SLONG_TOUCH_TIME) {
        //  Serial.println("Touch timer found a long touch.");
        *iX = initialX; *iY = initialY; *eX = 0; *eX = 0; *dX = 0; *dY = 0;
        //  Serial.printf("%lu : long touch = true - eventStart %lu, touchStart %lu\r\n",
        //                millis(), eventStart, touchStart);
        long_touch_by_time = true;
        //  Serial.printf("%lu : long touch = true - eventStart %lu, touchStart %lu\r\n",
        //                millis(), eventStart, touchStart);
#if defined LONG_PRESS_DELAYED_RETURN
        while (tft.getTouch(&x, &y));  // Wait for unpress
#else
        tft.getTouch(&x, &y);
#endif
        //  Serial.printf("y %i, endingY %i\r\n", y, endingY);
        endingX = x; endingY = y;
        //  Serial.println("Now, determine if long touch or swipe");
        distanceX = initialX - endingX; distanceY = initialY - y;
        if ((abs(distanceX) < SWIPE_MIN_X) && (abs(distanceY) < SWIPE_MIN_Y)) {
          //  Serial.println("Touch distance below the minimum for swipe. Returning long touch");
          return LONG_TOUCH;
        } else {
          //  Serial.println("Swipe distance achieved.");
          eventStart = millis();  // Extend the timer so we get to the "else" code.
        }
      }
    } else {  // The touch is ended.  If the time is less than SLONG_TOUCH_TIME, increment click count
      // First, determine if there was movement above the minimum for a swipe.
      // Note that swipe must be first, then long click, then single click(s) checked for, in order.
      //  Serial.printf("%lu : Touch ended - eS %lu, tS %lu\r\n", millis(), eventStart, touchStart);
      if (eventStart > 0 && touchStart > 0) {  // If we were in an event, show finish.
        endingX = x; endingY = y;
        distanceX = endingX - initialX;
        distanceY = endingY - initialY;
        eventTime = millis() - touchStart;
        //  Serial.printf("%lu - Event time %i, distX %i, distY %i\r\n",
        //                millis(), eventTime, distanceX, distanceY);
        // Does this event qualify as a Swipe?
        if ((abs(distanceX) > SWIPE_MIN_X) || (abs(distanceY) > SWIPE_MIN_Y))
        {
          //  Serial.println("Sub found a swipe.");
          *iX = initialX; *iY = initialY; *eX = endingX; *eX = endingY;
          *dX = distanceX; *dY = distanceY;
          if (abs(distanceX)      < SWIPE_MIN_X  && distanceY      < -SWIPE_MIN_Y) Swipe_Type_Return = SWIPE_UP;
          else if (distanceX      > SWIPE_MIN_X  && distanceY      < -SWIPE_MIN_Y) Swipe_Type_Return =  SWIPE_UP_RIGHT;
          else if (distanceX      > SWIPE_MIN_X  && abs(distanceY) < SWIPE_MIN_Y)  Swipe_Type_Return =  SWIPE_RIGHT;
          else if (distanceX      > SWIPE_MIN_X  && distanceY      > SWIPE_MIN_Y)  Swipe_Type_Return =  SWIPE_DOWN_RIGHT;
          else if (abs(distanceX) < SWIPE_MIN_X  && distanceY      > SWIPE_MIN_Y)  Swipe_Type_Return =  SWIPE_DOWN;
          else if (distanceX      < -SWIPE_MIN_X && distanceY      > SWIPE_MIN_Y)  Swipe_Type_Return =  SWIPE_DOWN_LEFT;
          else if (distanceX      < -SWIPE_MIN_X && abs(distanceY) < SWIPE_MIN_Y)  Swipe_Type_Return =  SWIPE_LEFT;
          else if (distanceX      < -SWIPE_MIN_X && distanceY      < SWIPE_MIN_Y)  Swipe_Type_Return =  SWIPE_UP_LEFT;
#if defined SWIPE_DELAYED_RETURN
          while (tft.getTouch(&x, &y));  // Wait for unpress
#endif
          return Swipe_Type_Return;
        }
        else if (eventTime < SLONG_TOUCH_TIME) {
          //  Serial.println("Sub found a single click");
          touchCt++; *iX = initialX; *iY = initialY;
          *eX = 0; *eX = 0; *dX = 0; *dY = 0;
          if (touchCt == TRIPLE_TOUCH) return TRIPLE_TOUCH;  // End this now.
          touchStart = 0;
        }
      }
    }
  } while (millis() < (eventStart + TOUCH_EVENT_WINDOW));
  //  Serial.printf("Return from myGetTouch %lu\r\n", millis());
  return touchCt;  // All that is left is DOUBLE_TOUCH
}
/***************************************************************************/
void PayDaySearch()
/***************************************************************************/
{
  ofr.unloadFont();
  if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
    Serial.println("Render loadFont error for BritanicBoldTTF. PayDay 1");
    while (1);
  }
  WhenIsPayDay(tm->tm_mon + 1, tm->tm_mday,
               tm->tm_year + 1900);
  unsigned int startMS = millis();
  Serial.println("Showing PayDay info for 8 seconds.");
  // Wait for touch, then return.
  while (millis() < startMS + 8000) if (tft.getTouch(&tX, &tY)) break;
}
/***************************************************************************/
void WhenIsPayDay(int m, int d, int y)
/***************************************************************************/
{
  int testDay;

  ofr.setDrawer(tft); // Link renderer to base display.

  String sLine1 = "", sLine2 = "", sLine3 = "", sLine4 = "";
  bool gotHit = false;  // True if day found, else leave false for recurse.
  yPos = -5;  // Baseline anchor position and Line 1 position.
  int line1 = yPos, line2 = yPos + 80;
  int line3 = yPos + 160, line4 = yPos + 240;
  // Serial.printf("Entering WhenIsPayDay with m = %i, d = %i, y = %i\r\n",
  //                m, d, y);
  // Start on the 1st and find the first Wednesday.  Can't be past the 15th.
  if (myBirthdate == 0)
    SS_Pay_Wed_DOM = 0;
  else
    SS_Pay_Wed_DOM = Find_SS_Wed(m, y);

  tft.fillScreen(DarkerRed);  // Background good color for cleaning.

  ofr.setFontColor(TFT_YELLOW, DarkerRed);
  ofr.setFontSize(80);

  if (SS_Pay_Wed_DOM == 0) {
    sLine1 = "Birthday";
    sLine2 = "not defined";
    gotHit = true;
  } else {
    sLine1 = "Next payday";
    sLine2 = "is " + monthsS[m - 1] + String(SS_Pay_Wed_DOM);
    sLine2 += th(SS_Pay_Wed_DOM);
    sLine2 += ",";

    if (d == SS_Pay_Wed_DOM) {  //Is this a direct hit on payday?
      gotHit = true;
      sLine3 = "today!";
    }
    else if (d < SS_Pay_Wed_DOM) {  // If we are before the 2nd Wednesday.
      int nowJulian = calcJulian(tm->tm_year + 1900,
                                 tm->tm_mon + 1, tm->tm_mday);
      int pdJulian = calcJulian(y, m, SS_Pay_Wed_DOM);

      //      Serial.printf("Julians %i, %i, %i\r\n",
      //                    pdJulian, nowJulian, pdJulian - nowJulian);
      //      Serial.printf("nowJ %04i/%02i/%02i\r\n",
      //                    tm->tm_year + 1900, tm->tm_mon + 1,
      //                    tm->tm_mday);
      //      Serial.printf("pdJ  %04i/%02i/%02i\r\n",
      //                    y, m, SS_Pay_Wed_DOM);

      if ((pdJulian - nowJulian) > 1) {  // If more than 1 day till payday
        gotHit = true;
        switch (pdJulian - nowJulian)
        {
          case 28: sLine3 = "4 weeks"; break;
          case 21: sLine3 = "3 weeks"; break;
          case 14: sLine3 = "2 weeks"; break;
          case  7: sLine3 = "1 week";  break;
          default: sLine3 = String(pdJulian - nowJulian) + " days";
        }
        sLine4 = "from now.";
      } else {  // Not a direct hit so all that is left is "tomorrow".
        gotHit = true;
        sLine3 = "tomorrow";
      }
    }
  }
  if (gotHit) {  // If gotHit, update the screen, else recurse on next month.

    //    tft.drawString(sLine1, xCenter, line1);
    ofr.setCursor(xCenter, line1); ofr.cprintf(sLine1.c_str());
    //    tft.drawString(sLine2, xCenter, line2);
    ofr.setCursor(xCenter, line2); ofr.cprintf(sLine2.c_str());
    //    tft.drawString(sLine3, xCenter, line3);
    ofr.setCursor(xCenter, line3); ofr.cprintf(sLine3.c_str());
    //    tft.drawString(sLine4, xCenter, line4);
    ofr.setCursor(xCenter, line4); ofr.cprintf(sLine4.c_str());

  } else {
    //We are past the day, go to next month and look for it.
    d = 1; m++;  // Go to next month and find the 2nd Wednesday
    if (m > 12) {
      m = 1; y++;  // If we were in December and move to January, also increment year.
    }
    // We are past payday, check next month (maybe next year, too).
    WhenIsPayDay(m, d, y);
  }
}
/***************************************************************************/
void ShowCalendar(int showMonth, int showYear)
/****ShowCalendar***********************************************************/
{
  int testDay, RectColor, TextColor, showMonthLength, temp;

  Serial.printf("ShowCalendar called with month %i, year %i\r\n",
                showMonth, showYear);
  int x, y, calDate, calDOW;

  int horizontalOffset = 37;
  int weekBaseline = 49;
  tft.fillScreen(DarkerRed);  // Background good color for cleaning.

  // Draw the grid lines for the calendar dates.
  // Horizontal first, then vertical.
  y = 0;
  tft.drawFastHLine(5, 0, dispWidth - 11, ILI9341_YELLOW);

  for (x = 0; x < 6; x++) {  // Draw 6 more horizonatl lines
    y += 53;
    tft.drawFastHLine(5, y, dispWidth - 11, ILI9341_YELLOW);
  }

  // Draw first one on the left full length.
  // The 4 in this statement causes the x and x+= to be 4 off for equal boxes.
  tft.drawFastVLine(4, 0, dispHeight - 2, ILI9341_YELLOW);
  x = 71;
  for (y = 0; y < 6; y++) {
    if (y > 1)
      // 4 will be shorter then the rest to open up the space
      //  for the month and year.
      tft.drawFastVLine(x, 0, dispHeight - 54, ILI9341_YELLOW);
    else
      tft.drawFastVLine(x, 0, dispHeight - 2, ILI9341_YELLOW);
    x += 67;
  }
  // Draw last one on right full length. Well... almost.
  tft.drawFastVLine(x, 0, dispHeight - 2, ILI9341_YELLOW);

  // Foreground color, Background color
  //  tft.setTextColor(ILI9341_YELLOW, DarkerRed);

  calDOW = WhatDOWis(showMonth, 1, showYear);

  ofr.setDrawer(tft); // Link renderer to base display.

  ofr.unloadFont();
  if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
    Serial.println("Render loadFont error for BritanicBoldTTF. Calendar 1");
    while (1);
  }
  ofr.setFontSize(50);
  ofr.setFontColor(ILI9341_YELLOW, DarkerRed);  // Foreground color, Background color

  if (showMonth == 2) {  // Check if February and Leap Year. If so, 29. Else 28 days.
    if (isLeapYear(showYear)) {
      showMonthLength = 29;
    } else {
      showMonthLength = 28;
    }
  } else {
    showMonthLength = month_length[showMonth - 1]; // Get standard days of this particular month.
  }
  Serial.printf("First day of %s is a %s\r\n",  // -1 to adjust to the array
                monthsL[showMonth - 1], daysOfTheWeekL[calDOW]);
  Serial.printf("%s, %i has %i days.\r\n",  // -1 to adjust to the array
                monthsL[showMonth - 1], showYear, showMonthLength);

  //  tft.setTextDatum(BC_DATUM);  // Oh, yeah?!?!?!

  //Now, put in the day numbers.  Reverse video for today.
  for (calDate = 1; calDate <= showMonthLength; calDate++) {
    if (HighlightToday(showMonth, calDate, showYear, RectColor, TextColor)) {
      tft.fillRect((calDOW * 67) + 5, weekBaseline - 48,
                   66, 52, RectColor);  // Fill today's rectangle with color from struct
      ofr.setFontColor(TextColor, RectColor);  // Foreground color, Background color
    } else {  // Else normal colors for all the rest.
      ofr.setFontColor(ILI9341_YELLOW, DarkerRed);  // Foreground color, Background color
    }
    //    tft.drawString(String(calDate), horizontalOffset + calDOW * 67, weekBaseline);
    ofr.setCursor(horizontalOffset + calDOW * 67, weekBaseline - 48);
    //itoa(calDate, workDate, 100);
    sprintf(workDate, "%i", calDate);
    //    Serial.printf("Putting day before %i after ", calDate); Serial.println(workDate);
    ofr.cprintf((workDate));
    calDOW++;
    if (calDOW > 6) {
      calDOW = 0;
      weekBaseline += 53;
    }
  }
  // On the last day of the month, the reverse video might have been left set.
  //  Fix it back for the month name & year to look normal.
  ofr.setFontColor(ILI9341_YELLOW, DarkerRed);  // Foreground color, Background color

  ofr.setFontSize(44);

  ofr.setCursor(dispWidth - 14, dispHeight - 52);
  sprintf(workDate, "%s %i", monthStr(showMonth), showYear);
  //  Serial.print("Month name "); Serial.println(workDate);
  ofr.rprintf(workDate);

  //  Serial.println("Waiting up to 25 secs to exit calendar on time or press.");
  unsigned int startMS = millis();

  // Let's wait 25 seconds, or a short touch to exit or a swipe left or right to change month.
  Serial.println("Showing Calendar for up to 25 seconds, depending on touch.");
  while (millis() < startMS + 25000) {
    // Ignoring all position info.
    touchType = myGetTouch(&temp, &temp, &temp, &temp, &temp, &temp);
    if (touchType != 0) {
      if (touchType == SHORT_TOUCH) return;  // Quick exit on a short touch.

      else if ((touchType == SWIPE_LEFT) || (touchType == SWIPE_DOWN_LEFT) ||
               (touchType == SWIPE_UP_LEFT)) {
        Serial.printf("Swipe left. Advancing up calendar from %i / %i\r\n",
                      showMonth, showYear);
        showMonth++;
        if (showMonth > 12) {
          showMonth = 1; showYear++;
        }
        Serial.printf("Recursing to ShowCalendar with %i / %i\r\n",
                      showMonth, showYear);
        ShowCalendar(showMonth, showYear);
        return;
      }

      else if  ((touchType == SWIPE_RIGHT) || (touchType == SWIPE_DOWN_RIGHT) ||
                (touchType == SWIPE_UP_RIGHT)) {
        Serial.printf("Swipe right. Backing up calendar from %i / %i\r\n",
                      showMonth, showYear);
        showMonth--;
        Serial.printf("Swipe left. New month %i / %i\r\n", showMonth, showYear);
        if (showMonth < 1) {
          showMonth = 12; showYear--;
        }
        startMS = millis();  // Short circuit this one so, on the way back from recurse,
        //                       immediate exit!
        Serial.printf("Recursing to ShowCalendar with %i / %i\r\n",
                      showMonth, showYear);
        ShowCalendar(showMonth, showYear);
        //        Serial.println("Returned from this level");
        return;
      }
    }
  }
}

/***************************************************************************/
void showLegend()
/***************************************************************************/
{
  int event, vOffset = 0;

  bool foundSome = false;
  Serial.println("Show legend requested");
  tft.unloadFont(); tft.loadFont(BritanicBold32);  // Load a different font
  tft.fillScreen(0);  // was DarkerRed Blackdxf3 good color for cleaning.
  for (event = 0; event < CalHighlightCt; event++) {
    // Only show events that are showing on the calender, i.e. on or after today
    //  and in this month or all months.
    if ((HLEvents[event].DOM >= tm->tm_mday && HLEvents[event].DOM < 100)
        && (HLEvents[event].Month == tm->tm_mon + 1 || HLEvents[event].Month == 0)) {
      tft.setTextColor(ILI9341_YELLOW, DarkerRed);  // Foreground color, Background color
      if (HLEvents[event].DOM < 10) {
        tft.setTextDatum(TL_DATUM);  // This makes no sense to me.
        tft.drawString(" " + String(HLEvents[event].DOM), 10, vOffset);
      } else {
        tft.setTextDatum(TL_DATUM);
        tft.drawString(String(HLEvents[event].DOM), 0, vOffset);
      }
      tft.setTextColor(HLEvents[event].TextColor, HLEvents[event].RectColor);
      //                        Foreground color, Background color
      tft.setTextDatum(TL_DATUM);
      tft.drawString(HLEvents[event].Event, 40, vOffset);
      foundSome = true;
      vOffset += 30;
    }
  }
  if (!foundSome) {
    // This stuff makes no sense to me. (A few years later, now it does.)
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(ILI9341_YELLOW, DarkerRed);  // Foreground color, Background color
    tft.drawString("No future events found.", 0, 0);
  }
  unsigned int startMS = millis();
  Serial.println("Showing legend for 15 seconds");
  while (millis() < startMS + 15000) {
    if (tft.getTouch(&tX, &tY)) break;  // Wait for touch.
  }
}
