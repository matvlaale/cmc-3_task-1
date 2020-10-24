#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char endChar = '!'; //Символ конца ввода
struct list 
{
	char *string;
	struct list *next;
};

int isOutOfMemorySingle(char *link) //Проверка на недостаток памяти
{
	if(link == NULL) 
		return 1;
	else 
		return 0;
}

void freeMemory(struct list *textList) //Освобождение памяти
{
	if (textList == NULL)
		return;
	freeMemory(textList->next);
	free(textList);
}

void outputList(struct list *textList) //Вывод списка
{
	if (textList == NULL)
		return;
	printf("%s\n", textList->string);
	outputList(textList->next);
}

int insertInList(struct list *textList, char *nextString) //Вставить в список, рекурсия
{
	int insertedIsLower = 0, listIsNull = textList == NULL;
	if (!listIsNull) 
		insertedIsLower = strcmp(textList->string, nextString) >= 0; //Если вводимый меньше текущего
	if(insertedIsLower || listIsNull)
	{
		return 1;
	} 
	else
	{
		if (insertInList(textList->next, nextString) == 1) //Если этот меньше, а следующий больше, вставка между ними
		{
			struct list *newElement = malloc(sizeof(struct list));
			newElement->string = nextString;
			newElement->next = textList->next;
			textList->next = newElement;
		}
		return 0;
	}
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
	isEndOfString = nextChar == '\n' || nextChar == endChar;
	while(!isEndOfString) //Пока строка не закончилась на перенос строки или endChar
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
		isEndOfString = nextChar == '\n' || nextChar == endChar;
	}
	if(nextChar == '\n') //Последний символ
		nextString[length] = '\0';
	else 
		nextString[length] = endChar;
	*lastEnd = length; //Номер последнего элемента
	return nextString;
}

int main()
{
	int isEndOfFile = 0, isStringEmpty = 0, isStringMin = 0; //bool
	int continueInt = 1; //Проверка на желание продолжить ввод
	printf("Конец ввода - символ '!'\n");
	while (continueInt)
	{
		int *lastEnd = malloc(sizeof(int)); //Номер последнего элемента последней введённой строки
		char *nextString;
		struct list *textList = malloc(sizeof(struct list)); //Список вводимых строк
		textList->string = NULL;
		textList->next = NULL;
		do 
		{
			nextString = inputString(lastEnd); //Ввод очередной строки
			isEndOfFile = nextString[*lastEnd] == endChar;
			if(isEndOfFile)
				nextString[*lastEnd] = '\0';
			if(*lastEnd == 0) //Проверка на пустую строку
			{
				free(nextString);
				continue;
			}
			if(isOutOfMemorySingle(nextString)) //Проверка на недостаток памяти
				break;
			isStringEmpty = textList->string == NULL;
			if (!isStringEmpty)
				isStringMin = strcmp(nextString, textList->string) <= 0;
			if(isStringEmpty) //Если список пуст
			{
				textList->string = nextString;
			} 
			else if(isStringMin) //Вставить первым элементом
			{
				struct list *newBegin = malloc(sizeof(struct list));
				newBegin->string = nextString;
				newBegin->next = textList;
				textList = newBegin;
			} 
			else
			{
				insertInList(textList, nextString); //Вставить в список
			}
		} while (!isEndOfFile); //Пока последний элемент последней строки не равен endChar
		isStringEmpty = textList->string == NULL;
		if(isOutOfMemorySingle(nextString)) //Нехватка памяти
		{
			printf("Program is stopped!");
		} 
		else if (isStringEmpty) //Пустой ввод
		{
			printf("Empty input!\n");
		} 
		else 
			outputList(textList); //Вывод
		free(lastEnd);
		freeMemory(textList);
		printf("Для ввода ещё одной последовательности строк введите '1'. Для завершения программы введите '0'\n");
		scanf("%d", &continueInt);
	}
}