#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Company.h"
#include "AirportManager.h"
#include "General.h"

typedef enum
{
	eAddFlight, eAddAirport, ePrintCompany, ePrintAirports, ePrintFlightOrigDest,
	eSortAirports, eSearchAirports, eNofOptions
} eMenuOptions;

const char* str[eNofOptions] = { "Add Flight", "Add Airport","PrintCompany", "Print Airport Manager",
	"Print flights between origin-destination",
	"Sort Airports", "Search Airports" };

#define EXIT			-1
#define MANAGER_FILE_NAME "airport_authority.bin"
#define COMPANY_FILE_NAME "company.txt"

int initManagerAndCompany(AirportManager* pManager, Company* pCompany);
int menu();

int main()
{
	AirportManager	manager;
	Company			company;

	initManagerAndCompany(&manager, &company);

	int option;
	int stop = 0;

	do
	{
		option = menu();
		switch (option)
		{
		case eAddFlight:
			if (!addFlight(&company, &manager))
				printf("Error adding flight\n");
			break;


		case eAddAirport:
			if (!addAirport(&manager))
				printf("Error adding airport\n");
			break;

		case ePrintCompany:
			printCompany(&company, "Hachi", "Babit", "Ba", "Olam", NULL);
			break;

		case ePrintAirports:
			printAirportManager(&manager);
			break;

		case ePrintFlightOrigDest:
			printFlightsCount(&company);
			break;

		case eSortAirports:
			sortAirport(&manager);
			break;

		case eSearchAirports:
			findAirport(&manager);
			break;

		case EXIT:
			printf("Bye bye\n");
			stop = 1;
			break;

		default:
			printf("Wrong option\n");
			break;
		}
	} while (!stop);


	saveManagerToFile(&manager, MANAGER_FILE_NAME);
	saveCompanyToFile(&company, COMPANY_FILE_NAME);

	freeManager(&manager);
	freeCompany(&company);
	return 1;
}

int menu()
{
	int option;
	printf("\n\n");
	printf("Please choose one of the following options\n");
	for (int i = 0; i < eNofOptions; i++)
		printf("%d - %s\n", i, str[i]);
	printf("%d - Quit\n", EXIT);
	scanf("%d", &option);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	return option;
}

int initManagerAndCompany(AirportManager* pManager, Company* pCompany)
{
	int res = initManagerFromFile(pManager, MANAGER_FILE_NAME);
	if (res == 1)
	{
		if (!initCompanyFromFile(pCompany, pManager, COMPANY_FILE_NAME))
			initCompany(pCompany);
	}
	else {
		printf("Cannot load from file\n");
		initManager(pManager);
		initCompany(pCompany);
	}

	return 1;
}