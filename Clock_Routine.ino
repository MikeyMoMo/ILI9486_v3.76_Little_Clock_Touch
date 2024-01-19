//void drawBubbles() {
//  int loopControl, rndX, rndY;
//  for (loopControl = 0; loopControl < 2000; loopControl++) {
//    rndX = random(tft.width());
//    rndY = random(tft.height());
//    tft.fillCircle(rndX, rndY, random(40) + 4, GetRndRGB565());
//    myWait(1);  // Just slow it down slightly for each circle.
//  }
//}
/***************************************************************************/
uint32_t GetRndRGB565()
/***************************************************************************/
{
  R = random(200) + 25;
  G = random(200) + 25;
  B = random(200) + 25;
  return (RGB565(R, G, B));
}
/***************************************************************************/
void myWait(unsigned long waitMillis)
/***************************************************************************/
{
  unsigned long startMillis = millis();
  while (millis() < startMillis + waitMillis) {
    //wait approx. [period] ms
  }
  return;
}
/***************************************************************************/
void draw_Clock_Face()
/****draw_Clock_Face********************************************************/
{
  int indiciaNear = dispHeight * 0.43;  // 0.408333333;  // By ratio
  int indiciaFar = dispHeight * 0.48;   // By ratio
  int nearX, nearY, farX, farY;
  int nearX1, nearY1, farX1, farY1;
  int nearX2, nearY2, farX2, farY2;

  // draw hour pointers around the face of a clock
  for (int i = 0; i < 12; i++) {
    nearX  = (indiciaNear * sin(PI - (2 * PI) / 12 * i)) + xCenter;
    nearY  = (indiciaNear * cos(PI - (2 * PI) / 12 * i)) + yCenter;
    farX   = (indiciaFar  * sin(PI - (2 * PI) / 12 * i)) + xCenter;
    farY   = (indiciaFar  * cos(PI - (2 * PI) / 12 * i)) + yCenter;
    tft.drawLine(nearX, nearY, farX, farY, ILI9341_YELLOW);

    //These 4 will be thicker than the others (0, 3, 6, & 9).
    if (i == 0 || i == 6) {
      nearX1 = (indiciaNear * sin(PI - (2 * PI) / 12 * i)) + xCenter + 1;
      nearY1 = nearY;
      farX1  = (indiciaFar  * sin(PI - (2 * PI) / 12 * i)) + xCenter + 1;
      farY1  = farY;
      nearX2 = (indiciaNear * sin(PI - (2 * PI) / 12 * i)) + xCenter - 1;
      nearY2 = nearY;
      farX2  = (indiciaFar  * sin(PI - (2 * PI) / 12 * i)) + xCenter - 1;
      farY2  = farY;
    }
    if (i == 3 || i == 9) {
      nearX1 = nearX; nearY1 = (indiciaNear * cos(PI - (2 * PI) / 12 * i)) +
                               yCenter + 1;
      farX1  = farX;  farY1  = (indiciaFar  * cos(PI - (2 * PI) / 12 * i)) +
                               yCenter + 1;
      nearX2 = nearX; nearY2 = (indiciaNear * cos(PI - (2 * PI) / 12 * i)) +
                               yCenter - 1;
      farX2  = farX;  farY2  = (indiciaFar  * cos(PI - (2 * PI) / 12 * i)) +
                               yCenter - 1;
    }
    if (i % 3 == 0) {
      tft.drawLine(nearX1, nearY1, farX1, farY1, ILI9341_YELLOW);
      tft.drawLine(nearX2, nearY2, farX2, farY2, ILI9341_YELLOW);
    }
  }
}
/***************************************************************************/
void draw_Hands(int hour, int minute, int second)
/****draw_Hands*************************************************************/
{
  float handRadians;

  time(&UTC); workTime_t = UTC + iHomeOffset;
  tm = localtime(&workTime_t);

  // Settings for 240

  // Length of the pointer end of the hand.
  //  int hourHandLength = 48;
  // Length the twin tails.
  //  int HourHandBackLength = 15;
  //
  // Length of the pointer end of the hand.
  //  int minuteHandLength = 62;
  // Length the twin tails.
  //  int minuteHandBackLength = 19;

  // By ratio

  // Length of the pointer end of the hand.
  int hourHandLength = dispHeight * 0.2;
  // Length the twin tails.
  int HourHandBackLength =  dispHeight * 0.0625;

  // Length of the pointer end of the hand.
  int minuteHandLength = dispHeight * 0.258333333;
  // Length the twin tails.
  int minuteHandBackLength = dispHeight * 0.079166667;

  hour = hour % 12;  // Only 12 hour indicia on a clock face.
  // First, calculate the hand positions for both.
  // Each hour gives 30° to the hour hand.
  // Every minute gives 1/2° to the hour hand.
  handRadians = ((float(hour) * -30. - float(minute) / 2.) - 180) /
                57.2957795130823;
  // This is the tip of the pointer end.
  myHX  = (hourHandLength * sin(handRadians)) + xCenter;
  myHY  = (hourHandLength * cos(handRadians)) + yCenter;
  //Now calculate the hour hand overhang past the center for hour hand.
  //Do this twice with +/-20 degrees then draw 2 triangles.
  handRadians = ((float(hour) * -30. - float(minute) / 2.) - 20.)
                / 57.2957795130823;
  myHXb1  = (HourHandBackLength * sin(handRadians)) + xCenter;
  myHYb1  = (HourHandBackLength * cos(handRadians)) + yCenter;
  handRadians = ((float(hour) * -30. - float(minute) / 2.) + 20.)
                / 57.2957795130823;
  myHXb2  = (HourHandBackLength * sin(handRadians)) + xCenter;
  myHYb2  = (HourHandBackLength * cos(handRadians)) + yCenter;

  //Now calculate the minute hand.
  //Every minute gives 6° to the minute hand. Every second gives .1°
  // to the minute hand.
  handRadians = ((float(minute) * -6. - float(second) / 10.) - 180.)
                / 57.2957795130823;
  // This is the tip of the pointer end.
  myMX = (minuteHandLength * sin(handRadians)) + xCenter;
  myMY = (minuteHandLength * cos(handRadians)) + yCenter;
  //Now the back (short) side of the minute hand across the center.
  //Do this twice with +/-20 degrees then draw 2 triangles.
  handRadians = ((float(minute) * -6. - float(second) / 10.) - 20.)
                / 57.2957795130823;
  myMXb1 = (minuteHandBackLength * sin(handRadians)) + xCenter;
  myMYb1 = (minuteHandBackLength * cos(handRadians)) + yCenter;
  handRadians = ((float(minute) * -6. - float(second) / 10.) + 20.)
                / 57.2957795130823;
  myMXb2 = (minuteHandBackLength * sin(handRadians)) + xCenter;
  myMYb2 = (minuteHandBackLength * cos(handRadians)) + yCenter;

  eraseColor = DarkerRed;
  //  if (myIsPM) eraseColor = DarkerBlue;
  //If either hand moved, must erase and rewrite both.
  // initHands opens the timed gate early.
  if (tm->tm_sec % 10 == 0 || initHands) {
    //First, see if this is a new place to draw.
    if (prevHX != myHX || prevMX != myMX || prevHY != myHY || prevMY != myMY
        || initHands) {  // initHands overrides the hands moved update check.
      initHands = false;
      if (prevHX < 10000) {  // Don't erase if nothing there already!
        //It is a new place to draw so remove the previous triangles
        tft.fillTriangle(xCenter, yCenter,
                         prevHX, prevHY,
                         prevHXb1,  prevHYb1,
                         eraseColor);
        tft.fillTriangle(xCenter, yCenter,
                         prevHX, prevHY,
                         prevHXb2, prevHYb2,
                         eraseColor);
      }
      if (prevMX < 10000) {  // Don't erase if nothing there already!
        //It is a new place to draw so remove the previous triangles
        tft.fillTriangle(xCenter, yCenter,
                         prevMX, prevMY,
                         prevMXb1,  prevMYb1,
                         eraseColor);
        tft.fillTriangle(xCenter, yCenter,
                         prevMX, prevMY,
                         prevMXb2,  prevMYb2,
                         eraseColor);
      }
      //Remember for next time through, for erasing.
      prevHX   = myHX;   prevHY   = myHY;
      prevHXb1 = myHXb1; prevHYb1 = myHYb1;
      prevHXb2 = myHXb2; prevHYb2 = myHYb2;
      prevMX   = myMX;   prevMY   = myMY;
      prevMXb1 = myMXb1; prevMYb1 = myMYb1;
      prevMXb2 = myMXb2; prevMYb2 = myMYb2;
      //Now, finally, put in the new hour and minute triangles.
      tft.fillTriangle(xCenter, yCenter, myHX, myHY,
                       myHXb1, myHYb1, HHDarker);
      tft.fillTriangle(xCenter, yCenter, myHX, myHY,
                       myHXb2, myHYb2, HHDarker);
      tft.fillTriangle(xCenter, yCenter, myMX, myMY,
                       myMXb1, myMYb1, MHDarker);
      tft.fillTriangle(xCenter, yCenter, myMX, myMY,
                       myMXb2, myMYb2, MHDarker);
    }
  }
}
/***************************************************************************/
void update_Seconds(int myMin, int mySec)
/****update_Seconds*********************************************************/
{ // This routine takes about 3 ms.  Way more than I thought it would.
  // I force the dots because an hour dance may take longer than 1 second
  //  and that would leave the display messed up, so totally rebuild,
  //  every second.
  int i;
  if (myMin % 2 == 0) {
    // Even minute, drawing second dots from 1 to now
    for (i = 1; i <= mySec; i++) draw_Seconds_Dot(i, aModeDraw);
    // Erasing each dot from now + 1 second through 60 seconds.
    for (i = mySec + 1; i <= 60; i++) draw_Seconds_Dot(i, aModeErase);
  } else {
    // Odd minute, erasing second dots from 1 to now
    for (i = 1; i <= mySec; i++) draw_Seconds_Dot(i, aModeErase);
    // Drawing each dot from now + 1 second through 60 seconds.
    for (i = mySec + 1; i <= 60; i++) draw_Seconds_Dot(i, aModeDraw);
  }
}
/***************************************************************************/
void draw_Seconds_Dot(int mySec, int myMode)
/****draw_Seconds_Dot*******************************************************/
{
  // Not really hand length but the distance from center where the dot goes.
  handLength = dispHeight * 0.4;

  mySX = (handLength * sin(PI - (2 * PI) / 60 * mySec)) + xCenter;
  mySY = (handLength * cos(PI - (2 * PI) / 60 * mySec)) + yCenter;
  myColor = DarkYellow;  // Default color
  if (myMode == aModeDraw) myColor = ILI9341_YELLOW;
  tft.fillCircle(mySX, mySY, 3, myColor);
}
/***************************************************************************/
void initAnalog(int myHour, int myMinute, int mySecond)
/****initAnalog*************************************************************/
{
  tft.fillScreen(DarkerRed);

  initHands = true;  // Allow past the 10 second update gate.
  draw_Clock_Face();

  draw_Hands(myHour, myMinute, mySecond);
  tft.fillCircle(xCenter, yCenter, 4, ILI9341_DARKGREY);  //draw_CenterDot();
  update_Seconds(myMinute, mySecond);  // Catch up the seconds to now.

  // Cause a complete rewrite of the 4 corners.
  prev_month = -1; prev_DOM = -1; cornerPrev_DOW = -1; prev_myYear = -1;
  showCorners();
}
/***************************************************************************/
void updateAnalogScreen()
/****updateAnalogScreen*****************************************************/
{

  //Draw in the proper place.
  draw_Hands(tm->tm_hour, tm->tm_min, tm->tm_sec);
  tft.fillCircle(xCenter, yCenter, 4, ILI9341_DARKGREY);  //draw center dot
  update_Seconds(tm->tm_min, tm->tm_sec);
  showCorners();
}
/***************************************************************************/
void showCorners()
/****showCorners************************************************************/
{
  ofr.setDrawer(tft); // Link renderer to base display.
  // Foreground color, Background color
  ofr.unloadFont();
  if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
    Serial.println("Render loadFont error for BritanicBoldTTF. showCorners 1");
    while (1);
  }
  ofr.setFontSize(60);
  ofr.setFontColor(ILI9341_YELLOW, DarkerRed);  // Foreground color, Background color

  //Month name, abbreviated, in top, left corner.
  if (prev_month != tm->tm_mon) {
    if (tm->tm_mon == 6 || tm->tm_mon == 7)
      myBuildString = monthStr(tm->tm_mon + 1);
    else
      myBuildString = monthShortStr(tm->tm_mon + 1);
    ofr.setCursor(0, 0); ofr.printf(myBuildString.c_str());  // Month name
    prev_month = tm->tm_mon + 1;
  }
  //Number of day of the month (1-~31) in top, right corner.
  if (prev_DOM != tm->tm_mday) {
    myBuildString = String(tm->tm_mday + th(tm->tm_mday));
    ofr.setCursor(dispWidth, 0); ofr.rprintf(myBuildString.c_str());  // Day of month
    prev_DOM = tm->tm_mday;
  }
  //Day of Week name, abbreviated, (DOW) in bottom, left corner.
  if (cornerPrev_DOW != tm->tm_wday) {
    myBuildString = dayShortStr(tm->tm_wday + 1);
    myTH = ofr.getTextHeight(myBuildString.c_str());
    // Name of day of the week
    ofr.setCursor(0, dispHeight - myTH - 16); ofr.printf(myBuildString.c_str());
    cornerPrev_DOW = tm->tm_wday;
  }
  //Year value (4 digits) in right, bottom corner.
  if (prev_myYear != tm->tm_year + 1900) {
    myBuildString = String(tm->tm_year + 1900);
    myTW = ofr.getTextWidth(myBuildString.c_str());
    ofr.setCursor(dispWidth, dispHeight - myTH - 16); ofr.rprintf(myBuildString.c_str()); // Year
    prev_myYear = tm->tm_year + 1900;
  }
}
/***************************************************************************/
void doDTZ_Show()
/****doDTZ_Show*************************************************************/
{
  // ------------------> TOP CITY
  ofr.setDrawer(sprite61);  // And... we're off and sprite'ing.
  // Foreground color, Background color
  ofr.unloadFont();
  if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
    Serial.println("Render loadFont error for BritanicBoldTTF. doDTZ_Show 1");
    while (1);
  }
  ofr.setFontSize(72);
  ofr.setFontColor(DarkerRed, ILI9341_YELLOW);  // Foreground color, Background color

  time(&UTC); workTime_t = UTC + iTopOffset;
  tm = localtime(&workTime_t);
  //  Serial.printf("------\r\nUTC:\t%lu\r\n", UTC);
  //  Serial.printf("iTopOffset:\t%lu\r\n", iTopOffset);
  //  Serial.printf("Top adjusted %lu\r\n", workTime_t);
  workDate[0] = 0; fullDate[0] = 0;  // Set to zero length string
  sprintf(workDate, "%s ", dayShortStr(tm->tm_wday + 1));
  strcat(fullDate, workDate);
  sprintf(workDate, "%s", monthShortStr(tm->tm_mon + 1));
  strcat(fullDate, workDate);
  sprintf(workDate, " %i", tm->tm_mday);
  strcat(fullDate, workDate);
  strcat(fullDate, th(tm->tm_mday).c_str());

  workTime[0] = 0; fullTime[0] = 0;  // Set to zero length string
  sprintf(workTime, "%02i",  tm->tm_hour);
  strcat(fullTime, workTime);
  sprintf(workTime, ":%02i", tm->tm_min);
  strcat(fullTime, workTime);
  sprintf(workTime, ":%02i", tm->tm_sec);
  strcat(fullTime, workTime);
  //    if (tzTopCity.isAM())
  //      strcat(fullTime, " AM");
  //    else
  //      strcat(fullTime, " PM");
  if (strcmp(prevTopDate, fullDate) != 0) {  // If they don't compare...
    sprite61.fillSprite(TFT_YELLOW); sprite61.setTextColor(DarkerRed);  // Yellow box
    ofr.setCursor(spriteWC - 1, -6); ofr.cprintf(fullDate);
    sprite61.pushSprite(spriteM, 38);
    strcpy(prevTopDate, fullDate);
  }
  sprite61.fillSprite(TFT_YELLOW); sprite61.setTextColor(DarkerRed);  // Yellow box
  ofr.setCursor(70, -6); ofr.printf(fullTime);  // Variable pitch font, 70 fixes it in place
  sprite61.pushSprite(spriteM, 99);

  // ------------------> BOTTOM CITY

  time(&UTC); workTime_t = UTC + iBotOffset;
  tm = localtime(&workTime_t);
  //  Serial.printf("UTC:\t%lu\r\n", UTC);
  //  Serial.printf("iBotOffset:\t%lu\r\n", iBotOffset);
  //  Serial.printf("Bot adjusted %lu\r\n", workTime_t);
  workDate[0] = 0; fullDate[0] = 0;  // Set to zero length string
  sprintf(workDate, "%s ", dayShortStr(tm->tm_wday + 1));
  strcat(fullDate, workDate);
  sprintf(workDate, "%s", monthShortStr(tm->tm_mon + 1));
  strcat(fullDate, workDate);
  sprintf(workDate, " %i", tm->tm_mday);  strcat(fullDate, workDate);
  strcat(fullDate, th(tm->tm_mday).c_str());

  workTime[0] = 0; fullTime[0] = 0;  // Set to zero length string
  sprintf(workTime, "%02i",  tm->tm_hour);
  strcat(fullTime, workTime);
  sprintf(workTime, ":%02i", tm->tm_min);
  strcat(fullTime, workTime);
  sprintf(workTime, ":%02i", tm->tm_sec);
  strcat(fullTime, workTime);
  //    if (tzBottomCity.isAM())
  //      strcat(fullTime, " AM");
  //    else
  //      strcat(fullTime, " PM");

  if (strcmp(prevBottomDate, fullDate) != 0) {  // If they don't compare...
    //    sprite61.fillSprite(DarkerRed); sprite61.setTextColor(TFT_YELLOW);  // "Normal"
    sprite61.fillSprite(TFT_YELLOW); sprite61.setTextColor(DarkerRed);  // Yellow box
    //    sprite61.setTextDatum(TC_DATUM);
    //    sprite61.drawString(fullDate, spriteWC - 1, 1);
    ofr.setCursor(spriteWC - 1, -6); ofr.cprintf(fullDate);
    sprite61.pushSprite(spriteM, yCenter + 38);
    strcpy(prevBottomDate, fullDate);
  }
  //    sprite61.fillSprite(DarkerRed); sprite61.setTextColor(TFT_YELLOW);  // "Normal"
  sprite61.fillSprite(TFT_YELLOW); sprite61.setTextColor(DarkerRed);  // Yellow box
  //  sprite61.setTextDatum(TL_DATUM);
  //  sprite61.drawString(fullTime, 70, 7);
  ofr.setCursor(70, -6); ofr.printf(fullTime);  // Variable pitch font, 70 fixes it in place
  sprite61.pushSprite(spriteM, yCenter + 99);
}
/***************************************************************************/
void doBCDShow()
/****doBCDShow**************************************************************/
{
  // Hour Decade ***********************************

  //This routine squirts out the bits backwards.  0001 is 8.
  n = tm->tm_hour / 10;
  for (i = 0; i < 4; i++) BCDBreak[i] = 0;
  for (i = 0; n > 0; i++)
  {
    BCDBreak[i] = n % 2; n = n / 2;
  }
  if (hourDecade[0] == false && BCDBreak[0] == 1) {
    tft.pushImage(0, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Bright_Red_68);
    hourDecade[0] = true;
  }
  if (hourDecade[0] == true && BCDBreak[0] == 0) {
    tft.pushImage(0, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Dark_Red_68);
    hourDecade[0] = false;
  }

  if (hourDecade[1] == false && BCDBreak[1] == 1) {
    tft.pushImage(0, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Bright_Red_68);
    hourDecade[1] = true;
  }
  if (hourDecade[1] == true && BCDBreak[1] == 0) {
    tft.pushImage(0, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Dark_Red_68);
    hourDecade[1] = false;
  }

  // Hour Unit   ***********************************

  //This routine squirts out the bits backwards.  0001 is 8.
  n = tm->tm_hour % 10;
  for (i = 0; i < 4; i++) BCDBreak[i] = 0;
  for (i = 0; n > 0; i++)
  {
    BCDBreak[i] = n % 2; n = n / 2;
  }
  if (hourUnit[0] == false && BCDBreak[0] == 1) {
    tft.pushImage(segWidth * 1, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Bright_Red_68);
    hourUnit[0] = true;
  }
  if (hourUnit[0] == true && BCDBreak[0] == 0) {
    tft.pushImage(segWidth * 1, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Dark_Red_68);
    hourUnit[0] = false;
  }

  if (hourUnit[1] == false && BCDBreak[1] == 1) {
    tft.pushImage(segWidth * 1, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Bright_Red_68);
    hourUnit[1] = true;
  }
  if (hourUnit[1] == true && BCDBreak[1] == 0) {
    tft.pushImage(segWidth * 1, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Dark_Red_68);
    hourUnit[1] = false;
  }

  if (hourUnit[2] == false && BCDBreak[2] == 1) {
    tft.pushImage(segWidth * 1, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Bright_Red_68);
    hourUnit[2] = true;
  }
  if (hourUnit[2] == true && BCDBreak[2] == 0) {
    tft.pushImage(segWidth * 1, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Dark_Red_68);
    hourUnit[2] = false;
  }

  if (hourUnit[3] == false && BCDBreak[3] == 1) {
    tft.pushImage(segWidth * 1, 52, LEDWidth, LEDHeight, Bright_Red_68);
    hourUnit[3] = true;
  }
  if (hourUnit[3] == true && BCDBreak[3] == 0) {
    tft.pushImage(segWidth * 1, 52, LEDWidth, LEDHeight, Dark_Red_68);
    hourUnit[3] = false;
  }

  // Minute Decade *********************************

  //This routine squirts out the bits backwards.  0001 is 8.
  n = tm->tm_min / 10;
  for (i = 0; i < 4; i++) BCDBreak[i] = 0;
  for (i = 0; n > 0; i++)
  {
    BCDBreak[i] = n % 2; n = n / 2;
  }
  if (minuteDecade[0] == false && BCDBreak[0] == 1) {
    tft.pushImage(segWidth * 2, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteDecade[0] = true;
  }
  if (minuteDecade[0] == true && BCDBreak[0] == 0) {
    tft.pushImage(segWidth * 2, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteDecade[0] = false;
  }

  if (minuteDecade[1] == false && BCDBreak[1] == 1) {
    tft.pushImage(segWidth * 2, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteDecade[1] = true;
  }
  if (minuteDecade[1] == true && BCDBreak[1] == 0) {
    tft.pushImage(segWidth * 2, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteDecade[1] = false;
  }

  if (minuteDecade[2] == false && BCDBreak[2] == 1) {
    tft.pushImage(segWidth * 2, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteDecade[2] = true;
  }
  if (minuteDecade[2] == true && BCDBreak[2] == 0) {
    tft.pushImage(segWidth * 2, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteDecade[2] = false;
  }

  // Minute Unit ***********************************

  //This routine squirts out the bits backwards.  0001 is 8.
  n = tm->tm_min % 10;
  for (i = 0; i < 4; i++) BCDBreak[i] = 0;
  for (i = 0; n > 0; i++)
  {
    BCDBreak[i] = n % 2; n = n / 2;
  }
  if (minuteUnit[0] == false && BCDBreak[0] == 1) {
    tft.pushImage(segWidth * 3, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteUnit[0] = true;
  }
  if (minuteUnit[0] == true && BCDBreak[0] == 0) {
    tft.pushImage(segWidth * 3, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteUnit[0] = false;
  }

  if (minuteUnit[1] == false && BCDBreak[1] == 1) {
    tft.pushImage(segWidth * 3, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteUnit[1] = true;
  }
  if (minuteUnit[1] == true && BCDBreak[1] == 0) {
    tft.pushImage(segWidth * 3, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteUnit[1] = false;
  }

  if (minuteUnit[2] == false && BCDBreak[2] == 1) {
    tft.pushImage(segWidth * 3, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteUnit[2] = true;
  }
  if (minuteUnit[2] == true && BCDBreak[2] == 0) {
    tft.pushImage(segWidth * 3, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteUnit[2] = false;
  }

  if (minuteUnit[3] == false && BCDBreak[3] == 1) {
    tft.pushImage(segWidth * 3, 52, LEDWidth, LEDHeight, Bright_Yellow_68);
    minuteUnit[3] = true;
  }
  if (minuteUnit[3] == true && BCDBreak[3] == 0) {
    tft.pushImage(segWidth * 3, 52, LEDWidth, LEDHeight, Dark_Yellow_68);
    minuteUnit[3] = false;
  }

  // Second Decade *********************************

  //This routine squirts out the bits backwards.  0001 is 8.
  n = tm->tm_sec / 10;
  for (i = 0; i < 4; i++) BCDBreak[i] = 0;
  for (i = 0; n > 0; i++)
  {
    BCDBreak[i] = n % 2; n = n / 2;
  }
  if (secondDecade[0] == false && BCDBreak[0] == 1) {
    tft.pushImage(segWidth * 4, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Bright_Green_68);
    secondDecade[0] = true;
  }
  if (secondDecade[0] == true && BCDBreak[0] == 0) {
    tft.pushImage(segWidth * 4, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Dark_Green_68);
    secondDecade[0] = false;
  }

  if (secondDecade[1] == false && BCDBreak[1] == 1) {
    tft.pushImage(segWidth * 4, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Bright_Green_68);
    secondDecade[1] = true;
  }
  if (secondDecade[1] == true && BCDBreak[1] == 0) {
    tft.pushImage(segWidth * 4, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Dark_Green_68);
    secondDecade[1] = false;
  }

  if (secondDecade[2] == false && BCDBreak[2] == 1) {
    tft.pushImage(segWidth * 4, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Bright_Green_68);
    secondDecade[2] = true;
  }
  if (secondDecade[2] == true && BCDBreak[2] == 0) {
    tft.pushImage(segWidth * 4, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Dark_Green_68);
    secondDecade[2] = false;
  }

  // Second Unit ***********************************

  n = tm->tm_sec % 10;
  for (i = 0; i < 4; i++) BCDBreak[i] = 0;
  for (i = 0; n > 0; i++)
  {
    BCDBreak[i] = n % 2; n = n / 2;
  }
  if (secondUnit[0] == false && BCDBreak[0] == 1) {
    tft.pushImage(segWidth * 5, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Bright_Green_68);
    secondUnit[0] = true;
  }
  if (secondUnit[0] == true && BCDBreak[0] == 0) {
    tft.pushImage(segWidth * 5, (segHeight * 3) + 52,
                  LEDWidth, LEDHeight, Dark_Green_68);
    secondUnit[0] = false;
  }

  if (secondUnit[1] == false && BCDBreak[1] == 1) {
    tft.pushImage(segWidth * 5, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Bright_Green_68);
    secondUnit[1] = true;
  }
  if (secondUnit[1] == true && BCDBreak[1] == 0) {
    tft.pushImage(segWidth * 5, (segHeight * 2) + 52,
                  LEDWidth, LEDHeight, Dark_Green_68);
    secondUnit[1] = false;
  }

  if (secondUnit[2] == false && BCDBreak[2] == 1) {
    tft.pushImage(segWidth * 5, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Bright_Green_68);
    secondUnit[2] = true;
  }
  if (secondUnit[2] == true && BCDBreak[2] == 0) {
    tft.pushImage(segWidth * 5, (segHeight * 1) + 52,
                  LEDWidth, LEDHeight, Dark_Green_68);
    secondUnit[2] = false;
  }

  if (secondUnit[3] == false && BCDBreak[3] == 1) {
    tft.pushImage(segWidth * 5, 52, LEDWidth, LEDHeight, Bright_Green_68);
    secondUnit[3] = true;
  }
  if (secondUnit[3] == true && BCDBreak[3] == 0) {
    tft.pushImage(segWidth * 5, 52, LEDWidth, LEDHeight, Dark_Green_68);
    secondUnit[3] = false;
  }
}
/***************************************************************************/
void do3LineShow()
/****do3LineShow************************************************************/
{
  ofr.setDrawer(tft); // Link renderer to base display.

  if (prevHour != tm->tm_hour) {  // On the hour, it is time to clean
    Serial.println("Clearing screen for new update based on hour change");
    tft.fillScreen(DarkerRed);  // Background good color for cleaning.
    prev_Line1 = ""; prev_Line2 = ""; prev_Line3 = "";  // Force redraw of all 3 lines.
  }

  //asdf5
  myBuildString = "";
  tempHour = tm->tm_hour;
  if (showAMPM) {  // 12 hour (AM/PM) formatting
    if (tempHour == 0) tempHour = 12;
    if (tempHour > 12) tempHour -= 12;
  }
  if (tempHour > 9) {
    myBuildString = "1";
    tempHour -= 10;
  }
  myBuildString += String(tempHour) + ":";
  myBuildString += String(tm->tm_min / 10);
  myBuildString += String(tm->tm_min % 10);
  if (prev_Line1 != myBuildString) {
    prev_Line1 = myBuildString;
    ofr.unloadFont();
    if (ofr.loadFont(LiquidCrystalTTF, sizeof(LiquidCrystalTTF))) {
      Serial.println("Render loadFont error for LiquidCrystalTTF. 3-Line 1");
      while (1);
    }
    // Right here, we have to make a decision whether to use the SpriteHH (no flashing)
    //  or regular tft updating (flashing).  It depends on whether it was properly allocated
    //  during initialization.

    ofr.setFontColor(TFT_YELLOW, DarkerRed);
    ofr.setFontSize(190);
    if (spriteHHOK == 0) {  // 0 means there was no PSRAM allocation.  Use tft drawer.
      ofr.setDrawer(tft);
      //      Serial.println("spriteHH creation failed. Using tft drawing.");
      tft.fillRect(0, 0, dispWidth, dispHeight / 2, DarkerRed);
      ofr.setCursor(dispWidth / 2, 10);
      ofr.cprintf(myBuildString.c_str());
    } else {  // We have PSRAM, use spriteHH that is there so no flickering.
      ofr.setDrawer(spriteHH);  // Use big sprite from PSRAM
      //      Serial.println("spriteHH creation succeeded. Using spriteHH drawing.");
      spriteHH.fillSprite(DarkerRed);
      ofr.setCursor(spCenterW_HH, 10);
      ofr.cprintf(myBuildString.c_str());
      spriteHH.pushSprite(0, 0);
      ofr.setDrawer(tft); // Link renderer to base display.
    }

    //asdf6  // Only look at the rest if the minute field has changed.
    myBuildString = dayStr(tm->tm_wday + 1);
    if (prev_Line2 != myBuildString) {
      prev_Line2 = myBuildString;
      ofr.unloadFont();
      if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
        Serial.println("Render loadFont error for BritanicBoldTTF 1. 3-Line 2");
        while (1);
      }
      ofr.setFontColor(TFT_YELLOW, DarkerRed);
      ofr.setFontSize(80);
      ofr.setCursor(dispWidth / 2, 160);
      tft.fillRect(0, 160, dispWidth, ofr.getTextHeight(myBuildString.c_str()), DarkerRed);
      ofr.cprintf(myBuildString.c_str());
    }
    //asdf7
    myBuildString =  monthShortStr(uint8_t(tm->tm_mon + 1));
    myBuildString += " ";
    myBuildString += String(tm->tm_mday);
    myBuildString += th(tm->tm_mday);
    if (prev_Line3 != myBuildString) {
      prev_Line3 = myBuildString;
      ofr.unloadFont();
      if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
        Serial.println("Render loadFont error for BritanicBoldTTF  3-Line 3");
        while (1);
      }
      ofr.setFontColor(TFT_YELLOW, DarkerRed);
      ofr.setFontSize(80);
      ofr.setCursor(dispWidth / 2, 240);
      tft.fillRect(0, 240, dispWidth, ofr.getTextHeight(myBuildString.c_str()), DarkerRed);
      ofr.cprintf(myBuildString.c_str());
    }
  }
}
/***************************************************************************/
void do4LineShow()
/****do4LineShow************************************************************/
{
  ofr.setDrawer(tft); // Link renderer to base display.

  if (prevHour != tm->tm_hour) {     // On the hour, it is time to clean
    // prevHour == tm->tm_hour;
    tft.fillScreen(DarkerRed);  // Background good color for cleaning.
    prev_Line1 = ""; prev_Line2 = ""; prev_Line3 = "";  // Force redraw
  }
  // Line 1, Day of the week  asdf1
  myBuildString = dayStr(tm->tm_wday + 1);
  if (prev_Line1 != myBuildString) {
    prev_Line1 = myBuildString;
    ofr.unloadFont();
    if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
      Serial.println("Render loadFont error for BritanicBoldTTF. 4-Line 1");
      while (1);
    }
    ofr.setFontColor(TFT_YELLOW, DarkerRed);
    ofr.setFontSize(80);
    ofr.setCursor(dispWidth / 2, -5);
    tft.fillRect(0, 0, dispWidth, ofr.getTextHeight(myBuildString.c_str()), DarkerRed);
    ofr.cprintf(myBuildString.c_str());
  }

  // Line 2, Month and day number  asdf2
  myBuildString =  monthShortStr(tm->tm_mon + 1);
  myBuildString += " ";
  myBuildString += String(tm->tm_mday);
  myBuildString += th(SS_Pay_Wed_DOM);
  if (prev_Line2 != myBuildString) {
    prev_Line2 = myBuildString;
    ofr.unloadFont();
    if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
      Serial.println("Render loadFont error for BritanicBoldTTF. 4-Line 2");
      while (1);
    }
    ofr.setFontColor(TFT_YELLOW, DarkerRed);
    ofr.setFontSize(80);
    ofr.setCursor(dispWidth / 2, 75);
    tft.fillRect(0, 75, dispWidth, ofr.getTextHeight(myBuildString.c_str()), DarkerRed);
    ofr.cprintf(myBuildString.c_str());
  }

  // Line 3, Year  asdf3
  myBuildString = String(tm->tm_year + 1900);
  if (prev_Line3 != myBuildString) {
    prev_Line3 = myBuildString;
    ofr.unloadFont();
    if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
      Serial.println("Render loadFont error for BritanicBoldTTF.  4-Line 3");
      while (1);
    }
    ofr.setFontColor(TFT_YELLOW, DarkerRed);
    ofr.setFontSize(80);
    ofr.setCursor(dispWidth / 2, 155);
    tft.fillRect(0, 155, dispWidth, ofr.getTextHeight(myBuildString.c_str()), DarkerRed);
    ofr.cprintf(myBuildString.c_str());
  }

  // Line 4, Time  asdf4

  // Link renderer to sprite (font will be rendered in sprite sprite61_HH)
  ofr.setDrawer(sprite61);

  calcFourLineScreenTime();
  sprite61.fillSprite(DarkerRed);
  ofr.unloadFont();
  if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
    Serial.println("Render loadFont error for BritanicBoldTTF.  4-Line 4");
    while (1);
  }
  ofr.setFontColor(TFT_YELLOW, DarkerRed);
  ofr.setFontSize(75);
  ofr.setCursor(spriteW / 2, -12);
  ofr.cprintf(fourLineScreenTime);
  sprite61.pushSprite(spriteM, 250);
}
/****************************************************************************/
String th(int theDate)
/****************************************************************************/
{
  switch (theDate) {  // Make it English-like a little bit
    case 1: case 21: case 31: return "st"; break;
    case 2: case 22:          return "nd"; break;
    case 3: case 23:          return "rd"; break;
    default:                  return "th"; break;
  }
}
/***************************************************************************/
void showSelectedClock(bool initScreen)
/****showSelectedClock******************************************************/
{
  //int analogHandsShow = 0;
  //int four_LineShow   = 1;
  //int three_LineShow  = 2;
  //int BCD_Show        = 3;
  //int dtz_Show        = 4;
  //int TooHigh         = 5;

  time(&UTC); workTime_t = UTC + iHomeOffset;
  tm = localtime(&workTime_t);

  if (tm->tm_wday == 2)
    if (tm->tm_hour == 2)
      if (prevHour != tm->tm_hour)
        ESP.restart();  // Weekly reboot

  if ((tm->tm_min % 10 == 0) && tm->tm_sec == 0)
    if (getXchangeRate()) initScreen = true;  // If true, the screen was stolen.
  // Will be extended every time valid data received.  If data too old, zero it out to clear
  if (millis() > PHP_Rate_Clear_millis) PHP_Rate = 0.;
  if (whichToShow >= TooHigh) whichToShow = analogHandsShow;

  if (whichToShow == dtz_Show) {
    if (initScreen) {  // Hourly init

      // This may or may not result in network activity.  If it does,
      //  it will put up messages while it tries to get a rate.

      tft.fillScreen(DarkerRed);
      ofr.setDrawer(tft); // Link renderer to base display.
      ofr.unloadFont();
      if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
        Serial.println("Render loadFont error for BritanicBoldTTF. showCorners 1");
        while (1);
      }
      ofr.setFontSize(32);
      ofr.setFontColor(TFT_WHITE, DarkerRed);  // Foreground color, Background color

      ofr.setCursor(10, 3);           ofr.printf(cTopCityname);    // Top city name
      ofr.setCursor(10, yCenter + 3); ofr.printf(cBotCityname); // Bottom city name

      //      if (pktValidity) {  // Good data from the web site? Print results.
      if (PHP_Rate > 0.) {  // Good data from the web site? Print results.
        ofr.setCursor(tft.width(), 3); ofr.rprintf(readingTime);  // Bottom city name
        myBuildString = "PHP/USD " + String(PHP_Rate);
        ofr.setCursor(tft.width(), yCenter + 3); ofr.rprintf(myBuildString.c_str());
      }
      prevTopDate[0] = 0; prevBottomDate[0] = 0;  // Easy way to clear char array.
    }
    doDTZ_Show();
  }
  else if (whichToShow == BCD_Show) {
    if (initScreen) {
      for (i = 0; i < 2; i++) hourDecade[i] = false;
      for (i = 0; i < 4; i++) hourUnit[i] = false;
      for (i = 0; i < 3; i++) minuteDecade[i] = false;
      for (i = 0; i < 4; i++) minuteUnit[i] = false;
      for (i = 0; i < 3; i++) secondDecade[i] = false;
      for (i = 0; i < 4; i++) secondUnit[i] = false;
      tft.fillScreen(TFT_BLACK);
      ofr.setFontColor(TFT_WHITE, TFT_BLACK);  // Foreground color, Background color
      ofr.unloadFont();
      if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
        Serial.println("Render loadFont error for BritanicBoldTTF. BCD 1");
        while (1);
      }
      ofr.setFontSize(50);
      myBuildString = "BCD Binary Clock";
      // Heading for Binary Clock
      ofr.setCursor(dispWidth / 2, 0); ofr.cprintf(myBuildString.c_str());

      //Hour LEDs
      tft.pushImage(0,            (segHeight * 3) + 52, LEDWidth, LEDHeight, Dark_Red_68);
      tft.pushImage(0,            (segHeight * 2) + 52, LEDWidth, LEDHeight, Dark_Red_68);
      tft.pushImage(segWidth,     (segHeight * 3) + 52, LEDWidth, LEDHeight, Dark_Red_68);
      tft.pushImage(segWidth,     (segHeight * 2) + 52, LEDWidth, LEDHeight, Dark_Red_68);
      tft.pushImage(segWidth,     (segHeight    ) + 52, LEDWidth, LEDHeight, Dark_Red_68);
      tft.pushImage(segWidth,                       52, LEDWidth, LEDHeight, Dark_Red_68);
      //Minute
      tft.pushImage(segWidth * 2, (segHeight * 3) + 52, LEDWidth, LEDHeight, Dark_Yellow_68);
      tft.pushImage(segWidth * 2, (segHeight * 2) + 52, LEDWidth, LEDHeight, Dark_Yellow_68);
      tft.pushImage(segWidth * 2, (segHeight * 1) + 52, LEDWidth, LEDHeight, Dark_Yellow_68);
      tft.pushImage(segWidth * 3, (segHeight * 3) + 52, LEDWidth, LEDHeight, Dark_Yellow_68);
      tft.pushImage(segWidth * 3, (segHeight * 2) + 52, LEDWidth, LEDHeight, Dark_Yellow_68);
      tft.pushImage(segWidth * 3, (segHeight    ) + 52, LEDWidth, LEDHeight, Dark_Yellow_68);
      tft.pushImage(segWidth * 3,                   52, LEDWidth, LEDHeight, Dark_Yellow_68);
      //Second
      tft.pushImage(segWidth * 4, (segHeight * 3) + 52, LEDWidth, LEDHeight, Dark_Green_68);
      tft.pushImage(segWidth * 4, (segHeight * 2) + 52, LEDWidth, LEDHeight, Dark_Green_68);
      tft.pushImage(segWidth * 4, (segHeight * 1) + 52, LEDWidth, LEDHeight, Dark_Green_68);
      tft.pushImage(segWidth * 5, (segHeight * 3) + 52, LEDWidth, LEDHeight, Dark_Green_68);
      tft.pushImage(segWidth * 5, (segHeight * 2) + 52, LEDWidth, LEDHeight, Dark_Green_68);
      tft.pushImage(segWidth * 5, (segHeight    ) + 52, LEDWidth, LEDHeight, Dark_Green_68);
      tft.pushImage(segWidth * 5,                   52, LEDWidth, LEDHeight, Dark_Green_68);
    }
    doBCDShow();
  }

  else if (whichToShow == three_LineShow) {
    if (initScreen) {
      tft.fillScreen(DarkerRed);
      prev_Line1 = ""; prev_Line2 = ""; prev_Line3 = "";  // Force redraw
    }
    do3LineShow();
  }
  else if (whichToShow == four_LineShow) {
    if (initScreen) {
      tft.fillScreen(DarkerRed);
      prev_Line1 = ""; prev_Line2 = ""; prev_Line3 = "";  // Force redraw
    }
    do4LineShow();
  }
  else if (whichToShow == analogHandsShow) {
    if (initScreen) {
      tft.fillScreen(DarkerRed);
      initAnalog(tm->tm_hour, tm->tm_min, tm->tm_sec);
    } else {
      updateAnalogScreen();  //Draw all hands in the proper place immediately.
    }
  }
}
/****************************************************************************/
bool getXchangeRate()
/****************************************************************************/
{
  bool fetchOK, initScreen = false;
  // This is all done on UTC. Actual time does not matter, just interval.
  time(&UTC); workTime_t = UTC + iHomeOffset;
  tm = localtime(&workTime_t);
unsigned long entryEpoch = workTime_t;

  Serial.printf("%02i:%02i:%02i Entering X Rate fetch routine. %lu/%lu.\r\n",
                tm->tm_hour, tm->tm_min, tm->tm_sec,
                workTime_t - lastXRateFetchEpoch, XRateFetchInterval);
  if (firstXPass) {
    firstXPass = false;
    Serial.println("This is the initial pass, not fetching X Rate this time.");
  } else {
    if ((entryEpoch - lastXRateFetchEpoch + 60) > XRateFetchInterval) {

      Serial.printf("rebootPrev_DOM %i, tm->tm_mday %i\r\n",
                    rebootPrev_DOM, tm->tm_mday);
      Serial.printf("tm->tm_wday %i, bootDay %i\r\n",
                    tm->tm_wday, bootDay);
      if (rebootPrev_DOM != tm->tm_mday)
        if (tm->tm_wday == bootDay)  // Time for weekly reboot ?
          if (tm->tm_hour > 0)  // From 1 to 4 am.
            ESP.restart();
      
      return(false);  // Deactivate XRate Fetch
      
      Serial.println("I will try an X Rate fetch now. (try 1)");
      initScreen = true;  // The screen is about to be changed.  Need to reinit it later.
      fetchOK = xRateWorker(1);  // Try to get the data from apilayer server.
      if (!fetchOK) {
        Serial.println("First X Rate fetch failed, waiting 5 seconds to retry. (try 2)");
        delay(5000);  // Wait 5 seconds and try a second time.
        fetchOK = xRateWorker(2);  // Try to get the data from apilayer server again.
      }
      if (!fetchOK) {
        Serial.println("Second X Rate fetch failed, waiting 5 seconds for final try. (try 3)");
        delay(5000);  // Wait 5 seconds and try a second time.
        fetchOK = xRateWorker(3);  // Try to get the data from apilayer server for the last time.
      }
      if (fetchOK) lastXRateFetchEpoch = entryEpoch;  // Got a live one, update timer.
    } else {
      Serial.println("Not time to fetch rate yet. Not long enough since last fetch.");
    }
  }
  //  Serial.printf("Returning initScreen status %s\r\n", initScreen ? "true." : "false.");
  return initScreen;
}
/****************************************************************************/
bool xRateWorker(int iTry)
/****************************************************************************/
{
  /* Returned JSON packet looks something like this:
    {
    "success": true,
    "timestamp": 1651599013,
    "base": "USD",
    "date": "2022-05-03",
    "rates": {
    "PHP": 52.511497
    }
    }
  */

  StaticJsonDocument<200> doc;  // Allocate 200 bytes on the stack.
  //Get just PHP compared to USD.  Returns about 136 bytes.
  //https://api.apilayer.com/exchangerates_data/latest?symbols=PHP
  // &base=USD&apikey=k5MJFkvlen6ebpAvKRpUlbbBd7uPAzAC
  //Get every rate they have in one go
  //https://api.apilayer.com/exchangerates_data/latest
  // &apikey=k5MJFkvlen6ebpAvKRpUlbbBd7uPAzAC
  // This gets stepped up when one key is used up. Reset by weekly reboot.
  static int useAPIkey = 0;
  int httpResponseCode = 429;  // Set entry condition to fetch loop.
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    while (useAPIkey < maxAPIkeys && httpResponseCode == 429) {
      serverPath = "https://api.apilayer.com/exchangerates_data/latest?"
                   "symbols=PHP&base=USD&apikey=";
      serverPath = serverPath + apiArray[useAPIkey];
      // Serial.printf("Fetching exchange rate with API key %i with the following URI:\r\n%s\r\n",
      //               useAPIkey + 1, serverPath.c_str());

      sprintf(workTime, "Try %i, Key %i ", iTry, useAPIkey + 1);
      //      Serial.println(workTime);
      Serial.print(workTime);
      tft.fillScreen(DarkerRed);
      ofr.setDrawer(tft); // Link renderer to base display.
      ofr.unloadFont();
      if (ofr.loadFont(BritanicBoldTTF, sizeof(BritanicBoldTTF))) {
        Serial.println("Render loadFont error for BritanicBoldTTF. xRateWorker 1");
        while (1);
      }
      ofr.setFontSize(72);
      ofr.setFontColor(TFT_YELLOW, DarkerRed);  // Foreground color, Background color

      int yPos = 40;
      ofr.setCursor(xCenter, yPos);       ofr.cprintf("Attempting to");
      ofr.setCursor(xCenter, yPos + 90);  ofr.cprintf("fetch X rate.");
      ofr.setCursor(xCenter, yPos + 180); ofr.cprintf(workTime);

      http.begin(serverPath.c_str());
      httpResponseCode = http.GET();
      Serial.printf("got response code: %i\r\n", httpResponseCode);
      if (httpResponseCode == 429) useAPIkey++;  // Saying key used up?  Use the next one.
    }

    if (httpResponseCode == 200) {
      String payload = http.getString();
      // Serial.println("Returned data:"); Serial.print(payload);
      http.end();  // Free resources
      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return false;
      }
      pktValidity = doc["success"];
      // It is called ternary!
      //  Compare ? True return action : False return action
      // Serial.print(pktValidity ? "V" : "Inv");
      // Serial.println("alid packet received.");
      if (!pktValidity) {
        // Serial.println("Rate packet not valid. Not decoded further.");
        // Serial.println("Leaving it as it was until timeout clear or refresh.");
        // PHP_Rate = 0.;  // This is done, now, on a timer when too stale.
        return pktValidity;
      }
      unsigned long packetTime = doc["timestamp"];
      packetTime += iHomeOffset;
      // Serial.printf("Packet epoch: %lu\r\n", packetTime);
      // Serial.printf("homeTime Offset: %i\r\n", os);
      // Serial.printf("Adjusted packet epoch: %lu or ", packetTime);
      // Either...
      //  time(&UTC); workTime_t = UTC + iHomeOffset;
      //  tm = localtime(&packetTime);  // Wow, there goes another 38 µs!
      //  Serial.printf("Currency Conversion packet time: "
      //                "%02i/%02i/%02i %02i:%02i:%02i\r\n",
      //                tm->tm_month, tm->tm_day,
      //                tm->tm_year + 1970,
      //                tm->tm_hour, tm->tm_minute,
      //                tm->tm_second);
      // ...or...
      // A leftover but looks pretty good to use.
      //  Shorter code text but can get old time.  Not using it.
      // breakTime(packetTime, tm);
      // Serial.printf("%02i/%02i/%02i %02i:%02i:%02i UTC\r\n",
      //               tm.Month, tm.Day, tm.Year + 1970,
      //               tm.Hour, tm.Minute, tm.Second);
      PHP_Rate = doc["rates"]["PHP"];
      // "Now" plus "valid time" limit = time to zero out PHP_Rate,
      //   done elsewhere.  This will be updated each valid update.
      //   Currently set for 18 hours.
      // Set the time to clear PHP_Rate if no update.
      PHP_Rate_Clear_millis = millis() + PHP_Rate_Valid_millis;
      // Serial.printf("Current PHP Conversion rate %.2f\r\n", PHP_Rate);
      // time(&UTC); workTime_t = UTC + iHomeOffset;
      // tm = localtime(&workTime_t);
      // sprintf(readingTime, "As of %02i:%02i", tm->tm_hour,
      //         tm->tm_min);
      //      breakTime(packetTime, tm);
      //      sprintf(readingTime, "As of %02i:%02i", tm.Hour, tm.Minute);
      // The following 3 should not be necessary but, otherwise, UTC used.
      time(&UTC); workTime_t = UTC + iHomeOffset;
      tm = localtime(&workTime_t);
      sprintf(readingTime, "As of %02i:%02i",
              tm->tm_hour, tm->tm_min);
      Serial.print(readingTime);
      Serial.println(", PHP/USD " + String(PHP_Rate, 2));
    } else {
      pktValidity = false;
      // Serial.println("Bad HTTP return code. Exchange rate packet invalid.");
      // Serial.println("Leaving it as it was until timeout clear or refresh.");
    }
  } else {
    // Serial.println("WiFi not available now. No api fetch possible");
  }
  return pktValidity;
}
/****************************************************************************/
void initTime()
/****************************************************************************/
{
  Serial.print("Time sync interval was ");
  Serial.println(sntp_get_sync_interval());
  sntp_set_sync_interval(86400000);  // 1 day in ms.
  Serial.print("Time sync interval now ");
  Serial.println(sntp_get_sync_interval());

  sntp_set_time_sync_notification_cb(timeSyncCallback);
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", cZulu, 1); tzset();
  Serial.println("Waiting for correct time...");

  strftime(workTime, sizeof(workTime), "%Y", localtime(&workTime_t));
  iYear = atoi(workTime);
  int iLooper = 0;
  while (iYear < 2023) {
    // Serial.println(timeNeedsSync);
    time(&UTC);
    delay(1000);
    strftime (workTime, 100, "%Y", localtime(&UTC));
    iYear = atoi(workTime);
    Serial.println(localtime(&UTC), "UTC %a %m-%d-%Y %T");
    if (iLooper++ > 30) {
      Serial.println("Cannot get time set. Rebooting.");
      ESP.restart();
    }
  }
  Serial.println("\r\nDetermining zone offsets.");
  deduceOffsets();
  time(&UTC);
}
/***************************************************************************/
void deduceOffsets()
/***************************************************************************/
{
  // Calculate Offsets and TZ names for the 3 needed zones: Top, Bottom and Home.

  // Only update the TZ hourly to get the current offset and DST setting.
  // That's really more than necessary but not such a big deal.
  // I was doing it every second.  However, it only takes 32 ms., so... no biggy.
  // It totally saves me from having to figure out DST for myself.
  // I did it once and it was NOT pretty and only applied to the U.S.!
  // I put in the time env string and activate it then see what conditions it creates.
  //  It gives me complete time, the time type (standard or DST) and the seconds offset.
  //  SO much easier than trying to do all of this myself.  It takes care of changing the
  //  time from DST to standard time and I just use that number.  Sweet!
  //  I just have to be sure that the time is valid and I do that by being sure that the
  //  computed year is less than or equal to my birth year (2023).
  //  The format of the offset is a little odd: +/- then 2 digit hours
  //   then 2 digit minutes.  The + applies to the entire thing.

  //  iStartMillis = millis();

  // Get TOP city offset and TZ
  Serial.print(cTopCityname);
  setenv("TZ", cTopTZ, 1); tzset();  // Anybody but me see how silly that 1 is?
  time(&workTime_t);
  tm = localtime(&workTime_t);
  iYear = tm->tm_year + 1900;
  //  Serial.println(tm, " initial set %a, %d-%m-%Y %T %Z %z");
  while (iYear < 2023) {
    delay(1000);
    time(&workTime_t);
    tm = localtime(&workTime_t);
    iYear = tm->tm_year + 1900;
    Serial.print(cTopCityname);
    Serial.println(tm, " waiting %a, %d-%m-%Y %T %Z %z");
    Serial.println(ctime(&workTime_t));
  }
  //  Serial.println(tm, " after waiting %a, %d-%m-%Y %T %Z %z");
  Serial.print(tm, " %a, %d-%m-%Y %T %Z %z");

  //  strftime (cTopDST, 10, "%Z", tm);
  strftime (workTime, sizeof(workTime), "%z", tm);
  iTempOffset = atoi(workTime);
  iTopOffset = (iTempOffset / 100) * 3600 + iTempOffset % 100 * 60;
  //  Serial.print(cTopCityname);
  Serial.printf(", offset = %+i seconds.\r\n", iTopOffset);

  // Get BOTTOM city offset and TZ
  Serial.print(cBotCityname);
  setenv("TZ", cBotTZ, 1); tzset();
  time(&workTime_t);
  tm = localtime(&workTime_t);
  iYear = tm->tm_year + 1900;
  //  Serial.println(tm, " initial set %a, %d-%m-%Y %T %Z %z");
  while (iYear < 2023) {
    delay(1000);
    time(&workTime_t);
    tm = localtime(&workTime_t);
    iYear = tm->tm_year + 1900;
    Serial.print(cBotCityname);
    Serial.println(tm, " waiting %a, %d-%m-%Y %T %Z %z");
    Serial.println(ctime(&workTime_t));
  }
  //  Serial.println(tm, " after waiting %a, %d-%m-%Y %T %Z %z");
  Serial.print(tm, " %a, %d-%m-%Y %T %Z %z");

  //  strftime (cBotDST, 10, "%Z", tm);
  strftime (workTime, sizeof(workTime), "%z", tm);
  iTempOffset = atoi(workTime);
  iBotOffset = (iTempOffset / 100) * 3600 + iTempOffset % 100 * 60;
  //  Serial.print(cBotCityname);
  Serial.printf(", offset = %+i seconds.\r\n", iBotOffset);

  // Get LOCAL time offset and TZ.
  // May be be the same as either Top or Bottom but need not be.
  // Top and Bottom times are used on the Dual Time Zones screen.
  //  Home time on all others.
  Serial.print("Home Time");
  setenv("TZ", cHomeTZ, 1); tzset();
  time(&workTime_t);
  tm = localtime(&workTime_t);
  iYear = tm->tm_year + 1900;
  //  Serial.println(tm, " initial set %a, %d-%m-%Y %T %Z %z");
  while (iYear < 2023) {
    delay(1000);
    time(&workTime_t);
    tm = localtime(&workTime_t);
    iYear = tm->tm_year + 1900;
    Serial.print("Home Time");
    Serial.println(tm, " waiting %a, %d-%m-%Y %T %Z %z");
  }
  //  Serial.println(tm, " after waiting %a, %d-%m-%Y %T %Z %z");
  Serial.print(tm, " %a, %d-%m-%Y %T %Z %z");
  // Characters of time zone and DST indicator.
  //  strftime (cHomeDST, 10, "%Z", tm);
  // Seconds offset from UTC.
  strftime (workTime, sizeof(workTime), "%z", localtime(&workTime_t));
  iTempOffset = atoi(workTime);
  iHomeOffset = (iTempOffset / 100) * 3600 + iTempOffset % 100 * 60;
  //  Serial.print("Home Time");
  Serial.printf(", offset = %+i seconds.\r\n", iHomeOffset);

  // This MUST be done and MUST be last.  The local time(s) are based off of this.
  //  They are not kept separately, only created when needed by adding the offset
  //  to UTC. They can be negative of positive so always add.

  Serial.print("UTC/Zulu");
  setenv("TZ", cZulu, 1); tzset();
  time(&UTC);
  tm = localtime(&UTC);
  iYear = tm->tm_year + 1900;
  //  Serial.println(tm, ""UTC/Zulu initial set %a, %d-%m-%Y %T %Z %z");
  while (iYear < 2023) {
    delay(1000);
    time(&UTC);  // was now.  Not sure if it matters.  Maybe only on Dec 31/Jan 1st.
    tm = localtime(&UTC);
    iYear = tm->tm_year + 1900;
    Serial.println(tm, "UTC/Zulu waiting %a, %d-%m-%Y %T %Z %z");
  }
  // Serial.println(tm, " after waiting %a, %d-%m-%Y %T %Z %z");
  Serial.println(tm, " %a, %d-%m-%Y %T %Z %z");

  // It has been taking about 32-33ms to run this routine.
  //  Serial.printf("Total time for computing offsets %lu ms.\r\n", millis() - iStartMillis);

  // This is the default time for a lot of things.  Set it here as a favor.
  // Some routines will reset it for their own use. UTC is not changed.
  workTime_t = UTC + iHomeOffset;  // The default display time.
  tm = localtime(&workTime_t);
}
/***************************************************************************/
void timeSyncCallback(struct timeval *tv)
/***************************************************************************/
{
  //  struct timeval {
  //   time_t      tv_sec;   // Number of whole seconds of elapsed time
  //   long int    tv_usec;  // Number of microseconds of rest of elapsed time minus tv_sec.
  //                             Always less than one million
  //};
  Serial.println("\n----Time Sync Obtained-----");
  Serial.printf("Time sync at %u ms.\r\nUTC Epoch: ", millis());
  Serial.println(tv->tv_sec);
  Serial.println(ctime(&tv->tv_sec));
  time(&UTC);
}
