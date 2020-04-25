#ifndef __ELEVATOR_MON_H
#define __ELEVATOR_MON_H

#include <vector>
#include <algorithm>
#include <string>
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

        int currentFloor;
        int currentWeight;
        int peopleCount;

        int state;
        
        Condition personEntered;

        int reqCount;
        int hasReset;

    public: 
        ElevatorMonitor():personEntered(this) {
            this->currentWeight = 0;
            this->peopleCount = 0;
            this->state = IDLE;   
            this->currentFloor = 0;   
            this->reqCount = 0;
            this->hasReset = 0; 
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

        int getCurrentFloor() {
            return this->currentFloor;
        }

        void moveUp() {
            this->state = MOVING_UP;
            this->currentFloor++;  
            //checkQueue();
            printElevInfo();          
        }

        void moveDown() {
            this->state = MOVING_DOWN;
            this->currentFloor--;
            //checkQueue();
            printElevInfo();
        }

        void checkQueue() {
            if (this->currentFloor == this->destQueue[0]) {
                this->destQueue.erase(this->destQueue.begin());
            }
        }

        int getCurrentWeight() {
            return this->currentWeight;
        }

        int getPeopleCount() {
            return this->peopleCount;
        }

        void sortDestQueue() {
            for (const auto &i: this->destQueue)
                std::sort(this->destQueue.begin(), this->destQueue.end());

            this->destQueue.erase(unique(this->destQueue.begin(), this->destQueue.end()), this->destQueue.end());
        }

        std::string getDestQueueStr() {
            std::string q = "";

            for (int i = 0; i < this->destQueue.size(); i++) {
                std::string i_str = std::to_string(this->destQueue[i]);
                q += i_str;

                if (i != this->destQueue.size() - 1)
                    q += ",";
            }

            return q;
        }

        bool isThereRequest() {
            return this->destQueue.size() > 0;
        }

        int getDestination() {
            return this->destQueue[0];
        }

        void setReachedDestToGet(Person* p) {
            // std::cout << "REACHED DEST" << std::endl;
            for (int i = 0; i < this->destQueue.size(); i++) {
                if (this->destQueue[i] == p->getInitialFloor()) {
                    this->destQueue.erase(this->destQueue.begin() + i);
                }
            }

            this->insertPerson(p);
            p->setInside();
            
        }

        void setReachedDestToLeave(Person* p) {
            // std::cout << "REACHED DEST" << std::endl;
            for (int i = 0; i < this->destQueue.size(); i++) {
                if (this->destQueue[i] == p->getDestFloor()) {
                    this->destQueue.erase(this->destQueue.begin() + i);
                }
            }
            
            this->removePerson(p);
            p->setFinished();

            //std::cout << "set finished for " << p->getId() << std::endl;

            
        }

        std::vector<Person*> checkIdle(std::vector<Person*> people) {
            if (this->destQueue.size() == 0) {
                this->setState(IDLE);
                this->hasReset = 1;

                // std::cout << "set as idle" << std::endl;

                for (int i = 0; i < people.size(); i++) {
                    people[i]->resetRequested();
                    
                    // std::cout << "reset requested for " << people[i]->getId() << std::endl;
                }
            }

            return people;
        }

        void insertPerson(Person* newPerson) {
            
            this->peopleInside.push_back(newPerson);

            int personDest = newPerson->getDestFloor();
            this->destQueue.push_back(personDest);
            sortDestQueue();
            
            this->peopleCount++;
            this->currentWeight += newPerson->getWeight();

            newPerson->printEntered();
            this->printElevInfo();

            // signal a new person is available
            personEntered.notify();
        }

        void removePerson(Person* personRemoved) {
            __synchronized__ ;

            while (peopleCount == 0) {

                personEntered.wait();
            
            }
            int weightDecrement = personRemoved->getWeight();

            // remove from start ?????? change
            this->peopleInside.erase(this->peopleInside.begin());

            this->peopleCount--;
            this->currentWeight -= weightDecrement;

            personRemoved->printLeft();
            this->printElevInfo();
        }

        bool directionCond(Person* p) {
        
            if (p->isMovingUp() && this->state == MOVING_UP)
                return true;
            else if (!p->isMovingUp() && this->state == MOVING_DOWN)
                return true;
            else
                return false;  
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

            this->reqCount++;
            if (reqCount == 1 || hasReset == 1) {
                this->setState(MOVING_UP);
                hasReset = 0;
            }

            if (directionCond(p) && locationCond(p)) {
                p->printMadeReq();
                if (this->destQueue.size() > 0)
                    printElevInfo();
                p->setRequested();
                
                
                if (capacityCond(p, wc, pc)) {

                    int personDest = p->getInitialFloor();
                    this->destQueue.push_back(personDest);
                    sortDestQueue();

                
                    // pthread_exit(NULL);

                    return;
                }

                else {
                    p->setWaiting();
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
                      << this->getPeopleCount() << ", "
                      << this->currentFloor
                      << " -> "
                      << this->getDestQueueStr()
                      << ")" << std::endl;
        }

};

#endif