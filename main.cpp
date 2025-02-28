#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <iostream>
#include <Windows.h>
//#include "SDL3/SDL.h"
#include "TimeHandler.h"
#include "Entity.h"
#include "LinkedList.h"

using namespace std;

Klein::TimeHandler TIME;
Klein::LinkedList LIST;
Klein::Entity * e;
int Klein::Entity::time_direction = 1;
Klein::TimeHandler* Klein::Entity::TIMER = &TIME;


int puntato = -19;

int main()
{
    //TIME.ChangeSpeed(0.33);
    srand((unsigned)time(NULL));
    e = NULL;
    int first = rand()%5 + 1;
    int second = first + rand()%5 + 1;
    int third = second + rand()%5 + 1;

    LIST.GetNodeAtPosition(0)->data = (void*)&puntato;

    cout << *(unsigned int*)LIST.GetData(0) << endl;
    return 0;

}