#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>
#include "Company.h"
#include "Airport.h"
#include "General.h"
#include "Date.h"

void	initCompany(Company* pComp)
{
	printf("-----------  Init Airline Company\n");
	pComp->name = getStrExactLength("Enter company name", stdin);
	pComp->address = getCompanyAddress();
	L_init(&pComp->flightList);
	pComp->flightCount = 0;
}

int	initCompanyFromFile(Company* pComp, AirportManager* pManaer, const char* fileName)
{
	L_init(&pComp->flightList);
	pComp->flightCount = 0;
	return loadCompanyFromFile(pComp, pManaer, fileName);
}


char*	getCompanyAddress()
{
	//0 - country, 1 - city, 2 - street
	char* paramsArr[3];
	char* paramsArrStr[3] = { "country","city","street" };
	int num;
	char numStr[MAX_STR_LEN];

	printf("Enter airline adrees:\n");
	for (int i = 0; i < 3; i++)
	{
		paramsArr[i] = getParam(paramsArrStr[i]);
		if (!paramsArr[i])
			return NULL;
	}
	printf("\nEnter house number:\t");
	scanf("%d", &num);
	sprintf(numStr, "%d", num);

	char* address = combineParams(paramsArr, 3, numStr);

	for (int i = 0; i < 3; i++)
		free(paramsArr[i]);

	return address;
}



int	addFlight(Company* pComp, const AirportManager* pManager)
{
	if (pManager->airportsCount < 2)
	{
		printf("There are not enoght airport to set a flight\n");
		return 0;
	}

	Flight* pF = (Flight*)calloc(1, sizeof(Flight));
	if (!pF)
		return 0;
	initFlight(pF, pManager);
	if (!insertFlightToListSorted(&pComp->flightList, pF))
	{
		free(pF);
		return 0;
	}
	pComp->flightCount++;
	return 1;
}

//need to find the correct place to insert the flight - sorted by date
int	insertFlightToListSorted(LIST* pflightList, Flight* pF)
{
	//insert the new flight to list sorted by date
	NODE* tmp = &pflightList->head;

	while (tmp->next != NULL)
	{
		if (dateCmp(&pF->date, &((Flight*)(tmp->next->key))->date) < 0)
			break; //found the correct place
		tmp = tmp->next;
	}

	if (L_insert(tmp, pF) == NULL)
		return 0;
	return 1;
}

///THIS IS CRETICAL LOGIC TO ME FOR THE EXAM
void printCompany( Company* pComp, const char* str, ...)
{
	/*char* newName = NULL;
	int count = 0;

	va_list list;
	va_start(list, str);
	char* word = str;
	while (word != NULL)
	{
		newName = (char*)realloc(newName, (count + 1) * sizeof(char));
		if (!newName)
			return;

		newName[count] = _strdup(*word);
		strcat(newName, "_");

		count++;
		word = va_arg(list, char*);
	}
	va_end(list);
*/
	/*strcpy(newName, pComp->name);
	strcat(newName, "_");
	strcat(newName, str);*/


	char* newName = getCompName(str);

	pComp->name = newName;

	printf("Company %s\t Address:%s\n", pComp->name, pComp->address);
	printf("Has %d flights\n", pComp->flightCount);
	printAllFlights(&pComp->flightList);
}

char*	getCompName(const char* str, ...)
{
	va_list list;
	const char* strTemp;
	char* strNew = NULL;

	int combineLength = 0;
	int len;

	va_start(list, str); //init the list
	strTemp = str;

	while (strTemp != NULL)
	{
		len = (int)strlen(strTemp);
		strNew = (char*)realloc(strNew, (combineLength + len + 2) * sizeof(char));
		if (!strNew)
			return NULL;

		if (combineLength == 0)
		{
			strcpy(strNew, strTemp);
			combineLength = len;
		}
		else {
			strcat(strNew, strTemp);

			combineLength += len;
		}
		strcat(strNew, "_");
		combineLength++;

		strTemp = va_arg(list, const char*);
	}

	strNew[combineLength - 1] = '\0';
	va_end(list);
	return strNew;
}

void	printFlightsCount(const Company* pComp)
{
	char codeOrigin[CODE_LENGTH + 1];
	char codeDestination[CODE_LENGTH + 1];

	if (pComp->flightCount == 0)
	{
		printf("No flight to search\n");
		return;
	}

	printf("Origin Airport\n");
	getAirportCode(codeOrigin);
	printf("Destination Airport\n");
	getAirportCode(codeDestination);

	int count = countFlightsInRoute(&pComp->flightList, codeOrigin, codeDestination);
	if (count != 0)
		printf("There are %d flights ", count);
	else
		printf("There are No flights ");

	printf("from %s to %s\n", codeOrigin, codeDestination);
}

