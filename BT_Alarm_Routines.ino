//Note: Internally, alarms go from 0 to 19.  For printing, add 1 for human consumption.

#define helpLines 13  // Inputting ? or h/H via BT or Serial will bring up these help lines
#define BTxmitWait 100  // Time between lines to let transmission catch up.
String helpString[helpLines] = {
  "The following commands are recognized:",
  "A - Acknowledge alarm (stop flashing)",
  "C - Cancel Alarm: C,Time",
  "L - List active alarms",
  "S - Set Alarm: S,Time,SMTWHFA,N,R,Message",
  "Notes:",
  "If days of the week is blank, no repeat",
  "H for Thursday, A for Saturday.",
  "N is # minutes to flash (default 1)",
  "R parm not null means repeat",
  "X and X again clears all alarms",
  "Message up to 100 bytes",
  "? or H for help",
};
#define MAX_ALARMS 20
//struct structAlarms {
//  bool aActive;
//  int  aHr;
//  int  aMin;
//  int  alarmDays
//  int  aFlashMins;
//  bool aRepeat;
//  char aMessage[100];
//} lcAlarms[MAX_ALARMS];
struct AlarmSort {
  int  AlarmNo;
  int  aHr;
  int  aMin;
  //  int  alarmDays;
  //  int  aFlashMins;
  //  int  aRepeat;
  //  char aMessage[100];
};
String sMessage;
#define AlarmSun 0x01
#define AlarmMon 0x02
#define AlarmTue 0x04
#define AlarmWed 0x08
#define AlarmThu 0x10
#define AlarmFri 0x20
#define AlarmSat 0x30

#define chrBufSize 250
bool alarmFlashing = false;
char cUserReq;
char inStr[chrBufSize];
char workStr[chrBufSize];
char aMessage[100];
int  inStrCt = 0, mySetTime;
char *ptr;
byte alarmDays;
int  aFlashMins, aRepeat;
int aHr, aMin;  // This is the real time for setting or listing
int cHr, cMin;  // This is a temp time for comparing to aHr and aMin values.
char prevCmd = '\0';
bool whichInput;
bool BTin = true;
bool SerialIn = false;

//The following is the name of the preferences areas for storing alarm info.
const char* Alarms[] = {"Alarm01", "Alarm02", "Alarm03", "Alarm04", "Alarm05",
                        "Alarm06", "Alarm07", "Alarm08", "Alarm09", "Alarm10",
                        "Alarm11", "Alarm12", "Alarm13", "Alarm14", "Alarm15",
                        "Alarm16", "Alarm17", "Alarm18", "Alarm19", "Alarm20"
                       };
