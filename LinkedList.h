#pragma once
#include <stdlib.h>
#include "Klein.h"

namespace Klein
{
    class LinkedList
    {
        private:
            Node* LIST;    
            int number_of_nodes;       
        public:
            LinkedList()
            {
                LIST = nullptr;
                //LIST->data = nullptr;
                //LIST->next = nullptr;
                number_of_nodes = 0;
            }
            ~LinkedList() = default;   
            Node* AddNodeEnd();
            Node* AddNodeBeginning();
            Node* AddNodePosition(int p);
            Node* FindLastNode();
            Node* GetNodeAtPosition(int p);
            bool DeleteNodeEnd();
            bool DeleteNodeBeginning();
            bool DeleteNodePosition(int p);
            bool DeleteNodeWithPointer(void* ptr);
            void* GetData(int p);
    };
}