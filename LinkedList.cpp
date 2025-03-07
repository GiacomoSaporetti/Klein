#include "LinkedList.h"
using namespace Klein;

Node * LinkedList::FindLastNode()
{
     
    Node* current_node = LIST;
    if(LIST == nullptr)
        return nullptr;   
    while(current_node->next != nullptr)
        current_node = current_node->next;
    return current_node;
}

Node* LinkedList::AddNodeEnd()
{ 
    Node* last = LinkedList::FindLastNode();
    if(last == nullptr)
    {
        LIST = new Node;
        LIST->data = nullptr;
        LIST->next = nullptr;
        number_of_nodes ++;
        return LIST;
    }
    Node* newer = new Node;
    newer->data = nullptr;
    newer->next = nullptr;

    last->next = newer;
    number_of_nodes ++;
    return newer;
}
Node* LinkedList::AddNodeEnd(void* ptr)
{
    Node* last = LinkedList::FindLastNode();
    if(last == nullptr)
    {
        LIST = new Node;
        LIST->data = ptr;
        LIST->next = nullptr;
        number_of_nodes ++;
        return LIST;
    }
    Node* newer = new Node;
    newer->data = ptr;
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
        LIST->data = nullptr;
        LIST->next = nullptr;
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
    DEBUG_MSG("GetNodeAtPosition:", p);
    int cnt=0;
    Node* res=LIST;
    
    DEBUG_MSG("GetNodeAtPosition:LIST at", res);

    if(res==nullptr)
        return nullptr;
    while(cnt<p)
    {
        if(res->next == nullptr)
            return nullptr;
        res = res->next;
        cnt++;
    }
    DEBUG_MSG("GetNodeAtPosition:return value", res);
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

int LinkedList::GetDataInt(int p)
{
    void* ptr = LinkedList::GetData(p);
    if(ptr == nullptr)
        return INT_MIN;
    return *(int*)ptr;
}

float LinkedList::GetDataFloat(int p)
{
     
    void* ptr = LinkedList::GetData(p);
    if(ptr == nullptr)
        return -INFINITY;
    return *(float*)ptr;
}

bool LinkedList::SetData(int p, void* ptr)
{
    Node* n = LinkedList::GetNodeAtPosition(p);
    if(n == nullptr)
        return false;
    n->data = ptr;
    return true;
}

int LinkedList::GetNumberOfNodes()
{return number_of_nodes;}

void LinkedList::Clear()
{
    for(int i=number_of_nodes-1; i>=0; i--)
        delete GetNodeAtPosition(i); 
    LIST = nullptr;
    number_of_nodes = 0;
}

void LinkedList::AppendList(LinkedList* append)
{
    DEBUG_MSG("AppendList:", append);
    if(append == nullptr) 
        return;
    Node* current_last_node = GetNodeAtPosition(number_of_nodes-1);

    Node*first = GetNodeAtPosition(0);
    DEBUG_MSG("AppendList:First node", first);
    DEBUG_MSG("AppendList:Last node", current_last_node);

    Node* first_node_to_append = append->GetNodeAtPosition(0);

    DEBUG_MSG("AppendList:Node to append", first_node_to_append);

    if(first_node_to_append == nullptr)
        return;
    DEBUG_MSG("AppendList:# of nodes", number_of_nodes);
    
    int nodes_to_append = append->number_of_nodes;

    DEBUG_MSG("AppendList:# of new nodes", nodes_to_append);

    if(current_last_node == nullptr)
        LIST = first_node_to_append;
    else
        current_last_node->next = first_node_to_append;
    number_of_nodes += nodes_to_append;

    DEBUG_MSG("AppendList:# of nodes now" ,number_of_nodes); 
}