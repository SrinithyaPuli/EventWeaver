#include "event_manager.h"
#include <sstream>
#include <cmath>
#include <cctype>

// ---------- small helper functions used only in this file ----------

// true if the text is empty or contains only spaces
static bool isBlank(const string& text)
{
    return text.find_first_not_of(" \t\n\r") == string::npos;
}

// returns a lower-case copy, used for searching names
static string toLowerText(string text)
{
    for (size_t i = 0; i < text.size(); i++)
        text[i] = (char)tolower((unsigned char)text[i]);

    return text;
}

// prints a time like 9:05
static void printTime(int hour, int minute)
{
    cout << hour << ":";

    if (minute < 10)
        cout << "0";

    cout << minute;
}

EventManager::EventManager()
{
    inputEnded = false;
}

int EventManager::readInt(const string& prompt)
{
    string line;
    int value;
    char extra;

    while (!inputEnded)
    {
        cout << prompt;

        if (!getline(cin, line))
        {
            inputEnded = true;
            return -1;
        }

        stringstream ss(line);

        if (ss >> value && !(ss >> extra))
            return value;

        cout << "Invalid input. Enter an integer.\n";
    }

    return -1;
}

double EventManager::readDouble(const string& prompt)
{
    string line;
    double value;
    char extra;

    while (!inputEnded)
    {
        cout << prompt;

        if (!getline(cin, line))
        {
            inputEnded = true;
            return -1;
        }

        stringstream ss(line);

        if (ss >> value && !(ss >> extra) &&
            std::isfinite(value))
        {
            return value;
        }

        cout << "Invalid input. Enter a valid number.\n";
    }

    return -1;
}

string EventManager::readLine(const string& prompt)
{
    string value;

    if (inputEnded)
        return "";

    cout << prompt;

    if (!getline(cin, value))
    {
        inputEnded = true;
        return "";
    }

    return value;
}

// Reads a date and a start/end time. 'word' is "" or "new ".
TimeSlot EventManager::readTimeSlot(const string& word)
{
    int day = readInt("Enter " + word + "day: ");
    int month = readInt("Enter " + word + "month: ");
    int year = readInt("Enter " + word + "year: ");

    int sh = readInt("Enter " + word + "start hour (0-23): ");
    int sm = readInt("Enter " + word + "start minute (0-59): ");
    int eh = readInt("Enter " + word + "end hour (0-23): ");
    int em = readInt("Enter " + word + "end minute (0-59): ");

    return TimeSlot(day, month, year, sh, sm, eh, em);
}

// ---------- finders (return nullptr if not found) ----------

Event* EventManager::findEvent(int eventId)
{
    for (size_t i = 0; i < events.size(); i++)
    {
        if (events[i].getEventId() == eventId)
            return &events[i];
    }

    return nullptr;
}

const Event* EventManager::findEvent(int eventId) const
{
    for (size_t i = 0; i < events.size(); i++)
    {
        if (events[i].getEventId() == eventId)
            return &events[i];
    }

    return nullptr;
}

Venue* EventManager::findVenue(int venueId)
{
    for (size_t i = 0; i < venues.size(); i++)
    {
        if (venues[i].getVenueId() == venueId)
            return &venues[i];
    }

    return nullptr;
}

const Venue* EventManager::findVenue(int venueId) const
{
    for (size_t i = 0; i < venues.size(); i++)
    {
        if (venues[i].getVenueId() == venueId)
            return &venues[i];
    }

    return nullptr;
}

Resource* EventManager::findResource(int resourceId)
{
    for (size_t i = 0; i < resources.size(); i++)
    {
        if (resources[i].getResourceId() == resourceId)
            return &resources[i];
    }

    return nullptr;
}

const Resource* EventManager::findResource(int resourceId) const
{
    for (size_t i = 0; i < resources.size(); i++)
    {
        if (resources[i].getResourceId() == resourceId)
            return &resources[i];
    }

    return nullptr;
}

// ---------- adding data ----------

