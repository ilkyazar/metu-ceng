#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <algorithm>
#include <sys/time.h>

#include "person.h"
#include "monitor.h"

using namespace std;

int num_floors, num_people;
int weight_capacity, person_capacity;

//  in microseconds
int TRAVEL_TIME, IDLE_TIME, IN_OUT_TIME;

vector<Person*> people;


#define IDLE 0
#define MOVING_UP 1
#define MOVING_DOWN 2

void readInpFile(string filename) {
    ifstream file(filename.c_str());

    if (file) {
        string num_floors_str, num_people_str, weight_capacity_str, person_capacity_str, travel_time_str, idle_time_str, in_out_time_str;

        getline(file, num_floors_str, ' ');
        getline(file, num_people_str, ' ');
        getline(file, weight_capacity_str, ' ');
        getline(file, person_capacity_str, ' ');
        getline(file, travel_time_str, ' ');
        getline(file, idle_time_str, ' ');
        getline(file, in_out_time_str, '\n');
                
        num_floors = stoi(num_floors_str);
        num_people = stoi(num_people_str);
        weight_capacity = stoi(weight_capacity_str);
        person_capacity = stoi(person_capacity_str);
        TRAVEL_TIME = stoi(travel_time_str);
        IDLE_TIME = stoi(idle_time_str);
        IN_OUT_TIME = stoi(in_out_time_str);

        int i = 0;
        while (i < num_people) {
            string weight, initial_floor, dest_floor, priority;

            getline(file, weight, ' ');
            getline(file, initial_floor, ' ');
            getline(file, dest_floor, ' ');
            getline(file, priority, '\n');
            Person* newPerson = new Person(i, stoi(weight), stoi(initial_floor), stoi(dest_floor), stoi(priority));
            people.push_back(newPerson);
            i++;
        }

        file.close();
    }
}

class Elevator: public Monitor {
    private:
        vector<int> destQueue;
        int currentFloor;
        int currentWeight;
        int currentPeopleCount;
        int numOfPeopleServed;
        int state;
        bool isStationary;

        vector<int> peopleLeftAtFloors;

        Condition requestsActive;
        Condition canEnter, canLeave;

        Condition sleepCond;

        Condition hpEntered;

        bool personCouldNotLeave = false;

    public:
        Elevator():requestsActive(this), canEnter(this), canLeave(this), sleepCond(this), hpEntered(this) {
            this->currentFloor = 0; 
            this->currentWeight = 0;
            this->currentPeopleCount = 0;
            this->numOfPeopleServed = 0;
            this->state = IDLE;
            this->isStationary = true;

            for (int i = 0; i < num_floors; i++) {
                peopleLeftAtFloors.push_back(0);
            }
        }

        ~Elevator() {};

        void intervalWait(int t) {
            struct timespec timeSpec;
            struct timeval timeValNow;
            gettimeofday(&timeValNow, NULL);
            timeSpec.tv_nsec = (1000 * t) + (1000 * timeValNow.tv_usec);
            timeSpec.tv_sec = timeValNow.tv_sec;
            
            sleepCond.timed_wait(&timeSpec);
        }

        int getState() {
            return this->state;
        }

        string getStateStr() {
            if (this->state == MOVING_UP)
                return "Moving-up";
            else if (this->state == MOVING_DOWN)
                return "Moving-down";
            else
                return "Idle";           
        }

        int getNumOfPeopleServed() {
            return this->numOfPeopleServed;
        }

        void sortDestQueue() {
            if (this->destQueue.size() > 0) {
                
                if (state == MOVING_UP) {
                    for (const auto &i: this->destQueue) {                    
                        sort(this->destQueue.begin(), this->destQueue.end());
                    }
                }
                
                else if (state == MOVING_DOWN) {
                    for (const auto &i: this->destQueue) {
                        sort(this->destQueue.begin(), this->destQueue.end(), greater<int>()); 
                    }
                }
                
                
            }
            
        }

        void removeDuplicates() {
            if (this->destQueue.size() > 0)
                this->destQueue.erase(unique(this->destQueue.begin(), this->destQueue.end()), this->destQueue.end());
        }

        string getDestQueueStr() {
            string q = "";

            if (destQueue.size() > 0) q = " ";

            for (int i = 0; i < this->destQueue.size(); i++) {
                string i_str = to_string(this->destQueue[i]);
                q += i_str;

                if (i != this->destQueue.size() - 1)
                    q += ",";
            }

            return q;
        }

