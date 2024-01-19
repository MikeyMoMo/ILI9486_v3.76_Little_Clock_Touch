//char   replaced by
// d   Day of the month, 2 digits with leading zero
// D   First three letters of day in English, like Tue
// j   Day of the month without leading zeros
// l   (lowercase L) Day of the week in English, like Tuesday
// N   ISO-8601 numeric representation of the day of the week. (1 = Monday, 7 = Sunday)
// S   English ordinal suffix for the day of the month, 2 characters (st, nd, rd, th)
// w   Numeric representation of the day of the week (0 = Sunday)
// F   A month's name, such as January
// m   Numeric representation of a month, with leading zeros
// M   Three first letters of a month in English, like Apr
// n   Numeric representation of a month, without leading zeros
// t   Number of days in the given month
// Y   A full numeric representation of the year, 4 digits
// y   Last two digits of the year
// a   am or pm
// A   AM or PM
// g   12-hour format of an hour without leading zeros
// G   24-hour format of an hour without leading zeros
// h   12-hour format of an hour with leading zeros
// H   24-hour format of an hour with leading zeros
// i   Minutes with leading zeros 
// s   Seconds with leading zero
// T   abbreviation for timezone, like CEST
// v   milliseconds as three digits
// e   Timezone identifier (Olson name), like Europe/Berlin

// O   Difference to Greenwich time (GMT) in hours and minutes written together, like
//     +0200. Here a positive offset means east of UTC.

// P   Same as O but with a colon between hours and minutes, like +02:00

// Z   Timezone offset in seconds. West of UTC is negative, east of UTC is positive.

// z   The day of the year (starting from 0)

// W   ISO-8601 week number. See right below for explanation link.

// X   ISO-8601 year for year-week notation as four digit year. Warning: Not
//     guaranteed to be same as current year, may be off by one at start or end of year.

// B   One-letter military code for the timezone, or ? if the offset is not a whole
//     number of hours.

// \   Not printed, but escapes the following character, meaning it will not be
//     replaced. But inserting a backslash in the string means you have to supply two
//     backslashes \\ to be interpreted as one.

// ~   (tilde) Same as backslash above, except easier to insert in the string.
//     Example: ~t~h~e will print the word "the" in the string. Letters should be escaped
//     even if they are not on the list because they may be replaced in future versions.

//     So as an example: UTC.dateTime("l ~t~h~e jS ~o~f F Y, g:i A") yields date and time
//     in this format: Saturday the 25th of August 2018, 2:23 PM.
