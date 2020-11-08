#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int isOutOfMemory(char *link) 
{
	if(link == NULL) 
		return 1;
	else 
		return 0;
}

void freeArray(char** array, int size)
{
	for (int i = 0; i < size; i++)
	{
		free(array[i]);
	}
	free(array);
}

char *inputString(int file, int *lastEnd) 
{
	int isEndOfString = 0, isSpace = 0; //Bool
	int length = 0, capacity = 16, checkInt = sizeof(char);
	char *nextString = malloc(capacity * sizeof(char));
	if(isOutOfMemory(nextString)) //Если память закончилась
	{
		printf("No dynamic memory\n");
		return NULL;
	}
	char nextChar, EOFC = '.';
	do
	{
		if(file == -1) 
			nextChar = getchar();
		else
			checkInt = read(file, &nextChar, sizeof(char));
		isSpace = nextChar == ' ';
		isEndOfString = (nextChar == EOF || checkInt == 0);
	}
	while (isSpace && !isEndOfString);
	
	int quoteCheck = nextChar == '"';
	isEndOfString = (nextChar == EOF || checkInt == 0);
	while(!isEndOfString)
	{
		if (nextChar != '"')
			nextString[length++] = nextChar;
		if(length >= capacity) //Проверка на то, нужно ли выделить память
		{
			capacity *= 2;
			nextString = realloc(nextString, capacity * sizeof(char));
			if(isOutOfMemory(nextString)) 
			{
				printf("No dynamic memory\n");
				return NULL;
			}
		}
		if(file == -1) 
			nextChar = getchar();
		else
			checkInt = read(file, &nextChar, sizeof(char));
		
		if (nextChar == '"')
			quoteCheck = (quoteCheck + 1) % 2;
		
		if (quoteCheck == 1) 
			isEndOfString = 0;
		else
			isEndOfString = (nextChar == ' ' || nextChar == EOF || checkInt == 0);
	}
	nextString[length++] = nextChar;
	nextString[length] = '\0';
	*lastEnd = length - 1;
	return nextString;
}

int main(int argc, char **argv)
{
	int fileInput = -1, textCapacity = 8, size = 0;
	int isEndOfFile = 0, isEOF = 0;
	char EOFC = '.';
	if (argc >= 1)
		fileInput = open(argv[1], O_RDONLY | O_CREAT, 0644); //Если есть имя файла - вводим из него
	int *lastEnd = malloc(sizeof(int));
	char **textArray = malloc(textCapacity * sizeof(char*));
	do
	{
		textArray[size++] = inputString(fileInput, lastEnd);
		if (isOutOfMemory(textArray[size - 1]))
		{
			printf("No dynamic memory\n");
			break;
		}
		isEndOfFile = *lastEnd == 0;
		if (isEndOfFile)
			break;
		
		isEndOfFile = textArray[size - 1][*lastEnd] == EOF;
		if (isEndOfFile)
			break;
		
		if(size >= textCapacity)
		{
			textCapacity *= 2;
			textArray = realloc(textArray, textCapacity * sizeof(char*));
			if(isOutOfMemory(textArray)) 
			{
				printf("No dynamic memory\n");
				break;
			}
		}
		isEOF = textArray[size - 1][*lastEnd] == EOF;
	}
	while (!isEOF);
	textArray[size - 1][*lastEnd] = '\0';
	int i = 0;
	while (i < size)
		printf("%s\n", textArray[i++]);
	free(lastEnd);
	freeArray(textArray, size);
}