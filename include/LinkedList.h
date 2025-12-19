#pragma once
#include <stdlib.h>
#include "Klein.h"
#include <iostream>

namespace Klein
{
    class LinkedList
    {
        private:
            node_t* firstNode;    
            node_t* lastNode;     
            int numberOfNodes;  
        public:
            LinkedList()
            {
                firstNode = nullptr;
                lastNode = firstNode;
                numberOfNodes = 0;
            }
            ~LinkedList() = default;
            
            int appendNode(void* data);
            int appendNode(node_t& node);
            /*node_t& addNodeBeginning();
            node_t& addNodePosition(int p);*/
            node_t* getLastNode();
            node_t* getNodeAtPosition(int p);
            node_t* getFirstNode();
            bool deleteNodeEnd();
            bool deleteNodeBeginning();
            bool deleteNodeAtPosition(int p);
            bool deleteNodeWithPointer(void* ptr);
            void* getData(int p);
            int getDataInt(int p);
            float getDataFloat(int p);
            bool setData(int p, void* ptr);
            int getNumberOfNodes();
            void clear();
            void appendList(LinkedList& listToAppend);
            void appendList(LinkedList* listToAppend);
    };
     
}