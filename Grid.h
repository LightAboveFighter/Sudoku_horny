#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <random>

// #include "amount_of_solutions.h"
#include "Nicolas.h"

// Объект, отвечающий за корректное создание чисел - клеток таблицы Судоку,
// а также некоторые операции с этой таблицей.
class Grid
{
public:
    // Генерация начального вида поля.
    void init();

    // Красивый вывод матрицы чиселок в консоль.
    void show();

    // Транспонирование.
    void transposing();

    // Меняет местами строки чисел.
    void swap_rows_small();

    // Меняет местами столбцы чисел.
    void swap_columns_small();

    // Меняет местами строчки квадратов чисел.
    void swap_rows_area();

    // Меняет местами столбцы квадратов чисел.
    void swap_columns_area();

    // Перемешивает матрицу, задавая случайное количество ранее описанных перестановок.
    void mix();

    // Возврашает двумерный вектор чисел заполненной таблицы.
    std::vector<std::vector<int>> gettable();

    // В зависимости от заданной сложности возвращает двумерный массив чисел
    // незаполненной таблицы, гарантированно имуеющей единственное решение.
    std::vector<std::vector<int>> erased(char given_difficulty);

private:
    const int sudoku_size = 3;           // магическая константа, размерность судоку(3*3*3*3)
    std::vector<std::vector<int>> table; // матрица, хранящая чиселки
    const int max_swaps = 10;            
    // для рандомности перестановок больше 10 не нужно, размеры всех различных
    // областей судоку - 9

    std::vector<int> check_cage(std::vector<std::vector<int>> const &grid, const int &cage_row, const int &cage_column)
    {                                                                    // находим недоступные варианты
        std::vector<int> blocked_values{},
                        available_values = {1, 2 ,3 , 4, 5, 6, 7, 8, 9};

        for (int row = 3*(cage_row/3); row<(3*(1 + cage_row/3)); row++)  // тут мы чекаем квадрат
            for (int col = 3*(cage_column/3); col<(3*(1 + cage_column/3)); col++)
                if (grid[row][col] != 0)
                    blocked_values.push_back(grid[row][col]);

        for (int row=0; row < grid.size(); row++)                        // а тут чекаем строку
            if (grid[row][cage_column] != 0)
                blocked_values.push_back(grid[row][cage_column]);
        
        for (int col=0; col < grid.size(); col++)                        // и тут чекаем столбец
            if (grid[cage_row][col] != 0)
                blocked_values.push_back(grid[cage_row][col]);
        
        // sort(blocked_values.begin(), blocked_values.end());
        if (!blocked_values.empty())
            for (int i = 0; i < blocked_values.size(); i++)              // стираем все невозможные значения из вектора возможных
                available_values.erase(std::remove(available_values.begin(),
                available_values.end(), blocked_values[i]),
                available_values.end());  

        return available_values;                                         // возвращаем возможные для подстановки цифры
    }

