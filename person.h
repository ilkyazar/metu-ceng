#ifndef __PERSON_H
#define __PERSON_H

class Person {
    private: 
        int id;
        int weight;
        int initial_floor, dest_floor;
        int priority;
        bool isIn;
        bool triedUntilIdle;
        bool isAccepted;

    public: 
        Person(int id, int weight, int initial_floor, int dest_floor, int priority) {
            this->id = id;
			this->weight = weight;
			this->initial_floor = initial_floor;
			this->dest_floor = dest_floor;
			this->priority = priority;
            this->isIn = false;
            this->triedUntilIdle = false;
            this->isAccepted = false;
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

        std::string getPriorityStr() {
            return this->priority == 2 ? "lp" : "hp";
        }

        bool isMovingUp() {
            return this->dest_floor > this->initial_floor;
        }

        bool isInside() {
            return this->isIn;
        }

        void setInside() {
            this->isIn = true;
        }

        bool isReqAccepted() {
            return this->isAccepted;
        }

        void acceptRequest() {
            this->isAccepted = true;
        }

        void rejectRequest() {
            this->isAccepted = false;
        }

        bool didTryUntilIdle() {
            return this->triedUntilIdle;
        }

        void setTriedUntilIdle() {
            this->triedUntilIdle = true;
        }

        void resetTriedUntilIdle() {
            this->triedUntilIdle = false;
        }

        friend bool operator >(Person p1, Person p2) {
            // If the priority is 1, the person has higher priority.
            // If it is 2, the person has lower priority.
            
            if (p1.priority < p2.priority) return true;
            return false;
        }

        void printMadeReq() {
            std::cout << "Person (" << this->getId() << ", " << this->getPriorityStr() << ", " 
                      << this->getInitialFloor() << " -> " << this->getDestFloor() << ", "
                      << this->getWeight() << ") made a request" << std::endl;
        }

        void printEntered() {
            std::cout << "Person (" << this->getId() << ", " << this->getPriorityStr() << ", " 
                      << this->getInitialFloor() << " -> " << this->getDestFloor() << ", "
                      << this->getWeight() << ") entered the elevator" << std::endl;
        }

        void printLeft() {
            std::cout << "Person (" << this->getId() << ", " << this->getPriorityStr() << ", " 
                      << this->getInitialFloor() << " -> " << this->getDestFloor() << ", "
                      << this->getWeight() << ") has left the elevator" << std::endl;
        }

};

#endif