        void printElevInfo() {
            if (state == IDLE) {
                cout << "Elevator (" << this->getStateStr() << ", " << this->currentWeight << ", " 
                      << this->currentPeopleCount << ", "
                      << this->currentFloor
                      << " ->"
                      << ")" << endl;
            }
            else {
                cout << "Elevator (" << this->getStateStr() << ", " << this->currentWeight << ", " 
                        << this->currentPeopleCount << ", "
                        << this->currentFloor
                        << " ->"
                        << this->getDestQueueStr() << ")" << endl;
            }
        }

        void checkForRequests() {
            __synchronized__;

            if (isStationary) {
                while (destQueue.size() == 0 && !allHasBeenIn()) {
                    if (state == IDLE) {
                        for (int person = 0; person < people.size(); person++) {
                            people[person]->resetTriedUntilIdle();
                        }
                    }
                    requestsActive.notifyAll();
                    intervalWait(IDLE_TIME);
                }                
            }
        }

        void moveUp() {
            this->state = MOVING_UP;
            intervalWait(TRAVEL_TIME);
            
            currentFloor++;
        }

        void moveDown() {
            this->state = MOVING_DOWN;
            intervalWait(TRAVEL_TIME);
            
            currentFloor--; 
        }

        void move() {
            __synchronized__;

            while (destQueue.size() > 0 && destQueue[0] != currentFloor) {
            
                if (destQueue[0] > currentFloor) {
                    while (destQueue[0] != currentFloor) {
                        isStationary = false;
                        if (currentFloor != num_floors)
                            moveUp();
                        isStationary = true;
                        if (destQueue[0] != currentFloor) {
                            printElevInfo();
                        }
                    }
                }
                else if (destQueue[0] < currentFloor) {
                    while (destQueue[0] != currentFloor) {
                        isStationary = false;
                        if (currentFloor != 0)
                            moveDown();    
                        isStationary = true;
                        if (destQueue[0] != currentFloor)
                            printElevInfo();
                    }       
                }   
                
            }

            if (destQueue.size() > 0 && destQueue[0] == currentFloor) {
                    if (destQueue[0] == currentFloor && destQueue.size() != 0) {
                        destQueue.erase(destQueue.begin());
                    }

                    if (destQueue.size() == 0) {
                        state = IDLE;
                        for (int person = 0; person < people.size(); person++) {
                            people[person]->resetTriedUntilIdle();

                        }
                    }

                    if (!personCouldNotLeave) {
                        printElevInfo();
                    }
                    personCouldNotLeave = false;

                    canLeave.notifyAll();   
                    canEnter.notifyAll();
                    intervalWait(IN_OUT_TIME);   

                    if (getPeopleLeaveAt(currentFloor) != peopleLeftAtFloors[currentFloor]) {
                        destQueue.push_back(currentFloor);
                        sortDestQueue();
                        removeDuplicates();
                        personCouldNotLeave = true;
                    }

                    if (numOfPeopleServed != people.size()) {
                        requestsActive.notifyAll();
                    }
            }

            /*
                Elevator (Idle, 70, 1, 9 -> )
                Person (7, hp, 9 -> 3, 100) made a request
                Elevator (Moving-down, 70, 1, 9 -> )
                ...
            */
            
            else if (destQueue.size() == 0 && state != IDLE) {
                canEnter.notifyAll();
                intervalWait(IN_OUT_TIME);
            }

            isStationary = true;
        
        }

        void makeRequestSync(Person* p) {

            if (p->getInitialFloor() != currentFloor)
                destQueue.push_back(p->getInitialFloor());
            
            p->printMadeReq();
                
            p->setTriedUntilIdle();

            if (state == IDLE) {
                if (p->getInitialFloor() < currentFloor) state = MOVING_DOWN;
                
                else if (p->getInitialFloor() == currentFloor) {
                    if (p->isMovingUp()) state = MOVING_UP;
                    else state = MOVING_DOWN;
                }

                else state = MOVING_UP;
                
            }

            
            sortDestQueue();
            removeDuplicates();
            printElevInfo();
        }

        void makeRequest(Person* p) {
            __synchronized__;
            
            while (!p->isInside()) {
                while ((!directionCond(p) || !locationCond(p))
                         || p->didTryUntilIdle()) {
                    if (p->isReqAccepted()) break;
                    requestsActive.wait();
                }


                if (p->isInside()) break;
                
                if (!p->isReqAccepted()) {
                    makeRequestSync(p);
                    enterPerson(p);
                    if (p->isInside())
                        leavePerson(p);
                }
            }            
        }

