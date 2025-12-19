#include "LinkedList.h"
#include <typeinfo>

using namespace Klein;


node_t* LinkedList::getFirstNode(){return firstNode;}

node_t* LinkedList::getLastNode(){return lastNode;}

node_t* LinkedList::getNodeAtPosition(int p)
{
    int cnt=0;
    node_t* res=firstNode;
    
    if(res==nullptr)    return nullptr;
    while(cnt<p)
    {
        if(res->next == nullptr)    return nullptr;
        res = res->next;
        cnt++;
    }
    return res;
}



int LinkedList::appendNode(void* data)
{
    if(lastNode == nullptr)
    {
        ASSERT(firstNode == nullptr)
        firstNode = new node_t;
        firstNode->data = data;
        firstNode->next = nullptr;
        numberOfNodes = 1;
        lastNode = firstNode;
        return numberOfNodes;
    }

    node_t* newNode = new node_t;
    ASSERT(newNode != nullptr)

    newNode->data = data;
    newNode->next = nullptr;

    lastNode->next = newNode;
    numberOfNodes ++;
    lastNode = newNode;
    return numberOfNodes;
}

int LinkedList::appendNode(node_t& node)
{
    if(lastNode == nullptr)
    {
        ASSERT(firstNode == nullptr)
        firstNode = &node;
        firstNode->next = nullptr;
        numberOfNodes = 1;
        lastNode = firstNode;
        return numberOfNodes;
    }

    node.next = nullptr;

    lastNode->next = &node;
    numberOfNodes ++;
    lastNode = &node;
    return numberOfNodes;
}

/*node_t& LinkedList::addNodeBeginning()
{
    if(firstNode == nullptr)
    {
        firstNode = new node_t;
        ASSERT(firstNode != nullptr)
        firstNode->data = nullptr;
        firstNode->next = nullptr;
        numberOfNodes ++;
        return firstNode;
    } 

    node_t* newer = new node_t;
    newer->data = nullptr;
    //newer.next = nullptr;

    node_t* previos_first = firstNode;
    firstNode = newer;
    firstNode->next = previos_first;
    numberOfNodes ++;

    return newer;
}

node_t* LinkedList::addNodePosition(int p)
{
    node_t* currentNode = LinkedList::getNodeAtPosition(p);

    if(currentNode == nullptr)  return nullptr;

    node_t* newer = new node_t;
    newer->next = currentNode->next;
    currentNode->next = newer;
    numberOfNodes ++;
    return newer;
}*/


bool LinkedList::deleteNodeEnd()
{
    node_t* newLast =  LinkedList::getNodeAtPosition(numberOfNodes-1);
    node_t* currentLast = newLast->next;

    if(currentLast == nullptr) return false;

    delete currentLast;

    newLast->next = nullptr;
    numberOfNodes --;
    lastNode = newLast;
    return true;
}

bool LinkedList::deleteNodeBeginning()
{
    node_t* temp = firstNode;

    firstNode = firstNode->next;
    delete temp;
    numberOfNodes --;
    return true;
}

bool LinkedList::deleteNodeAtPosition(int position)
{
    if(position<0 || position>(numberOfNodes-1)) return false;

    node_t* previousNode = LinkedList::getNodeAtPosition(position-1);
    node_t* node_p = previousNode->next;
    
    if(node_p == nullptr)  return false;
    
    previousNode->next = node_p->next;
    delete node_p;
    numberOfNodes --;
    return true;
}

bool LinkedList::deleteNodeWithPointer(void * ptr)
{
    node_t* currentNode = firstNode;
    int cnt=0;
    while(currentNode->data != ptr && currentNode->next != nullptr)
    {
        currentNode = currentNode->next;
        cnt++;
    }
    if(currentNode->data != ptr)    return false;
    
    LinkedList::getNodeAtPosition(cnt-1)->next = currentNode->next;
    delete currentNode;
    numberOfNodes --;
    return true;
}

void* LinkedList::getData(int p)
{ 
    node_t*n = LinkedList::getNodeAtPosition(p);
     
    if(n == nullptr)    return nullptr;
     
    return n->data;
}

int LinkedList::getDataInt(int p)
{
    void* ptr = LinkedList::getData(p);
    if(ptr == nullptr)  return INT_MIN;
    return *(int*)ptr;
}

float LinkedList::getDataFloat(int p)
{
     
    void* ptr = LinkedList::getData(p);
    if(ptr == nullptr)  return -INFINITY;
    return *(float*)ptr;
}

bool LinkedList::setData(int p, void* ptr)
{
    node_t* n = LinkedList::getNodeAtPosition(p);
    if(n == nullptr)    return false;
    n->data = ptr;
    return true;
}

int LinkedList::getNumberOfNodes(){return numberOfNodes;}

void LinkedList::clear()
{
    firstNode = nullptr;
    lastNode = firstNode;
    numberOfNodes = 0;
}

void LinkedList::appendList(LinkedList& listToAppend)
{
    node_t* firstNodeInAppendList = listToAppend.getFirstNode();

    if(firstNodeInAppendList == nullptr)    return;
        
    int numberOfNodesInAppendList = listToAppend.getNumberOfNodes();
    
    if(firstNode == nullptr) firstNode = firstNodeInAppendList;
    else    lastNode->next = firstNodeInAppendList;  
   
    numberOfNodes += numberOfNodesInAppendList;
    lastNode = listToAppend.getLastNode();
}

void LinkedList::appendList(LinkedList* listToAppend)
{
    LinkedList::appendList(*listToAppend);
}