    std::vector<std::vector<Nicolas>> make_Nicolas_grid(std::vector<std::vector<int>> const &grid)
    {
        std::vector<std::vector<Nicolas>> Nicolas_grid(9, std::vector<Nicolas>(9));
        std::vector<int> available_options;
        int cage_value;
        
        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                cage_value = grid[row][col];
                (Nicolas_grid[row][col]).cage_put(cage_value);  
                if (grid[row][col] != 0)
                    continue;
                
                available_options = check_cage(grid, row, col);
                (Nicolas_grid[row][col]).available_put(available_options);
            }
        }
        return Nicolas_grid;
    }

    void change_available(std::vector<std::vector<Nicolas>> &grid, int const &cage_row, int const &cage_column)
    {
        int clear_value = (grid[cage_row][cage_column]).cage_get();      // значение, которое нужно убрать из available

        for (int row = 3*(cage_row/3); row<(3*(1 + cage_row/3)); row++)  // тут мы чистим квадрат
            for (int col = 3*(cage_column/3); col<(3*(1 + cage_column/3)); col++)
                if ((grid[row][col]).cage_get() == 0)
                    (grid[row][col]).available_remove(clear_value);
                    // blocked_values.push_back(grid[row][col]);

        for (int row=0; row < grid.size(); row++)                        // а тут чистим строку
            if ((grid[row][cage_column]).cage_get() == 0)
                (grid[row][cage_column]).available_remove(clear_value);
                // blocked_values.push_back(grid[row][cage_column]);
        
        for (int col=0; col < grid.size(); col++)                        // и тут чистим столбец
            if ((grid[cage_row][col]).cage_get() == 0)
                (grid[cage_row][col]).available_remove(clear_value);
                // blocked_values.push_back(grid[cage_row][col]);
    }

    int solutions(std::vector<std::vector<Nicolas>> &grid)
    {
        std::vector<int> m_possible;
        std::vector<std::vector<Nicolas>> grid_copy;
        int possible_amount,
            amount_of_solutions = 0,
            min_amount = 10,
            m_row,
            m_col;

        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                if ((grid[row][col]).cage_get() != 0)
                    continue;

                possible_amount = ((grid[row][col]).available_get()).size();

                if (possible_amount == 0)
                    return 0;

                if (min_amount > possible_amount);
                {
                    min_amount = possible_amount;
                    m_possible = (grid[row][col]).available_get();
                    m_row = row;
                    m_col = col;
                    if (min_amount == 1)
                        break;
                }
            }
            if (min_amount == 1)
                        break;
        }

        if (min_amount == 10)
            return 1;

        for (auto x : m_possible)
        {
            grid_copy = grid;
            (grid_copy[m_row][m_col]).cage_put(x);

            change_available(grid_copy, m_row, m_col);
            amount_of_solutions += solutions(grid_copy);
            if (amount_of_solutions > 1)
                return amount_of_solutions;
        }

        return amount_of_solutions;    
    }
};

//     std::vector<int> check_cage(std::vector<std::vector<int>> const &grid,
//         const int &cage_row, const int &cage_column)
//     {        // находим недоступные варианты
//         std::vector<int> blocked_values{},
//                         available_values = {1, 2 ,3 , 4, 5, 6, 7, 8, 9};

//         for (int row = 3*(cage_row/3); row<(3*(1 + cage_row/3)); row++)  // тут мы чекаем квадрат
//             for (int col = 3*(cage_column/3); col<(3*(1 + cage_column/3)); col++)
//                 if (grid[row][col] != 0)
//                     blocked_values.push_back(grid[row][col]);

//         for (int row=0; row < grid.size(); row++)                        // а тут чекаем строку
//             if (grid[row][cage_column] != 0)
//                 blocked_values.push_back(grid[row][cage_column]);
        
//         for (int col=0; col < grid.size(); col++)                        // и тут чекаем столбец
//             if (grid[cage_row][col] != 0)
//                 blocked_values.push_back(grid[cage_row][col]);
        
//         // sort(blocked_values.begin(), blocked_values.end());
//         if (!blocked_values.empty())
//                     // стираем все невозможные значения из вектора возможных
//             for (int i = 0; i < blocked_values.size(); i++)
//                 available_values.erase(std::remove(available_values.begin(),
//                     available_values.end(), blocked_values[i]), available_values.end());  

//         return available_values;                                         
//             // возвращаем возможные для подстановки цифры
//     }

//     // Возвращает количество возможных решений заданной таблицы Судоку.
//     int solutions(std::vector<std::vector<int>> const &grid)
//     {
//         int amount_of_solutions = 0,
//             min_count = 10,
//             m_row = -1,
//             m_col = -1;

//         std::vector<int> m_possible_values;

//         for (int row = 0; row < 9; row++)
//         {
//             for (int col = 0; col < 9; col++)
//             {
//                 if (grid[row][col] !=0)
//                     continue;

//                 std::vector<int> possible_values = check_cage(grid, row, col);
//                 int values_count = possible_values.size();

//                 if (values_count == 0)
//                     return 0;
//                 if (values_count < min_count)
//                 {
//                     min_count = values_count;
//                     m_row = row;
//                     m_col = col;
//                     m_possible_values = possible_values;
//                 }
//             }
//         }
//         if (min_count == 10)
//             return 1;
//         for (auto x : m_possible_values)
//         {
//             std::vector<std::vector<int>> grid_copy = grid;
//             grid_copy[m_row][m_col] = x;
//             amount_of_solutions += solutions(grid_copy);
//         }
//         return amount_of_solutions;    
//     }
// };

#endif