bool EventManager::addEvent(const Event& event)
{
    if (event.getEventId() <= 0 ||
        event.getEventName().find_first_not_of(" \t\n\r") == string::npos ||
        !event.getSchedule().isValidDate() ||
        !event.getSchedule().isValidTime() ||
        event.getExpectedAttendees() <= 0 ||
        !std::isfinite(event.getBudget()) ||
        !std::isfinite(event.getTotalExpenses()) ||
        event.getBudget() < 0 ||
        event.getTotalExpenses() < 0 ||
        event.getTotalExpenses() > event.getBudget() ||
        findEvent(event.getEventId()) != nullptr)
    {
        return false;
    }

    events.push_back(event);
    return true;
}

bool EventManager::addVenue(const Venue& venue)
{
    if (venue.getVenueId() <= 0 ||
        venue.getName().find_first_not_of(" \t\n\r") == string::npos ||
        venue.getCapacity() <= 0)
    {
        return false;
    }

    if (findVenue(venue.getVenueId()) != nullptr)
        return false;

    venues.push_back(venue);
    return true;
}

bool EventManager::addResource(const Resource& resource)
{
    if (resource.getResourceId() <= 0 ||
        resource.getName().find_first_not_of(" \t\n\r") == string::npos ||
        !std::isfinite(resource.getUnitRate()) ||
        resource.getUnitRate() < 0 ||
        resource.getTotalStock() < 0)
    {
        return false;
    }

    if (findResource(resource.getResourceId()) != nullptr)
        return false;

    resources.push_back(resource);
    return true;
}

// ---------- conflict checks ----------

// Uses isOverlapping() (through Event::hasBookingConflict), NOT ==.
bool EventManager::isVenueFree(
    int eventId, const Venue& venue,
    const TimeSlot& schedule) const
{
    for (size_t i = 0; i < events.size(); i++)
    {
        const Event& event = events[i];

        if (event.getEventId() == eventId)
            continue;

        if (event.hasBookingConflict(venue, schedule))
            return false;
    }

    return true;
}

// Checks that using 'quantity' units of a resource during 'schedule'
// never goes above the total stock, counting the other events
// that run at the same time. Events at different times are ignored,
// so equipment can be reused.
bool EventManager::canFitResourceAtSchedule(
    int eventId, int resourceId, int quantity,
    const TimeSlot& schedule) const
{
    const Resource* resource = findResource(resourceId);

    if (resource == nullptr ||
        quantity < 0 ||
        !schedule.isValidDate() ||
        !schedule.isValidTime())
    {
        return false;
    }

    if (quantity > resource->getTotalStock())
        return false;

    if (quantity == 0)
        return true;

    int candidateStart =
        schedule.getStartHour() * 60 +
        schedule.getStartMinute();

    int candidateEnd =
        schedule.getEndHour() * 60 +
        schedule.getEndMinute();

    if (candidateStart >= candidateEnd)
        return false;

    vector<int> checkTimes;
    checkTimes.push_back(candidateStart);

    for (size_t i = 0; i < events.size(); i++)
    {
        const Event& otherEvent = events[i];

        if (otherEvent.getEventId() == eventId)
            continue;

        TimeSlot otherSchedule = otherEvent.getSchedule();

        if (schedule.getDay() != otherSchedule.getDay() ||
            schedule.getMonth() != otherSchedule.getMonth() ||
            schedule.getYear() != otherSchedule.getYear() ||
            !schedule.isOverlapping(otherSchedule))
        {
            continue;
        }

        int otherStart =
            otherSchedule.getStartHour() * 60 +
            otherSchedule.getStartMinute();

        if (otherStart >= candidateStart &&
            otherStart < candidateEnd)
        {
            checkTimes.push_back(otherStart);
        }
    }

    for (size_t i = 0; i < checkTimes.size(); i++)
    {
        int time = checkTimes[i];
        int simultaneousUsage = quantity;

        for (size_t j = 0; j < events.size(); j++)
        {
            const Event& otherEvent = events[j];

            if (otherEvent.getEventId() == eventId)
                continue;

            TimeSlot otherSchedule = otherEvent.getSchedule();

            if (schedule.getDay() != otherSchedule.getDay() ||
                schedule.getMonth() != otherSchedule.getMonth() ||
                schedule.getYear() != otherSchedule.getYear() ||
                !schedule.isOverlapping(otherSchedule))
            {
                continue;
            }

            int otherStart =
                otherSchedule.getStartHour() * 60 +
                otherSchedule.getStartMinute();

            int otherEnd =
                otherSchedule.getEndHour() * 60 +
                otherSchedule.getEndMinute();

            if (otherStart <= time && time < otherEnd)
            {
                simultaneousUsage +=
                    otherEvent.getAllocatedQuantity(resourceId);
            }
        }

        if (simultaneousUsage > resource->getTotalStock())
            return false;
    }

    return true;
}