/***************************************************************************/
void handleUserReq()
/***************************************************************************/
{
  /*  if (SerialBT.available()) {
      cUserReq = toupper(SerialBT.read());  // Read command character
      Serial.println("SerialBT Input");
      whichInput = BTin;
    }
    else */ if (Serial.available()) {
    cUserReq = toupper(Serial.read());  // Read command character
    if (cUserReq == 0x0a) return;
    Serial.printf("Serial input byte: %X\r\n", cUserReq);
    whichInput = SerialIn;
  }
  else
    return;  // If nothing, false alarm.

  Serial.print("Command char read: "); Serial.println(cUserReq);
  delay(50);  // Wait for all of the input. (May not be necessary)


  // Clear buffer and reset pointer.
  memset(inStr, 0, chrBufSize); inStrCt = 0;

  //  if (whichInput == BTin) {
  //    while (SerialBT.available()) {
  //      inStr[inStrCt++] = SerialBT.read(); delay(20);
  //      if (inStrCt == chrBufSize) {
  //        Serial.println("BT Input line too long. Ignored.");
  //        return;
  //      }
  //    }
  //  }
  while (Serial.available()) {
    inStr[inStrCt++] = Serial.read(); delay(20);
    if (inStrCt == chrBufSize) {
      Serial.println("Serial Input line too long. Ignored.");
      return;
    }
  }

  if (inStrCt > 0) {
    Serial.printf("%i characters read from BT\r\n", inStrCt);
    Serial.print("BT input: "); Serial.println(inStr);
  }

  time(&UTC); workTime_t = UTC + iHomeOffset; 
  tm = localtime(&workTime_t);
  switch (cUserReq)
  {
    // Show Help screen on H or ?
    case 'H': case '?':
      Serial.println(); Serial.print(ctime(&workTime_t));
      Serial.printf(" vers %.2f\r\n", Vers);
      for (int temp = 0; temp < helpLines; temp++) {
        Serial.println(helpString[temp]);
      }
      break;
    // Set the stop bool when A is entered
    case 'A':
      Serial.println("\r\nA - Acknowledge alarm (stop flashing)");
      if (alarmFlashing)
        StopAlarm();
      else {
        Serial.println("\r\nNo Alarm flashing now."); delay(BTxmitWait);
      }
      break;
    // Cancel an alarm
    case 'C':
      Serial.println("\r\nC - Cancel Alarm: C,Time"); delay(BTxmitWait);
      CancelAlarm();
      break;

    // List active alarms
    case 'L':
      ListAlarms();
      break;

    // Set an alarm
    case 'S':
      if (whichInput == BTin) {
        //        SerialBT.println("\r\nS - Set Alarm: S,Time,SMTWHFS,N,R,Message");
        delay(BTxmitWait);
      }
      Serial.println("\r\nS - Set Alarm: S,Time,SMTWHFS,N,R,Message");
      SetAlarm();
      break;
    // Clear all memories.  Takes entering X twice in a row.
    case 'X':
      if (prevCmd == 'X') {
        if (whichInput == BTin) {
          //          SerialBT.println("\r\nWill clear all memories now.");
          delay(BTxmitWait);
        }
        Serial.println("\r\nWill clear all memories now.");
        for (int i = 0; i < MAX_ALARMS; i++) {
          preferences.begin(Alarms[i], false);
          if (preferences.getBool("aActive", false)) {
            if (whichInput == BTin) {
              //              SerialBT.printf("Turning off alarm %i\r\n", i + 1);
              delay(BTxmitWait);
            }
            Serial.printf("Turning off alarm %i\r\n", i + 1);
            preferences.putBool("aActive", false);
          }
        }
        preferences.end();
      } else {
        if (whichInput == BTin) {
          //          SerialBT.println("\r\nEnter X as the next input to clear alarms.");
          delay(BTxmitWait);
        }
        Serial.println("\r\nEnter X as the next input to clear alarms.");
      }
      break;
    default:
      if (whichInput == BTin) {
        //        SerialBT.println("\r\nCommand not recognized");
        delay(BTxmitWait);
      }
      Serial.println("\r\nCommand not recognized");
      //Missing "End Switch" causing double outdent.
  }
  prevCmd = cUserReq;  // Remember the previous command in case of X
}
/***************************************************************************/
void SetAlarm()
/****SetAlarm***************************************************************/
{
  //  ShowAStatus();

  int alarmLoc, i;

  if (whichInput == BTin) {
    //    SerialBT.println("Preparing to set alarm."); delay(BTxmitWait);
  }
  Serial.println("Preparing to set alarm.");

  //First, get rid of that leading comma, if present.
  if (inStr[0] == ',')
    strcpy(inStr, inStr + 1);
  else {
    if (whichInput == BTin) {
      //      SerialBT.println("No comma after command.  Malformed expression. Exiting.");
      //      delay(BTxmitWait);
    }
    Serial.println("No comma after command.  Malformed expression. Exiting.");
    return;
  }
  Serial.print("After comma removal: "); Serial.println(inStr);

  // Find the Time parm.  Required!
  ptr = strtoke(inStr, ",");
  if (strlen(ptr) == 0) {
    if (whichInput == BTin) {
      //      SerialBT.println("Malformed expression looking for Time");
      //      delay(BTxmitWait);
    }
    Serial.println("Malformed expression looking for Time");
    return;
  }
  Serial.printf("Time req: %s\r\n", ptr);
  if (strlen(ptr) > 4 || strlen(ptr) == 0) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Value input for time (%s) is too long.\r\n"
      //                      "Maximum length is 4 characters.\r\n", ptr);
      //      delay(BTxmitWait);
    }
    Serial.printf("Value input for time ( %s) is long. "
                  "Maximum length is 4 characters.\r\n", ptr);
    return;
  }
  if (!isNumber(ptr)) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Value input for time (%s) is not numeric.\r\n"
      //                      " Cannot continue.\r\n", ptr); delay(BTxmitWait);
      delay(BTxmitWait);
    }
    Serial.printf("Value input for time (%s) is not numeric."
                  " Cannot continue.\r\n", ptr);
    return;
  }
  mySetTime = atoi(ptr);
  aHr = mySetTime / 100; aMin = mySetTime - (aHr * 100);
  if (aHr > 23 || aMin > 60) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Value entered for time (%s) is invalid.\r\n"
      //                      "0000-2399 only.\r\n", ptr);
      delay(BTxmitWait);
    }
    Serial.printf("Value input for time (%s) is invalid. "
                  "0000-2399 only.\r\n", ptr);
    return;
  }
  Serial.printf("Alarm time to be added is %i:%02i\r\n", aHr, aMin);

  // Now, look for a duplicate already there with the same time.
  for (i = 0; i < MAX_ALARMS; i++) {
    Serial.printf("Opening preference %s\r\n", Alarms[i]);
    preferences.begin(Alarms[i], false);
    //    Serial.printf("1-Alarm %02i is %s\r\n", i + 1,
    //                  preferences.getBool("aActive", false) ? "in use" : "available");
    if (preferences.getBool("aActive", false)) {
      cHr = preferences.getInt("aHr", -1);
      cMin = preferences.getInt("aMin", -1);
      Serial.printf("Found Hr %i, Min %i in alarm %i\r\n", cHr, cMin, i + 1);
      if ((cHr == aHr) && (cMin == aMin)) {
        Serial.println("The time entered is already active.\r\n"
                       "Not setting a duplicate.");
        if (whichInput == BTin) {
          //          SerialBT.println("The time entered is already active.\r\n"
          //                           "Not setting a duplicate.");
          delay(BTxmitWait);
        }

        return;
      }
    }
    preferences.end();
  }
  Serial.println("Time is unique. Continuing.");

  // Find the Days parm.  Can be no characters or up to 7, max.
  ptr = strtoke(NULL, ",");
  Serial.printf("Days parm found: %s\r\n", ptr);
  if (strlen(ptr) == 0)
    if (whichInput == BTin) {
      //      SerialBT.println("No days found. Will only alarm as the next time match.\r\n");
      delay(BTxmitWait);
    }
  Serial.println("No days found. Will only alarm as the next time match.\r\n");
  if (strlen(ptr) > 7) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Too many day specifiers (%i). Aborting alarm setting.\r\n",
      //                      strlen(ptr));
      delay(BTxmitWait);
    }
    Serial.printf("Too many day specifiers (%i). Aborting alarm setting.\r\n",
                  strlen(ptr));
    return;
  }
  //  memset(workStr, 0, chrBufSize);
  //  for (i = 0; i < strlen(ptr); i++)
  //    workStr[i] = toupper(ptr[i]);
  //  Serial.printf("Days : %s\r\n", ptr);
  alarmDays = 0;  // Clear all bits

  for (i = 0; i < strlen(ptr); i++) {
    // Looking for SMTWHFA  H for Thursday, A for Saturday
    switch (ptr[i])
    {
      case 'S': case 's':
        alarmDays |= AlarmSun; Serial.println("Found Sunday"); break;
      case 'M': case 'm':
        alarmDays |= AlarmMon; Serial.println("Found Monday"); break;
      case 'T': case 't':
        alarmDays |= AlarmTue; Serial.println("Found Tuesday"); break;
      case 'W': case 'w':
        alarmDays |= AlarmWed; Serial.println("Found Wednesday"); break;
      case 'H': case 'h':
        alarmDays |= AlarmThu; Serial.println("Found Thursday "); break;
      case 'F': case 'f':
        alarmDays |= AlarmFri; Serial.println("Found Friday"); break;
      case 'A': case 'a':
        alarmDays |= AlarmSat; Serial.println("Found Saturday"); break;
      default:
        if (whichInput == BTin) {
          //          SerialBT.printf("Unknown day specifier %s\r\n", ptr[i]);
          delay(BTxmitWait);
        }
        //        SerialBT.printf("Unknown day specifier %s\r\n", ptr[i]);
        return;
    }
    Serial.printf("alarmDays bit coded byte is: %X\r\n", alarmDays);
  }

  //Find the Flash Minutes count.  Can be empty, default to 1.
  ptr = strtoke(NULL, ",");
  if (strlen(ptr) == 0) {
    if (whichInput == BTin) {
      //      SerialBT.println("No minute specified.  Using default of 1 minute.");
      delay(BTxmitWait);
    }
    Serial.println("No minute specified.  Using default of 1 minute.");
    aFlashMins = 1;
  } else {
    if (isNumber(ptr)) {
      aFlashMins = atoi(ptr);
      Serial.printf("Flash mins set to: %i\r\n", aFlashMins);
    } else {
      if (whichInput == BTin) {
        //        SerialBT.print("Flash minutes invalid.  Cannot continue.");
        delay(BTxmitWait);
      }
      Serial.print(("Flash minutes invalid.  Cannot continue."));
      return;
    }
  }

  //Find the Repeat parm.  Can be missing for no repeat.
  ptr = strtoke(NULL, ",");
  if (strlen(ptr) == 0) {
    if (whichInput == BTin) {
      //      SerialBT.println("Repeats parm is null.  Will not repeat.");
      delay(BTxmitWait);
      aRepeat = 0;
    }
  } else {
    if (whichInput == BTin) {
      //      SerialBT.println("Repeats parm not null.  Will repeat till cancelled.");
      delay(BTxmitWait);
      aRepeat = 1;
    }
  }
  //Find the message.  Required!
  ptr = strtoke(NULL, ",");
  //  Serial.printf("Penultimate condition %s\r\n", ptr);
  if (strlen(ptr) == 0) {
    if (whichInput == BTin) {
      //      SerialBT.println("No message specified.  Alarm will not be set.");
      delay(BTxmitWait);
    }
    Serial.println("No message specified.  Alarm will not be set.");
    return;
  } else {
    if (strlen(ptr) > 100) {
      if (whichInput == BTin) {
        //    SerialBT.printf("Message length (%i) .GT. 100. Revise. Cannot continue.\r\n",
        //                    strlen(ptr));
        delay(BTxmitWait);
      }
      Serial.printf("Message length (%i) .GT. 100. Revise. Cannot continue.\r\n",
                    strlen(ptr));
      return;
    }
    Serial.printf("Message: %s\r\n", ptr);
    strcpy(aMessage, ptr);
  }

  //  Serial.printf("Ending string contents %s\r\n", ptr);
  ptr = strtoke(NULL, ",");
  if (ptr == NULL)
    Serial.println("String parse ended successfully");
  else {
    if (whichInput == BTin) {
      //      SerialBT.println("String did not end successfully");
      delay(BTxmitWait);
    }
    Serial.println("String did not end successfully");
    return;
  }

  // Well, we got to the end.  Any error stops the routine, so...
  //  everything must be OK. Go ahead and stuff the data into preferences.

  // Look for a random location to store this alarm.
  alarmLoc = FindAlarmLoc();
  if (alarmLoc < 0) {
    if (whichInput == BTin) {
      //      SerialBT.print("No alarm available.\r\nDelete one then add a new one.");
      delay(BTxmitWait);
    }
    Serial.print("No alarm available.\r\nDelete one then add a new one.");
    return;
  }
  Serial.printf("Opening Alarm preferences file %i: %s\r\n",
                alarmLoc + 1, Alarms[alarmLoc]);
  preferences.begin(Alarms[alarmLoc], false);
  preferences.putInt("aHr", aHr);
  preferences.putInt("aMin", aMin);
  preferences.putInt("alarmDays", alarmDays);
  preferences.putInt("aFlashMins", aFlashMins);
  preferences.putInt("aRepeat", aRepeat);
  preferences.putString("aMessage", aMessage);
  Serial.println("And, finally, setting the alarm active.");
  preferences.putBool("aActive", true);
  if (whichInput == BTin) {
    //    SerialBT.printf("Alarm %i set.\r\n", alarmLoc + 1);
    delay(BTxmitWait);
  }
  Serial.printf("Alarm %i set.\r\n", alarmLoc + 1); delay(BTxmitWait);
  preferences.end();
}
/***************************************************************************/
void ListAlarms()
/***************************************************************************/
{
  int i;

  //  ShowAStatus();

  if (whichInput == BTin) {
    //    SerialBT.println();
    delay(BTxmitWait);
  }
  AlarmSort SortAlarms[MAX_ALARMS];
  int AlarmsCt = 0;

  for (int i = 0; i < MAX_ALARMS; i++) {
    //    Serial.printf("Opening preference %s\r\n", Alarms[i]);
    preferences.begin(Alarms[i], false);
    //    Serial.printf("5-Alarm %02i is %s\r\n", i + 1,
    //                  preferences.getBool("aActive", false) ? "in use" : "available");
    if (preferences.getBool("aActive", false)) {
      aHr = preferences.getInt("aHr", 50);
      aMin = preferences.getInt("aMin", 50);
      if (whichInput == BTin) {
        //        SerialBT.printf("Alarm %02i is active - Time: %02i:%02i\r\n",
        //                        i + 1, aHr, aMin);
        delay(BTxmitWait);
      }
      Serial.printf("Alarm %02i is active - Time: %02i:%02i\r\n",
                    i + 1, aHr, aMin);
      SortAlarms[AlarmsCt].AlarmNo = i;
      SortAlarms[AlarmsCt].aHr = preferences.getInt("aHr");
      SortAlarms[AlarmsCt].aMin = preferences.getInt("aMin");
      AlarmsCt++;
    }
    preferences.end();
  }
  Serial.printf("%i active alarms found.\r\n", AlarmsCt);

  if (AlarmsCt == 0) {  // Nothing to do here.  Squeal and run home!
    if (whichInput == BTin) {
      //      SerialBT.println("No alarms are set.");
      delay(BTxmitWait);
    }
    Serial.println("No alarms are set.");
    return;
  } else {
    if (whichInput == BTin) {
      //      SerialBT.printf("%i alarms are set.\r\n", AlarmsCt);
      delay(BTxmitWait);
    }
    Serial.printf("%i alarms are set.\r\n", AlarmsCt);
  }

  // Now print out what we found.
  if (AlarmsCt == 1) {
    if (whichInput == BTin) {
      //      SerialBT.printf("1 alarm is active\r\n", AlarmsCt); delay(BTxmitWait);
      //      SerialBT.printf("It is set for %i:%02i\r\n",
      //                      SortAlarms[0].aHr, SortAlarms[0].aMin);
      delay(BTxmitWait);
    }
    Serial.printf("1 alarm is active\r\n", AlarmsCt); delay(BTxmitWait);
    Serial.printf("It is set for %i:%02i\r\n",
                  SortAlarms[0].aHr, SortAlarms[0].aMin);
    return;
  }
  // Not 0 or 1 so we have enough to sort alarms that were found
  //  and print them.
  Serial.printf("Time to sort %i items.\r\n", AlarmsCt);
  bubbleSort(SortAlarms, AlarmsCt);
  if (whichInput == BTin) {
    //    SerialBT.println("Sorted:");
    delay(BTxmitWait);
  }
  Serial.println("Sorted:"); delay(BTxmitWait);
  for (i = 0; i < AlarmsCt; i++) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Alarm %02i set for %2i:%02i\r\n", i + 1,
      //                      SortAlarms[i].aHr, SortAlarms[i].aMin);
      delay(BTxmitWait);
    }
    Serial.printf("Alarm %02i set for %2i:%02i\r\n", i + 1,
                  SortAlarms[i].aHr, SortAlarms[i].aMin);
  }
}
/***************************************************************************/
int FindAlarmLoc()
/***************************************************************************/
{
  time(&UTC); workTime_t = UTC + iHomeOffset; 
  tm = localtime(&workTime_t);
  const int loopCt = 30;
  bool tested[MAX_ALARMS];
  int i, myRand;
  randomSeed(tm->tm_hour + tm->tm_min + tm->tm_sec + millis());
  for (i = 0; i < MAX_ALARMS; i++) tested[i] = false;
  for (i = 0; i < loopCt; i++) {
    // Try arc4random
    myRand = random(MAX_ALARMS);
    Serial.printf("Trying random selection %i\r\n", myRand + 1);
    if (tested[myRand]) {
      Serial.printf("Alarm loc %i has already been tested. Skipping.\r\n",
                    myRand + 1);
      continue;
    }
    tested[myRand] = true;
    preferences.begin(Alarms[myRand], false);
    Serial.printf("3-Alarm %02i is %s\r\n", myRand + 1,
                  preferences.getBool("aActive", false) ? "in use" : "available");
    if (!preferences.getBool("aActive", false))
    {
      Serial.printf("Found available alarm %i on random try %i\r\n",
                    myRand + 1, i);
      return (myRand);
    }
  }

  // Well, we got here after 30 quick hits.  Bad luck, dude!
  //  Now, start at the top and look for something.  If none, return -1.
  Serial.println("Fell through after 30 randoms.  Looking from 0.");
  for (i = 0; i < MAX_ALARMS; i++) {
    preferences.begin(Alarms[i], false);
    if (!preferences.getBool("aActive", false))
    {
      Serial.printf("Found available alarm %i\r\n", i + 1);
      return (i);
    } else {
      Serial.printf("4-Alarm %02i is %s\r\n", i + 1,
                    preferences.getBool("aActive", false) ?
                    "in use" : "available");
    }
  }
  Serial.println("No available alarm slots.");
  return (-1);
}
/***************************************************************************/
void CancelAlarm()
/***************************************************************************/
{
  if (whichInput == BTin) {
    //    SerialBT.println("\r\nPreparing to cancel alarm.");
    delay(BTxmitWait);
  }
  Serial.println("Preparing to cancel alarm.");

  //First, get rid of that leading comma, if present.
  if (inStr[0] == ',')
    strcpy(inStr, inStr + 1);
  else {
    Serial.println("No leading comma.  Malformed expression. Exiting.");
    return;
  }
  Serial.print("After possible comma removal: "); Serial.println(inStr);

  //Find the Time parm.  Required!
  ptr = strtoke(inStr, ", ");
  if (strlen(ptr) == 0) {
    Serial.println("String did not end up right looking for Time");
    return;
  }
  //  Serial.printf("Time : %s\r\n", ptr);
  if (strlen(ptr) > 4 || strlen(ptr) == 0) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Value input for time ( %s) is long.\r\n"
      //                      "Maximum length is 4 characters.\r\n", ptr);
      delay(BTxmitWait);
    }
    Serial.printf("Value input for time ( %s) is long. "
                  "Maximum length is 4 characters.\r\n", ptr);
    return;
  }
  if (!isNumber(ptr)) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Value input for time ( %s) is not numeric.\r\n"
      //                      " Cannot continue.\r\n", ptr);
      delay(BTxmitWait);
    }
    Serial.printf("Value input for time ( %s) is not numeric."
                  " Cannot continue.\r\n", ptr);
    return;
  }
  aHr = atoi(ptr) / 100; aMin = atoi(ptr) - (aHr * 100);
  if (aHr > 23 || aMin > 60) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Value input for time ( %s) is invalid.\r\n"
      //                      "0000 - 2399 only.\r\n", ptr);
      delay(BTxmitWait);
    }
    Serial.printf("Value input for time ( %s) is not numeric."
                  " Cannot continue.\r\n", ptr);
    return;
  }
  Serial.printf("Alarm time to be cancelled is %i : %02i\r\n", aHr, aMin);

  ptr = strtoke(inStr, ", ");
  if (ptr == NULL)
    Serial.println("Input parsing ended normally.");
  else
    Serial.println("There seems to be junk beyond the time parm.");

  bool bFound = false;
  for (int i = 0; i < MAX_ALARMS; i++) {
    preferences.begin(Alarms[i], false);
    if (preferences.getBool("aActive", false)) {
      if (aHr == preferences.getInt("aHr", 50) &
          (aMin == preferences.getInt("aMin", 50))) {
        preferences.putBool("aActive", false);
        if (whichInput == BTin) {
          //          SerialBT.printf("Alarm %i for %i : %i cancelled.\r\n",
          //                          i + 1, aHr, aMin);
          delay(BTxmitWait);
        }
        Serial.printf("Alarm %i for %i : %i cancelled.\r\n",
                      i + 1, aHr, aMin); delay(BTxmitWait);
        bFound = true;
      }
    }
  }
  if (!bFound) {
    if (whichInput == BTin) {
      //      SerialBT.printf("Alarm for %i:%i not found. Can't cancel.\r\n",
      //                      aHr, aMin);
      delay(BTxmitWait);
    }
    Serial.printf("Alarm for %i:%i not found. Can't cancel.\r\n",
                  aHr, aMin);
  }
}
/***************************************************************************/
char* strtoke(char *str, const char *delim)
/***************************************************************************/
{
  static char *start = NULL; /* stores string str for consecutive calls */
  char *token = NULL; /* found token */
  /* assign new start in case */
  if (str) start = str;
  /* check whether text to parse left */
  if (!start) return NULL;
  /* remember current start as found token */
  token = start;
  /* find next occurrence of delim */
  start = strpbrk(start, delim);
  /* replace delim with terminator and move start to follower */
  if (start) *start++ = '\0';
  /* done */
  return token;
}
/***************************************************************************/
int isNumber(char s[])
/***************************************************************************/
{
  for (int i = 0; s[i] != '\0'; i++)
    if (isdigit(s[i]) == 0)
      return 0;
  return 1;
}
/***************************************************************************/
void bubbleSort(struct AlarmSort arr[], int n)
/***************************************************************************/
{
  AlarmSort alarmTemp;  // Temp save for swapping in the bubble sort.
  int i, j, minutes1, minutes2;
  Serial.printf("Bubbling up %i items.\r\n", n);
  for (i = 0; i < n - 1; i++) {
    for (j = 0; j < n - i - 1; j++) {
      minutes1 = arr[j].aHr * 60 + arr[j].aMin;
      minutes2 = arr[j + 1].aHr * 60 + arr[j + 1].aMin;
      if (minutes1 > minutes2) {
        alarmTemp = arr[j];
        arr[j] = arr[j + 1];
        arr[j + 1] = alarmTemp;
      }
    }
  }
}
/***************************************************************************/
void StopAlarm() // Acknowledge
/***************************************************************************/
{
  // I expect there will be more to this, soon so I made a separate routine.
  if (whichInput == BTin) {
    //    SerialBT.println("Alarm stopped."); delay(BTxmitWait);
  }
  Serial.println("Alarm stopped."); delay(BTxmitWait);
  alarmFlashing = false;
}
///***************************************************************************/
//void swap(int *xp, int *yp)
///***************************************************************************/
//{
//  int temp = *xp;
//  *xp = *yp;
//  *yp = temp;
//}
///***************************************************************************/
//void ShowAStatus()
///***************************************************************************/
//{
//  AlarmSort SortAlarms[MAX_ALARMS];
//  int AlarmsCt = 0;
//
//  for (int i = 0; i < MAX_ALARMS; i++) {
//    Serial.printf("Opening preference %s\r\n", Alarms[i]);
//    preferences.begin(Alarms[i], false);
//    Serial.printf("5-Alarm %02i is %s\r\n", i + 1,
//                  preferences.getBool("aActive", false) ? "in use" : "available");
//    if (preferences.getBool("aActive", false)) {
//      aHr = preferences.getInt("aHr", 50);
//      aMin = preferences.getInt("aMin", 50);
//      SerialBT.printf("Alarm %02i is active - Time: %02i:%02i\r\n",
//                      i + 1, aHr, aMin);
//      Serial.printf("Alarm %02i is active - Time: %02i:%02i\r\n",
//                    i + 1, aHr, aMin);
//      delay(BTxmitWait);
//      SortAlarms[AlarmsCt].AlarmNo = i;
//      SortAlarms[AlarmsCt].aHr = aHr;
//      SortAlarms[AlarmsCt].aMin = aMin;
//      AlarmsCt++;
//    }
//    preferences.end();
//  }
//  Serial.printf("%i active alarms found.\r\n", AlarmsCt);
//}
// Memory tickler for Mikey
//    memset(workStr, 0, chrBufSize);
//    for (i = 0; i < strlen(ptr); i++)
//      workStr[i] = toupper(ptr[i]);
//    Serial.printf("Repeat : %s\r\n", workStr);
