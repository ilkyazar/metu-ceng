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
            int currentWeight = 0;
            int peopleCount = 0;                        
        }

        ~ElevatorMonitor() {};

        void insertPerson(Person* newPerson) {
            __synchronized__ ;

            this->peopleInside.push_back(newPerson);

            std::cout << "New person is inserted with id = " << newPerson->getId() << " and destination floor = " << newPerson->getDestFloor() << ".\n" << std::endl;
            
            // TODO: sort according to the destination

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

        bool isPersonIn(int id) {
            for (int i = 0; i < peopleInside.size(); i++) {
                if (peopleInside[i]->getId() == id) {
                    return true;
                }
            }
            return false;
        }

        void printPeopleIn() {
            for (int i = 0; i < peopleInside.size(); i++) {
                Person* p = peopleInside[i];
                std::cout << "ID: " << p->getId() << " WEIGHT: " << p->getWeight() << " INIT FLOOR: " << p->getInitialFloor() << " DEST FLOOR: " << p->getDestFloor() << " PRIORITY : " << p->getPriority() << std::endl;
            }
        }

};

#endif