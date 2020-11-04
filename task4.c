#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int inputNext(int file, char *nextString, int maxStringLength)
{
	int isEndOfString = 0, tooMuchChars = 0, isEndOfInput = 0;
	char *nextChar = malloc(sizeof(char));
	int checkInt = 0, numberOfChars = 0;
	do
	{
		checkInt = read(file, nextChar, sizeof(char)); //Считывание посимвольно
		nextString[numberOfChars++] = *nextChar;
		isEndOfString = *nextChar == '\n' || *nextChar == EOF;
		tooMuchChars = numberOfChars >= maxStringLength;
		isEndOfInput = checkInt < sizeof(char);
		tooMuchCharsOrIsEndOfInput = tooMuchChars || isEndOfInput;
	}
	while (!isEndOfString && !tooMuchChars && !isEndOfInput);
		
	nextString[numberOfChars] = '\0';
	
	free(nextChar);
	return numberOfChars - 1;
}

void sort(int file, int *lengths, int *slides, int length, int maxStringLength)
{
	int isSameLength = 0, isFirstBigger = 0, isLexicographicUnappropriate = 0;
	char *nextString1 = malloc((maxStringLength + 1) * sizeof(char));
	char *nextString2 = malloc((maxStringLength + 1) * sizeof(char));
	int i = 0, j = 0
	for (; i < length - 1; i++)
	{
		for (; j < length - i - 1; j++)
		{
			isSameLength = lengths[j] == lengths[j + 1];
			if (isSameLength)
			{
				lseek(file, slides[j], SEEK_SET);
				read(file, nextString1, lengths[j] * sizeof(char));
				nextString1[lengths[j]] = '\0';
				lseek(file, slides[j + 1], SEEK_SET);
				read(file, nextString2, lengths[j] * sizeof(char));
				nextString2[lengths[j]] = '\0';
			}
			
			isFirstBigger = lengths[j] > lengths[j + 1];
			isLexicographicUnappropriate = strcmp(nextString1, nextString2) > 0;
			if (isFirstBigger || isSameLength && isLexicographicUnappropriate)
			{
				int temp = lengths[j];
				lengths[j] = lengths[j + 1];
				lengths[j + 1] = temp;
				
				temp = slides[j];
				slides[j] = slides[j + 1];
				slides[j + 1] = temp;
			}
		}
	}
	free(nextString1);
	free(nextString2);
}

void rewriteFile(int file, int buffer, int *lengths, int *slides, int length, int maxStringLength)
{
	int isEqual = 0, isBiggerThenMax = 0;
	char *nextString = malloc((maxStringLength + 1) * sizeof(char));
	int count = 0, max = 0;
	char *tempString = malloc((maxStringLength + 1) * sizeof(char));
	char *mostPopularString = malloc((maxStringLength + 1) * sizeof(char));;
	tempString[0] = '\0';
	int i = 0;
	for(; i < length; i++)
	{
		lseek(file, slides[i], SEEK_SET);
		read(file, nextString, (lengths[i] + 1) * sizeof(char));
		nextString[lengths[i] + 1] = '\0';
		write(buffer, nextString, (lengths[i] + 1) * sizeof(char));
		
		isEqual = strcmp(tempString, nextString) == 0;
		if (isEqual)
			count++;
		else
		{
			isBiggerThenMax = max < count;
			if (isBiggerThenMax)
			{
				max = count;
				strcpy(mostPopularString, tempString);
			}
			count = 1;
			strcpy(tempString, nextString);
		}
	}
	isBiggerThenMax = max < count;
	if (isBiggerThenMax)
	{
		max = count;
		strcpy(mostPopularString, tempString);
	}
	printf("Наиболее часто встречается строка: %s", mostPopularString);
	nextString[0] = EOF;
	write(buffer, nextString, sizeof(char));
	free(nextString);
	free(tempString);
	free(mostPopularString);
}

void copy(char *fileName, int buffer)
{
	char *nextChar = malloc(sizeof(char));
	lseek(buffer, 0, SEEK_SET);
	int file = open(fileName, O_RDWR | O_TRUNC, 0644);
	int isEndOfFile = 0;
	do
	{
		read(buffer, nextChar, sizeof(char));
		isEndOfFile = *nextChar == 0 || *nextChar == EOF;
		if (!isEndOfFile) 
			write(file, nextChar, sizeof(char));
	} 
	while (!isEndOfFile);
	free(nextChar);
}

int main (int argc, char **argv)
{
	int isEnoughArguments = 0, areFilesOpened = 0, isEndOfFile = 0, isEmptyString = 0, isFirstString = 0;
	int isEnoughCapacity = 0, isEnoughMemory = 0, isEOF = 0; // логические переменные
	int lastEnd = 0, capacity = 16, numberOfStrings = 0, koef = 1, maxStringLength;
	isEnoughArguments = argc >= 2;
	if (!isEnoughArguments)
	{
		printf("Мало аргументов!");
		return 0;
	}
	maxStringLength = atoi(argv[2]);
	int *lengths = malloc(capacity * sizeof(int));
	int *slides = malloc(capacity * sizeof(int));
	char *nextString = malloc((maxStringLength + 1) * sizeof(char));
	
	int fileInput, fileAdd;
	fileInput = open(argv[1], O_RDWR, 0644);
	fileAdd = open("add.txt", O_RDWR | O_CREAT | O_TRUNC, 0644);
	areFilesOpened = fileInput != -1 && fileAdd != -1;
	if (!areFilesOpened)
	{
		printf("Ошибка открытия файла");
		return 0;
	}
	do
	{
		lastEnd = inputNext(fileInput, nextString, maxStringLength); //Ввод новой строки и возврат номера последнего элемента
		
		isEndOfFile = nextString[lastEnd] == 0;
		if (isEndOfFile)
			break;
		isEmptyString = lastEnd == 0;
		if (isEmptyString)
		{
			koef++;
			continue;
		}
		
		lengths[numberOfStrings] = lastEnd;
		isFirstString = numberOfStrings == 0;
		if (!isFirstString)
			slides[numberOfStrings] = slides[numberOfStrings - 1] + 
				(lastEnd + koef) * sizeof(char); //Сдвиги относительно начала файла
		else 
			slides[0] = koef - 1;
		numberOfStrings++;
		koef = 1;
		
		isEnoughCapacity = capacity > numberOfStrings;
		if (!isEnoughCapacity)
		{
			capacity *= 2;
			lengths = realloc(lengths, capacity * sizeof(int));
			slides = realloc(slides, capacity * sizeof(int));
			isEnoughMemory = lengths != NULL && slides != NULL;
			if (!isEnoughMemory)
			{
				printf("Недостаточно памяти");
				return 0;
			}
		}
		isEOF = nextString[lastEnd] == EOF;
	}
	while (!isEOF);
	
	sort(fileInput, lengths, slides, numberOfStrings, maxStringLength);
	rewriteFile(fileInput, fileAdd, lengths, slides, numberOfStrings, maxStringLength);
	copy(argv[1], fileAdd);
	free(lengths);
	free(slides);
	free(nextString);
	return 0;
}