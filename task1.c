#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isOutOfMemorySingle(char *link) 
{
	if(link == NULL) 
		return 1;
	else 
		return 0;
}

int isOutOfMemoryDouble(char **link) 
{
	if(link == NULL) 
		return 1;
	else 
		return 0;
}

char *inputString(int *lastEnd) 
{
	int isEndOfString = 0; //Bool
	int length = 0, capacity = 16;
	char *nextString = malloc(capacity * sizeof(char));
	if(isOutOfMemorySingle(nextString)) //Если память закончилась
	{
		printf("No dynamic memory\n");
		return NULL;
	}
	char nextChar = getchar();
	isEndOfString = nextChar == '\n' || nextChar == EOF;
	while(!isEndOfString) //Пока строка не закончилась на перенос строки или EOF
	{
		nextString[length++] = nextChar;
		if(length >= capacity) //Проверка на то, нужно ли выделить память
		{
			capacity *= 2;
			nextString = realloc(nextString, capacity * sizeof(char));
			if(isOutOfMemorySingle(nextString)) 
			{
				printf("No dynamic memory\n");
				return NULL;
			}
		}
		nextChar = getchar(); //Ввод очередного символа
		isEndOfString = nextChar == '\n' || nextChar == EOF;
	}
	if(nextChar == '\n') //Последний символ
		nextString[length] = '\0';
	else 
		nextString[length] = EOF;
	*lastEnd = length; //Номер последнего элемента
	return nextString;
}

void sortStrings(char ** textArray, int size) //Сортировка пузырьком
{
	int isPreviousBigger = 0;
	for (int i = 0; i < size; i++) 
	{
		for (int j = 1; j < size - i; j++) 
		{
			isPreviousBigger = strcmp(textArray[j - 1], textArray[j]) > 0;
			if (isPreviousBigger) 
			{
				char *temp = textArray[j - 1];
				textArray[j - 1] = textArray[j];
				textArray[j] = temp;
			}
		}
	}
}

void outputText(char ** textArray, int size) 
{
	for (int i = 0; i < size; i++) 
	{
		printf("%s\n", textArray[i]);
		free(textArray[i]); //Освободить все ссылки на строку
	}
	free(textArray); //Освободить сам массив строк
}

int main() 
{
	int isEndOfFile = 0, isStringEmpty = 0; //bool
	int size = 0, textCapacity = 16;
	int *lastEnd = malloc(sizeof(int)); //Номер последнего элемента последней введённой строки
	char **textArray = malloc(textCapacity * sizeof(char*)); //Массив вводимых строк
	do 
	{
		isStringEmpty = size != 0 && textArray[size - 1][0] == '\0';
		if(isStringEmpty) //Проверка на пустые строки
			size--;
		textArray[size++] = inputString(lastEnd); //Ввод очередной строки
		if(isOutOfMemorySingle(textArray[size - 1])) //Проверка на недостаток памяти
		{
			textArray = NULL;
			break;
		}
		if(size >= textCapacity) //Проверка на то, нужно ли выделить память
		{
			textCapacity *= 2;
			textArray = realloc(textArray, textCapacity * sizeof(char*));
			if(isOutOfMemoryDouble(textArray)) 
			{
				printf("No dynamic memory\n");
				break;
			}
		}
		isEndOfFile = textArray[size - 1][*lastEnd] == EOF;
	} while (!isEndOfFile); //Пока последний элемент последней строки не равен EOF
	if(isOutOfMemoryDouble(textArray)) 
	{
		printf("Program is stopped!");
	} else 
	{
		textArray[size - 1][*lastEnd] = '\0';
		free(lastEnd);
		sortStrings(textArray, size); //Сортировка
		outputText(textArray, size); //Вывод
	}
	return 0;
}