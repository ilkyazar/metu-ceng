#ifndef __ELEVATOR_MON_H
#define __ELEVATOR_MON_H

#include <vector>
#include "monitor.h"
#include "person.h"

#define IDLE 0
#define MOVING_UP 1
#define MOVING_DOWN 2

// regulates the access to the critical regions
class ElevatorMonitor:public Monitor {
    private: 
        std::vector<Person*> peopleInside;
        std::vector<int> destQueue;

        int currentWeight;
        int peopleCount;

        int state;
        
        Condition personEntered;

    public: 
        ElevatorMonitor():personEntered(this) {
            this->currentWeight = 0;
            this->peopleCount = 0;
            this->state = IDLE;                        
        }

        ~ElevatorMonitor() {};

        int getState() {
            return this->state;
        }

        void setState(int s) {
            this->state = s;
        }

        std::string getStateStr() {
            if (this->state == IDLE)
                return "Idle";
            else if (this->state == MOVING_UP)
                return "Moving-up";
            else
                return "Moving-down";            
        }

        int getCurrentWeight() {
            return this->currentWeight;
        }

        int getPeopleCount() {
            return this->peopleCount;
        }

        void insertPerson(Person* newPerson) {

            this->peopleInside.push_back(newPerson);

            // TODO: sort according to the destination

            this->peopleCount++;
            this->currentWeight += newPerson->getWeight();

            newPerson->printEntered();
            this->printElevInfo();

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

            personRemoved->printLeft();
            this->printElevInfo();
        }

        bool directionCond(Person* p) {
            return true;
        }

        bool locationCond(Person* p) {
            return true;
        }

        bool capacityCond(Person* p, int wc, int pc) {
            return wc >= p->getWeight() + this->getCurrentWeight()
                                && pc >= 1 + this->getPeopleCount();
        }

        void personMakeReq(Person* p, int wc, int pc) {

            __synchronized__ ;

            if (directionCond(p) && locationCond(p)) {

                p->printMadeReq();
                
                if (capacityCond(p, wc, pc)) {

                    p->setInside();

                    this->insertPerson(p);
                    
                    // pthread_exit(NULL);

                    return;
                }

            }
            

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

        void printElevInfo() {
            // TODO: add current floor -> destination floors

            std::cout << "Elevator (" << this->getStateStr() << ", " << this->getCurrentWeight() << ", " 
                      << this->getPeopleCount() << ", " << " -> "
                      << ")" << std::endl;
        }

};

#endif