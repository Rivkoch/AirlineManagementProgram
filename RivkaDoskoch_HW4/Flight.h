#ifndef __FLIGHT__
#define __FLIGHT__

#include "AirportManager.h"
#include "Date.h"
#include "General.h"

typedef struct
{
	char		originCode[CODE_LENGTH + 1];
	char		destCode[CODE_LENGTH + 1];
	float		price;
	Date		date;
}Flight;

void	initFlight(Flight* pFlight, const AirportManager* pManager);
int		isFlightInRoute(const Flight* pFlight, const char* codeSource, const char* codeDest);
void	printFlight(const Flight* pFlight);
float	getFlightPrice();
Airport*	setAiportToFlight(const AirportManager* pManager, const char* msg);

int		loadFlightFromFile(Flight* pF, const AirportManager* pManager, FILE* fp);
int		saveFlightToFile(const Flight* pF, FILE* fp);

void	freeFlight(Flight* pFlight);


#endif