bool EventManager::canAllocateResource(
    int eventId, const Resource& resource,
    int quantity) const
{
    const Event* event = findEvent(eventId);
    const Resource* registeredResource =
        findResource(resource.getResourceId());

    if (event == nullptr ||
        registeredResource == nullptr ||
        quantity <= 0)
    {
        return false;
    }

    int currentQuantity =
        event->getAllocatedQuantity(
            registeredResource->getResourceId());

    // uses the overloaded checkAvailability(quantity, alreadyInUse)
    if (!registeredResource->checkAvailability(quantity, currentQuantity))
        return false;

    int requestedTotal = currentQuantity + quantity;

    return canFitResourceAtSchedule(
        eventId,
        registeredResource->getResourceId(),
        requestedTotal,
        event->getSchedule());
}

bool EventManager::allocateResource(
    int eventId, int resourceId, int quantity)
{
    Event* event = findEvent(eventId);
    Resource* resource = findResource(resourceId);

    if (event == nullptr || resource == nullptr)
        return false;

    if (!canAllocateResource(eventId, *resource, quantity))
        return false;

    return event->allocateResource(*resource, quantity);
}

bool EventManager::releaseResource(
    int eventId, int resourceId, int quantity)
{
    Event* event = findEvent(eventId);
    Resource* resource = findResource(resourceId);

    if (event == nullptr || resource == nullptr)
        return false;

    return event->releaseResource(*resource, quantity);
}

bool EventManager::assignVenue(int eventId, int venueId)
{
    Event* event = findEvent(eventId);
    Venue* selectedVenue = findVenue(venueId);

    if (event == nullptr || selectedVenue == nullptr)
        return false;

    if (!selectedVenue->canAccommodate(
            event->getExpectedAttendees()))
    {
        return false;
    }

    if (!isVenueFree(eventId, *selectedVenue, event->getSchedule()))
        return false;

    return event->assignVenue(*selectedVenue);
}

// The schedule is changed only after every check passes.
// If any check fails, the old schedule and allocations stay as they were.
bool EventManager::updateEventSchedule(
    int eventId, const TimeSlot& newSchedule)
{
    Event* event = findEvent(eventId);

    if (event == nullptr ||
        !newSchedule.isValidDate() ||
        !newSchedule.isValidTime())
    {
        return false;
    }

    if (event->hasVenue() &&
        !isVenueFree(eventId, event->getVenue(), newSchedule))
    {
        return false;
    }

    vector<ResourceAllocation> allocations =
        event->getAllocations();

    for (size_t i = 0; i < allocations.size(); i++)
    {
        if (!canFitResourceAtSchedule(
                eventId,
                allocations[i].resourceId,
                allocations[i].quantity,
                newSchedule))
        {
            return false;
        }
    }

    return event->updateSchedule(newSchedule);
}

// ---------- searching (overloaded) ----------

// Search by event ID
void EventManager::searchEvent(int eventId) const
{
    const Event* event = findEvent(eventId);

    if (event == nullptr)
        cout << "Event not found.\n";
    else
        cout << *event << "\n";
}

