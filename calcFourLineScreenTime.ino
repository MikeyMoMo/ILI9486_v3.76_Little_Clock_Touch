/* To use this routine, copy these to the top of your main program. That
   will make them global and you can access them from anywhere.

  fourLineScreenTime

*/
/***************************************************************************/
void calcFourLineScreenTime()
/***************************************************************************/
{
  int tempHour = tm->tm_hour;
  if (tempHour > 11) {
    myIsPM = true;
  } else {
    myIsPM = false;
  }
  if (showAMPM)
  {
    if (tempHour == 0) tempHour += 12;
    if (tempHour > 12) tempHour -= 12;
    sprintf(fourLineScreenTime, "%2i", tempHour);
  } else {
    sprintf(fourLineScreenTime, "%02d", tempHour);
  }
  strcat(fourLineScreenTime, ":");
  //  sprintf(workDate, "%02i", tm.tm_min);
  sprintf(workDate, "%02i", tm->tm_min);
  strcat(fourLineScreenTime, workDate); strcat(fourLineScreenTime, ":");
  sprintf(workDate, "%02i", tm->tm_sec);
  strcat(fourLineScreenTime, workDate);

  if (showAMPM) {
    //    if (myIsPM) {
    if (tm->tm_hour > 11) {
      sprintf(workDate, "%s", " PM");
      strcat(fourLineScreenTime, workDate);
    } else {
      sprintf(workDate, "%s", " AM");
      strcat(fourLineScreenTime, workDate);
    }
  }
}
/***************************************************************************/
static int isLeapYear(unsigned int year)
/***************************************************************************/
{ // Must be divisible by 4 but not by 100, unless it is divisible by 400
  return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}
/***************************************************************************/
unsigned int calcJulian(unsigned int myyear, unsigned int mymonth, unsigned int myday)
/***************************************************************************/
{
  // Returns the number of days since 1 Jan. 4713 BCE.
  if (!valiDATE(mymonth, myday, myyear)) return (0);

  myyear += 8000;
  if (mymonth < 3) {
    myyear--;
    mymonth += 12;
  }
  return (myyear * 365) + (myyear / 4) - (myyear / 100) + (myyear / 400) - 1200820
         + (mymonth * 153 + 3) / 5 - 92
         + myday - 1
         ;
}
/***************************************************************************/
bool valiDATE(unsigned int month, unsigned int day, unsigned int year)
/***************************************************************************/
{
  unsigned int flag = 0;
  unsigned int days = 0;

  // Validate the month first
  if (month < 1 || month > 12) {
    Serial.printf ("\r\nValue for month %i is out of range.\r\n", month);
    return false;
  }

  // Validate the day of month
  days = days_in_month(month, year);

  if (day < 1 || day > days) {
    Serial.printf ("\r\nValue for day %i is out of range.\r\n", day);
    return false;
  }

  // Validate the year
  if (year < 1990 || year > 2030) {
    Serial.printf ("\r\nValue for year %i is out of range.\r\n", year);
    return false;
  }
  return true;  // If we got here, it is good!
}
/***************************************************************************/
unsigned int days_in_month(unsigned int month, unsigned int year)
/***************************************************************************/
{
  unsigned int days = 0;

  if (month == 2) {
    days = 28 + leap(year);  // function leap returns 1 for leap years
  }
  else if (month == 4 || month == 6 || month == 9 || month == 11) {
    days = 30;
  }
  else {
    days = 31;
  }
  return days;
}
/***************************************************************************/
unsigned int leap(unsigned int year)
/***************************************************************************/
{
  if (year % 400 == 0)
    return 1;
  else if (year % 100 != 0 && year % 4 == 0)  //
    return 1;
  else
    return 0;
}
/***************************************************************************/
int WhatDOWis(int m, int d, int y)  // What is the day of the week
/***************************************************************************/
{
  int weekDay = (d += m < 3 ? y-- : y - 2, 23 * m / 9 + d + 4 + y / 4 - y / 100 + y / 400) % 7;
  return (weekDay);
}
/***************************************************************************/
unsigned int Find_SS_Wed(int m, int y)
/***************************************************************************/
{
  int testDay;
  for (testDay = 1; testDay < 15; testDay++)   // Rude and crude but it works fast!
    if (WhatDOWis(m, testDay, y) == 3) break; // Stop looking when Wednesday(3) found.
  if (myBirthdate < 11)
    return (testDay + 7);   // Now have DOM for second Wednesday.
  if (myBirthdate < 21)
    return (testDay + 14);  // Now have DOM for second Wednesday.
  return (testDay + 21);
}
