#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#include <iostream>
#include <string>
using namespace std;

class Participant
{
private:
    int participantId;
    string name;
    string email;

public:
    Participant();
    Participant(int id, string n, string e);

    void displayParticipant() const;
    void displayName() const;
    void displayEmail() const;

    bool operator==(const Participant& other) const;

    friend ostream& operator<<(ostream& out, const Participant& p);
};

#endif
