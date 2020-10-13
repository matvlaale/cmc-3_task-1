// Ввод матрицы. Каждая новая строка матрицы должна идти на новой строке ввода!
int **inputMatrix(const int length, const int height);
// Проверка матрицы на то, что она не пуста.
int matrixIsInvalid(int **matrix);
// Вывод матрицы.
void outputMatrix(const int length, const int height, int **matrix);
// Умножение матрицы на число.
int** multiplyMatrixNumber(const int length, const int height, int **matrix, int number);
// Сложение двух матриц.
int** addMatrixMatrix(const int length, const int height, int **firstMatrix, int **secondMatrix);
// Умножение двух матриц, длина первой = высоте второй.
int** multiplyMatrixMatrix(const int firstLength, const int firstHeight, const int secondLength, int **firstMatrix, int **secondMatrix);
// Освобождение памяти.
void freeMatrix(const int size, int **matrix);
// Получение матрицы без строки и столбца.
int **getMatrixWithoutRowAndColumn(int size, int **matrix, int rowNumber, int columnNumber);
// Нахождение определителя. Матрица - квадратная.
int findDeterminant(const int size, int **matrix);