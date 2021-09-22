#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AirportManager.h"
#include "fileHelper.h"


static const char* sortOptStr[eNofSortOpt] = {
	"None","Name", "Country", "Airport code" };


int	initManagerFromFile(AirportManager* pManager, const char* fileName)
{
	baseInit(pManager);
	return loadManagerFromFile(pManager, fileName);
}

int	initManager(AirportManager* pManager)
{
	baseInit(pManager);

	int count = 0;
	do {
		printf("How many airport?\t");
		scanf("%d", &count);
	} while (count < 0);
	//clean buffer
	char tav;
	scanf("%c", &tav);
	if (count == 0)
		return 1;

	pManager->airportsArr = (Airport*)malloc(count * sizeof(Airport));
	if (!pManager->airportsArr)
		return 0;

	for (int i = 0; i < count; i++)
	{
		setAirport(&pManager->airportsArr[i], pManager);
		insertCountryToList(pManager, i, pManager->airportsArr[i].country);
		pManager->airportsCount++;
	}

	return 1;
}

void	baseInit(AirportManager* pManager)
{
	printf("-----------  Init airport Manager\n");
	pManager->airportsCount = 0;
	pManager->airportsArr = NULL;
	L_init(&pManager->airportCountryList);
	pManager->sortOpt = eNone;
}

int	insertCountryToList(AirportManager* pManager, int index, char* country)
{
	for (int i = 0; i < index; i++)
	{
		if (strcmp(country, pManager->airportsArr[i].country) == 0) //country no unique
			return 1;
	}

	if (L_insert(&(pManager->airportCountryList.head), country) == NULL)
		return 0;
	return 1;
}

int	addAirport(AirportManager* pManager)
{
	pManager->airportsArr = (Airport*)realloc(pManager->airportsArr, (pManager->airportsCount + 1) * sizeof(Airport));
	CHECK_RETURN_0(pManager->airportsArr);

	setAirport(&pManager->airportsArr[pManager->airportsCount], pManager);
	insertCountryToList(pManager, pManager->airportsCount, pManager->airportsArr[pManager->airportsCount].country);
	pManager->airportsCount++;
	return 1;
}

void  setAirport(Airport* pPort, AirportManager* pManager)
{
	while (1)
	{
		getAirportCode(pPort->code);
		if (checkUniqeCode(pPort->code, pManager))
			break;

		printf("This code already in use - enter a different code\n");
	}

	initAirportNoCode(pPort);
}

Airport* findAirportByCode(const AirportManager* pManager, const char* code)
{
	for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (isAirportCode(&pManager->airportsArr[i], code))
			return &pManager->airportsArr[i];
	}
	return NULL;
}

int checkUniqeCode(const char* code, const AirportManager* pManager)
{
	Airport* port = findAirportByCode(pManager, code);

	if (port != NULL)
		return 0;

	return 1;
}

void	printAirportManager(const AirportManager* pManager)
{
	printf("There are %d airports\n", pManager->airportsCount);
	generalArrayFunction((void*)pManager->airportsArr, pManager->airportsCount, sizeof(Airport), printAirport);
	printf("\nCountries List:\n");
#ifdef DETAIL_PRINT
	L_print(&pManager->airportCountryList, printStr);

#endif // DETAIL_PRINT

}

void	sortAirport(AirportManager* pManager)
{
	pManager->sortOpt = showSortMenu();
	int(*compare)(const void* air1, const void* air2) = NULL;

	switch (pManager->sortOpt)
	{
	case eName:
		compare = compareByName;
		break;
	case eCountry:
		compare = compareByCountry;
		break;
	case eCode:
		compare = compareByCode;
		break;
	}

	if (compare != NULL)
		qsort(pManager->airportsArr, pManager->airportsCount, sizeof(Airport), compare);

}

void	findAirport(const AirportManager* pManager)
{
	int(*compare)(const void* air1, const void* air2) = NULL;
	Airport port = { 0 };

	switch (pManager->sortOpt)
	{
	case eName:
		port.name = getStrExactLength("Enter airport name to search", stdin);
		compare = compareByName;
		break;
	case eCountry:
		getchar();
		port.country = getStrExactLength("Enter airport country to search", stdin);
		compare = compareByCountry;
		break;
	case eCode:
		getchar();
		getAirportCode(port.code);
		compare = compareByCode;
		break;

	default:
		printf("The search cannot be performed, array not sorted\n");
		break;
	}

	qsort(pManager->airportsArr, pManager->airportsCount, sizeof(Airport), compare);

	Airport* pFound = bsearch(&port, pManager->airportsArr, pManager->airportsCount, sizeof(Airport), compare);
	if (pFound == NULL)
		printf("Airport was not found\n");
	else {
		printf("Airport found, ");
		printAirport(pFound);
	}
}

