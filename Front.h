#ifndef _FRONT_H_
#define _FRONT_H_

#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/fltk.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Simple_window.h"

#include "generator.h"

#include <string>
#include <vector>
#include <iostream>
#include <random>

/* Класс Судоку-кнопки, наследумый от стандартной кнопки Graph_lib
Стандартной кнопке блокируется label - вместо него над ней рисуется
Graph_lib::Text - так мы можем регулировать размер, шрифт и цвет значения кнопки.
Также Судоку-кнопка запоминает свое расположение в таблице - колонка и строка,
так как она будет находиться в Судоку-таблице.
*/
class SDButton : public Graph_lib::Button {
public:
    Graph_lib::Text text;
    // p - положение кнопки, ww - ширина, hh - высота, lb - значение кнопки,
    // cb - callback функция, привязанная к кнопке, c - столбец положения
    // в таблице, l - линия положения в таблице
    SDButton(Graph_lib::Point p, int ww, int hh,
        const std::string& lb, Graph_lib::Callback cb, int c, int l) :
        Graph_lib::Button{p, ww, hh, "", cb}, column{c}, line{l},
        text{Graph_lib::Point{p.x + 18, p.y + 42}, lb} {};
    
    void attach(Graph_lib::Window& win) override; //добавляем в attach присоединение
                                                  //текста - Graph_lib::Text
    void setColor(Graph_lib::Color c);
    void setTextColor(Graph_lib::Color c);
    void resetColor();
    void resetTextColor();
    int getColor() { return color; }
    int getTextColor() {return text.color().as_int(); }
    int getcolumn() { return column; };
    int getline() {return line; };

    void operator= (SDButton&) = delete;
    SDButton(SDButton&) = delete;
private:
    int column{0};
    int line{0};
    int color;
    using Graph_lib::Button::label;  //блокируем стандартное значение кнопки
};

class Sudoku;

/* Класс под-окна, наследуемый от стандартного Graph_lib окна.
Основное различие - это окно запоминает родителя, в нашем случае класс, вызвавший
его. Это нужно для корректной работы callback-функций кнопок в нашем будущем
классе Судоку-окна.
*/
class SubWindow : public Graph_lib::Window {
public:
    SubWindow(int ww, int hh, const std::string& lb, Sudoku* par) :
        Window{ww, hh, lb}, parent{par} {};
    SubWindow(Graph_lib::Point p, int ww, int hh, const std::string& lb, Sudoku* par) :
        Window{p, ww, hh, lb}, parent{par} {};
    Sudoku* parent;
};

/* Судоку-окно наследуется от SubWindow, в итоге создавая
три окна - меню начала, само Судоку, и меню завершения(конец).
К каждому окну прикреплено несколько кнопок, с помощью параметра parent
кнопки в этих окнах могут отправлять свое нажатие нашему классу.
*/
class Sudoku : public SubWindow {
public:
    Sudoku(Graph_lib::Point p, int ww, int hh, const std::string& lb);

    void attach(Graph_lib::Shape& s);
    void attach(Graph_lib::Widget& s);

    ~Sudoku();

private:
    Window* menu;
    Window* play_win;
    Window* end_win;
    // Указатель на активное окно
    Window* active_window;

    // Параметр выбранной сложности
    char difficulty{'E'};

    // Булевая переменная, отвечаюшая за то, показано ли под-меню сложности
    // в первом окне.
    bool difficulty_showed{False};

    // Все объекты первого окна
    std::vector<void*> menu_obj{7, nullptr};
    // Все объекты конечного окна.
    std::vector<void*> end_obj{3, nullptr};

    // Параметр для деактивации изменений игрового поля после окончания игры
    // (Но с возможностью смотреть на это поле)
    bool is_playing{false};

    // Массив начальных чисел игры
    std::vector<std::vector<int>> start;
    // Массив конечных чисел игры
    std::vector<std::vector<int>> finish;
    // Таблица указателей на кнопки игры
    std::vector<std::vector<SDButton*>> play_buttons;
    // Указатель на выбранную кнопку
    SDButton* selected{nullptr};

