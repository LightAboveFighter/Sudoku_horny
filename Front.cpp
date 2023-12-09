#include "Front.h"

using namespace Graph_lib;

void SDButton::attach(Graph_lib::Window& win) {
    Button::attach(win);
    win.attach(text);
    resetColor();
}

void SDButton::setColor(Color c) {
    pw->color(c.as_int());
    color = c.as_int();
}

void SDButton::setTextColor(Color c) {
    text.set_color(c);
}

void SDButton::resetColor() {
    pw->color(Color::white);
    color = Color::white;
}

void SDButton::resetTextColor() {
    text.set_color(Color::black);
}

Sudoku::Sudoku(Graph_lib::Point p, int ww, int hh, const std::string& lb) :
    SubWindow{Point{400, 200}, ww, hh, lb, this}, start{9, std::vector<int>(9)},
    finish{9, std::vector<int>(9)}, play_buttons{9, std::vector<SDButton*>(9)} {
        play_win = this;
        menu = new SubWindow{300, 400, lb, this};
        end_win = new SubWindow{400, 300, lb, this};
        play_win->hide();
        end_win->hide();
        active_window = menu;

        Graph_lib::Text* t = new Graph_lib::Text{Point{100, 50}, "Menu"};
        t->set_font_size(40);
        t->set_color(Graph_lib::Color::dark_green);
        Graph_lib::Button* bt_start = new Graph_lib::Button{Point{90, 130},
            120, 50, "Start", cb_next};
        attach(*t);
        attach(*bt_start);
        menu_buttons[0] = bt_start;
    };

void Sudoku::attach(Graph_lib::Shape& s) {
    active_window->Window::attach(s);
    links_s.push_back(&s);
}

void Sudoku::attach(Graph_lib::Widget& s) {
    active_window->Window::attach(s);
    links_w.push_back(&s);
}


Sudoku::~Sudoku() {
    for (std::size_t i = 0; i < links_s.size(); ++i) {
        delete links_s[i];
    }
    for (std::size_t i = 0; i < links_w.size(); ++i) {
        delete links_w[i];
    }
}