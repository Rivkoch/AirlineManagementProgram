#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

#include "Airport.h"
#include "General.h"
#include "fileHelper.h"

int	isSameAirport(const Airport* pPort1, const Airport* pPort2)
{
	if (!pPort1 || !pPort2)
		return 0;
	if (strcmp(pPort1->code, pPort2->code) == 0)
		return 1;

	return 0;
}

int	isAirportCode(const Airport* pPort1, const char* code)
{
	if (!pPort1)
		return 0;
	if (strcmp(pPort1->code, code) == 0)
		return 1;

	return 0;
}

void	initAirportNoCode(Airport* pPort)
{
	pPort->name = getStrExactLength("Enter airport name", stdin);
	pPort->country = getStrExactLength("Enter airport country", stdin);
}

void	printAirport(const Airport* pPort)
{
	printf("Airport name:%-20s\t", pPort->name);
	printf("Country: %-20s\t Code:%s\n", pPort->country, pPort->code);

}

void getAirportCode(char* code)
{
	char temp[MAX_STR_LEN];
	int ok = 1;
	do {
		ok = 1;
		printf("Enter airport code  - %d UPPER CASE letters\t", CODE_LENGTH);
		myGets(temp, MAX_STR_LEN, stdin);
		if (strlen(temp) != CODE_LENGTH)
		{
			printf("code should be %d letters\n", CODE_LENGTH);
			ok = 0;
		}
		else {
			for (int i = 0; i < 3; i++)
			{
				if (isupper(temp[i]) == 0)
				{
					printf("Need to be upper case letter\n");
					ok = 0;
					break;
				}
			}
		}
	} while (!ok);

	strcpy(code, temp);
}

int	compareByName(const void* airport1, const void* airport2)
{
	const Airport* pAirport1 = (const Airport*)airport1;
	const Airport* pAirport2 = (const Airport*)airport2;
	return strcmp(pAirport1->name, pAirport2->name);
}

int	compareByCountry(const void* airport1, const void* airport2)
{
	const Airport* pAirport1 = (const Airport*)airport1;
	const Airport* pAirport2 = (const Airport*)airport2;
	return strcmp(pAirport1->country, pAirport2->country);
}

int	compareByCode(const void* airport1, const void* airport2)
{
	const Airport* pAirport1 = (const Airport*)airport1;
	const Airport* pAirport2 = (const Airport*)airport2;
	return strcmp(pAirport1->code, pAirport2->code);
}

void	freeAirport(Airport* pPort)
{
	free(pPort->name);
	free(pPort->country);
}

int	saveAirportToFile(const Airport* pAir, FILE* fp)
{
	if (!pAir)
		return 0;

	if (!writeStringToFile(pAir->name, fp, "Error write airport name\n"))
		return 0;
	if (!writeStringToFile(pAir->country, fp, "Error write airport country\n"))
		return 0;

	if (!writeCharsToFile(pAir->code, CODE_LENGTH, fp, "Error write airport code\n"))
		return 0;

	return 1;
}

int		loadAirportFromFile(Airport* pAir, FILE* fp)
{
	if (!pAir)
		return 0;

	pAir->name = readStringFromFile(fp, "Error reading airport name\n");
	if (!pAir->name)
		return 0;

	pAir->country = readStringFromFile(fp, "Error reading airport country\n");
	CHECK_RETURN_0 (pAir->country);

	memset(pAir->code, 0, CODE_LENGTH + 1);
	if (!readCharsFromFile(pAir->code, CODE_LENGTH, fp, "Error reading airport code\n"))
		return 0;

	return 1;
}

int bitsOpSaveAirportToFile(Airport* pAirport, FILE* fp)
{
	int nameLen = (int)strlen(pAirport->name);
	int countryLen = (int)strlen(pAirport->country);

	BYTE airportData[2];

	airportData[0] |= (nameLen & 0x1F) << 3 | (countryLen & 0xFF) >> 5;
	airportData[1] |= (countryLen & 0x1F) << 5;

	if (fwrite(airportData, sizeof(BYTE), 2, fp) != 2)
	{
		fclose(fp);
		return 0;
	}

	if (fwrite(pAirport->name, sizeof(char), (nameLen+1), fp) != (nameLen+1))
	{
		fclose(fp);
		return 0;
	}

	if (fwrite(pAirport->country, sizeof(char), (countryLen + 1), fp) != (countryLen + 1))
	{
		fclose(fp);
		return 0;
	}

	if (fwrite(pAirport->code, sizeof(char), CODE_LENGTH, fp) != CODE_LENGTH)
	{
		fclose(fp);
		return 0;
	}

	return 1;
}

int bitsOpLoadAirportFromFile(Airport * pAirport, FILE * fp)
{
	BYTE airportData[2];

	pAirport = (Airport*)malloc(sizeof(Airport));
	int nameLen, countryLen;

	if (fread(airportData, sizeof(BYTE), 2, fp) != 2)
	{
		fclose(fp);
		return 0;
	}

	nameLen = (airportData[0] >> 3) & 0x1F;
	countryLen = (int)((airportData[0] & 0x7) | (airportData[1] & 0x1F) << 3);
	pAirport->name = (char*)malloc((nameLen + 1) * sizeof(char));
	if (!pAirport->name)
	{	//aloocation didnt work
		fclose(fp);
		return 0;
	}

	if (fread(pAirport->name, sizeof(char), (nameLen+1), fp) != (nameLen+1))
	{
		fclose(fp);
		return 0;
	}
	
	pAirport->country = (char*)malloc((countryLen + 1) * sizeof(char));
	if (!pAirport->name)
	{	//aloocation didnt work
		fclose(fp);
		return 0;
	}
	if (fread(pAirport->country, sizeof(char), (countryLen+1), fp) != (countryLen+1))
	{
		fclose(fp);
		return 0;
	}

	if (fwrite(pAirport->code, sizeof(char), CODE_LENGTH, fp) != CODE_LENGTH)
	{
		fclose(fp);
		return 0;
	}

	return 0;
}
