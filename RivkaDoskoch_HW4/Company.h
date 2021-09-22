#ifndef __COMP__
#define __COMP__

#include "Flight.h"
#include "AirportManager.h"
#include "GeneralList.h"

typedef struct
{
	char*		name;
	char*		address;
	int			flightCount;
	LIST		flightList;
	//	Flight**	flightArr;
}Company;

void	initCompany(Company* pComp);
int		initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName);
char*	getCompanyAddress();
int		addFlight(Company* pComp, const AirportManager* pManager);
int		insertFlightToListSorted(LIST* pflightList, Flight* pF);
void	printCompany( Company* pComp, const char* strNew,...);
char*	getCompName(const char* str, ...);
void	printFlightsCount(const Company* pComp);

int		countFlightsInRoute(const LIST* pList, const char* codeSource,
	const char* codeDest);

void	printAllFlights(const LIST* pFlightList);
void	freeAllFlights(LIST* pFlightList);

int		saveCompanyToFile(const Company* pComp, const char* fileName);
int		loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName);



void	freeCompany(Company* pComp);

char*	getParam(const char* msg);
char*	fixAddressParam(char* param);
void	changeEvenWord(char* str);
char*	combineParams(char** strArr, int length, char* numStr);
#endif

