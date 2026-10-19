#include <iostream>
#include "participant.h"
using namespace std;

int main()
{
    Participant p1(101, "Rahul", "rahul@gmail.com");
    Participant p2(102, "Priya", "priya@gmail.com");
    Participant p3(101, "Rahul Kumar", "rahul2@gmail.com");

    cout << "Participant Details:" << endl;
    p1.displayParticipant();

    cout << endl;

    cout << "Comparing p1 and p2:" << endl;

    if (p1 == p2)
        cout << "Both participants are the same." << endl;
    else
        cout << "Participants are different." << endl;

    cout << endl;

    cout << "Comparing p1 and p3:" << endl;

    if (p1 == p3)
        cout << "Both participants are the same." << endl;
    else
        cout << "Participants are different." << endl;

    cout << endl;

    p2.displayName();
    p2.displayEmail();

    cout << endl;

    cout << "Using overloaded << operator:" << endl;
    cout << p1;

    return 0;
}