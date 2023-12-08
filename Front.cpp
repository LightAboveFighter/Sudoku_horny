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


void Sudoku::init_start_finish_pos(std::vector<std::vector<int>> st,
    std::vector<std::vector<int>> fin) {
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (st[i][j] > 9) {
                throw "Invalid number for default sudoku";
            }
        }
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            std::string label = std::to_string(st[i][j]);
            buttons[i][j] = new SDButton{Point{300 + 63*i, 20 + 63*j}, 60, 60,
                label == "0" ? "" : label, cb_clicked, i, j};
            buttons[i][j]->text.set_font_size(38);
            start[i][j] = st[i][j];
            attach(*buttons[i][j]);
        }
    }
    mistakes = new Graph_lib::Text{Point{50, 50}, "Mistakes: 0/3"};
    attach(*mistakes);

    for (int i = 1; i < 3; ++i) {
        Rectangle* p = new Rectangle(Point{300 + 189*i - 3, 20}, Point{300 + 189*i, 584});
        p->set_fill_color(Graph_lib::Color::black);
        attach(*p);
    }
    for (int i = 1; i < 3; ++i) {
        Rectangle* p = new Rectangle(Point{300, 20 + 189*i - 3}, Point{864, 20 + 189*i});
        p->set_fill_color(Graph_lib::Color::black);
        attach(*p);
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (fin[i][j] > 9 || ( fin[i][j] != start[i][j] && start[i][j] != 0)) {
                throw "Invalid number for default sudoku";
            }
        }
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            finish[i][j] = fin[i][j];
        }
    }
}

Sudoku::~Sudoku() {
    Graph_lib::Window::~Window();
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            delete buttons[i][j];
        }
    }
    delete selected;
}