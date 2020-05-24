#include "UserInterface.h"
UserInterface::UserInterface()
{

}
UserInterface::~UserInterface()
{

}
void UserInterface::enterDateTime()
{
    
    cout << "Please, enter the START DATE as YYYY_MM_DD: ";
    cin >> startDate;
    cout << "Please, enter START TIME as hh:mm:ss:";
    cin >> startTime;
    state.isValidDateTime(startDate, startTime);

    cout << "Please, enter the END DATE as YYYY_MM_DD: ";
    cin >> endDate;
    cout << "Please, enter END TIME as hh:mm:ss:";
    cin >> endTime;
    state.isValidDateTime(endDate, endTime);
   

}
void UserInterface::getAVG() 
{
    state.getAVG(startDate, startTime, endDate, endTime);

}