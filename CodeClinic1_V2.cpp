// CodeClinic1_V2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "WeatherStatistic.h"
#include "UserInterface.h"

int main()
{
    UserInterface userInterface;
    userInterface.enterDateTime();
    userInterface.getAVG();

    return 0;


}

