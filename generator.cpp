#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
class Grid
{
public:
    void init() // генерация начального вида поля
    {
        table.resize(sudoku_size * sudoku_size, std::vector<int>(sudoku_size * sudoku_size)); // подгон двумерной матрицы до нужных размеров
        for (int i = 0; i < sudoku_size * sudoku_size; i++)
        {
            for (int j = 0; j < sudoku_size * sudoku_size; j++)
            {
                table[i][j] = ((i * sudoku_size + i / sudoku_size + j) % (sudoku_size * sudoku_size) + 1); // магическая формула задания стандартного поля судоку
            }
        }
        std::cout << "The base table is ready!" << std::endl;
    }

    void show() // красивый вывод матрицы чиселок в консоль
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

    void transposing() // транспонирование
    {
        std::vector<std::vector<int>> newTable;
        for (int j = 0; j < sudoku_size * sudoku_size; ++j)
        {
            std::vector<int> row;
            for (int i = 0; i < sudoku_size * sudoku_size; ++i)
            {
                row.push_back(table[i][j]); // создаем вспомогательный вектор, собираем туда столбики судоку
            }
            newTable.push_back(row); // в новой матрице превращаем их в строчки
        }
        table = newTable;
    }

    void swap_rows_small() // меняет местами строки чисел
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

    void swap_columns_small() // меняет местами столбцы чисел
    {
        transposing();
        swap_rows_small();
        transposing();
    }

    void swap_rows_area() // меняет местами строчки квадратов чисел
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

    void swap_columns_area() // меняет местами столбцы квадратов чисел
    {
        transposing();
        swap_rows_area();
        transposing();
    }

    void mix() // перемешивает матрицу, задавая случайное количество ранее описанных перестановок
    {
        std::vector<void (Grid::*)()> mix_func = {// список команд для перестановки
                                                  &Grid::transposing,
                                                  &Grid::swap_rows_small,
                                                  &Grid::swap_columns_small,
                                                  &Grid::swap_rows_area,
                                                  &Grid::swap_columns_area};

        for (int i = 1; i < max_swaps; ++i)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distribution(0, mix_func.size() - 1);
            int id_func = distribution(gen);
            (this->*mix_func[id_func])();
        }
    }

    std::vector<std::vector<int>> gettable() {
        std::vector<std::vector<int>> copy = table;
        return copy;
    }

private:
    const int sudoku_size = 3;           // магическая константа, размерность судоку(3*3*3*3)
    std::vector<std::vector<int>> table; // матрица, хранящая чиселки
    const int max_swaps = 10;            // для рандомности перестановок больше 10 не нужно, размеры всех различных областей судоку - 9
};

// int main()
// {
//     Grid g;
//     g.init();
//     Grid j = g;
//     g.mix();
//     g.show();
//     j.show();
// }