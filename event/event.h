#ifndef EVENT_H
#define EVENT_H

#include <iostream>
#include <string>
#include <vector>

#include "../timeslot/timeslot.h"
#include "../venue/venue.h"
#include "../resource/resource.h"
#include "../participant/participant.h"

using namespace std;

// Remembers how many units of one resource an event uses
// and the rate that was charged for them.
struct ResourceAllocation
{
    int resourceId;
    int quantity;
    double unitRate;
};

class Event
{
private:
    int eventId;
    string eventName;
    TimeSlot schedule;
    int expectedAttendees;
    double budget;
    double totalExpenses;
    Venue venue;
    bool venueAssigned;
    string status;
    vector<Participant> participants;
    vector<ResourceAllocation> allocatedResources;

public:
    Event();
    Event(int id, string name, TimeSlot slot,
          int attendees, double eventBudget);

    int getEventId() const;
    string getEventName() const;
    TimeSlot getSchedule() const;
    int getExpectedAttendees() const;
    double getBudget() const;
    double getTotalExpenses() const;
    double getRemainingBudget() const;
    Venue getVenue() const;
    bool hasVenue() const;
    string getStatus() const;
    int getParticipantCount() const;

    int getAllocatedQuantity(int resourceId) const;
    vector<ResourceAllocation> getAllocations() const;

    bool setEventName(string name);
    bool setExpectedAttendees(int attendees);
    bool setBudget(double amount);

    bool updateSchedule(TimeSlot slot);
    bool updateStatus(string newStatus);

    bool assignVenue(const Venue& v);

    // true if this event already uses venue v at a time that overlaps slot
    bool hasBookingConflict(const Venue& v, const TimeSlot& slot) const;

    bool addExpense(double amount);
    Event& operator+=(double amount);
    bool operator>(const Event& other) const;

    bool registerParticipant(const Participant& p);

    bool allocateResource(const Resource& r, int quantity);
    bool releaseResource(const Resource& r, int quantity);

    void displayEvent() const;

    friend ostream& operator<<(ostream& out, const Event& e);
};

#endif