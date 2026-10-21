#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>
#include <string>

using namespace std;

class Resource
{
private:
    int resourceId;
    string name;
    double unitRate;
    int totalStock;

public:
    Resource();
    Resource(int id, string n, double rate, int stock);

    // checks a quantity against the full stock
    bool checkAvailability(int quantity) const;

    // overloaded: checks a quantity when some units are already in use
    bool checkAvailability(int quantity, int alreadyInUse) const;

    int getResourceId() const;
    string getName() const;
    double getUnitRate() const;
    int getTotalStock() const;

    friend ostream& operator<<(ostream& out, const Resource& r);
};

#endif