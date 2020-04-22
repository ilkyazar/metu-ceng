#include <iostream>
#include <fstream>
#include <vector>
#include "monitor.h"

int num_floors, num_people;
int weight_capacity, person_capacity;

//  in microseconds
int TRAVEL_TIME, IDLE_TIME, IN_OUT_TIME;


class Person {
    private: 
        int id;
        int weight;
        int initial_floor, dest_floor;
        int priority;
    public: 
        Person(int id, int weight, int initial_floor, int dest_floor, int priority) {
            this->id = id;
			this->weight = weight;
			this->initial_floor = initial_floor;
			this->dest_floor = dest_floor;
			this->priority = priority;

            std::cout << "New Person created with " << id << " " << weight << " " << initial_floor << " " << dest_floor << " " << priority << std::endl;
        }

        ~Person();

        int getId() {
            return this->id;
        }

        int getWeight() {
            return this->weight;
        }

        int getInitialFloor() {
            return this->initial_floor;
        }

        int getDestFloor() {
            return this->dest_floor;
        }

        int getPriority() {
            return this->priority;
        }

};

std::vector<Person*> people;

int main(int argc, char** argv) {
    std::string filename = "";

    if (argc == 2) {
        filename = argv[1];

        std::ifstream file(filename.c_str());
        if (file) {
            std::string num_floors_str, num_people_str, weight_capacity_str, person_capacity_str, travel_time_str, idle_time_str, in_out_time_str;

            getline(file, num_floors_str, ' ');
            getline(file, num_people_str, ' ');
            getline(file, weight_capacity_str, ' ');
            getline(file, person_capacity_str, ' ');
            getline(file, travel_time_str, ' ');
            getline(file, idle_time_str, ' ');
            getline(file, in_out_time_str, '\n');
                    
            num_floors = std::stoi(num_floors_str);
            num_people = std::stoi(num_people_str);
            weight_capacity = std::stoi(weight_capacity_str);
            person_capacity = std::stoi(person_capacity_str);
            TRAVEL_TIME = std::stoi(travel_time_str);
            IDLE_TIME = std::stoi(idle_time_str);
            IN_OUT_TIME = std::stoi(in_out_time_str);

            std::cout << "number of floors = " << num_floors << std::endl;
            std::cout << "number of people = " << num_people << std::endl;
            std::cout << "weight capacity = " << weight_capacity << std::endl;
            std::cout << "person capacity = " << person_capacity << std::endl;
            std::cout << "travel time = " << TRAVEL_TIME << std::endl;
            std::cout << "idle time = " << IDLE_TIME << std::endl;
            std::cout << "in out time = " << IN_OUT_TIME << std::endl;

            int i = 0;
            while (i < num_people) {
                std::string weight, initial_floor, dest_floor, priority;

                getline(file, weight, ' ');
                getline(file, initial_floor, ' ');
                getline(file, dest_floor, ' ');
                getline(file, priority, '\n');
                /*
                std::cout << "weight = " << weight << std::endl;
                std::cout << "initial_floor = " << initial_floor << std::endl;
                std::cout << "dest_floor = " << dest_floor << std::endl;
                std::cout << "priority = " << priority << std::endl;
                */
                Person* newPerson = new Person(i, std::stoi(weight), std::stoi(initial_floor), std::stoi(dest_floor), std::stoi(priority));
                people.push_back(newPerson);
                i++;
            }

            file.close();

            /*
            for(int i = 0; i < people.size(); i++){
		    		std::cout << people[i]->getInitialFloor() << std::endl;
			}
            */
        }
    }
    else {
        std::cout << "Usage: ./Elevator inp.txt\n";
        return 0;
    }


}