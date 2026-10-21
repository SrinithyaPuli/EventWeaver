#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "../Event/event.h"

using namespace std;

class EventManager
{
private:
    vector<Event> events;
    vector<Venue> venues;
    vector<Resource> resources;
    bool inputEnded;

    Event* findEvent(int eventId);
    const Event* findEvent(int eventId) const;

    Venue* findVenue(int venueId);
    const Venue* findVenue(int venueId) const;

    Resource* findResource(int resourceId);
    const Resource* findResource(int resourceId) const;

    bool canFitResourceAtSchedule(int eventId, int resourceId,
                                  int quantity,
                                  const TimeSlot& schedule) const;

    int readInt(const string& prompt);
    double readDouble(const string& prompt);
    string readLine(const string& prompt);
    TimeSlot readTimeSlot(const string& word);

public:
    EventManager();

    // ----- core logic -----
    bool addEvent(const Event& event);
    bool addVenue(const Venue& venue);
    bool addResource(const Resource& resource);

    // true if no OTHER event uses this venue at an overlapping time
    bool isVenueFree(int eventId, const Venue& venue,
                     const TimeSlot& schedule) const;

    bool canAllocateResource(int eventId,
                             const Resource& resource,
                             int quantity) const;

    bool allocateResource(int eventId, int resourceId, int quantity);
    bool releaseResource(int eventId, int resourceId, int quantity);

    bool assignVenue(int eventId, int venueId);
    bool updateEventSchedule(int eventId, const TimeSlot& newSchedule);

    // ----- searching (function overloading) -----
    void searchEvent(int eventId) const;
    void searchEvent(string eventName) const;

    // ----- displaying -----
    void viewEvents() const;
    void displayAllVenues() const;
    void displayAllResources() const;
    void displayScheduleAndBudget() const;

    // ----- menu screens -----
    void createEvent();
    void updateEventDetails();
    void registerVenue();
    void allocateVenueUI();
    void manageResourceUI();
    void recordExpenseUI();
    void registerParticipantUI();
    void compareEventBudgets();

    void runMenu();
};

#endif