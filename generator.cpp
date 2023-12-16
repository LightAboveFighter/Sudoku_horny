#include "generator.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include <random>

std::vector<int> check_cage(std::vector<std::vector<int>> const &grid,
    const int &cage_row, const int &cage_column)
{                                         // находим недоступные варианты
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
        for (int i = 0; i < blocked_values.size(); i++) // стираем все невозможные значения из вектора возможных
            available_values.erase(std::remove(available_values.begin(),
                available_values.end(), blocked_values[i]), available_values.end());  

    return available_values;                                         
        // возвращаем возможные для подстановки цифры
}

int solutions(std::vector<std::vector<int>> const &grid)
{
    int amount_of_solutions = 0,
        min_count = 10,
        m_row = -1,
        m_col = -1;

    std::vector<int> m_possible_values;

    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] !=0)
                continue;

            std::vector<int> possible_values = check_cage(grid, row, col);
            int values_count = possible_values.size();

            if (values_count == 0)
                return 0;
            if (values_count < min_count)
            {
                min_count = values_count;
                m_row = row;
                m_col = col;
                m_possible_values = possible_values;
            }
        }
    }
    if (min_count == 10)
        return 1;
    for (auto x : m_possible_values)
    {
        std::vector<std::vector<int>> grid_copy = grid;
        grid_copy[m_row][m_col] = x;
        amount_of_solutions += solutions(grid_copy);
    }
    return amount_of_solutions;    
}

// Генерация начального вида поля
void Grid::init()
{
    table.resize(sudoku_size * sudoku_size, std::vector<int>(sudoku_size * sudoku_size));
    // подгон двумерной матрицы до нужных размеров
    for (int i = 0; i < sudoku_size * sudoku_size; i++)
    {
        for (int j = 0; j < sudoku_size * sudoku_size; j++)
            table[i][j] = ((i * sudoku_size + i / sudoku_size + j) % (sudoku_size *
                sudoku_size) + 1); // магическая формула задания стандартного поля судоку
    }
    std::cout << "The base table is ready!" << std::endl;
}

// Красивый вывод матрицы чиселок в консоль
void Grid::show()
{
    std::cout << "-----------------------------------" << std::endl;
    for (int i = 0; i < sudoku_size * sudoku_size; i++)
    {
        for (int j = 0; j < sudoku_size * sudoku_size; j++)
        {
            std::cout << table[i][j] << " | ";
        }
        std::cout << "\n"
                    << std::endl;
        std::cout << "-----------------------------------" << std::endl;
    }

    std::cout << "++++++++++++++++++++++++++++++++++++\n====================================\n++++++++++++++++++++++++++++++++++++\n"
                << std::endl;
}

// транспонирование
void Grid::transposing()
{
    std::vector<std::vector<int>> newTable;
    for (int j = 0; j < sudoku_size * sudoku_size; ++j)
    {
        std::vector<int> row;
        for (int i = 0; i < sudoku_size * sudoku_size; ++i)
        {
            row.push_back(table[i][j]);
            // создаем вспомогательный вектор, собираем туда столбики судоку
        }
        newTable.push_back(row);
        // в новой матрице превращаем их в строчки
    }
    table = newTable;
}

// меняет местами строки чисел
void Grid::swap_rows_small()
{
    std::random_device rd;  // генерация случайного числа
    std::mt19937 gen(rd()); // построение списка множества чисел на основе заданного
    std::uniform_int_distribution<int> distribution(0, sudoku_size - 1);

    int area = distribution(gen);
    int line1 = distribution(gen);

    int N1 = area * sudoku_size + line1;

    int line2 = distribution(gen);
    while (line1 == line2)
    {
        line2 = distribution(gen);
    }

    int N2 = area * sudoku_size + line2;

    std::swap(table[N1], table[N2]);
}

// Меняет местами столбцы чисел
void Grid::swap_columns_small()
{
    transposing();
    swap_rows_small();
    transposing();
}

// меняет местами строчки квадратов чисел
void Grid::swap_rows_area()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, sudoku_size - 1);

    int area1 = distribution(gen);
    int area2 = distribution(gen);
    while (area1 == area2)
    {
        area2 = distribution(gen);
    }

    for (int i = 0; i < sudoku_size; ++i)
    {
        int N1 = area1 * sudoku_size + i;
        int N2 = area2 * sudoku_size + i;
        std::swap(table[N1], table[N2]);
    }
}