// Search by event name (part of the name, ignoring capital letters)
void EventManager::searchEvent(string eventName) const
{
    if (isBlank(eventName))
    {
        cout << "Please enter a name to search.\n";
        return;
    }

    string wanted = toLowerText(eventName);
    bool found = false;

    for (size_t i = 0; i < events.size(); i++)
    {
        string current = toLowerText(events[i].getEventName());

        if (current.find(wanted) != string::npos)
        {
            cout << events[i] << "\n";
            found = true;
        }
    }

    if (!found)
        cout << "Event not found.\n";
}

// ---------- displaying ----------

void EventManager::viewEvents() const
{
    if (events.empty())
    {
        cout << "No events registered.\n";
        return;
    }

    for (size_t i = 0; i < events.size(); i++)
        cout << events[i] << "\n";
}

void EventManager::displayAllVenues() const
{
    if (venues.empty())
    {
        cout << "No venues registered.\n";
        return;
    }

    for (size_t i = 0; i < venues.size(); i++)
        cout << venues[i] << "\n";
}

void EventManager::displayAllResources() const
{
    if (resources.empty())
    {
        cout << "No resources registered.\n";
        return;
    }

    for (size_t i = 0; i < resources.size(); i++)
        cout << resources[i] << "\n";
}

void EventManager::displayScheduleAndBudget() const
{
    if (events.empty())
    {
        cout << "No events registered.\n";
        return;
    }

    for (size_t i = 0; i < events.size(); i++)
    {
        const Event& event = events[i];
        TimeSlot slot = event.getSchedule();

        cout << "\n" << event.getEventName()
             << " (ID " << event.getEventId() << ") - "
             << event.getStatus() << "\n";

        cout << "  Date: " << slot.getDay() << "/"
             << slot.getMonth() << "/" << slot.getYear()
             << "   Time: ";
        printTime(slot.getStartHour(), slot.getStartMinute());
        cout << " - ";
        printTime(slot.getEndHour(), slot.getEndMinute());
        cout << "\n";

        if (event.hasVenue())
        {
            cout << "  Venue: " << event.getVenue().getName()
                 << " (capacity " << event.getVenue().getCapacity()
                 << ")\n";
        }
        else
        {
            cout << "  Venue: Not assigned\n";
        }

        cout << "  Expected attendees: " << event.getExpectedAttendees()
             << "   Participants registered: "
             << event.getParticipantCount() << "\n";

        cout << "  Budget: " << event.getBudget()
             << "   Spent: " << event.getTotalExpenses()
             << "   Remaining: " << event.getRemainingBudget() << "\n";

        vector<ResourceAllocation> list = event.getAllocations();

        if (list.empty())
        {
            cout << "  Resources: None\n";
        }
        else
        {
            cout << "  Resources:\n";

            for (size_t j = 0; j < list.size(); j++)
            {
                const Resource* resource =
                    findResource(list[j].resourceId);

                string resourceName = "Unknown";
                if (resource != nullptr)
                    resourceName = resource->getName();

                cout << "    " << resourceName
                     << " x " << list[j].quantity
                     << " (rate " << list[j].unitRate << " each)\n";
            }
        }
    }
}

// ---------- menu screens ----------

void EventManager::createEvent()
{
    int id = readInt("Enter event ID: ");

    if (inputEnded)
        return;

    if (id <= 0)
    {
        cout << "Event ID must be a positive number.\n";
        return;
    }

    if (findEvent(id) != nullptr)
    {
        cout << "An event with this ID already exists.\n";
        return;
    }

    string name = readLine("Enter event name: ");
    int attendees = readInt("Enter expected attendees: ");
    double budget = readDouble("Enter budget: ");
    TimeSlot slot = readTimeSlot("");

    if (inputEnded)
        return;

    if (isBlank(name))
        cout << "Event name cannot be empty. Event not created.\n";
    else if (attendees <= 0)
        cout << "Attendees must be positive. Event not created.\n";
    else if (budget < 0)
        cout << "Budget cannot be negative. Event not created.\n";
    else if (!slot.isValidDate())
        cout << "Invalid date. Event not created.\n";
    else if (!slot.isValidTime())
        cout << "Invalid time (end must be after start). Event not created.\n";
    else
    {
        Event event(id, name, slot, attendees, budget);

        if (addEvent(event))
            cout << "Event created successfully.\n";
        else
            cout << "Could not create event. Check the details and ID.\n";
    }
}