    // Указатель на строку количества ошибок
    Graph_lib::Text* mistakes;

    // Счетчик количества ошибок. Его ненулевое значение в момент активации
    // второго и третьего окна меняет пересоздание окон на из изменение.
    int count_mistakes;

    // Вектор всех фигур нашего класса. Нужны
    // для корректного удаления объекта, отсутствия утечек памяти.
    std::vector<Graph_lib::Shape*> links_s;
    // Вектор всех виджетов нашего класса. Нужны
    // для корректного удаления объекта, отсутствия утечек памяти.
    std::vector<Graph_lib::Widget*> links_w;

    // Callback функция нажатия кнопки - члена таблицы Судоку.
    static void cb_play_clicked(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<SDButton>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->clicked(bt);
    }

    // Сallback функция нажатия кнопки Start или New game,
    // переключаящей активное окно.
    static void cb_next(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->next_view();
    }

    // Сallback функция раскрытия подменю сложности в первом окне.
    static void cb_menu_toggle(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->menu_toggle();
    }

    // Сallback функция изменения уровня сложности в подменю первого окна.
    static void cb_difficulty_toggle(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->difficulty_toggle(bt);
    }

    // Сallback функция выхода из программы, реализуется при нажатии Quit в последнем окне.
    static void cb_quit(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->quit();
    }

    // Завершает выполнение программы.
    void quit() {
        active_window->hide();
        play_win->hide();
    }

    // Реализует выделение и снятие выделения активной кнопки.
    void clicked(SDButton& bt) {
        if (!is_playing) return;
        if (!selected) {
            selected = &bt;
            activated(*selected);
        } else {
            deactivated();
            if (selected == &bt) {
                selected = nullptr;
            } else {
                selected = &bt;
                activated(bt);
            }
        }
        play_win->redraw();   
    }

    // Смена активного окна.
    void next_view() {
        if (active_window == menu) {
            play_win->show();
            is_playing = true;
            active_window->hide();
            active_window = play_win;
            init_play();
        }
        else if (active_window == play_win) {
            end_win->show();
            is_playing = false;
            active_window = end_win;
            count_mistakes = 1;
            // ставим ненулевое значение количества ошибок для того, чтобы
            // дать в будущем знать программе, что нужно изменить, а не пересоздать окно
        }
        else if (active_window == end_win) {
            menu->show();
            active_window->hide();
            play_win->hide();
            active_window = menu;
            if (difficulty_showed) menu_toggle();
        }
    }

    // Раскрытие подменю сложности в первом окне.
    void menu_toggle() {
        if (!difficulty_showed) {
            static_cast<Graph_lib::Button*>(menu_obj[2])->hide();
            for (int i = 0; i < 3; ++i) {
                static_cast<Graph_lib::Button*>(menu_obj[i+3])->show();
            }
            difficulty_showed = True;
            static_cast<Graph_lib::Circle*>(menu_obj[6])->set_fill_color(Graph_lib::Color::cyan);
            static_cast<Graph_lib::Circle*>(menu_obj[6])->set_fill_color(Graph_lib::Color::cyan);
            return;
        }
        static_cast<Graph_lib::Button*>(menu_obj[2])->show();
        for (int i = 0; i < 3; ++i) {
            static_cast<Graph_lib::Button*>(menu_obj[i+3])->hide();
        }
        difficulty_showed = False;
        static_cast<Graph_lib::Circle*>(menu_obj[6])->set_fill_color(Graph_lib::Color::invisible);
        static_cast<Graph_lib::Circle*>(menu_obj[6])->set_color(Graph_lib::Color::invisible);
    }

