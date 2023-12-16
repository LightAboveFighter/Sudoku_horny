// #include "solutions.h"

// std::vector<int> check_cage(std::vector<std::vector<int>> const &grid,
//     const int &cage_row, const int &cage_column)
// {                                         // находим недоступные варианты
//     std::vector<int> blocked_values{},
//                      available_values = {1, 2 ,3 , 4, 5, 6, 7, 8, 9};

//     for (int row = 3*(cage_row/3); row<(3*(1 + cage_row/3)); row++)  // тут мы чекаем квадрат
//         for (int col = 3*(cage_column/3); col<(3*(1 + cage_column/3)); col++)
//             if (grid[row][col] != 0)
//                 blocked_values.push_back(grid[row][col]);

//     for (int row=0; row < grid.size(); row++)                        // а тут чекаем строку
//         if (grid[row][cage_column] != 0)
//             blocked_values.push_back(grid[row][cage_column]);
    
//     for (int col=0; col < grid.size(); col++)                        // и тут чекаем столбец
//         if (grid[cage_row][col] != 0)
//             blocked_values.push_back(grid[cage_row][col]);
    
//     // sort(blocked_values.begin(), blocked_values.end());
//     if (!blocked_values.empty())
//         for (int i = 0; i < blocked_values.size(); i++) // стираем все невозможные значения из вектора возможных
//             available_values.erase(std::remove(available_values.begin(),
//                 available_values.end(), blocked_values[i]), available_values.end());  

//     return available_values;                                         
//         // возвращаем возможные для подстановки цифры
// }

// int solutions(std::vector<std::vector<int>> const &grid)
// {
//     int amount_of_solutions = 0,
//         min_count = 10,
//         m_row = -1,
//         m_col = -1;

//     std::vector<int> m_possible_values;

//     for (int row = 0; row < 9; row++)
//     {
//         for (int col = 0; col < 9; col++)
//         {
//             if (grid[row][col] !=0)
//                 continue;

//             std::vector<int> possible_values = check_cage(grid, row, col);
//             int values_count = possible_values.size();

//             if (values_count == 0)
//                 return 0;
//             if (values_count < min_count)
//             {
//                 min_count = values_count;
//                 m_row = row;
//                 m_col = col;
//                 m_possible_values = possible_values;
//             }
//         }
//     }
//     if (min_count == 10)
//         return 1;
//     for (auto x : m_possible_values)
//     {
//         std::vector<std::vector<int>> grid_copy = grid;
//         grid_copy[m_row][m_col] = x;
//         amount_of_solutions += solutions(grid_copy);
//     }
//     return amount_of_solutions;    
// }