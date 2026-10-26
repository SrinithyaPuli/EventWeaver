#include "venue.h"

// Default constructor: safe starting values
Venue::Venue() : venueId(0), name(""), capacity(0) {}

// Parameterized constructor: if anything is invalid, fall back to safe defaults
Venue::Venue(int id, const std::string& n, int cap)
    : venueId(0), name(""), capacity(0) {
    if (id > 0 && !n.empty() && cap > 0) {
        venueId = id;
        name = n;
        capacity = cap;
    } else {
        std::cout << "Invalid venue details. Venue created with default values.\n";
    }
}

int Venue::getVenueId() const { return venueId; }
std::string Venue::getName() const { return name; }
int Venue::getCapacity() const { return capacity; }

bool Venue::setName(const std::string& n) {
    if (n.empty()) return false;   // reject empty name
    name = n;
    return true;
}

bool Venue::setCapacity(int cap) {
    if (cap <= 0) return false;    // capacity must be positive
    capacity = cap;
    return true;
}

bool Venue::canAccommodate(int attendees) const {
    if (attendees <= 0) return false;      // invalid attendee count
    return attendees <= capacity;
}

std::ostream& operator<<(std::ostream& os, const Venue& v) {
    os << "Venue ID: " << v.venueId
       << " | Name: " << v.name
       << " | Capacity: " << v.capacity;
    return os;
}