#pragma once
#include <stdlib.h>
#include "Klein.h"
#include <iostream>

namespace Klein
{
    class LinkedList
    {
        private:
            Node* LIST;    
            int number_of_nodes;  
            Node* last_node;     
        public:
            LinkedList()
            {
                LIST = nullptr;
                last_node = LIST;
                /*LIST = new Node;
                LIST->data = nullptr;
                LIST->next = nullptr;*/
                number_of_nodes = 0;
            }
            ~LinkedList() = default;   
            Node* AddNodeEnd();
            Node* AddNodeEnd(void* ptr);
            Node* AddNodeBeginning();
            Node* AddNodePosition(int p);
            Node* FindLastNode();
            Node* GetNodeAtPosition(int p);
            bool DeleteNodeEnd();
            bool DeleteNodeBeginning();
            bool DeleteNodePosition(int p);
            bool DeleteNodeWithPointer(void* ptr);
            void* GetData(int p);
            int GetDataInt(int p);
            float GetDataFloat(int p);
            bool SetData(int p, void* ptr);
            int GetNumberOfNodes();
            void Clear();
            void AppendList(LinkedList* append);
    };
     
}