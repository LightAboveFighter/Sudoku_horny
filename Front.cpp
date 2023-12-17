#include "Front.h"

using namespace Graph_lib;

// p - положение окна, ww - ширина окна, hh - высота окна, lb - строка с названием окна
Sudoku::Sudoku(Graph_lib::Point p, int ww, int hh, const std::string& lb) :
    SubWindow{Point{400, 200}, ww, hh, lb, this}, start{9, std::vector<int>(9)},
    finish{9, std::vector<int>(9)}, play_buttons{9, std::vector<SDButton*>(9)} {
        play_win = this;
        menu = new SubWindow{300, 400, lb, this};
        end_win = new SubWindow{400, 300, lb, this};
        play_win->hide();
        end_win->hide();
        active_window = menu;
        menu->size_range(300, 400, 300, 400);
        play_win->size_range(ww, hh, ww, hh);
        end_win->size_range(400, 300, 400, 300);

        Graph_lib::Text* menu_text = new Graph_lib::Text{Point{80, 80}, "Sudoku"};
        menu_text->set_font_size(40);
        menu_text->set_color(Graph_lib::Color::dark_green);
        Graph_lib::Button* bt_start = new Graph_lib::Button{Point{90, 130},
            120, 50, "Start", cb_next};
        Graph_lib::Button* bt_difficulty = new Graph_lib::Button{Point{90, 200},
            120, 50, "Difficulty", cb_menu_toggle};
        Graph_lib::Button* bt_easy = new Graph_lib::Button{Point{90, 200},
            120, 50, "Easy", cb_difficulty_toggle};
        Graph_lib::Button* bt_medium = new Graph_lib::Button{Point{90, 270},
            120, 50, "Medium", cb_difficulty_toggle};
        Graph_lib::Button* bt_hard = new Graph_lib::Button{Point{90, 340},
            120, 50, "Hard", cb_difficulty_toggle};
        Graph_lib::Circle* chosen_diff = new Graph_lib::Circle{Point{250, 225}, 16};

        attach(*menu_text);
        attach(*bt_start);
        attach(*bt_difficulty);
        attach(*bt_easy);
        attach(*bt_medium);
        attach(*bt_hard);
        attach(*chosen_diff);
        bt_easy->hide();
        bt_medium->hide();
        bt_hard->hide();
        chosen_diff->set_fill_color(Graph_lib::Color::invisible);
        chosen_diff->set_color(Graph_lib::Color::invisible);
        menu_obj = {menu_text, bt_start, bt_difficulty, bt_easy, bt_medium, bt_hard, chosen_diff};
    };

// Присоединение любой фигуры или виджета происходит
// только к активному окну.
// Параллельно происходит записывание прикрепленного объекта в соответствуюший вектор
// для дальнейшего корректного их удаления.
void Sudoku::attach(Graph_lib::Shape& s) {
    active_window->Window::attach(s);
    links_s.push_back(&s);
}
// Присоединение любой фигуры или виджета происходит
// только к активному окну.
// Параллельно происходит записывание прикрепленного объекта в соответствуюший вектор
// для дальнейшего корректного их удаления.
void Sudoku::attach(Graph_lib::Widget& w) {
    active_window->Window::attach(w);
    links_w.push_back(&w);
}

void Sudoku::detach(Graph_lib::Shape& s) {
    active_window->Window::detach(s);
}
void Sudoku::detach(Graph_lib::Widget& w) {
    active_window->Window::detach(w);
}

Sudoku::~Sudoku() {
    for (std::size_t i = 0; i < links_s.size(); ++i) {
        delete links_s[i];
    }
    for (std::size_t i = 0; i < links_w.size(); ++i) {
        delete links_w[i];
    }
}