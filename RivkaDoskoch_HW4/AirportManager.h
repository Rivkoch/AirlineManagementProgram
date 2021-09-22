#ifndef __AIR_MANAGER__
#define __AIR_MANAGER__

#include "Airport.h"
#include "GeneralList.h"

typedef enum { eNone, eName, eCountry, eCode, eNofSortOpt } eSortOption;
static const char* sortOptStr[eNofSortOpt];


typedef struct
{
	Airport*	airportsArr;
	int			airportsCount;
	eSortOption	sortOpt;
	LIST		airportCountryList;
}AirportManager;

int		initManagerFromFile(AirportManager* pManager, const char* fileName);
int		initManager(AirportManager* pManager);
void	baseInit(AirportManager* pManager);
int		insertCountryToList(AirportManager* pManager, int index, char* country);
int		addAirport(AirportManager* pManager);
void	setAirport(Airport* pPort, AirportManager* pManager);
Airport* findAirportByCode(const AirportManager* pManager, const char* code);
int		checkUniqeCode(const char* code, const AirportManager* pManager);
void	printAirportManager(const AirportManager* pManager);
void	sortAirport(AirportManager* pManager);
void	findAirport(const AirportManager* pManager);
eSortOption showSortMenu();


int		saveManagerToFile(const AirportManager* pManager, const char* fileName);
int		loadManagerFromFile(AirportManager* pManager, const char* fileName);

int		bitsOpSaveManagerToFile(const AirportManager* pManager, FILE* fp);
int		bitsOpLoadManagerFromFile(AirportManager* pManager, FILE* fp);

void	freeManager(AirportManager* pManager);
#endif