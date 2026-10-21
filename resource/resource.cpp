#include "resource.h"
#include <cmath>

Resource::Resource()
{
    resourceId = 0;
    name = "";
    unitRate = 0.0;
    totalStock = 0;
}

Resource::Resource(int id, string n, double rate, int stock)
{
    if (id > 0 &&
        n.find_first_not_of(" \t\n\r") != string::npos &&
        std::isfinite(rate) &&
        rate >= 0 &&
        stock >= 0)
    {
        resourceId = id;
        name = n;
        unitRate = rate;
        totalStock = stock;
    }
    else
    {
        resourceId = 0;
        name = "";
        unitRate = 0.0;
        totalStock = 0;
    }
}

bool Resource::checkAvailability(int quantity) const
{
    return resourceId > 0 &&
           quantity > 0 &&
           quantity <= totalStock;
}

// Overloaded version: some units are already in use,
// so only (totalStock - alreadyInUse) units are free.
bool Resource::checkAvailability(int quantity, int alreadyInUse) const
{
    return resourceId > 0 &&
           quantity > 0 &&
           alreadyInUse >= 0 &&
           alreadyInUse <= totalStock &&
           quantity <= totalStock - alreadyInUse;
}

int Resource::getResourceId() const
{
    return resourceId;
}

string Resource::getName() const
{
    return name;
}

double Resource::getUnitRate() const
{
    return unitRate;
}

int Resource::getTotalStock() const
{
    return totalStock;
}

ostream& operator<<(ostream& out, const Resource& r)
{
    out << "Resource ID: " << r.resourceId << endl;
    out << "Resource Name: " << r.name << endl;
    out << "Unit Rate: " << r.unitRate << endl;
    out << "Total Stock: " << r.totalStock << endl;

    return out;
}