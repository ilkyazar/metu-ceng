#include <iostream>
#include <fstream>
#include <vector>
#include "elevatorMonitor.h"
#include "person.h"

using namespace std;

int num_floors, num_people;
int weight_capacity, person_capacity;

//  in microseconds
int TRAVEL_TIME, IDLE_TIME, IN_OUT_TIME;

vector<Person*> people;

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

        cout << "number of floors = " << num_floors << endl;
        cout << "number of people = " << num_people << endl;
        cout << "weight capacity = " << weight_capacity << endl;
        cout << "person capacity = " << person_capacity << endl;
        cout << "travel time = " << TRAVEL_TIME << endl;
        cout << "idle time = " << IDLE_TIME << endl;
        cout << "in out time = " << IN_OUT_TIME << endl;

        int i = 0;
        while (i < num_people) {
            string weight, initial_floor, dest_floor, priority;

            getline(file, weight, ' ');
            getline(file, initial_floor, ' ');
            getline(file, dest_floor, ' ');
            getline(file, priority, '\n');
            /*
            cout << "weight = " << weight << endl;
            cout << "initial_floor = " << initial_floor << endl;
            cout << "dest_floor = " << dest_floor << endl;
            cout << "priority = " << priority << endl;
            */
            Person* newPerson = new Person(i, stoi(weight), stoi(initial_floor), stoi(dest_floor), stoi(priority));
            people.push_back(newPerson);
            i++;
        }

        file.close();
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


    /*
        for(int i = 0; i < people.size(); i++){
                cout << people[i]->getInitialFloor() << endl;
        }
    */

}