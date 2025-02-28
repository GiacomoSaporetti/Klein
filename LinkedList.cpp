#include "LinkedList.h"

using namespace Klein;

Node * LinkedList::FindLastNode()
{
    Node* current_node = LIST;

    if(LIST == nullptr)
    {
        LIST = new Node;
        return LIST;
    }    

    while(current_node->next != nullptr)
    {
        current_node = current_node->next;
    }
    return current_node;
}

Node* LinkedList::AddNodeEnd()
{
    Node* last = LinkedList::FindLastNode();
    Node* newer = new Node;
    newer->data = nullptr;
    newer->next = nullptr;

    last->next = newer;
    number_of_nodes ++;
    return newer;
}

Node* LinkedList::AddNodeBeginning()
{
    if(LIST == nullptr)
    {
        LIST = new Node;
        number_of_nodes ++;
        return LIST;
    } 

    Node* newer = new Node;
    newer->data = nullptr;
    //newer.next = nullptr;

    Node* previos_first = LIST;
    LIST = newer;
    LIST->next = previos_first;
    number_of_nodes ++;
    return newer;
}

Node* LinkedList::AddNodePosition(int p)
{
    Node* current_node = LinkedList::GetNodeAtPosition(p);

    if(current_node == nullptr)
        return nullptr;

    Node* newer = new Node;
    newer->next = current_node->next;
    current_node->next = newer;
    number_of_nodes ++;
    return newer;
}

Node* LinkedList::GetNodeAtPosition(int p)
{
    int cnt=0;
    Node* res=LIST;
    while(cnt<p)
    {
        if(res->next == nullptr)
            return nullptr;
        res = res->next;
        cnt++;
    }

    return res;
}

bool LinkedList::DeleteNodeEnd()
{
    Node* last = LinkedList::GetNodeAtPosition(number_of_nodes);

    if(last == nullptr)
        return false;

    delete last;

    Node* new_last = LinkedList::GetNodeAtPosition(number_of_nodes-1);
    new_last->next = nullptr;
    number_of_nodes --;
    return true;
}

bool LinkedList::DeleteNodeBeginning()
{
    Node* temp = LIST;

    LIST = LIST->next;
    delete temp;
    number_of_nodes --;
    return true;
}

bool LinkedList::DeleteNodePosition(int p)
{
    Node* node_p = LinkedList::GetNodeAtPosition(p);
    if(node_p == nullptr)
        return false;
    Node*   previous_node = LinkedList::GetNodeAtPosition(p-1);
    previous_node->next = node_p->next;
    delete node_p;
    number_of_nodes --;
    return true;
}

bool LinkedList::DeleteNodeWithPointer(void * ptr)
{
    Node* current_node = LIST;
    int cnt=0;
    while(current_node->data != ptr && current_node->next != nullptr)
    {
        current_node = current_node->next;
        cnt++;
    }
    if(current_node->data != ptr)
        return false;
    
    LinkedList::GetNodeAtPosition(cnt-1)->next = current_node->next;
    delete current_node;
    number_of_nodes --;
    return true;
}

void* LinkedList::GetData(int p)
{
    Node*n = LinkedList::GetNodeAtPosition(p);
    if(n == nullptr)
        return nullptr;
    return n->data;
}