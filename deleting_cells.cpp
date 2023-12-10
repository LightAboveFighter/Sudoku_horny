#include "amount_of_solutions.cpp"
#include <string>

void erase(char given_difficulty) // удаляет из матрицы элементы взависимости от
                                        //сложности. Проверяет полученную матрицу на решаемость.
    {
        std::vector<std::vector<int>> seen(sudoku_size * sudoku_size,
            std::vector<int>(sudoku_size * sudoku_size, 0)); // Создаем табличку, в которой будем
                                                            //отмечать, рассматривали
                                                           // ли мы элемент (1) или еще нет (0).
        int need_to_erase;
        std::vector<std::vector<int>> final_table; // матрица, в которой содержится финальное поле,
                                                  //предназначенное для игры.

        enum{
            Hard = 'H',
            Medium = 'M',
            Easy = 'E'
        };
        switch (given_difficulty){ // Исходя из ожидаемой пользователем сложности определяем,
                                  //сколько клеточек надо стереть.
            case Hard:
                need_to_erase = 61;
            case Medium:
                need_to_erase = 51;
            case Easy:
                need_to_erase = 41;
            default:
                need_to_erase = 0;
        }
        int erased = 0, iterator = 0;
        while ((erased <= need_to_erase) || (iterator <= sudoku_size *
            sudoku_size * sudoku_size * sudoku_size)) // Пока не сотрем нужное число клеточек
                                                     // или пока не кончатся все клеточки,
                                                    //vкоторые возможно стереть
        {
            int low = 0, high = 8;
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dist_i(low, high); // генерируем случайный номер ячейки
            std::uniform_int_distribution<> dist_j(low, high);
            int i = dist_i(gen), j = dist_j(gen);
            if (seen[i][j] == 0) // если мы раньше не смотрели эту клетку
            {
                ++iterator;
                seen[i][j] = 1; // Посмотрели))

                int temp = table[i][j]; // Сохраним эдемент на случай, если нивегго не получится
                table[i][j] = 0;
                ++erased;

                // Затем следует функция Максимилиана, которая определяет, есть ли у такой
                // таблицы единственное решение.
                // Если нет  - возвращаем все обратно
                table[i][j] = temp;
                --erased;

            } 
        }
    }