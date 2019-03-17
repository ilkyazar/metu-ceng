#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Playlist.hpp"
#include "Entry.hpp"

using namespace std;


Playlist::Playlist()
{
    srand(15);
}

int Playlist::getRandomNumber(int i, int n) const
{
    int range=n-i;
    int random = rand() % range + i;
    return random;
}

void Playlist::print()
{
    cout << "[PLAYLIST SIZE=" << entries.getSize() <<"]";
    entries.print();
}
void Playlist::printHistory()
{
    cout<<"[HISTORY]";
    history.print();
}

/* TO-DO: method implementations below */
/*load the list of playlist entries from the given file.*/
void Playlist::load(std::string fileName) {
    fstream file;
    string line;

    file.open(fileName.c_str());

    while(file.good()) {
      getline(file, line);
      istringstream ss(line);
      std::string split1, split2, split3;

      getline(ss, split1, ';');
      getline(ss, split2, ';');
      getline(ss, split3, ';');
      Entry e(split1, split2, split3);
      insertEntry(e);

    }

    file.close();
}

/*Inserts a new entry to the end of playlist entries.
 *For UNDO operation, you should save the insert operation.*/
void Playlist::insertEntry(const Entry &e) {
    Node<Entry>* prev = this->entries.getTail();
    this->entries.insertNode(prev, e);
    Operation operation = INSERT;
    HistoryRecord hist(operation, e);
    history.push(hist);
}

/*Deletes the entry with given title from the list.
 *If the delete operation is successful (i.e. the entry with given title is in the playlist and deleted successfully)..
 *you should save the this  operation for UNDO operation.*/
void Playlist::deleteEntry(const std::string &_title) {
    Node<Entry>* p = entries.getHead();
    while (p) {
        if (p->getData().getTitle() == _title) {
                Node<Entry>* prev = this->entries.findPrev(_title);
                this->entries.deleteNode(prev);

                Operation operation = DELETE;
                HistoryRecord hist(operation, p->getData());
                history.push(hist);
        }
        p = p->getNext();
  }
}

void Playlist::swap(Node<Entry>* prev, Node<Entry>* node) {
    Node<Entry>* prevOfPrev = this->entries.findPrev(prev->getData());

    prev->setNext(node->getNext());
    node->setNext(prev);

    if (prevOfPrev) prevOfPrev->setNext(node);
    else entries.setHead(node);
}


/*Moves the entry with given title to the left.*/
void Playlist::moveLeft(const std::string &title) {
    Node<Entry>* node = this->entries.findNode(title);
    Node<Entry>* prev = this->entries.findPrev(title);
    if (prev) {
        swap(prev, node);
    }
}

/*Moves the entry with given title to the right.*/
void Playlist::moveRight(const std::string &title) {
    Node<Entry>* node = this->entries.findNode(title);
    Node<Entry>* next = node->getNext();
    if (next) {
        swap(node, next);
    }
}

/*Reverse the playlist entries.*/
void Playlist::reverse() {
    MyStack<Entry> stack;
    while (this->entries.getHead()) {
        Node<Entry>* p = this->entries.getHead();
        Node<Entry>* pNext = this->entries.getHead()->getNext();
        stack.push(p->getData());
        this->entries.setHead(pNext);
    }
    entries.clear();

    while (stack.Top()) {
        Node<Entry>* top = stack.Top();

        Node<Entry>* tail = entries.getTail();
        entries.insertNode(tail, top->getData());


        top = stack.Top()->getNext();
        stack.pop();
    }

    Operation operation = REVERSE;
    Entry e("", "", "");
    HistoryRecord hist(operation, e);
    history.push(hist);

}

/*Sort the entries of the playlist from lowest to highest according to their “title”s.*/

