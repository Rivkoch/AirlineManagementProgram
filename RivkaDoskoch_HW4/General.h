#ifndef __GENERAL__
#define __GENERAL__

#define MAX_STR_LEN 255
#define CODE_LENGTH 3

typedef unsigned char BYTE;
#define BITS_OP


char*	getStrExactLength(const char* msg, FILE* fp);
char*	myGets(char* buffer, int size, FILE* fp);
char*	getDynStr(char* str);
char**	splitCharsToWords(char* str, int* pCount, int* pTotalLength);
void	printStr(const void* str);
void	freePtr(void* str);
void	generalArrayFunction(void* arr, int size, int typeSize, void(*func)(void* element));


#endif

