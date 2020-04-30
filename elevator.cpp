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

        // printInpInfo();

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

        Condition requestsActive, requestCame;
        Condition canEnter, canLeave;
        //Condition canMove;

        Condition sleepCond;

    public:
        Elevator():requestsActive(this), requestCame(this), canEnter(this), canLeave(this), sleepCond(this) {
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
                
                this->destQueue.erase(unique(this->destQueue.begin(), this->destQueue.end()), this->destQueue.end());
            }
            
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

        void printElevInfo() {
            cout << "Elevator (" << this->getStateStr() << ", " << this->currentWeight << ", " 
                      << this->currentPeopleCount << ", "
                      << this->currentFloor
                      << " -> "
                      << this->getDestQueueStr()
                      << ")" << endl;
        }

        void waitForRequests() {
            __synchronized__;

            //cout << "IN WAIT REQUESTS" << endl;
            if (isStationary /*state == IDLE*/) {
                while (destQueue.size() == 0 && getNumOfPeopleServed() != people.size()) {
                    cout << "WAITING...." << endl;
                    //intervalWait(IDLE_TIME);
                    requestCame.wait();
                    intervalWait(IDLE_TIME);

                }
                //cout << "REQUEST CAME" << endl;
                this->isStationary = false;

                
            }
            //else {
                //cout << "ELEVATOR IS NOT STATIONARY" << endl;

            //}
        }

        void moveUp() {
            //__synchronized__;
            intervalWait(TRAVEL_TIME);
            this->state = MOVING_UP;
            
            //intervalWait(TRAVEL_TIME);
            currentFloor++;
            this->isStationary = false;
        }

        void moveDown() {
            //__synchronized__;
            intervalWait(TRAVEL_TIME);
            this->state = MOVING_DOWN;
            
            //intervalWait(TRAVEL_TIME);
            currentFloor--; 
            this->isStationary = false;
        }

        void move() {
            __synchronized__;
            //cout << "IN MOVE" << endl;
            if (destQueue.size() > 0) {
                /*
                while (isStationary) {
                    canMove.wait();
                }*/
            
                int destFloor = destQueue[0];


                if (destFloor > currentFloor) {
                    while (destQueue[0] != currentFloor) {
                        moveUp();
                        if (destQueue[0] != currentFloor) {
                            //cout << endl;
                            //cout << "dest = " << destQueue[0] << " curr = " << currentFloor << endl;
                            //cout << "not arrived yet, so print" << endl;
                            printElevInfo();
                        }
                    }
                }
                else if (destFloor < currentFloor) {
                    while (destQueue[0] != currentFloor) {
                        moveDown();    
                        if (destQueue[0] != currentFloor)
                            printElevInfo();
                    }       
                }

                else if (destFloor == currentFloor) {
                    isStationary = true;
                    canLeave.notifyAll();
                    /*
                    intervalWait(IN_OUT_TIME/2);  
                    std::cout<<"notifying enter"<<std::endl;
                    */
                    canEnter.notifyAll();       
                    //intervalWait(IN_OUT_TIME/2);             
                    intervalWait(IN_OUT_TIME);   
                    isStationary = false;         
                }
                
            }
            //cout << "OUT OF MOVE" << endl;
            
        }

        void makeRequestSync(Person* p) {
            __synchronized__;

            destQueue.push_back(p->getInitialFloor());
            p->printMadeReq();

            requestCame.notifyAll();
                
            p->setTriedUntilIdle();

            if (state == IDLE) {
                if (p->getInitialFloor() < currentFloor) state = MOVING_DOWN;
                else state = MOVING_UP;
            }

            
            sortDestQueue();
            printElevInfo();
        }

        void makeRequest(Person* p) {
            
            while (1) {

                while ((!directionCond(p) || !locationCond(p))
                         || p->didTryUntilIdle()) {
                    if (p->isReqAccepted()) break;

                    requestsActive.wait();
                }

                if (p->isInside()) break;
                
                if (!p->isReqAccepted()) {
                    makeRequestSync(p);
                    enterPerson(p);
                }
            }
            
            leavePerson(p);

            cout << "PERSON " << p->getId() << " DONE" << endl;
            
        }

        void enterPersonSync(Person *p) {
            //__synchronized__;

            p->setInside();
            p->acceptRequest();
            

            if (destQueue.size() == 0) {
                if (p->isMovingUp()) state = MOVING_UP;
                else state = MOVING_DOWN;
            }

            destQueue.push_back(p->getDestFloor());
            sortDestQueue();
            
            currentWeight += p->getWeight();
            currentPeopleCount++;
            p->printEntered();
            printElevInfo();

        }


        void enterPerson(Person* p) {
            __synchronized__;

            while (currentFloor != p->getInitialFloor()) {
                canEnter.wait();
            }

            //cout << "PERSON " << p->getId() << " GOT THE NOTIFICATION TO ENTER" << endl;

            if (capacityCond(p) == false || (directionCond(p) == false && currentPeopleCount != 0)) {
                //cout << "REJECTED " << p->getId() << endl;
                p->rejectRequest();
                return;
            }

            if (destQueue.size() > 0) {
                //cout << "DELETE FROM QUEUE: " << destQueue[0] << endl;
                destQueue.erase(destQueue.begin());
            }

            
            printElevInfo();
            enterPersonSync(p);
            

            //canMove.notifyAll();
            
        }

        int getPeopleLeaveAt(int floor) {
            int numOfPeopleLeave = 0;

            for (int i = 0; i < people.size(); i++) {
                if (people[i]->getDestFloor() == floor) {
                    numOfPeopleLeave++;
                }
            }

            return numOfPeopleLeave;
        }

        void leavePersonSync(Person* p) {
            //__synchronized__;
            
            int total = getPeopleLeaveAt(p->getDestFloor());
            //cout << total << " people should leave at this floor (Floor = " << currentFloor << ")" << endl;
            int peopleLeftTilNow = peopleLeftAtFloors[p->getDestFloor()];
            //cout << peopleLeftTilNow << " people left til now" << endl; 

            if (destQueue.size() != 0 && peopleLeftTilNow == 0) {
                //cout << "DELETE FROM QUEUE: " << destQueue[0] << endl;
                destQueue.erase(destQueue.begin());
            }
            if (state != IDLE)
                printElevInfo();

            if (destQueue.size() == 0) {

                state = IDLE;
                

                for (int person = 0; person < people.size(); person++) {
                    people[person]->resetTriedUntilIdle();
                }

                requestsActive.notifyAll();

            }
            numOfPeopleServed++;
            cout << "NUM OF PEOPLE SERVED = " << numOfPeopleServed << endl;
            peopleLeftAtFloors[p->getDestFloor()]++;

            currentWeight -= p->getWeight();
            currentPeopleCount--;

            p->printLeft();
            printElevInfo();


        }

        void leavePerson(Person* p) {
            __synchronized__;
            
            while (currentFloor != p->getDestFloor()) {
                canLeave.wait();
            }

            leavePersonSync(p);
            
            if (destQueue.size() == 0) isStationary = true;
            else {
                //canMove.notifyAll();
            }            
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

};

Elevator* elev;

void* generatePeople(void * personPtr) {
    Person *p = (Person *) personPtr;

    elev->makeRequest(p);

}

void* elevatorController(void *) {
    while (1) {
        elev->waitForRequests(); 
        elev->move();
        if (elev->getNumOfPeopleServed() == people.size()) {
            cout << "ELEV CONTROLLER OUT" << endl;
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


    return 0;
}