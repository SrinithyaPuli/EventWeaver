#include "event.h"
#include <cmath>

// A tiny tolerance so that money values like 0.1 + 0.2 are not
// wrongly rejected because of how computers round decimals.
static const double EPS = 0.000001;

// true if the text is empty or contains only spaces
static bool isBlank(const string& text)
{
    return text.find_first_not_of(" \t\n\r") == string::npos;
}

Event::Event()
{
    eventId = 0;
    eventName = "";
    expectedAttendees = 0;
    budget = 0;
    totalExpenses = 0;
    venueAssigned = false;
    status = "Planned";
}

Event::Event(int id, string name, TimeSlot slot,
             int attendees, double eventBudget)
{
    eventId = 0;
    eventName = "";
    schedule = slot;
    expectedAttendees = 0;
    budget = 0;
    totalExpenses = 0;
    venueAssigned = false;
    status = "Planned";

    if (id > 0 && !isBlank(name) &&
        slot.isValidDate() && slot.isValidTime() &&
        attendees > 0 &&
        std::isfinite(eventBudget) && eventBudget >= 0)
    {
        eventId = id;
        eventName = name;
        expectedAttendees = attendees;
        budget = eventBudget;
    }
}

int Event::getEventId() const
{
    return eventId;
}

string Event::getEventName() const
{
    return eventName;
}

TimeSlot Event::getSchedule() const
{
    return schedule;
}

int Event::getExpectedAttendees() const
{
    return expectedAttendees;
}

double Event::getBudget() const
{
    return budget;
}

double Event::getTotalExpenses() const
{
    return totalExpenses;
}

double Event::getRemainingBudget() const
{
    double remaining = budget - totalExpenses;

    if (remaining < 0)
        remaining = 0;

    return remaining;
}

Venue Event::getVenue() const
{
    return venue;
}

bool Event::hasVenue() const
{
    return venueAssigned;
}

string Event::getStatus() const
{
    return status;
}

int Event::getParticipantCount() const
{
    return participants.size();
}

int Event::getAllocatedQuantity(int resourceId) const
{
    for (size_t i = 0; i < allocatedResources.size(); i++)
    {
        if (allocatedResources[i].resourceId == resourceId)
            return allocatedResources[i].quantity;
    }

    return 0;
}

vector<ResourceAllocation> Event::getAllocations() const
{
    return allocatedResources;
}

bool Event::setEventName(string name)
{
    if (isBlank(name))
        return false;

    eventName = name;
    return true;
}

bool Event::setExpectedAttendees(int attendees)
{
    if (attendees <= 0)
        return false;

    if (venueAssigned && !venue.canAccommodate(attendees))
        return false;

    expectedAttendees = attendees;
    return true;
}

bool Event::setBudget(double amount)
{
    if (!std::isfinite(amount) || amount < 0 ||
        amount < totalExpenses - EPS)
        return false;

    budget = amount;
    return true;
}

// Only checks that the new date and time are valid.
// EventManager checks venue and resource conflicts before calling this.
bool Event::updateSchedule(TimeSlot slot)
{
    if (!slot.isValidDate() || !slot.isValidTime())
        return false;

    schedule = slot;
    return true;
}

bool Event::updateStatus(string newStatus)
{
    if (newStatus != "Planned" &&
        newStatus != "Confirmed" &&
        newStatus != "Completed")
        return false;

    status = newStatus;
    return true;
}

bool Event::assignVenue(const Venue& v)
{
    if (v.getVenueId() <= 0 ||
        !v.canAccommodate(expectedAttendees))
        return false;

    venue = v;
    venueAssigned = true;
    return true;
}

// Uses isOverlapping(), not operator==, to detect booking clashes.
bool Event::hasBookingConflict(const Venue& v, const TimeSlot& slot) const
{
    return venueAssigned &&
           venue.getVenueId() == v.getVenueId() &&
           schedule.isOverlapping(slot);
}

bool Event::addExpense(double amount)
{
    if (!std::isfinite(amount) || amount <= 0 ||
        totalExpenses + amount > budget + EPS)
        return false;

    totalExpenses += amount;
    return true;
}

Event& Event::operator+=(double amount)
{
    addExpense(amount);
    return *this;
}

bool Event::operator>(const Event& other) const
{
    return budget > other.budget;
}

bool Event::registerParticipant(const Participant& p)
{
    for (const Participant& existing : participants)
    {
        if (existing == p)
            return false;
    }

    participants.push_back(p);
    return true;
}

bool Event::allocateResource(const Resource& r, int quantity)
{
    int currentQuantity = getAllocatedQuantity(r.getResourceId());

    if (r.getResourceId() <= 0 ||
        quantity <= 0 ||
        r.getUnitRate() < 0 ||
        !r.checkAvailability(quantity, currentQuantity))
        return false;

    double rate = r.getUnitRate();

    for (size_t i = 0; i < allocatedResources.size(); i++)
    {
        if (allocatedResources[i].resourceId == r.getResourceId())
        {
            rate = allocatedResources[i].unitRate;
            break;
        }
    }

    double cost = rate * quantity;

    // a free resource (rate 0) adds no expense
    if (cost > 0 && !addExpense(cost))
        return false;

    for (size_t i = 0; i < allocatedResources.size(); i++)
    {
        if (allocatedResources[i].resourceId == r.getResourceId())
        {
            allocatedResources[i].quantity += quantity;
            return true;
        }
    }

    ResourceAllocation a;
    a.resourceId = r.getResourceId();
    a.quantity = quantity;
    a.unitRate = r.getUnitRate();

    allocatedResources.push_back(a);

    return true;
}

bool Event::releaseResource(const Resource& r, int quantity)
{
    if (quantity <= 0)
        return false;

    for (size_t i = 0; i < allocatedResources.size(); i++)
    {
        if (allocatedResources[i].resourceId == r.getResourceId())
        {
            if (quantity > allocatedResources[i].quantity)
                return false;

            double refund =
                allocatedResources[i].unitRate * quantity;

            allocatedResources[i].quantity -= quantity;
            totalExpenses -= refund;

            if (totalExpenses < 0)
                totalExpenses = 0;

            if (allocatedResources[i].quantity == 0)
                allocatedResources.erase(allocatedResources.begin() + i);

            return true;
        }
    }

    return false;
}

void Event::displayEvent() const
{
    cout << *this;
}

ostream& operator<<(ostream& out, const Event& e)
{
    out << "\nEvent ID: " << e.eventId
        << "\nEvent Name: " << e.eventName
        << "\nDate: " << e.schedule.getDay()
        << "/" << e.schedule.getMonth()
        << "/" << e.schedule.getYear()
        << "\nStart Time: " << e.schedule.getStartHour() << ":";

    if (e.schedule.getStartMinute() < 10)
        out << "0";

    out << e.schedule.getStartMinute()
        << "\nEnd Time: " << e.schedule.getEndHour() << ":";

    if (e.schedule.getEndMinute() < 10)
        out << "0";

    out << e.schedule.getEndMinute()
        << "\nExpected Attendees: " << e.expectedAttendees
        << "\nBudget: " << e.budget
        << "\nTotal Expenses: " << e.totalExpenses
        << "\nRemaining Budget: " << e.getRemainingBudget()
        << "\nStatus: " << e.status
        << "\nParticipants Registered: " << e.participants.size()
        << "\nResource Types Allocated: " << e.allocatedResources.size();

    if (e.venueAssigned)
        out << "\nAssigned Venue: " << e.venue.getName();
    else
        out << "\nAssigned Venue: Not assigned";

    return out;
}