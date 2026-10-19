#ifndef TIMESLOT_H
#define TIMESLOT_H

class TimeSlot
{
private:
    int day, month, year;
    int startHour, startMinute;
    int endHour, endMinute;

public:
    TimeSlot();
    TimeSlot(int d, int m, int y,
             int sh, int sm, int eh, int em);

    bool isValidDate() const;
    bool isValidTime() const;
    bool isOverlapping(const TimeSlot& other) const;

    bool operator==(const TimeSlot& other) const;

    int getDay() const;
    int getMonth() const;
    int getYear() const;
    int getStartHour() const;
    int getStartMinute() const;
    int getEndHour() const;
    int getEndMinute() const;
};

#endif