    // Изменение уровня сложности.
    void difficulty_toggle(Graph_lib::Button& bt) {
        int move_coeff = 0;
        if (difficulty != bt.label[0]) {
            move_coeff = ((difficulty + bt.label[0] == 'E' + 'H') + 1) * 70 * ( 1 + 2 * -(
                (difficulty == 'M' || difficulty == 'E')&&(bt.label[0] == 'H' || bt.label[0] == 'M')));
        }
        static_cast<Graph_lib::Circle*>(menu_obj[6])->move(0, -move_coeff);
        difficulty = bt.label[0];
        menu->redraw();
    }

    // Создание нового уровня - начальных и конечных чисел таблицы Судоку
    // в зависимости от выбранного уровня сложности.
    void generate_nums() {
        Grid* table = new Grid;
        table->init();
        table->mix();
        finish = table->gettable();
        // table->erase(difficulty);
        start = table->erased(difficulty);
    }

    // Подготовка главного игрового окна - либо создание, либо
    // изменение значений для следующей игры.
    void init_play() {
        selected = nullptr;
        generate_nums();
        if (count_mistakes) {
            // если окно вызвано не первый раз - мы не пересоздаем его,
            // а только меняем значения.

            for (int i = 0; i < 9; ++i) {
                for (int j = 0; j < 9; ++j) {
                    std::string lb = start[i][j] == 0 ? "" : std::to_string(start[i][j]);
                    play_buttons[i][j]->text.set_label(lb);
                    play_buttons[i][j]->resetColor();
                    play_buttons[i][j]->resetTextColor();
                }
            }
            count_mistakes = 0;
            mistakes->set_label("Mistakes: 0/3");
            return;
        }
        // в ином случае - создаем объекты второго окна

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                std::string label = std::to_string(start[i][j]);
                play_buttons[i][j] = new SDButton{Graph_lib::Point{300 + 63*i, 20 + 63*j}, 60, 60,
                    label == "0" ? "" : label, cb_play_clicked, i, j};
                play_buttons[i][j]->text.set_font_size(38);
                attach(*play_buttons[i][j]);
            }
        }

        for (int i = 1; i < 3; ++i) {
            Graph_lib::Rectangle* p = new Graph_lib::Rectangle(Graph_lib::Point{300 + 189*i - 3,
                20}, Graph_lib::Point{300 + 189*i, 584});
            p->set_fill_color(Graph_lib::Color::black);
            attach(*p);
        }
        for (int i = 1; i < 3; ++i) {
            Graph_lib::Rectangle* p = new Graph_lib::Rectangle(Graph_lib::Point{300,
                20 + 189*i - 3}, Graph_lib::Point{864, 20 + 189*i});
            p->set_fill_color(Graph_lib::Color::black);
            attach(*p);
        }
        mistakes = new Graph_lib::Text{Graph_lib::Point{50, 50}, "Mistakes: 0/3"};
        mistakes->set_font_size(25);
        attach(*mistakes);
    }

    void active_line(int line) {
        for (int i = 0; i < 9; ++i) {
            play_buttons[i][line]->setColor(Graph_lib::Color::green);
        }
    }

    void active_column(int col) {
        for (int i = 0; i < 9; ++i) {
            play_buttons[col][i]->setColor(Graph_lib::Color::green);
        }
    }

    void active_square(int col, int line) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if ( j/3 == line/3 && i/3 == col/3) {
                    play_buttons[i][j]->setColor(Graph_lib::Color::green);
                }
            }
        }
    }

    void active_labels(const std::string& lb) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if ( play_buttons[i][j]->text.label() == lb && lb != "") {
                    play_buttons[i][j]->setColor(Graph_lib::Color::cyan);
                }
            }
        }
    }

    // Выделение активной линии, столбца, кнопок с такими же цифрами
    // и самой активной кнопки.
    void activated(SDButton& bt) {
        if (is_playing) {
            int col = bt.getcolumn();
            int line = bt.getline();
            active_column(col);
            active_line(line);
            active_square(col, line);
        }
        std::string lb = bt.text.label();
        active_labels(lb);
        if (is_playing) bt.setColor(Graph_lib::Color::dark_magenta);
    }

    // Установка всем кнопкам стандартного цвета.
    void deactivated() {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (play_buttons[i][j]->getColor() == Graph_lib::Color::red) 
                    selected->setTextColor(Graph_lib::Color::red);
                play_buttons[i][j]->setColor(Graph_lib::Color::white);
            }
        }
    }

    // Проверка введенного пользователем числа.
    bool is_corr(SDButton& bt) {
        int col = bt.getcolumn();
        int lin = bt.getline();
        if (bt.text.label() == std::to_string(finish[col][lin]) || bt.text.label() == "")
            return true;
        return false;
    }

    // Проверка успешного окончания игры.
    bool is_end() {
        if (start == finish) {
            count_mistakes = 1;
            return true;
        }
        return false;
    }

    // При неверном введенном значении система увеличивает количество ошибок,
    // и обновляет строку с информацией об ошибках.
    // При 3х ошибках игра завершается.
    void mistake_occured() {
        selected->setColor(Graph_lib::Color::red);
        count_mistakes += 1;
        std::string str = "Mistakes: " + std::to_string(count_mistakes) + "/3";
        mistakes->set_label(str);
        if (count_mistakes == 3) game_over(false);
    }

    // Перехват клавиатуры - при нажатии кнопок 1-9 программа
    // фиксирует нажатие и изменяет значение активной кнопки.
    int handle(int event) {
        if (event == FL_KEYDOWN) {
            if (selected){
                if (!is_playing) return Graph_lib::Window::handle(event);
                int col = selected->getcolumn();
                int lin = selected->getline();

                if (selected->getColor() == Graph_lib::Color::red ||
                        selected->getTextColor() == Graph_lib::Color::red) {
                    deactivated();
                    selected->text.set_label("");
                    start[col][lin] = 0;
                    selected->resetTextColor();
                    selected = nullptr;
                    play_win->redraw();
                    return 1;
                }

                char val = *Fl::e_text;
                if ( val >= '1' && val <= '9')
                    if (selected->text.label() == "") {
                        selected->text.set_label(Fl::e_text);
                        start[col][lin] = *Fl::e_text - 48;
                        selected->text.set_color(FL_INACTIVE_COLOR);
                        activated(*selected);
                        if (!is_corr(*selected)) {
                            mistake_occured();
                        } else if(is_end()) game_over(true);
                        play_win->redraw();
                    }
            } else{
                char val = *Fl::e_text;
                if (val >= '1' && val <= '9') {
                    char str[2];
                    sprintf(str, "%c", val);
                    deactivated();
                    active_labels(str);
                    redraw();
                }
            }
            return 1;
        }
        return Graph_lib::Window::handle(event);
    }

    // Завершение решения Судоку - вывод окна завершения со строкой выигрыша,
    // в случае его первого вызова - инициализация.
    void game_over(bool winner) {
        next_view();
        if (count_mistakes) {
            init_end(winner);
        } else {
            static_cast<Graph_lib::Text*>(end_obj[0])->set_label(
                winner ? "Congratulations:)" : "      You lose:("
                );
        }
    }

    // Создание конечного окна.
    void init_end(bool winner) {
        Graph_lib::Text* t = new Graph_lib::Text{Graph_lib::Point{30, 50},
            winner ? "Congratulations:)" : "      You lose:("};
        t->set_font_size(40);
        end_obj[0] = t;
        
        Graph_lib::Button* bt_next = new Graph_lib::Button{Graph_lib::Point{20, 150},
            140, 60, "Next game", cb_next};
        Graph_lib::Button* bt_quit = new Graph_lib::Button{Graph_lib::Point{240, 150},
            140, 60, "Quit", cb_quit};
        end_obj[1] = bt_next;
        end_obj[2] = bt_quit;

        attach(*t);
        attach(*bt_next);
        attach(*bt_quit);
        std::cout << "End of the game" << std::endl;
    }

    void operator=(Sudoku&) = delete;
    Sudoku(Sudoku&) = delete;
    
};

#endif