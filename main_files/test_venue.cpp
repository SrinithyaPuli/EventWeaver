#include <iostream>
#include "Venue.h"
using namespace std;

int main() {
    cout << "--- Test 1: default constructor ---\n";
    Venue v0;
    cout << v0 << endl;

    cout << "\n--- Test 2: valid venue ---\n";
    Venue v1(1, "Main Auditorium", 300);
    cout << v1 << endl;

    cout << "\n--- Test 3: invalid venue (negative capacity) ---\n";
    Venue v2(2, "Broken Hall", -50);
    cout << v2 << endl;

    cout << "\n--- Test 4: canAccommodate ---\n";
    cout << "250 people: " << (v1.canAccommodate(250) ? "fits" : "does not fit") << endl;
    cout << "300 people: " << (v1.canAccommodate(300) ? "fits" : "does not fit") << endl;
    cout << "301 people: " << (v1.canAccommodate(301) ? "fits" : "does not fit") << endl;
    cout << "0 people:   " << (v1.canAccommodate(0) ? "fits" : "does not fit") << endl;
    cout << "-5 people:  " << (v1.canAccommodate(-5) ? "fits" : "does not fit") << endl;

    cout << "\n--- Test 5: setters ---\n";
    cout << "setCapacity(400): " << (v1.setCapacity(400) ? "success" : "failed") << endl;
    cout << "setCapacity(-10): " << (v1.setCapacity(-10) ? "success" : "failed") << endl;
    cout << "setName(\"\"):      " << (v1.setName("") ? "success" : "failed") << endl;
    cout << "setName(\"Seminar Hall\"): " << (v1.setName("Seminar Hall") ? "success" : "failed") << endl;
    cout << "After updates: " << v1 << endl;

    return 0;
}