        void enterPersonSync(Person *p) {

            p->setInside();
            p->acceptRequest();
            
            if (destQueue.size() == 0) {
                if (p->isMovingUp()) state = MOVING_UP;
                else state = MOVING_DOWN;
            }


            destQueue.push_back(p->getDestFloor());
            sortDestQueue();
            removeDuplicates();
            /*
                Elev (Idle, 2 ->)
                Person1 (0 -> 8) made a req
                Elev (Moving-down, 2 -> 0)
                Person2 (2 -> 6) made a req
                Elev (Moving-down, 2 -> 0)
                Person2 (2 -> 6) entered

                NO: Elev (Moving-down, 2 -> 6,0)
                NO: Elev (Moving-down, 2 -> 0,6)
                YES: Elev (Moving-up, 2 -> 6,0)
            */

            if (destQueue[0] > currentFloor) state = MOVING_UP;
            else if (destQueue[0] < currentFloor) state = MOVING_DOWN;
            //sortDestQueue();

            currentWeight += p->getWeight();
            currentPeopleCount++;
            p->printEntered();
            printElevInfo();

        }


        void enterPerson(Person* p) {

            while (currentFloor != p->getInitialFloor()) {
                
                if (state == IDLE) {
                    p->rejectRequest();
                    return;
                }

                canEnter.wait();        
            }

            if (state == IDLE) {
                p->rejectRequest();
                return;
            }

            if (p->getPriority() == 2 && gethpPeopleEnterAt(p->getInitialFloor()) > 0) {
                hpEntered.wait();
            }

            if (capacityCond(p) == false || (directionCond(p) == false && currentPeopleCount != 0)) {
                p->rejectRequest();

                return;
            }

            enterPersonSync(p);

            if (p->getPriority() == 1) {
                hpEntered.notifyAll();
            }
                        
        }

        int getPeopleLeaveAt(int floor) {
            int numOfPeopleLeave = 0;

            for (int i = 0; i < people.size(); i++) {
                if (people[i]->getDestFloor() == floor && people[i]->isInside()) {
                    numOfPeopleLeave++;
                }
            }

            return numOfPeopleLeave;
        }

        int gethpPeopleEnterAt(int floor) {
            int numPeople = 0;

            for (int i = 0; i < people.size(); i++) {
                if (people[i]->getInitialFloor() == floor && !people[i]->isInside() 
                    && people[i]->getPriority() == 1) { // high priority
                    numPeople++;
                }
            }

            return numPeople;
        }

        void leavePersonSync(Person* p) {
            
            p->printLeft();

            numOfPeopleServed++;

            peopleLeftAtFloors[p->getDestFloor()]++;
            currentWeight -= p->getWeight();
            currentPeopleCount--;

            printElevInfo();
            
        }

        void leavePerson(Person* p) {            
            while (this->currentFloor != p->getDestFloor() && p->isInside()) {
                canLeave.wait();
            }

            leavePersonSync(p);           
        }

        bool directionCond(Person* p) {
            if (state == IDLE) return true;
            if (p->isMovingUp() && this->state == MOVING_UP)
                return true;
            else if (!p->isMovingUp() && this->state == MOVING_DOWN)
                return true;
            else {
                return false; 
            } 
        }

        bool locationCond(Person* p) {
            if (state == IDLE) return true;
            if (this->state == MOVING_UP && p->getInitialFloor() < this->currentFloor) {
                return false;
            }
            if (this->state == MOVING_DOWN && p->getInitialFloor() > this->currentFloor) {
                return false;
            }               
            return true;
        }

        bool capacityCond(Person* p) {
            if (weight_capacity < p->getWeight() + this->currentWeight) return false;
            if (person_capacity < currentPeopleCount + 1) return false;
            return true;
        }

        bool allHasBeenIn() {
            for (int i = 0; i < people.size(); i++) {
                if (people[i]->isInside() == false)
                    return false;
            }
            return true;
        }

};

Elevator* elev;

void* generatePeople(void * personPtr) {
    Person *p = (Person *) personPtr;

    elev->makeRequest(p);
    //elev->leavePerson(p);

}

void* elevatorController(void *) {
    while (1) {
        elev->checkForRequests(); 
        elev->move();
        if (elev->getNumOfPeopleServed() == people.size()) {
            break;
        }
    }
}

int main(int argc, char** argv) {
    string filename = "";

    if (argc == 2) {
        filename = argv[1];
        readInpFile(filename);        
    }
    else {
        cout << "Usage: ./Elevator inp.txt\n";
        return 0;
    }

    elev = new Elevator();

    pthread_t *personThreads;
    personThreads = new pthread_t[num_people];

    for (int i = 0; i < num_people; i++) {
        pthread_create(&personThreads[i], NULL, generatePeople, (void*)people[i]);
    }

    pthread_t elevatorControllerThread;
    pthread_create(&elevatorControllerThread, NULL, elevatorController, NULL);

    for (int i = 0; i < num_people; i++) {
        pthread_join(personThreads[i], NULL);
    }

    pthread_join(elevatorControllerThread, NULL);

    delete[] personThreads;

    return 0;
}