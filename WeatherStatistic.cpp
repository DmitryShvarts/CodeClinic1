#include "WeatherStatistic.h"
WeatherStatistic::WeatherStatistic()
{
	loadData();
}

WeatherStatistic::~WeatherStatistic()
{
	closeDatabase();
	//UPDATE sqlite_sequence SET SEQ =0 WHERE NAME = 'WeatherStatistic';
}

void WeatherStatistic::loadData()
{
	
	connectToDatabase();
	if (isDBEmpty())
	{
		t1 = clock();
		string sqlQuery = "BEGIN TRANSACTION";
		executeQuery(sqlQuery);

		for (int year = 2012; year <= 2015; year++)
		{
			string fileName = getPathToDataFile(year);
			readDataAndInsertToDB(fileName);
		}
		sqlQuery = "END TRANSACTION";
		executeQuery(sqlQuery);
		
		t2 = clock();
		float difference = (((float)t2) - ((float)t1));
		float seconds = difference / 1000; // float value of seconds

		cout << "Elsapsed time: " << seconds << "sec.";
		cout << "Records created successfully\n";
	}
	else
	{
		cout << "Database is loaded. " << endl;
	}
	//closeDatabase();
}
string WeatherStatistic::getPathToDataFile(int year)
{
	ostringstream fileNameStream;
	fileNameStream << PATH_TO_DATA << "Environmental_Data_Deep_Moor_" << year << ".txt";
	string fileName = fileNameStream.str();
	cout << "\nLoading: " << fileName << endl;
	return fileName;
}
void WeatherStatistic::readDataAndInsertToDB(string fName)
{
	fstream dataFileStream;
	dataFileStream.open(fName);
	getline(dataFileStream, lineOfDataFromFile);   // discard top line with headers
	int i = 0;

	
	while (getline(dataFileStream, lineOfDataFromFile))
	{
		parseLine(lineOfDataFromFile);
		insertDataFromFileToDB();	
  	if (i == 10000)
		{
			cout << '.' ;
			i = 0;
		}
		i++;
	}

	
}
int WeatherStatistic::connectToDatabase() 
{
	rc = sqlite3_open(PATH_TO_DB, &db);
	if (rc)
	{
		cerr << "Error. Can't open database: " << sqlite3_errmsg(db);
		return -1;
	}
	cout << "Opened database successfully\n";
}
void WeatherStatistic::parseLine(string str)
{
	
	istringstream buffer(str);
	buffer >> date
		>> time
		>> Air_Temp
		>> Barometric_Press
		>> Dew_Point
		>> Relative_Humidity
		>> Wind_Dir
		>> Wind_Gust
		>> Wind_Speed;
	
}
void WeatherStatistic::insertDataFromFileToDB()
{
	convertStatisticFromFileToString();
	preparQuery();
	insertLine();

}
void WeatherStatistic::closeDatabase()
{
	sqlite3_close(db);
	cout << "Database is successfully closed" << endl;
}

bool WeatherStatistic::isValidDateTime(string date, string time)
{
	// ==== startDataTime > endDataTime
	getFirstDateTime();
	if (!(date >= response[0] && time >= "00:00:00"))
	{
		cout << "The insert TimeStamp is  outside the valid range" << endl;
		return false;
	}
	getLastDateTime();
	if(!(date <= response[0] && time<= "23:59:59"))
	{
		cout << "The insert TimeStamp is  outside the valid range" << endl;
		return false;
	}
	
	time_t td, startTD, endTD;
	td = convertDateTime(date, time);
	getFirstDateTime();
	startTD = convertDateTime(response[0], response[1]);
	getLastDateTime();
	endTD = convertDateTime(response[0], response[1]);

	if(td>=startTD && td <=endTD)
		return true;

}

time_t WeatherStatistic::convertDateTime(string date, string time)
{
	int yyyy=0, mm=0, dd = 0;
	if (sscanf_s(date.c_str(), "%d_%d_%d", &yyyy, &mm, &dd) !=3)
	{
		cerr << "ERROR: Failed to parse date string " << date << endl;
		return -1;
	}

	int hh=0, min=0, ss = 0;
	if (sscanf_s(time.c_str(), "%d:%d:%d", &hh, &min, &ss) != 3)
	{
		cerr << "ERROR: Failed to parse time string " << time << endl;
		return -1;
	}

	struct tm dateTime = {};
	dateTime.tm_year = yyyy- 1900;
	dateTime.tm_mon = mm;
	dateTime.tm_mday = dd;
	dateTime.tm_hour = hh;
	dateTime.tm_min = min;
	dateTime.tm_sec = ss;

	return mktime(&dateTime);
}
//
//
//
		//float difference = (((float)t2) - ((float)t1));
		//float seconds = difference / 1000; // float value of seconds
		//cout << seconds  <<"sec Is needed to stor one line of data to database\n";

