#ifndef VENUE_H
#define VENUE_H

#include <iostream>
#include <string>

// Venue: a place (hall, lab, seminar room etc..) where a college event is held.
class Venue {
private:
    int venueId;
    std::string name;
    int capacity;

public:
    // Constructors
    Venue();                                              // safe defaults: 0, "", 0
    Venue(int id, const std::string& n, int cap);         // validated

    // Getters
    int getVenueId() const;
    std::string getName() const;
    int getCapacity() const;
    
    // Setters (return true if update worked, false if input was invalid)
    bool setName(const std::string& n);
    bool setCapacity(int cap);

    // Checks if the expected attendees fit in this venue
    bool canAccommodate(int attendees) const;

    // Prints venue details with cout << venue
    friend std::ostream& operator<<(std::ostream& os, const Venue& v);
};

#endif