// Changes one detail of an event. Each change is validated first,
// and if it fails the old details are kept.
void EventManager::updateEventDetails()
{
    int eventId = readInt("Enter event ID to update: ");

    if (inputEnded)
        return;

    Event* event = findEvent(eventId);

    if (event == nullptr)
    {
        cout << "Event not found.\n";
        return;
    }

    cout << "1. Change name\n";
    cout << "2. Change schedule\n";
    cout << "3. Change expected attendees\n";
    cout << "4. Change budget\n";
    cout << "5. Change status\n";

    int choice = readInt("Enter choice: ");

    if (inputEnded)
        return;

    switch (choice)
    {
        case 1:
        {
            string name = readLine("Enter new name: ");

            if (inputEnded)
                return;

            if (event->setEventName(name))
                cout << "Name updated.\n";
            else
                cout << "Update rejected: name cannot be empty.\n";
            break;
        }
        case 2:
        {
            TimeSlot newSchedule = readTimeSlot("new ");

            if (inputEnded)
                return;

            if (!newSchedule.isValidDate())
                cout << "Update rejected: invalid date. Old schedule kept.\n";
            else if (!newSchedule.isValidTime())
                cout << "Update rejected: invalid time. Old schedule kept.\n";
            else if (newSchedule == event->getSchedule())
                cout << "The new schedule is exactly the same as the current one. Nothing changed.\n";
            else if (updateEventSchedule(eventId, newSchedule))
                cout << "Event schedule updated successfully.\n";
            else
                cout << "Update rejected: the venue is booked or resource stock is not enough at the new time. Old schedule kept.\n";
            break;
        }
        case 3:
        {
            int attendees = readInt("Enter new expected attendees: ");

            if (inputEnded)
                return;

            if (event->setExpectedAttendees(attendees))
                cout << "Expected attendees updated.\n";
            else
                cout << "Update rejected: must be positive and fit the assigned venue.\n";
            break;
        }
        case 4:
        {
            double amount = readDouble("Enter new budget: ");

            if (inputEnded)
                return;

            if (event->setBudget(amount))
                cout << "Budget updated.\n";
            else
                cout << "Update rejected: budget cannot be negative or less than the expenses already made.\n";
            break;
        }
        case 5:
        {
            cout << "1. Planned\n2. Confirmed\n3. Completed\n";
            int s = readInt("Choose status: ");

            if (inputEnded)
                return;

            string newStatus = "";

            if (s == 1)
                newStatus = "Planned";
            else if (s == 2)
                newStatus = "Confirmed";
            else if (s == 3)
                newStatus = "Completed";

            if (event->updateStatus(newStatus))
                cout << "Status updated to " << newStatus << ".\n";
            else
                cout << "Invalid status choice.\n";
            break;
        }
        default:
            cout << "Invalid choice.\n";
    }
}

void EventManager::registerVenue()
{
    int id = readInt("Enter venue ID: ");

    if (inputEnded)
        return;

    if (id <= 0)
    {
        cout << "Venue ID must be a positive number.\n";
        return;
    }

    if (findVenue(id) != nullptr)
    {
        cout << "A venue with this ID already exists.\n";
        return;
    }

    string name = readLine("Enter venue name: ");
    int capacity = readInt("Enter venue capacity: ");

    if (inputEnded)
        return;

    if (isBlank(name))
    {
        cout << "Venue name cannot be empty.\n";
    }
    else if (capacity <= 0)
    {
        cout << "Capacity must be positive.\n";
    }
    else
    {
        Venue venue(id, name, capacity);

        if (addVenue(venue))
            cout << "Venue registered successfully.\n";
        else
            cout << "Could not register venue. Check the details and ID.\n";
    }
}

