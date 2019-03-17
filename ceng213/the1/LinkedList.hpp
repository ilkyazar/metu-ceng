#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <iostream>
#include "Node.hpp"

using namespace std;

/*....DO NOT EDIT BELOW....*/
template <class T>
class LinkedList {
    private:
		/*First node of the linked-list*/
        Node<T>* head;
        /*Last node of the linked-list*/
		    Node<T>* tail;
		    /*size of the linked-list*/
		    size_t  size;
    public:

        LinkedList();
        LinkedList(const LinkedList<T>& ll);
        LinkedList<T>& operator=(const LinkedList<T>& ll);
        ~LinkedList();

        /* Return head of the linked-list*/
        Node<T>* getHead() const;
        /* Set head of the linked-list*/
        void setHead(Node<T>* n);
        /* Return tail of the linked-list*/
        Node<T>* getTail() const;
        /* Set tail of the linked-list*/
        void setTail(Node<T>* n);
        /* Get the previous node of the node that contains the data item.
         * If the head node contains the data item, this method returns NULL.*/
        Node<T>* findPrev(const T& data) const;
        /* Get the node that stores the data item.
         * If data is not found in the list, this function returns NULL.*/
        Node<T>* findNode(const T& data) const;
        /* Insert a new node to store the data item.
         * The new node should be placed after the “prev” node.
         * If prev is NULL then insert new node to the head.*/
        void insertNode(Node<T>* prev, const T& data);
        /* This method is used to delete the node that is next to “prevNode”.
         * PS:prevNode is not the node to be deleted. */
        void deleteNode(Node<T>* prevNode);
        /* This method is used to clear the contents of the list.*/
        void clear();
        /* This method returns true if the list empty, otherwise returns false.*/
        bool isEmpty() const;
        /* This method returns the current size of the list. */
        size_t getSize() const;
        /*Prints the list. This method was already implemented. Do not modify.*/
        void print() const;
};

template <class T>
void LinkedList<T>::print() const{
    const Node<T>* node = head;
    while (node) {
        std::cout << node->getData();
        node = node->getNext();
    }
    cout<<std::endl;
}

/*....DO NOT EDIT ABOVE....*/

/* TO-DO: method implementations below */
template <class T>
LinkedList<T>::LinkedList(): head(NULL), tail(NULL), size(0) { }

template <class T>
LinkedList<T>::LinkedList(const LinkedList<T>& ll) {
    //cout << "before75" << endl;
    //head = new Node<T>(ll.getHead()->getData());
    LinkedList<T> tmp;
    tmp = ll;
    *this = tmp;
    //cout << "after75" << endl;
}


template <class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& ll) {

  if (this != &ll) {
    //cout << "before clear" << endl;
    //clear();
    //cout << "after clear" << endl;

    Node<T>* secondOfLl = ll.getHead()->getNext();

    Node<T>* p = new Node<T>(ll.getHead()->getData());
    setHead(p);
    while (secondOfLl && p->getNext()) {
      p->setNext(new Node<T>(secondOfLl->getData()));
      p = p->getNext();
      if (secondOfLl->getNext()) {
        secondOfLl = secondOfLl->getNext();
      }
      else {
        p->setNext(NULL);
        setTail(p);
      }
    }
  }
  return *this;
}


template <class T>
LinkedList<T>::~LinkedList() {
    //cout << "clearing" << endl;
    clear();
    //cout << "cleared" << endl;
}

template <class T>
Node<T>* LinkedList<T>::getHead() const {
    return head;
}

//only modifies the head pointer to another node
template <class T>
void LinkedList<T>::setHead(Node<T>* n) {
    head = n;
}

template <class T>
Node<T>* LinkedList<T>::getTail() const {
    return tail;
}

template <class T>
void LinkedList<T>::setTail(Node<T>* n) {
    tail = n;
}

template <class T>
Node<T>* LinkedList<T>::findPrev(const T& data) const {
    Node<T>* p = head;
    while (p && p->getNext()) {
      if (p->getNext()->getData() == data)
        return p;
      p = p->getNext();
    }
    return NULL;
}


template <class T>
Node<T>* LinkedList<T>::findNode(const T& data) const {
    Node<T>* p = head;
    while (p) {
      if (p->getData() == data)
        return p;
      p = p->getNext();
    }
    return NULL;
}

template <class T>
void LinkedList<T>::insertNode(Node<T>* prev, const T& data) {
  if (prev) {
      Node<T>* newNode = new Node<T>(data);
      //Create a node whose next will point to prev's next.
      newNode->setNext(prev->getNext());
      if (!prev->getNext()) setTail(newNode);

      prev->setNext(newNode);
  }

  else {
      Node<T>* newNode = new Node<T>(data);
      setHead(newNode);
      newNode->setNext(NULL);
      setTail(newNode);
  }
}



//When NULL is provided as prevNode, delete the head node.
template <class T>
void LinkedList<T>::deleteNode(Node<T>* prevNode) {
    if (prevNode) {
      Node<T>* tmp = prevNode->getNext();
      prevNode->setNext(tmp->getNext());
      delete tmp;
    }
    else {
      Node<T>* p = getHead();
      setHead(p->getNext());
      delete p;
    }
}

//no nodes, no memory leak
template <class T>
void LinkedList<T>::clear() {
    //cout << head << endl;
    while(head) {
        //cout << "in while-------------" << endl;
        Node<T>* tmp = head;
        setHead(head->getNext());
        //cout << "will delete temp" << endl;
        delete tmp;

    }

    setTail(NULL);
}


template <class T>
bool LinkedList<T>::isEmpty() const {
  return head == NULL;
}

template <class T>
size_t LinkedList<T>::getSize() const {
  int count = 0;
  Node<T>* p = head;
  while (p) {
    p = p->getNext();
    count++;
  }
  return count;
}

/* end of your implementations*/

#endif
