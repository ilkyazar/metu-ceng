#include <iostream>
#include <fstream>
#include <vector>
#include <unistd.h>
#include <algorithm>

#include "elevatorMonitor.h"
#include "person.h"

using namespace std;

int num_floors, num_people;
int weight_capacity, person_capacity;

//  in microseconds
int TRAVEL_TIME, IDLE_TIME, IN_OUT_TIME;

vector<Person*> people;
ElevatorMonitor* elevMonitor;

void printInpInfo() {
    cout << "number of floors = " << num_floors << endl;
    cout << "number of people = " << num_people << endl;
    cout << "weight capacity = " << weight_capacity << endl;
    cout << "person capacity = " << person_capacity << endl;
    cout << "travel time = " << TRAVEL_TIME << endl;
    cout << "idle time = " << IDLE_TIME << endl;
    cout << "in out time = " << IN_OUT_TIME << endl;
}

void printPeopleVec() {
    for (int i = 0; i < people.size(); i++) {
        cout << "ID: " << people[i]->getId() << " STATUS: " << people[i]->getStatus() << endl;
    }
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

void* generatePeople(void * personPtr) {
    
    Person *p = (Person *) personPtr;

    //int i = p->getId();

    elevMonitor->personMakeRequest(p);
}

void* elevatorController(void *) {
    elevMonitor->elevatorController();
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

    elevMonitor = new ElevatorMonitor(weight_capacity, person_capacity, 
                                        TRAVEL_TIME, IDLE_TIME, IN_OUT_TIME);

    cout << elevMonitor->getState() << endl;

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