#include <stdio.h>
#include <stdlib.h>
#include "matroper.h"

int **inputMatrix(const int length, const int height)
{
	int isEndOfLine = 1, isSpace = 1;
	int **matrix = malloc(height * sizeof(int*));
	int nextInt;
	for(int i = 0; i < height; i++)
	{
		matrix[i] = malloc(length * sizeof(int));
		for(int j = 0; j < length; j++)
		{
			scanf("%d", &nextInt);
			matrix[i][j] = nextInt;
		}
		char endOfLine = 5;
		do
		{
			endOfLine = getchar();
			isSpace = endOfLine == ' ';
		} while (isSpace);
		isEndOfLine = endOfLine == '\n';
		if(!isEndOfLine)
		{
			printf("Not appropriate input\n");
			return NULL;
		}
	}
	return matrix;
}

int matrixIsInvalid(int **matrix)
{
	return matrix == NULL;
}

void outputMatrix(const int length, const int height, int **matrix)
{
	if (matrixIsInvalid(matrix))
	{
		printf("Invalid matrix\n");
		return;
	}
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < length; j++)
		{
			printf("%d ", matrix[i][j]);
		}
		printf("\n");
	}
}

int** multiplyMatrixNumber(const int length, const int height, int **matrix, int number)
{
	if (matrixIsInvalid(matrix))
	{
		printf("Invalid matrix\n");
		return NULL;
	}
	int **newMatrix = malloc(height * sizeof(int*));
	for(int i = 0; i < height; i++)
	{
		newMatrix[i] = malloc(length * sizeof(int));
		for(int j = 0; j < length; j++)
		{
			newMatrix[i][j] = matrix[i][j] * number;
		}
	}
	return newMatrix;
}

int** addMatrixMatrix(const int length, const int height, int **firstMatrix, int **secondMatrix)
{
	int oneMatrixIsInvalid = matrixIsInvalid(firstMatrix) ||
		matrixIsInvalid(secondMatrix);
	if (oneMatrixIsInvalid)
	{
		printf("Invalid matrix\n");
		return NULL;
	}
	int **newMatrix = malloc(height * sizeof(int*));
	for(int i = 0; i < height; i++)
	{
		newMatrix[i] = malloc(length * sizeof(int));
		for(int j = 0; j < length; j++)
		{
			newMatrix[i][j] = firstMatrix[i][j] + secondMatrix[i][j];
		}
	}
	return newMatrix;
}

int** multiplyMatrixMatrix(const int firstLength, const int firstHeight, const int secondLength, int **firstMatrix, int **secondMatrix)
{
	int oneMatrixIsInvalid = matrixIsInvalid(firstMatrix) ||
		matrixIsInvalid(secondMatrix);
	if (oneMatrixIsInvalid)
	{
		printf("Invalid matrix\n");
		return NULL;
	}
	int **newMatrix = malloc(firstHeight * sizeof(int*));
	for(int i = 0; i < firstHeight; i++)
	{
		newMatrix[i] = malloc(secondLength * sizeof(int));
		for(int j = 0; j < secondLength; j++)
		{
			int sum = 0;
			for(int k = 0; k < firstLength; k++)
			{
				sum += firstMatrix[i][k] * secondMatrix[k][j];
			}
			newMatrix[i][j] = sum;
		}
	}
	return newMatrix;
}

void freeMatrix(const int size, int **matrix)
{
	for(int i = 0; i < size; i++)
		free(matrix[i]);
	free(matrix);
	return;
}

int **getMatrixWithoutRowAndColumn(int size, int **matrix, int rowNumber, int columnNumber)
{
	int rowShift = 0, columnShift = 0;
	int **newMatrix = malloc((size - 1) * sizeof(int*));
	for(int i = 0; i < size - 1; i++)
	{
		if(i == rowNumber)
			rowShift = 1;
		columnShift = 0;
		newMatrix[i] = malloc((size - 1) * sizeof(int));
		for(int j = 0; j < size - 1; j++)
		{
			if(j == columnNumber)
				columnShift = 1;
			newMatrix[i][j] = matrix[i + rowShift][j + columnShift];
		}
	}
	return newMatrix;
}

int findDeterminant(const int size, int **matrix)
{
	int determinant = 0;
	int degree = 1;
	if(size == 1)
	{
		return matrix[0][0];
	} else if(size == 2)
	{
		return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
	} else
	{
		for(int j = 0; j < size; j++)
		{
			int **newMatrix = getMatrixWithoutRowAndColumn(size, matrix, 0, j);
			determinant = determinant + (degree * matrix[0][j] * findDeterminant(size - 1, newMatrix));
			degree *= -1;
			freeMatrix(size - 1, newMatrix);
		}
	}
	return determinant;
}