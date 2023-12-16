#ifndef _GENERATOR_CPP_
#define _GENERATOR_CPP_

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
// #include "amount_of_solutions.h"
// #include "rddth.h"

class Grid
{
public:
    // Генерация начального вида поля
    void init();

    // Красивый вывод матрицы чиселок в консоль
    void show();

    // транспонирование
    void transposing();

    // меняет местами строки чисел
    void swap_rows_small();

    // Меняет местами столбцы чисел
    void swap_columns_small();

    // меняет местами строчки квадратов чисел
    void swap_rows_area();

    // Меняет местами столбцы квадратов чисел
    void swap_columns_area();

    // Перемешивает матрицу, задавая случайное количество ранее описанных перестановок
    void mix();

    std::vector<std::vector<int>> gettable();

    // void erase(char given_difficulty);
    std::vector<std::vector<int>> erased(char given_difficulty);

private:
    const int sudoku_size = 3;           // магическая константа, размерность судоку(3*3*3*3)
    std::vector<std::vector<int>> table; // матрица, хранящая чиселки
    const int max_swaps = 10;            
    // для рандомности перестановок больше 10 не нужно, размеры всех различных
    // областей судоку - 9
};

#endif