int	countFlightsInRoute(const LIST* pList, const char* codeSource, const char* codeDest)
{
	NODE* tmp = pList->head.next;

	int count = 0;
	while (tmp != NULL)
	{
		if (isFlightInRoute(tmp->key, codeSource, codeDest))
			count++;
		tmp = tmp->next;
	}
	return count;
}


void	printAllFlights(const LIST* pFlightList)
{
	L_print(pFlightList, printFlight);
}

void	freeAllFlights(LIST* pFlightList)
{
	L_delete(pFlightList->head.next, freeFlight);
}

int		saveCompanyToFile(const Company* pComp, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "w");
	if (!fp) {
		printf("Error open copmpany file to write\n");
		return 0;
	}

	fprintf(fp, "%s\n", pComp->name);
	fprintf(fp, "%s\n", pComp->address);
	fprintf(fp, "%d\n", pComp->flightCount);

	NODE* pNode = pComp->flightList.head.next;
	while (pNode)
	{
		if (!saveFlightToFile(pNode->key, fp))
		{
			printf("Error write flight\n");
			fclose(fp);
			return 0;
		}
		pNode = pNode->next;
	}

	fclose(fp);
	return 1;
}

int loadCompanyFromFile(Company* pComp, const AirportManager* pManager, const char* fileName)
{
	FILE* fp;
	fp = fopen(fileName, "r");
	if (!fp)
	{
		printf("Error open company file\n");
		return 0;
	}

	pComp->name = getStrExactLength(NULL, fp);
	if (!pComp->name)
	{
		fclose(fp);
		return 0;
	}

	pComp->address = getStrExactLength(NULL, fp);
	if (!pComp->address)
	{
		fclose(fp);
		return 0;
	}

	fscanf(fp, "%d", &pComp->flightCount);

	Flight* pFlight;
	for (int i = 0; i < pComp->flightCount; i++)
	{
		pFlight = (Flight*)calloc(1, sizeof(Flight));
		if (!pFlight)
		{
			printf("Alocation error\n");
			fclose(fp);
			return 0;
		}
		if (!loadFlightFromFile(pFlight, pManager, fp))
		{
			fclose(fp);
			return 0;
		}

		insertFlightToListSorted(&pComp->flightList, pFlight);

	}

	fclose(fp);
	return 1;
}


void	freeCompany(Company* pComp)
{
	freeAllFlights(&pComp->flightList);
	free(pComp->name);
}

char*	getParam(const char* msg)
{
	char temp[MAX_STR_LEN];
	char* param;

	printf("Enter %s:\t", msg);
	myGets(temp, MAX_STR_LEN, stdin);

	param = fixAddressParam(temp);

	return param;
}

char*	fixAddressParam(char* param)
{
	char* fixParamStr;
	char** wordsArray = NULL;
	int totalLength;
	int count;

	wordsArray = splitCharsToWords(param, &count, &totalLength);
	//add size for the '_' between words
	fixParamStr = (char*)calloc(totalLength + count, sizeof(char));
	if (!fixParamStr)
	{
		for (int i = 0; i < count; i++)
			free(wordsArray[i]);
		free(wordsArray);
		return NULL;
	}

	if (count == 1)
	{
		wordsArray[0][0] = toupper(wordsArray[0][0]);
		strcpy(fixParamStr, wordsArray[0]);
	}
	else {

		int len;
		for (int i = 0; i < count; i++)
		{
			len = (int)strlen(wordsArray[i]);
			if (len % 2 == 1) //odd number of letters
				changeEvenWord(wordsArray[i]);
			else {
				if (i == count - 1) //last but not only!
					wordsArray[i][0] = toupper(wordsArray[i][0]); //big
				else
					wordsArray[i][0] = tolower(wordsArray[i][0]); //small
			}
			strcat(fixParamStr, wordsArray[i]);
			if (i != count - 1) //not last
				strcat(fixParamStr, "_");
		}
	}
	for (int i = 0; i < count; i++)
		free(wordsArray[i]);
	free(wordsArray);

	return fixParamStr;

}

void	changeEvenWord(char* str)
{
	int index = 0;
	while (*str)
	{
		if (index % 2 == 0)
			*str = tolower(*str);
		else
			*str = toupper(*str);
		str++;
		index++;
	}
}

char*	combineParams(char** strArr, int length, char* numStr)
{
	char* address;
	//totalLength  - combine length + 3# + 1 '\0'
	size_t totalLength = 4 + strlen(numStr);
	for (int i = 0; i < length; i++)
		totalLength += strlen(strArr[i]);

	address = (char*)calloc(totalLength, sizeof(char));
	if (!address)
		return NULL;

	strcpy(address, strArr[0]);
	for (int i = 1; i < length; i++)
	{
		strcat(address, "#");
		strcat(address, strArr[i]);
	}
	strcat(address, "#");
	strcat(address, numStr);
	return address;
}