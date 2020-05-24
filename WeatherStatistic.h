#pragma once

#include <iostream> //cout
#include <string>
#include <sqlite3.h>
#include <fstream>
#include <sstream>  //ostringstream
#include<time.h> // for clock
#include<stdio.h>
#include <ctime>
#include <map>

#define PATH_TO_DATA "resources/"
#define PATH_TO_DB "resources/Lake_Ponderay.db"
using namespace std;

static string response [5];

class WeatherStatistic
{
private:
	clock_t t1, t2;
	string lineOfDataFromFile;
	string date, time;
	double Air_Temp, Barometric_Press, Dew_Point, Relative_Humidity, Wind_Dir, Wind_Gust, Wind_Speed;
	string AIR_TEMP, BAROMETRIC_PRESS, DEW_POINT, RELATIVE_HUMIDITY, WIND_DIR, WIND_GUST, WIND_SPEED;

	sqlite3* db;
	char* zErrMsg = 0;
	int rc;
	string sql_statement;
	string numberOfRows;
	

	void loadData();
	void insertDataFromFileToDB();
	void parseLine(string);
	void convertStatisticFromFileToString();
	void preparQuery();
	void insertLine();
	bool isDBEmpty();
	void executeQuery();
	void executeQuery(string sqlQuery);
	
	

public:
	WeatherStatistic();
	~WeatherStatistic();
	string getPathToDataFile(int );
	void readDataAndInsertToDB(string);
	int connectToDatabase();
	void closeDatabase();	
	void getFirstDateTime();
	void getLastDateTime();
	time_t convertDateTime(string d, string t);
	bool isValidDateTime(string date, string time);
	static int callback(void* data, int argc, char** argv, char** azColName) {
		int i;
		fprintf(stderr, "%s: ", (const char*)data);
		printf("\n");

		for (i = 0; i < argc; i++)
		{
			printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
			response[i] = argv[i];

		}

		printf("\n");

		return 0;
	}
	void getAVG(string, string, string, string);


	
	

};

