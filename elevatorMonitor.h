#ifndef __ELEVATOR_MON_H
#define __ELEVATOR_MON_H

#include <vector>
#include "monitor.h"
#include "person.h"

// regulates the access to the critical regions
class ElevatorMonitor:public Monitor {
    private: 
        std::vector<Person*> peopleInside;

        int currentWeight;
        int peopleCount;
        
        Condition personEntered;

    public: 
        ElevatorMonitor():personEntered(this) {
        }

        ~ElevatorMonitor();

        void insertPerson(Person* newPerson) {
            __synchronized__ ;

            this->peopleInside.push_back(newPerson);

            this->peopleCount++;
            this->currentWeight += newPerson->getWeight();

            // signal a new person is available
            personEntered.notify();
        }

        void removePerson() {
            __synchronized__ ;

            while (peopleCount == 0) {

                personEntered.wait();
            
            }

            Person* personRemoved = this->peopleInside[0];
            int weightDecrement = personRemoved->getWeight();

            // remove from start
            this->peopleInside.erase(this->peopleInside.begin());

            this->peopleCount--;
            this->currentWeight -= weightDecrement;
        }

};

#endif