void Playlist::swapForSort(Node<Entry>* left, Node<Entry>* right) {
    if (left->getNext() == right) swap(left, right);

    else {
        Node<Entry>* prevOfLeft = entries.findPrev(left->getData());
        Node<Entry>* prevOfRight = entries.findPrev(right->getData());

        Node<Entry>* nextOfLeft = left->getNext();
        Node<Entry>* nextOfRight = right->getNext();

        right->setNext(nextOfLeft);
        left->setNext(nextOfRight);
        if (nextOfRight == NULL) entries.setTail(left);

        if (prevOfLeft) prevOfLeft->setNext(right);
        else entries.setHead(right);
        prevOfRight->setNext(left);
      }
}


/*This method is used to sort the entries of the playlist from lowest to highest according to their
“title”s.
• You are supposed to use “Selection Sort” algorithm.
• You will assume that all playlist entries have unique titles.
• Using any of the stl containers(array, vector, list, etc.) is not allowed in this
method. */
void Playlist::sort() {
    Node<Entry>* p = entries.getHead();
    while (p) {
        Node<Entry>* pNext = p->getNext();
        Node<Entry>* min = p;
        while (pNext) {
            if (pNext->getData().getTitle() <= min->getData().getTitle()) min = pNext;
            pNext = pNext->getNext();
        }
        if (p->getData().getTitle() != min->getData().getTitle()) {
          Node<Entry>* pWillBe = p->getNext();
          swapForSort(p, min);
          p = pWillBe;
        }
        else p = p->getNext();
    }
}

/*Merge the sorted lists while keeping the sort order*/
void Playlist::merge(const Playlist & pl) {
    Node<Entry>* p2 = pl.entries.getHead();
    Node<Entry>* p1 = this->entries.getHead();

    while (p1) {
        while (p2) {
            if (p2->getData().getTitle() <= p1->getData().getTitle()) {
              Node<Entry>* p1Prev = this->entries.findPrev(p1->getData());
              Node<Entry>* p2Next = p2->getNext();
              this->entries.insertNode(p1Prev, p2->getData());
              p2 = p2Next;
            }
            else p1 = p1->getNext();
        }
        p1 = p1->getNext();
    }

}

Node<Entry>* Playlist::findElement(int n) {
    Node<Entry>* p = this->entries.getHead();
    for (int i = 0; i < n; i++)
        p = p->getNext();

    return p;
  }

/*Shuffle the playlist entries. Use getRandomNumber function to generate a random number. */
void Playlist::shuffle() {
    int n = entries.getSize();
    for (int i = 0; i <= n-2 ; i++) {
        int j = getRandomNumber(i, n);
        Node<Entry>* l = findElement(i);
        Node<Entry>* r = findElement(j);
        swapForSort(l, r);
    }
}

/*UNDO the the operations*/
void Playlist::undo() {
    //if it is reverse
    if (history.Top()->getData().getOperation() == REVERSE) {
        MyStack<Entry> stack;
        while (this->entries.getHead()) {
            Node<Entry>* p = this->entries.getHead();
            Node<Entry>* pNext = this->entries.getHead()->getNext();
            stack.push(p->getData());
            this->entries.setHead(pNext);
        }
        entries.clear();

        while (stack.Top()) {
            Node<Entry>* top = stack.Top();

            Node<Entry>* tail = entries.getTail();
            entries.insertNode(tail, top->getData());


            top = stack.Top()->getNext();
            stack.pop();
        }
    }

    //if it is insert
    else if (history.Top()->getData().getOperation() == INSERT) {
          std::string _title = history.Top()->getData().getEntry().getTitle();
          Node<Entry>* p = entries.getHead();
          while (p->getData() == _title) {
                  Node<Entry>* prev = this->entries.findPrev(_title);
                  this->entries.deleteNode(prev);
          }
    }

    //if it is delete
    else if (history.Top()->getData().getOperation() == DELETE) {
          Entry e = history.Top()->getData().getEntry();
          Node<Entry>* prev = this->entries.getTail();
          this->entries.insertNode(prev, e);
    }

    history.pop();
}
