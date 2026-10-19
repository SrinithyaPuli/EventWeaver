#include <iostream>
#include "../timeslot/timeslot.h"
using namespace std;

int main()
{
    TimeSlot t1(26, 10, 2026, 9, 0, 11, 0);
    TimeSlot t2(26, 10, 2026, 10, 0, 12, 0);
    TimeSlot t3(27, 10, 2026, 9, 0, 11, 0);

    cout << "Testing TimeSlot Class\n\n";

    cout << "1. Valid date: "
         << (t1.isValidDate() ? "PASS" : "FAIL") << endl;

    cout << "2. Valid time: "
         << (t1.isValidTime() ? "PASS" : "FAIL") << endl;

    cout << "3. Overlapping slots: "
         << (t1.isOverlapping(t2) ? "PASS" : "FAIL") << endl;

    cout << "4. Non-overlapping dates: "
         << (!t1.isOverlapping(t3) ? "PASS" : "FAIL") << endl;

    cout << "5. Equality operator: "
         << ((t1 == t1) ? "PASS" : "FAIL") << endl;

    TimeSlot invalidDate(31, 2, 2026, 9, 0, 10, 0);
    cout << "6. Invalid date detection: "
         << (!invalidDate.isValidDate() ? "PASS" : "FAIL") << endl;

    TimeSlot invalidTime(26, 10, 2026, 14, 0, 13, 0);
    cout << "7. Invalid time detection: "
         << (!invalidTime.isValidTime() ? "PASS" : "FAIL") << endl;
     // Edge Case 1: Valid leap year
TimeSlot leapYear(29, 2, 2024, 9, 0, 10, 0);
cout << "8. Leap year date: "
     << (leapYear.isValidDate() ? "PASS" : "FAIL") << endl;

// Edge Case 2: Invalid leap year date
TimeSlot invalidLeap(29, 2, 2026, 9, 0, 10, 0);
cout << "9. Invalid leap year date: "
     << (!invalidLeap.isValidDate() ? "PASS" : "FAIL") << endl;

// Edge Case 3: Invalid month
TimeSlot invalidMonth(10, 13, 2026, 9, 0, 10, 0);
cout << "10. Invalid month: "
     << (!invalidMonth.isValidDate() ? "PASS" : "FAIL") << endl;

// Edge Case 4: Hour out of range
TimeSlot invalidHour(26, 10, 2026, 24, 0, 25, 0);
cout << "11. Invalid hour: "
     << (!invalidHour.isValidTime() ? "PASS" : "FAIL") << endl;

// Edge Case 5: Minute out of range
TimeSlot invalidMinute(26, 10, 2026, 9, 60, 10, 0);
cout << "12. Invalid minute: "
     << (!invalidMinute.isValidTime() ? "PASS" : "FAIL") << endl;

// Edge Case 6: Start and end times are equal
TimeSlot equalTime(26, 10, 2026, 9, 0, 9, 0);
cout << "13. Equal start and end time: "
     << (!equalTime.isValidTime() ? "PASS" : "FAIL") << endl;

// Edge Case 7: One slot ends when another begins
TimeSlot slotA(26, 10, 2026, 9, 0, 10, 0);
TimeSlot slotB(26, 10, 2026, 10, 0, 11, 0);
cout << "14. Slots touching at boundary: "
     << (!slotA.isOverlapping(slotB) ? "PASS" : "FAIL") << endl;
TimeSlot invalidSlot(26, 10, 2026, 14, 0, 13, 0);
TimeSlot validSlot(26, 10, 2026, 12, 0, 15, 0);

cout << "15. Invalid slot overlap check: "
     << (!invalidSlot.isOverlapping(validSlot) ? "PASS" : "FAIL")
     << endl;
    return 0;
}