#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <algorithm>

#include "elevatorMonitor.h"
#include "person.h"
#include "monitor.h"

using namespace std;

int num_floors, num_people;
int weight_capacity, person_capacity;

//  in microseconds
int TRAVEL_TIME, IDLE_TIME, IN_OUT_TIME;

vector<Person*> people;
ElevatorMonitor elevMonitor;

void printInpInfo() {
    cout << "number of floors = " << num_floors << endl;
    cout << "number of people = " << num_people << endl;
    cout << "weight capacity = " << weight_capacity << endl;
    cout << "person capacity = " << person_capacity << endl;
    cout << "travel time = " << TRAVEL_TIME << endl;
    cout << "idle time = " << IDLE_TIME << endl;
    cout << "in out time = " << IN_OUT_TIME << endl;
}

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

void printPeopleVec() {
    for (int i = 0; i < people.size(); i++) {
        cout << "ID: " << people[i]->getId() << " STATUS: " << people[i]->getStatus() << endl;
    }
}

bool isTherePersonWaiting() {
    for (int i = 0; i < people.size(); i++) {
        if (people[i]->getStatus() == WAITING) {
            return true;
        }
    }
    return false;
}

bool allPeopleFinished() {
    for (int i = 0; i < people.size(); i++) {
        if (people[i]->getStatus() != FINISHED) {
            return false;
        }
    }
    return true;
}

void waitInterval(int t) {
    /* E.g: 
    *  IDLE_TIME --> sleep for a fixed time + release the lock
    *                so that person threads can acquire
    *                & make requests
    */
    usleep(t);
}

void* generatePeople(void * personPtr) {
    Person *p = (Person *) personPtr;

    int i = p->getId();

    while (people[i]->getStatus() != FINISHED) {

        while (people[i]->getStatus() == WAITING && people[i]->hasRequested() == false) {   
            elevMonitor.personMakeReq(people[i], weight_capacity, person_capacity);
                        
        }
        /*
        if (people[i]->getStatus() == FINISHED) {
            cout << "Breaking for " << people[i]->getId() << endl;
            break;
        }
        */
            
    }

    // printPeopleVec();
    // elevMonitor.printPeopleIn();
    
}

Person* getPeopleAtFloor() {
    vector<Person*> peopleAtTheFloor;

    for (int i = 0; i < people.size(); i++) {
        if (people[i]->getInitialFloor() == elevMonitor.getCurrentFloor()
            && people[i]->getStatus() != FINISHED) {
            peopleAtTheFloor.push_back(people[i]);
        }
    }

    if (peopleAtTheFloor.size() > 0) {
        for (const auto &i: peopleAtTheFloor)
            std::sort(peopleAtTheFloor.begin(), peopleAtTheFloor.end());

        return peopleAtTheFloor[0];
    }
    return NULL;
}

vector<Person*> getPeopleToLeave() {
    vector<Person*> peopleWillLeave;

    for (int i = 0; i < people.size(); i++) {
        if (people[i]->getDestFloor() == elevMonitor.getCurrentFloor()
            && elevMonitor.isPersonIn(people[i]->getId())) {
            peopleWillLeave.push_back(people[i]);
        }
    }
    return peopleWillLeave;
}

void* elevatorController(void *) {
    
    while (!allPeopleFinished()) {

        while (elevMonitor.getState() == IDLE) {

            waitInterval(IDLE_TIME);

        }
        //cout << "OUT FROM IDLE" << endl;

        if (elevMonitor.isThereRequest()) {
            int currFl = elevMonitor.getCurrentFloor();
            int destFl = elevMonitor.getDestination();

            if (currFl < destFl) {
                waitInterval(TRAVEL_TIME);
                elevMonitor.moveUp();
            }
            else if (currFl > destFl) {
                waitInterval(TRAVEL_TIME);
                elevMonitor.moveDown();
            }
            else {
                // Checking priority in this function, multiple can try to enter, hp will
                Person* p = getPeopleAtFloor();

                // If p is NULL, this means the elevator reached the destination
                // and this destination is not to get a person
                // but to leave a person at the floor
                waitInterval(IN_OUT_TIME);
                if (p)
                    elevMonitor.setReachedDestToGet(p);
                else {
                    vector<Person*> ps = getPeopleToLeave();
                    for (int i = 0; i < ps.size(); i++) {
                        elevMonitor.setReachedDestToLeave(ps[i]);
                    }
                    people = elevMonitor.checkIdle(people);
                    //printPeopleVec();
                    //cout << "in else" << endl;
                }
                    
               
            }   
        }

        if (allPeopleFinished())
            break;
        
    }
    /*
    while (Not all people have been served) do
        if (elevator is stationary at floor x) then
            while (No one is calling the elevator) do
                Wait for requests to come ;
            end
            Person at floor y wants to go to floor z;
            if (x < y) then
                Move upwards after some delay ;
            end
                if (x > y) then
                    Move downwards after some delay ;
                else
                    Move towards the floor z;
                end
            end
            if (Another person wants to enter in currentFloor) then
                if (Person does not violate entrance constraints) then
                    Person enters the elevator ;
                end
            end
            if (Elevator reaches to a destination of a person inside the elevator) then
                Person leaves the elevator ;
        end
    end
    */
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

    

    pthread_t elevatorControllerThread;
    pthread_create(&elevatorControllerThread, NULL, elevatorController, NULL);

    pthread_t *personThreads;
    personThreads = new pthread_t[num_people];

    for (int i = 0; i < num_people; i++) {
        pthread_create(&personThreads[i], NULL, generatePeople, (void*)people[i]);
    }

    pthread_join(elevatorControllerThread, NULL);

    for (int i = 0; i < num_people; i++) {
        pthread_join(personThreads[i], NULL);
    }


    delete [] personThreads;

    return 0;
}