eSortOption showSortMenu()
{
	int opt;
	printf("Base on what field do you want to sort?\n");
	do {
		for (int i = 1; i < eNofSortOpt; i++)
			printf("Enter %d for %s\n", i, sortOptStr[i]);
		scanf("%d", &opt);
	} while (opt < 0 || opt >eNofSortOpt);

	return (eSortOption)opt;
}

int		bitsOpSaveManagerToFile(const AirportManager* pManager, FILE* fp)
{
	BYTE managerData[2];


	managerData[0] |= (pManager->sortOpt & 0x3) << 6 | (pManager->airportsCount & 0xFF) >> 2;
	managerData[1] |= (pManager->airportsCount & 0xFF) << 2;

	if (fwrite(managerData, sizeof(BYTE), 2, fp) != 2)
	{
		fclose(fp);
		return 0;
	}

	for (int i = 0; i < pManager->airportsCount; i++)
	{
		bitsOpSaveAirportToFile(&pManager->airportsArr[i], fp);
	}

	fclose(fp);
	return 0;
}

int		bitsOpLoadManagerFromFile(AirportManager* pManager, FILE* fp)
{		
	BYTE managerData[2];

	if (fread(&managerData, sizeof(BYTE), 2, fp) != 2)
	{
		fclose(fp);
		return 0;
	}

	if (fread(managerData, sizeof(BYTE), 2, fp) != 2)
		return 0;

	pManager->airportsCount = (int)(managerData[1]  | (managerData[0] & 0x3F) << 8);

	if (pManager->airportsCount > 0)
	{
		pManager->airportsArr = (Airport*)malloc(pManager->airportsCount * sizeof(Airport));
		if (!pManager->airportsArr)
		{
			fclose(fp);
			return 0;
		}
	}

	pManager->sortOpt = (int)((managerData[0] >> 6) & 0x3);

	for (int i = 0; i < pManager->airportsCount; i++)
	{
		bitsOpLoadAirportFromFile(&pManager->airportsArr[i], fp);
	}

	return 1;
}


int		saveManagerToFile(const AirportManager* pManager, const char* fileName)
{
	FILE* fp;

	fp = fopen(fileName, "wb");
	if (!fp)
	{
		printf("Error open airport manager file to write\n");
		return 0;
	}
	
	

#ifdef BITS_OP
	
	if (!(bitsOpSaveManagerToFile(pManager, fp)))
		return 0;

#else

	if (!writeIntToFile(pManager->airportsCount, fp, "Error write airport count\n"))
		return 0;

	if (!writeIntToFile(pManager->sortOpt, fp, "Error write sort option\n"))
		return 0;


	for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (!saveAirportToFile(&pManager->airportsArr[i], fp))
			return 0;
	}

	fclose(fp);
#endif

	return 1;
}


int		loadManagerFromFile(AirportManager* pManager, const char* fileName)
{
	FILE* fp;

	fp = fopen(fileName, "rb");
	if (!fp)
	{
		printf("Error open airport manager file\n");
		return 0;
	

#ifdef BITS_OP

	if (!bitsOpLoadManagerFromFile(pManager, fp))
		return 0;

#else

	

	if (!readIntFromFile(&pManager->airportsCount, fp, "Error reading airport count\n"))
	{
		fclose(fp);
		return 0;
	}

	pManager->airportsArr = (Airport*)malloc(pManager->airportsCount * sizeof(Airport));
	if (!pManager->airportsArr)
	{
		fclose(fp);
		return 0;
	}

	int opt;
	if (!readIntFromFile(&opt, fp, "Error reading sort option\n"))
	{
		fclose(fp);
		return 0;
	}

	pManager->sortOpt = (eSortOption)opt;

	for (int i = 0; i < pManager->airportsCount; i++)
	{
		if (!loadAirportFromFile(&pManager->airportsArr[i], fp))
		{
			printf("Error loading airport from file\n");
			free(pManager->airportsArr);
			fclose(fp);
			return 0;
		}
		insertCountryToList(pManager, i, pManager->airportsArr[i].country);
	}

	fclose(fp);

#endif

	return 1;
}

void	freeManager(AirportManager* pManager)
{
	generalArrayFunction((void*)pManager->airportsArr, pManager->airportsCount, sizeof(Airport), freeAirport);
	free(pManager->airportsArr);
	L_free(&pManager->airportCountryList, NULL); //the country will be free at airport
}
