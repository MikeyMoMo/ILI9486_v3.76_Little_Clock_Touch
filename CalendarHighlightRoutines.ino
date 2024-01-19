/***************************************************************************/
void InitCalendarHighlights()
/***************************************************************************/
{

  // Note on how this works.  If you want an anniversary and "monthsiversary", place
  //  the anniversary first, with different colors.  It will be found first.  Use a
  //  numbered month on it and a 0 for the month on the monthsiversary.  The annual one
  //  must be listed first. When there is a match, it uses it and exits.  Does not run
  //  the rest of the list.
  // Red for Anniversary, Blue for Birthdays
  String s;
  char tChar[15];
  int event, i, j, temp, arrayEnd = CalHighlightCt - 1;
  bool sorted = false;

  // Be sure that all entries are inactivated before filling -- HLEvents[event].DOM == 0.
  for (event = 0; event < CalHighlightCt; event++) {
    HLEvents[event].DOM = 100;  // Impossible date means inactive
    HLEvents[event].Event[0] = '\0';
  }

  event = 0;
  HLEvents[event].Month = 0; HLEvents[event].DOM = 26;
  HLEvents[event].TextColor = ILI9341_RED; HLEvents[event].RectColor = ILI9341_CYAN;
  s = "Anniversary"; strcpy(HLEvents[event].Event, s.c_str()); event++;

  HLEvents[event].Month = 8; HLEvents[event].DOM = 29;
  HLEvents[event].TextColor = DarkerRed; HLEvents[event].RectColor = ILI9341_BLUE;
  s = "Rafa's Birthday"; strcpy(HLEvents[event].Event, s.c_str()); event++;

  HLEvents[event].Month = 9; HLEvents[event].DOM = 2;
  HLEvents[event].TextColor = DarkerRed; HLEvents[event].RectColor = ILI9341_BLUE;
  s = "Mike's Birthday"; strcpy(HLEvents[event].Event, s.c_str()); event++;

  HLEvents[event].Month = 12; HLEvents[event].DOM = 25;
  HLEvents[event].TextColor = ILI9341_RED; HLEvents[event].RectColor = ILI9341_GREEN;
  s = "Christmas"; strcpy(HLEvents[event].Event, s.c_str()); event++;

  //  HLEvents[event].Month = 11; HLEvents[event].DOM = 9;
  //  HLEvents[event].TextColor = TFT_PURPLE; HLEvents[event].RectColor = ILI9341_YELLOW;
  //  s = "Testing"; strcpy(HLEvents[event].Event, s.c_str()); event++;

  //  Serial.println("Sorting Calendar Highlights");
  for (i = 0; i <= arrayEnd; i++) {
    while (!sorted) {
      sorted = true;
      for (j = 0; j < arrayEnd; j++) {
        //        Serial.printf("j DOM %i, j+1 DOM %i\r\n", HLEvents[j].DOM, HLEvents[j + 1].DOM);
        if (HLEvents[j].DOM > HLEvents[j + 1].DOM) { // switch them
          //          Serial.printf("Swapping %i with %i\r\n", j, j + 1);
          sorted = false;
          temp = HLEvents[j + 1].Month;     HLEvents[j + 1].Month =     HLEvents[j].Month;     HLEvents[j].Month     = temp;
          temp = HLEvents[j + 1].DOM;       HLEvents[j + 1].DOM =       HLEvents[j].DOM;       HLEvents[j].DOM       = temp;
          temp = HLEvents[j + 1].TextColor; HLEvents[j + 1].TextColor = HLEvents[j].TextColor; HLEvents[j].TextColor = temp;
          temp = HLEvents[j + 1].RectColor; HLEvents[j + 1].RectColor = HLEvents[j].RectColor; HLEvents[j].RectColor = temp;
          strncpy(tChar,                 HLEvents[j + 1].Event, 15);
          strncpy(HLEvents[j + 1].Event, HLEvents[j].Event    , 15);
          strncpy(HLEvents[j].Event,     tChar                , 15);
        }
      }
      //      printEvents();
      arrayEnd--;
    }
  }
}
/***************************************************************************/
void printEvents()
/***************************************************************************/
{
  int event;
  Serial.println("-----------------------");
  for (event = 0; event < CalHighlightCt; event++) {
    if (HLEvents[event].DOM < 32) {  // 100 means not used.  32 is less than 100, right?
      Serial.printf("Event\t\t#%i\r\n", event + 1);
      Serial.print("Month(s):\t");
      if (HLEvents[event].Month == 0)
        Serial.println("All");
      else
        Serial.println(HLEvents[event].Month);
      Serial.print("Day of month:\t"); 
      Serial.println(HLEvents[event].DOM);
      Serial.print("Text color:\t");   
      Serial.println(HLEvents[event].TextColor);
      Serial.print("Rect color:\t");   
      Serial.println(HLEvents[event].RectColor);
      Serial.print("Notation:\t");     
      Serial.println(HLEvents[event].Event);
      Serial.println("-----------------------");
    }
  }
}
/***************************************************************************/
bool HighlightToday(int Month, int DOM, int Year, int &RectColor, int &TextColor) {
  /***************************************************************************/
  int testDay, HLStruct;

  time(&UTC); workTime_t = UTC + iHomeOffset; 
  tm = localtime(&workTime_t);
  // Have taken care of today's highlight.  If not a hit, run the structure array looking for other
  //  special days to highlight and return the colors needed.
  for (HLStruct = 0; HLStruct < CalHighlightCt; HLStruct++) {
    if (DOM == HLEvents[HLStruct].DOM) {
      //What I want to do is: If the passed in Month does not match current month, then ignore the
      // past date check and do what you would do if it was a future date.  qwer
      if ((HLEvents[HLStruct].Month == Month) || (HLEvents[HLStruct].Month == 0)) {
        // I want to do highlighting of this date if this month and => today.
        // If a different month, highlight all entries.
        // This one is trying to not highlight special days in the past for the current month only.
        if  ((Month == tm->tm_mon) && (tm->tm_mday <= DOM) || (Month != tm->tm_mon)) {
          if (tm->tm_mday == DOM)  // If the event is today, override the user text color only.
            TextColor = ILI9341_YELLOW;
          else
            TextColor = HLEvents[HLStruct].TextColor;
          RectColor = HLEvents[HLStruct].RectColor;
          return (true);
        } else {

        }
      }
    }
  }
  if ((tm->tm_mday == DOM) && (Month == tm->tm_mon)) { // Don't highlight today's day in another month.
    RectColor = ILI9341_YELLOW; TextColor = DarkerRed;
    return (true);
  }
  return (false);
}
