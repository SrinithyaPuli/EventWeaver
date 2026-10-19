#include <iostream>
#include "Resource.h"

using namespace std;

int main()
{
    Resource r1;
    Resource r2(101, "Projector", 0.0, 5);
    Resource r3(102, "Decorative Lights", 100.0, 20);
    Resource r4(103, "Tent", 1500.0, 2);

    cout << "DEFAULT RESOURCE" << endl;
    cout << r1 << endl;

    cout << "RESOURCE DETAILS" << endl;
    cout << r2 << endl;
    cout << r3 << endl;
    cout << r4 << endl;

    cout << "GETTER FUNCTIONS" << endl;
    cout << "Resource ID: " << r2.getResourceId() << endl;
    cout << "Name: " << r2.getName() << endl;
    cout << "Unit Rate: " << r2.getUnitRate() << endl;
    cout << "Total Stock: " << r2.getTotalStock() << endl;

    cout << "\nAVAILABILITY TESTS" << endl;

    cout << "Quantity 3: "
         << (r2.checkAvailability(3) ? "Available" : "Not Available")
         << endl;

    cout << "Quantity 5: "
         << (r2.checkAvailability(5) ? "Available" : "Not Available")
         << endl;

    cout << "Quantity 6: "
         << (r2.checkAvailability(6) ? "Available" : "Not Available")
         << endl;

    cout << "Quantity 0: "
         << (r2.checkAvailability(0) ? "Available" : "Not Available")
         << endl;

    cout << "Quantity -2: "
         << (r2.checkAvailability(-2) ? "Available" : "Not Available")
         << endl;

    cout << "\nRESOURCE COST" << endl;
    cout << "Decorative Lights (10 units): Rs. "
         << r3.getUnitRate() * 10 << endl;

    cout << "Tent (1 unit): Rs. "
         << r4.getUnitRate() << endl;

    return 0;
}
