#include "timeslot.h"

TimeSlot::TimeSlot()
{
    day = 1;
    month = 1;
    year = 2000;
    startHour = 9;
    startMinute = 0;
    endHour = 10;
    endMinute = 0;
}

TimeSlot::TimeSlot(int d, int m, int y,
                   int sh, int sm, int eh, int em)
{
    day = d;
    month = m;
    year = y;
    startHour = sh;
    startMinute = sm;
    endHour = eh;
    endMinute = em;
}

bool TimeSlot::isValidDate() const
{
    if (year < 1 || month < 1 || month > 12 || day < 1)
        return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30,
                         31, 31, 30, 31, 30, 31};

    if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0))
        daysInMonth[1] = 29;

    return day <= daysInMonth[month - 1];
}

bool TimeSlot::isValidTime() const
{
    if (startHour < 0 || startHour > 23 ||
        endHour < 0 || endHour > 23 ||
        startMinute < 0 || startMinute > 59 ||
        endMinute < 0 || endMinute > 59)
        return false;

    int start = startHour * 60 + startMinute;
    int end = endHour * 60 + endMinute;

    return end > start;
}

bool TimeSlot::isOverlapping(const TimeSlot& other) const
{
    if (!isValidDate() || !isValidTime() ||
        !other.isValidDate() || !other.isValidTime())
    {
        return false;
    }
    if (day != other.day || month != other.month ||
        year != other.year)
        return false;

    int start1 = startHour * 60 + startMinute;
    int end1 = endHour * 60 + endMinute;
    int start2 = other.startHour * 60 + other.startMinute;
    int end2 = other.endHour * 60 + other.endMinute;

    return start1 < end2 && start2 < end1;
}

bool TimeSlot::operator==(const TimeSlot& other) const
{
    return day == other.day &&
           month == other.month &&
           year == other.year &&
           startHour == other.startHour &&
           startMinute == other.startMinute &&
           endHour == other.endHour &&
           endMinute == other.endMinute;
}

int TimeSlot::getDay() const { return day; }
int TimeSlot::getMonth() const { return month; }
int TimeSlot::getYear() const { return year; }
int TimeSlot::getStartHour() const { return startHour; }
int TimeSlot::getStartMinute() const { return startMinute; }
int TimeSlot::getEndHour() const { return endHour; }
int TimeSlot::getEndMinute() const { return endMinute; }