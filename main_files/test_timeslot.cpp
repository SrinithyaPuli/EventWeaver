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

    return 0;
}