void WeatherStatistic::convertStatisticFromFileToString()
{
	AIR_TEMP = to_string(Air_Temp);
	BAROMETRIC_PRESS = to_string(Barometric_Press);
	DEW_POINT = to_string(Dew_Point);
	RELATIVE_HUMIDITY = to_string(Relative_Humidity);
	WIND_DIR = to_string(Wind_Dir);
	WIND_GUST = to_string(Wind_Gust);
	WIND_SPEED = to_string(Wind_Speed);
}
void WeatherStatistic::preparQuery()
{
	string sql = "INSERT INTO WeatherStatistic (DATA, TIME,AIR_TEMP, BAROMETRIC_PRESS, DEW_POINT, RELATIVE_HUMIDITY, WIND_DIR, WIND_GUST,WIND_SPEED) VALUES (";
	string sql_line = '\'' + date + '\'' + ',' + '\'' + time + '\'' + ',' + '\'' + AIR_TEMP + '\'' + ',' + '\'' + BAROMETRIC_PRESS + '\'' + ',' + '\'' + DEW_POINT + '\'' + ',' + '\'' + RELATIVE_HUMIDITY + '\'' + ',' + '\'' + WIND_DIR + '\'' + ',' + '\'' + WIND_GUST + '\'' + ',' + '\'' + WIND_SPEED + '\'' + ')' + ';';
	
	//string sql = "INSERT INTO WeatherStatistic (DATA, TIME,AIR_TEMP, BAROMETRIC_PRESS,WIND_SPEED) VALUES (";
	//string sql_line = '\'' + data + '\'' + ',' + '\'' + time + '\'' + ',' + '\'' + AIR_TEMP + '\'' + ',' + '\'' + BAROMETRIC_PRESS + '\'' + ',' + '\'' + WIND_SPEED + '\'' + ')' + ';';

	sql_statement = sql + sql_line;
	//cout << sql_statement << endl;
}
void WeatherStatistic::insertLine()
{
	executeQuery();
	if (rc != SQLITE_OK)
	{
		cerr << "SQL Error. Can't record to database: " << zErrMsg;	//return -1;
	}
	//cout << "Records created successfully\n";
}

bool WeatherStatistic::isDBEmpty()
{
	sql_statement = "SELECT COUNT(*) FROM WeatherStatistic";
	executeQuery(sql_statement);
	if (rc != SQLITE_OK)
	{
		cerr << "SQL Error. Can't record to database: " << zErrMsg;	//return -1;
		return false;
	}
	else 
	{
		
		if (response[0] == "0")
		{
			cout << "Data base is empty" << endl;
			return true;
		}
		else
		{
			numberOfRows = response[0];
			return false;
		}

	}
}

void WeatherStatistic::executeQuery()
{
	const char* data = "Callback function called";
	rc = sqlite3_exec(db, sql_statement.c_str(), callback, (void*) data, &zErrMsg);
}

void WeatherStatistic::executeQuery(string sqlQuery)
{
	const char* data = "Callback function called";
	rc = sqlite3_exec(db, sqlQuery.c_str(), callback, (void*)data, &zErrMsg);
}
void WeatherStatistic::getFirstDateTime()
{
	sql_statement = "SELECT DATA,TIME FROM WeatherStatistic WHERE ID = 1;";
	executeQuery();
}
void WeatherStatistic::getLastDateTime()
{
	sql_statement = "SELECT DATA,TIME FROM WeatherStatistic WHERE ID = " + numberOfRows;
	executeQuery();
}

void WeatherStatistic::getAVG(string startDate, string startTime,string endDate, string endTime)
{
	
	string queryStatic = "SELECT AVG(AIR_TEMP) FROM WeatherStatistic WHERE (DATA >=";
	string queryPermoments = '\'' + startDate + '\'' + " AND DATA <= " + '\'' + endDate + '\'' + ") AND (TIME >= " + '\'' + startTime + '\'' + " AND TIME <= " + '\'' + endTime + '\'' + ")";
	sql_statement = queryStatic + queryPermoments;
	
	executeQuery();
}