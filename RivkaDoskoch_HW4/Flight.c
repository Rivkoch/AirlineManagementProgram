#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Flight.h"


void	initFlight(Flight* pFlight, const AirportManager* pManager)
{
	Airport* pPortOr = setAiportToFlight(pManager, "Enter code of origin airport:");
	strcpy(pFlight->originCode, pPortOr->code);
	int same;
	Airport* pPortDes;
	do {
		pPortDes = setAiportToFlight(pManager, "Enter code of destination airport:");
		same = isSameAirport(pPortOr, pPortDes);
		if (same)
			printf("Same origin and destination airport\n");
	} while (same);
	strcpy(pFlight->destCode, pPortDes->code);
	getCorrectDate(&pFlight->date);
	pFlight->price = getFlightPrice();
}

int	isFlightInRoute(const Flight* pFlight, const char* codeSource, const char* codeDest)
{
	if ((strcmp(pFlight->originCode, codeSource) == 0) &&
		(strcmp(pFlight->destCode, codeDest) == 0))
		return 1;

	return 0;
}



void	printFlight(const Flight* pFlight)
{
	printf("Flight From %s To %s\t", pFlight->originCode, pFlight->destCode);
	printDate(&pFlight->date);
	printf("Price: %.2f\n", pFlight->price);
}


float getFlightPrice()
{
	float price;
	do {
		printf("Enter flight basic price:\t");
		scanf("%f", &price);
	} while (price < 0);
	return price;
}

Airport* setAiportToFlight(const AirportManager* pManager, const char* msg)
{
	char code[MAX_STR_LEN];
	Airport* port;
	do
	{
		printf("%s\t", msg);
		myGets(code, MAX_STR_LEN, stdin);
		port = findAirportByCode(pManager, code);
		if (port == NULL)
			printf("No airport in this country - try again\n");
	} while (port == NULL);

	return port;

}

int saveFlightToFile(const Flight* pF, FILE* fp)
{
	fprintf(fp, " %s ", pF->originCode);
	fprintf(fp, " %s ", pF->destCode);
	fprintf(fp, "%f ", pF->price);
	saveDateToFile(fp, &pF->date);
	return 1;
}


int loadFlightFromFile(Flight* pF, const AirportManager* pManager, FILE* fp)
{
	fscanf(fp, "%s", pF->originCode);
	fscanf(fp, "%s", pF->destCode);
	fscanf(fp, "%f", &pF->price);
	loadDateFromFile(fp, &pF->date);
	return 1;

}


void	freeFlight(Flight* pFlight)
{
	free(pFlight);
}
