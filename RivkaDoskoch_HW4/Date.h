#ifndef __MYDATE__
#define __MYDATE__

#define MIN_YEAR 2020

typedef struct
{
	int			day;
	int			month;
	int			year;
}Date;

void	getCorrectDate(Date* pDate);
int		checkDate(char* date, Date* pDate);
void	printDate(const Date* pDate);
void	saveDateToFile(FILE* fp, const Date* pDate);
void	loadDateFromFile(FILE* fp, Date* pDate);
int		dateCmp(const Date* pDate1, const Date* pDate2);

#endif
