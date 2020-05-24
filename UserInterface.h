#pragma once
#include "WeatherStatistic.h"

class UserInterface
{
	WeatherStatistic state;
	string startDate, startTime, endDate, endTime;

public:
	UserInterface();
	~UserInterface();
	void enterDateTime();
	void getAVG();
};

