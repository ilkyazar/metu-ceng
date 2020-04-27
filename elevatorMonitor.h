#ifndef __ELEVATOR_MON_H
#define __ELEVATOR_MON_H

#include <vector>
#include <algorithm>
#include <string>
#include "monitor.h"
#include "person.h"

using namespace std;

#define STATIONARY -1
#define IDLE 0
#define MOVING_UP 1
#define MOVING_DOWN 2

// regulates the access to the critical regions
class ElevatorMonitor:public Monitor {
    private: 
        vector<Person*> peopleInside;
        vector<int> destQueue;

        int currentFloor;
        int currentWeight;
        int peopleCount;

        int state;

        int weight_capacity, person_capacity;

        int travel_time, idle_time, in_out_time;
        
        Condition canMakeRequests, canEnter, canLeave;

    public: 
        ElevatorMonitor(int wc, int pc, int tt, int it, int iot):canMakeRequests(this), canEnter(this), canLeave(this) {
            this->currentFloor = 0; 
            this->currentWeight = 0;
            this->peopleCount = 0;
            this->state = IDLE;    

            this->weight_capacity = wc;
            this->person_capacity = pc;
            this->travel_time = tt;
            this->idle_time = it;
            this->in_out_time = iot;         
        }

        ~ElevatorMonitor() {};

        int getState() {
            return this->state;
        }

        void setState(int s) {
            this->state = s;
        }

        string getStateStr() {
            if (this->state == MOVING_UP)
                return "Moving-up";
            else if (this->state == MOVING_DOWN)
                return "Moving-down";
            else if (this->state == STATIONARY)
                return "STATIONARY";
            else
                return "Idle";           
        }

        int getCurrentFloor() {
            return this->currentFloor;
        }

        void moveUp() {
            this->setState(MOVING_UP);
            this->currentFloor++; 
            checkDestination();
        }

        void moveDown() {
            this->setState(MOVING_DOWN);
            this->currentFloor--; 
            checkDestination();
        }

        void checkDestination() {
            if (!isThereDestination()) {
                this->setState(IDLE);
                canMakeRequests.notifyAll();
                this->printElevInfo();
            }
            else {
                this->printElevInfo();
                this->setState(STATIONARY);
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
                sort(this->destQueue.begin(), this->destQueue.end());

            this->destQueue.erase(unique(this->destQueue.begin(), this->destQueue.end()), this->destQueue.end());
        }

        string getDestQueueStr() {
            string q = "";

            for (int i = 0; i < this->destQueue.size(); i++) {
                string i_str = to_string(this->destQueue[i]);
                q += i_str;

                if (i != this->destQueue.size() - 1)
                    q += ",";
            }

            return q;
        }

        bool isThereDestination() {
            return this->destQueue.size() > 0;
        }

        int getDestination() {
            return this->destQueue[0];
        }

        void addDestination(int d) {
            this->destQueue.push_back(d);
            sortDestQueue();
        }

        void insertPerson(Person* newPerson) {

            __synchronized__;
            
            this->peopleInside.push_back(newPerson);

            int personDest = newPerson->getDestFloor();
            addDestination(personDest);
            
            this->peopleCount++;
            this->currentWeight += newPerson->getWeight();

            checkDestination();
            newPerson->printEntered();
            this->printElevInfo();

        }

        void removePerson(Person* personRemoved) {

            __synchronized__;
            
            int weightDecrement = personRemoved->getWeight();

            this->peopleInside.erase(this->peopleInside.begin());

            this->peopleCount--;
            this->currentWeight -= weightDecrement;

            checkDestination();
            personRemoved->printLeft();
            this->printElevInfo();
        }

        void personTryToMakeRequest(Person* p) {

            while (!isPersonEligible(p) || p->getStatus() == REJECTED) {

                // After moving up/down, if there are no requests, 
                // set state as idle and notify all

                canMakeRequests.wait();    
                p->setWaiting();           
            }

        }

        void personMakeReq(Person* p) {

            __synchronized__;

            if (p->getStatus() != ACCEPTED) {

                p->printMadeReq();

                if (p->isMovingUp()) this->setState(MOVING_UP);
                else this->setState(MOVING_DOWN);
              
            }

        } 

        void acceptReq(Person* p) {      

            __synchronized__;

            int getPersonFrom = p->getInitialFloor();
            addDestination(getPersonFrom);    
            printElevInfo();
        }

        void deleteFromDestQueue() {
            this->destQueue.erase(this->destQueue.begin());
        }

        vector<Person*> makePeopleLeave() {

            vector<Person*> peopleLeft;
            
            printPeopleIn();

            cout << this->peopleInside.size() << endl;

            for (int i = 0; i < this->peopleInside.size(); i++) {
                
                cout << i << " : " << this->currentFloor << " " << this->peopleInside[i]->getDestFloor() << endl;

                if (this->currentFloor == this->peopleInside[i]->getDestFloor()) {
                    removePerson(this->peopleInside[i]);
                    cout << "REMOVED" << endl;
                    peopleLeft.push_back(peopleInside[i]);
                }
                cout << "here?" << endl;
            }
            return peopleLeft;
        }

        vector<Person*> makePeopleEnter(vector<Person*> people) {

            vector<Person*> peopleRejected;

            for (int i = 0; i < people.size(); i++) {
                if (this->currentFloor == people[i]->getInitialFloor()
                    && people[i]->getStatus() == ACCEPTED) {

                        if (capacityCond(people[i])) {
                            insertPerson(people[i]);
                            //people[i]->setInside();
                        }
                        else
                            peopleRejected.push_back(people[i]);
                }

            }

            return peopleRejected;
        }

        bool isPersonEligible(Person* p) {
            return (this->getState() == IDLE
                    || (directionCond(p) && locationCond(p)));
        }


        bool directionCond(Person* p) {
            if (p->isMovingUp() && this->state == MOVING_UP)
                return true;
            else if (!p->isMovingUp() && this->state == MOVING_DOWN)
                return true;
            else {
                return false; 
            } 
        }

        bool locationCond(Person* p) {
            if (this->state == MOVING_UP && p->getInitialFloor() < this->currentFloor) {
                return false;
            
            }
            if (this->state == MOVING_DOWN && p->getInitialFloor() > this->currentFloor) {
                return false;
            }
                
            return true;
        }

        bool capacityCond(Person* p) {
            return this->weight_capacity >= p->getWeight() + this->getCurrentWeight()
                                && this->person_capacity >= 1 + this->getPeopleCount();
        }

        void printPeopleIn() {
            for (int i = 0; i < peopleInside.size(); i++) {
                Person* p = peopleInside[i];
                cout << "ID: " << p->getId() << " WEIGHT: " << p->getWeight() << " INIT FLOOR: " << p->getInitialFloor() << " DEST FLOOR: " << p->getDestFloor() << " PRIORITY : " << p->getPriority() << endl;
            }
        }

        void printElevInfo() {
            cout << "Elevator (" << this->getStateStr() << ", " << this->getCurrentWeight() << ", " 
                      << this->getPeopleCount() << ", "
                      << this->currentFloor
                      << " -> "
                      << this->getDestQueueStr()
                      << ")" << endl;
        }

};

#endif