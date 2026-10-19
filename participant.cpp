#include "participant.h"

Participant::Participant()
{
    participantId = 0;
    name = "";
    email = "";
}

Participant::Participant(int id, string n, string e)
{
    participantId = id;
    name = n;
    email = e;
}

void Participant::displayParticipant() const
{
    cout << "Participant ID: " << participantId << endl;
    cout << "Name: " << name << endl;
    cout << "Email: " << email << endl;
}

void Participant::displayName() const
{
    cout << "Name: " << name << endl;
}

void Participant::displayEmail() const
{
    cout << "Email: " << email << endl;
}

bool Participant::operator==(const Participant& other) const
{
    return participantId == other.participantId;
}

ostream& operator<<(ostream& out, const Participant& p)
{
    out << "Participant ID: " << p.participantId << endl;
    out << "Name: " << p.name << endl;
    out << "Email: " << p.email << endl;

    return out;
}