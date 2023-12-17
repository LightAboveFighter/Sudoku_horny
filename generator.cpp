#include "generator.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include <random>
#include <time.h>

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
    // std::cout << "The base table is ready!" << std::endl;
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

    std::cout << "++++++++++++++++++++++++++++++++++++\n==================="
              << "=================\n++++++++++++++++++++++++++++++++++++\n"
              << std::endl;
}

// Транспонирование.
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

// Меняет местами строки чисел.
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

// Меняет местами столбцы чисел.
void Grid::swap_columns_small()
{
    transposing();
    swap_rows_small();
    transposing();
}

// Меняет местами строчки квадратов чисел.
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

// Меняет местами столбцы квадратов чисел.
void Grid::swap_columns_area()
{
    transposing();
    swap_rows_area();
    transposing();
}

// Перемешивает матрицу, задавая случайное количество ранее описанных перестановок.
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

// Возвращает нерешенное Судоку, в зависимости от переданной сложности.
std::vector<std::vector<int>> Grid::erased(char given_difficulty) {
    std::vector<std::vector<int>> res = table;
    int amount_erase = 0;
    switch (given_difficulty){ // Исходя из ожидаемой пользователем сложности определяем,
                            //сколько клеточек надо стереть.
        case 'H':
            amount_erase = 57; //61 слишком редко достигается
            break;
        case 'M':
            amount_erase = 51;
            break;
        case 'E':
            amount_erase = 41;
            break;
    }
    std::vector<std::vector<int>> ref(81, std::vector<int>(2));
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            ref[i*9 + j] = std::vector<int>{i, j};
        }
    }
    std::vector<std::vector<int>> vars(81, std::vector<int>(2));
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            vars[i*9 + j] = std::vector<int>{i, j};
        }
    }
    int count = 0;
    int iter = 0;
    bool next = false;
    clock_t time_before = clock();
    // Стираем клетки таким образом, чтобы количество возможных решений
    // Судоку оставалось равным 1.
    while (iter < amount_erase) {

        // На всякий случай, если создание длится слишком долго, начинаем процесс 
        // заново, но не чаще двух раз.
        if (vars.size() <= 1 || clock() - time_before > 10000000) {
            ++count;
            if (count == 2) break;
            iter = 0;
            res = table;
            vars = ref;
            time_before = clock();
            // std::cout << "ignored" << std::endl;
        }

        std::vector<int>& x = vars[Graph_lib::randint(vars.size()-1)];
        if ( res[x[0]][x[1]] == 0 ) {
            for (int k = 0; k < vars.size(); ++k) {
                if (vars[k] == x) {
                    vars.erase(vars.begin() + k);
                    next = true;
                    break;
                }
            }
        };
        if (next) {
            next = false;
            continue;
        }

        res[x[0]][x[1]] = 0;
        if ( solutions(res) != 1 ) {
            res[x[0]][x[1]] = table[x[0]][x[1]];
            continue;
        }

        // удаляем стертую клетку из вектора возможных клеток -
        // так мы предотвращаем излишние итерации проверки решаемости.
        // При генерации поля на высокой сложности это может сыграть важную роль,
        // из за длительности генерации.
        for (int k = 0; k < vars.size(); ++k) {
            if (vars[k] == x) {
                vars.erase(vars.begin() + k);
                break;
            }
        }
        ++iter;
        // std::cout << iter << std::endl;
    }
    return res; 
}