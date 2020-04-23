#ifndef __PERSON_H
#define __PERSON_H

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

            std::cout << "New Person created with " << id << " " << weight << " " << initial_floor << " " << dest_floor << " " << priority  << "\n" << std::endl;
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

#endif