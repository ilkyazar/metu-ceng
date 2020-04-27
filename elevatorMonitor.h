#ifndef __ELEVATOR_MON_H
#define __ELEVATOR_MON_H

#include <vector>
#include <algorithm>
#include <string>
#include "monitor.h"
#include "person.h"

using namespace std;

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
        
        Condition c;

    public: 
        ElevatorMonitor(int wc, int pc, int tt, int it, int iot):c(this) {
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

        void moveUp(int numFloors) {
            this->state = MOVING_UP;
            this->currentFloor++;              
        }

        void moveDown() {
            this->state = MOVING_DOWN;
            this->currentFloor--;            
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

        void insertPerson(Person* newPerson) {
            
            this->peopleInside.push_back(newPerson);

            int personDest = newPerson->getDestFloor();
            this->destQueue.push_back(personDest);
            sortDestQueue();
            
            this->peopleCount++;
            this->currentWeight += newPerson->getWeight();

            newPerson->printEntered();
            //this->printElevInfo();

        }

        void removePerson(Person* personRemoved) {
            
            int weightDecrement = personRemoved->getWeight();

            this->peopleInside.erase(this->peopleInside.begin());

            this->peopleCount--;
            this->currentWeight -= weightDecrement;

            personRemoved->printLeft();
            //this->printElevInfo();
        }

        void personMakeRequest(Person* p) {

        }

        void elevatorController() {

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