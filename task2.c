#include <stdio.h>
#include <stdlib.h>
#include "matroper.h"

int main()
{
	// Данный пример написан с предположением о том, что длина первой матрицы
	// и высота второй совпадают, а первая матрица является квадратной.
	// иначе некоторые проверки могут выдать ошибку.
	const int firstLength = 3, firstHeight = 3, secondLength = 4, secondHeight = 3;
	printf("Введите: 1 матрица: длина высота");
	scanf(%d, firstLength);
	scanf(%d, firstHeight);
	printf("Введите: 2 матрица: длина высота");
	scanf(%d, secondLength);
	scanf(%d, secondHeight);
	int **matrix1 = inputMatrix(firstLength, firstHeight);
	int **matrix2 = inputMatrix(secondLength, secondHeight);
	outputMatrix(firstLength, firstHeight, 
		multiplyMatrixNumber(firstLength, firstHeight, matrix1, 3));
	outputMatrix(firstLength, firstHeight, 
		addMatrixMatrix(firstLength, firstHeight, matrix1, matrix1));
	outputMatrix(secondLength, firstHeight, 
		multiplyMatrixMatrix(firstLength, firstHeight, secondLength, matrix1, matrix2));
	printf("%d", findDeterminant(firstHeight, matrix1));
	freeMatrix(firstHeight, matrix1);
	freeMatrix(secondHeight, matrix2);
}