// Меняет местами столбцы квадратов чисел
void Grid::swap_columns_area()
{
    transposing();
    swap_rows_area();
    transposing();
}

// Перемешивает матрицу, задавая случайное количество ранее описанных перестановок
void Grid::mix()
{
    std::vector<void (Grid::*)()> mix_func = {  // список команд для перестановки
                                                &Grid::transposing,
                                                &Grid::swap_rows_small,
                                                &Grid::swap_columns_small,
                                                &Grid::swap_rows_area,
                                                &Grid::swap_columns_area};

    for (int i = 0; i < max_swaps; ++i)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(0, mix_func.size() - 1);
        int id_func = distribution(gen);
        (this->*mix_func[id_func])();
    }
}

std::vector<std::vector<int>> Grid::gettable() {
    std::vector<std::vector<int>> copy = table;
    return copy;
}

// void Grid::erase(char given_difficulty) // удаляет из матрицы элементы взависимости от
//                                         //сложности. Проверяет полученную матрицу на решаемость.
// {
//     std::vector<std::vector<int>> seen(sudoku_size * sudoku_size,
//         std::vector<int>(sudoku_size * sudoku_size, 0)); // Создаем табличку, в которой будем
//                                                         //отмечать, рассматривали
//                                                     // ли мы элемент (1) или еще нет (0).
//     int need_to_erase = 0;
//     std::vector<std::vector<int>> final_table; // матрица, в которой содержится финальное поле,
//                                             //предназначенное для игры.

//     enum{
//         Hard = 'H',
//         Medium = 'M',
//         Easy = 'E'
//     };
//     switch (given_difficulty){ // Исходя из ожидаемой пользователем сложности определяем,
//                             //сколько клеточек надо стереть.
//         case Hard:
//             need_to_erase = 61;
//         case Medium:
//             need_to_erase = 51;
//         case Easy:
//             need_to_erase = 41;
//         default:
//             need_to_erase = 0;
//     }
//     int erased = 0, iterator = 0;
//     while ((erased <= need_to_erase) || (iterator <= sudoku_size *
//         sudoku_size * sudoku_size * sudoku_size)) // Пока не сотрем нужное число клеточек
//                                                 // или пока не кончатся все клеточки,
//                                                 //vкоторые возможно стереть
//     {
//         int low = 0, high = 8;
//         std::random_device rd;
//         std::mt19937 gen(rd());
//         std::uniform_int_distribution<> dist_i(low, high); // генерируем случайный номер ячейки
//         std::uniform_int_distribution<> dist_j(low, high);
//         int i = dist_i(gen), j = dist_j(gen);
//         if (seen[i][j] == 0) // если мы раньше не смотрели эту клетку
//         {
//             ++iterator;
//             seen[i][j] = 1; // Посмотрели))
 
//             int temp = table[i][j]; // Сохраним элемент на случай, если ничего не получится
//             table[i][j] = 0;
//             std::cout << "vda" << std::endl;
//             ++erased;
//             if ( 0 == 1 ) {
//                 table[i][j] = temp;
//                 std::cout << "srgsg" << std::endl;
//                 --erased;
//             }
//             // Затем следует функция Максимилиана, которая определяет, есть ли у такой
//             // таблицы единственное решение.
//             // Если нет  - возвращаем все обратно

//         } 
//     }
// }

std::vector<std::vector<int>> Grid::erased(char given_difficulty) {
    std::vector<std::vector<int>> res(9, std::vector<int>(9));
    int need_to_erase = 81;
    switch (given_difficulty){ // Исходя из ожидаемой пользователем сложности определяем,
                            //сколько клеточек надо стереть.
        case 'H':
            need_to_erase -= 61;
            break;
        case 'M':
            need_to_erase -= 51;
            break;
        case 'E':
            need_to_erase -= 41;
            break;
    }
    std::cout << need_to_erase << std::endl;
    for (int i = 0 ; i < 9; ++i) {
        for (int j = 0 ; j < 9; ++j) {
            if (need_to_erase == 0) break;
            if (!(res[i][j]) && (Graph_lib::randint(1))) {
                --need_to_erase;
                res[i][j] = table[i][j];
                continue;
            }      
        }
    }
    return res; 
}