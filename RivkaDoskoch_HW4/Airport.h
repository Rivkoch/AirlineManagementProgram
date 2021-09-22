#ifndef __AIRPORT__
#define __AIRPORT__

#include "General.h"

typedef struct
{
	char*	name;
	char*	country;
	char	code[CODE_LENGTH + 1];
}Airport;

int		isSameAirport(const Airport* pPort1, const Airport* pPort2);
int		isAirportCode(const Airport* pPort1, const char* code);
void	initAirportNoCode(Airport* pPort);
void	printAirport(const Airport* pPort);
//void	getAirportName(Airport* pPort);
void    getAirportCode(char* code);

int	compareByName(const void* airport1, const void* airport2);
int	compareByCountry(const void* airport1, const void* airport2);
int	compareByCode(const void* airport1, const void* airport2);

void	freeAirport(Airport* pPort);

int		saveAirportToFile(const Airport* pAir, FILE* fp);
int		loadAirportFromFile(Airport* pAir, FILE* fp);


int bitsOpSaveAirportToFile(Airport* pAirport, FILE* fp);
int bitsOpLoadAirportFromFile(Airport* pAirport, FILE* fp);
#endif