void EventManager::allocateVenueUI()
{
    int eventId = readInt("Enter event ID: ");
    int venueId = readInt("Enter venue ID: ");

    if (inputEnded)
        return;

    Event* event = findEvent(eventId);
    Venue* venue = findVenue(venueId);

    if (event == nullptr)
    {
        cout << "Event not found.\n";
    }
    else if (venue == nullptr)
    {
        cout << "Venue not found.\n";
    }
    else if (!venue->canAccommodate(event->getExpectedAttendees()))
    {
        cout << "Venue assignment failed: capacity " << venue->getCapacity()
             << " is too small for " << event->getExpectedAttendees()
             << " attendees.\n";
    }
    else if (!isVenueFree(eventId, *venue, event->getSchedule()))
    {
        cout << "Venue assignment failed: the venue is already booked at an overlapping time.\n";
    }
    else if (assignVenue(eventId, venueId))
    {
        cout << "Venue assigned successfully.\n";
    }
    else
    {
        cout << "Venue assignment failed.\n";
    }
}

void EventManager::manageResourceUI()
{
    cout << "1. Add new resource\n";
    cout << "2. Allocate resource to an event\n";
    cout << "3. Release resource from an event\n";

    int choice = readInt("Enter choice: ");

    if (inputEnded)
        return;

    if (choice == 1)
    {
        int id = readInt("Enter resource ID: ");

        if (inputEnded)
            return;

        if (id <= 0)
        {
            cout << "Resource ID must be a positive number.\n";
            return;
        }

        if (findResource(id) != nullptr)
        {
            cout << "A resource with this ID already exists.\n";
            return;
        }

        string name = readLine("Enter resource name: ");
        double rate = readDouble("Enter unit rate: ");
        int stock = readInt("Enter total stock: ");

        if (inputEnded)
            return;

        if (isBlank(name))
        {
            cout << "Resource name cannot be empty.\n";
        }
        else if (rate < 0)
        {
            cout << "Unit rate cannot be negative.\n";
        }
        else if (stock < 0)
        {
            cout << "Stock cannot be negative.\n";
        }
        else
        {
            Resource resource(id, name, rate, stock);

            if (addResource(resource))
                cout << "Resource added successfully.\n";
            else
                cout << "Could not add resource. Check the details and ID.\n";
        }
    }
    else if (choice == 2 || choice == 3)
    {
        int eventId = readInt("Enter event ID: ");
        int resourceId = readInt("Enter resource ID: ");
        int quantity = readInt("Enter quantity: ");

        if (inputEnded)
            return;

        Event* event = findEvent(eventId);
        Resource* resource = findResource(resourceId);

        if (event == nullptr)
        {
            cout << "Event not found.\n";
        }
        else if (resource == nullptr)
        {
            cout << "Resource not found.\n";
        }
        else if (quantity <= 0)
        {
            cout << "Quantity must be positive.\n";
        }
        else if (choice == 2)
        {
            if (!canAllocateResource(eventId, *resource, quantity))
                cout << "Allocation failed: not enough stock during this event's time slot.\n";
            else if (allocateResource(eventId, resourceId, quantity))
                cout << "Resource allocated successfully.\n";
            else
                cout << "Allocation failed: the cost is more than the remaining budget.\n";
        }
        else
        {
            if (releaseResource(eventId, resourceId, quantity))
                cout << "Resource released successfully. The cost was returned to the budget.\n";
            else
                cout << "Release failed: this event has fewer units of that resource allocated.\n";
        }
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}

void EventManager::recordExpenseUI()
{
    int eventId = readInt("Enter event ID: ");
    double amount = readDouble("Enter expense amount: ");

    if (inputEnded)
        return;

    Event* event = findEvent(eventId);

    if (event == nullptr)
    {
        cout << "Event not found.\n";
    }
    else if (amount <= 0)
    {
        cout << "Expense rejected: amount must be positive. Total unchanged.\n";
    }
    else if (event->addExpense(amount))
    {
        cout << "Expense recorded. Remaining budget: "
             << event->getRemainingBudget() << "\n";
    }
    else
    {
        cout << "Expense rejected: it is more than the remaining budget ("
             << event->getRemainingBudget() << "). Total unchanged.\n";
    }
}

void EventManager::registerParticipantUI()
{
    int eventId = readInt("Enter event ID: ");

    if (inputEnded)
        return;

    Event* event = findEvent(eventId);

    if (event == nullptr)
    {
        cout << "Event not found.\n";
        return;
    }

    int participantId = readInt("Enter participant ID: ");
    string name = readLine("Enter participant name: ");
    string email = readLine("Enter participant email: ");

    if (inputEnded)
        return;

    if (participantId <= 0)
    {
        cout << "Participant ID must be a positive number.\n";
    }
    else if (isBlank(name))
    {
        cout << "Participant name cannot be empty.\n";
    }
    else if (isBlank(email))
    {
        cout << "Participant email cannot be empty.\n";
    }
    else
    {
        Participant participant(participantId, name, email);

        if (event->registerParticipant(participant))
            cout << "Participant registered successfully.\n";
        else
            cout << "This participant is already registered for this event.\n";
    }
}

// Uses the overloaded operator> to compare two events by budget.
void EventManager::compareEventBudgets()
{
    int idA = readInt("Enter first event ID: ");
    int idB = readInt("Enter second event ID: ");

    if (inputEnded)
        return;

    const Event* a = findEvent(idA);
    const Event* b = findEvent(idB);

    if (a == nullptr || b == nullptr)
    {
        cout << "One or both events were not found.\n";
    }
    else if (*a > *b)
    {
        cout << a->getEventName() << " has a larger budget than "
             << b->getEventName() << ".\n";
    }
    else if (*b > *a)
    {
        cout << b->getEventName() << " has a larger budget than "
             << a->getEventName() << ".\n";
    }
    else
    {
        cout << "Both events have the same budget.\n";
    }
}

void EventManager::runMenu()
{
    while (true)
    {
        cout << "\n===== EventWeaver =====\n";
        cout << "1.  Create Event\n";
        cout << "2.  View All Events\n";
        cout << "3.  Search Event by ID\n";
        cout << "4.  Search Event by Name\n";
        cout << "5.  Update Event Details\n";
        cout << "6.  Register Venue\n";
        cout << "7.  Assign Venue to Event\n";
        cout << "8.  Manage Resources\n";
        cout << "9.  Record Expense\n";
        cout << "10. Register Participant\n";
        cout << "11. Display Schedule and Budget\n";
        cout << "12. View Venues and Resources\n";
        cout << "13. Compare Event Budgets\n";
        cout << "0.  Exit\n";

        int choice = readInt("Enter choice: ");

        if (inputEnded)
        {
            cout << "\nInput ended. Exiting EventWeaver.\n";
            break;
        }

        switch (choice)
        {
            case 1:
                createEvent();
                break;
            case 2:
                viewEvents();
                break;
            case 3:
            {
                int id = readInt("Enter event ID: ");
                if (!inputEnded)
                    searchEvent(id);
                break;
            }
            case 4:
            {
                string name = readLine("Enter event name to search: ");
                if (!inputEnded)
                    searchEvent(name);
                break;
            }
            case 5:
                updateEventDetails();
                break;
            case 6:
                registerVenue();
                break;
            case 7:
                allocateVenueUI();
                break;
            case 8:
                manageResourceUI();
                break;
            case 9:
                recordExpenseUI();
                break;
            case 10:
                registerParticipantUI();
                break;
            case 11:
                displayScheduleAndBudget();
                break;
            case 12:
                cout << "\n--- Venues ---\n";
                displayAllVenues();
                cout << "--- Resources ---\n";
                displayAllResources();
                break;
            case 13:
                compareEventBudgets();
                break;
            case 0:
                cout << "Exiting EventWeaver.\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
        }

        if (inputEnded)
        {
            cout << "\nInput ended. Exiting EventWeaver.\n";
            break;
        }
    }
}