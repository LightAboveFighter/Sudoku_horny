#ifndef _FRONT_H_
#define _FRONT_H_

#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/fltk.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Simple_window.h"

#include "generator.cpp"

#include <string>
#include <vector>
#include <iostream>
#include <random>

class SDButton : public Graph_lib::Button {
public:
    Graph_lib::Text text;
    SDButton(Graph_lib::Point p, int ww, int hh,
        const std::string& lb, Graph_lib::Callback cb, int c, int l) :
        Graph_lib::Button{p, ww, hh, "", cb}, column{c}, line{l},
        text{Graph_lib::Point{p.x + 18, p.y + 42}, lb} {};
    
    void attach(Graph_lib::Window& win) override;
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
    using Graph_lib::Button::label;
};

class Sudoku;

class SubWindow : public Graph_lib::Window {
public:
    SubWindow(int ww, int hh, const std::string& lb, Sudoku* par) :
        Window{ww, hh, lb}, parent{par} {};
    SubWindow(Graph_lib::Point p, int ww, int hh, const std::string& lb, Sudoku* par) :
        Window{p, ww, hh, lb}, parent{par} {};
    Sudoku* parent;
};

class Sudoku : public SubWindow {
public:
    Sudoku(Graph_lib::Point p, int ww, int hh, const std::string& lb);

    void attach(Graph_lib::Shape& s);
    void attach(Graph_lib::Widget& s);

    char difficulty{'E'};

    ~Sudoku();

private:
    Window* menu;
    Window* play_win;
    Window* end_win;
    Window* active_window;
    bool difficulty_showed{False};

    std::vector<void*> menu_obj{7, nullptr};
    std::vector<Graph_lib::Button*> end_buttons{2, nullptr};

    std::vector<std::vector<int>> start;
    std::vector<std::vector<int>> finish;
    std::vector<std::vector<SDButton*>> play_buttons;
    SDButton* selected{nullptr};
    Graph_lib::Text* mistakes;
    int count_mistakes;

    std::vector<Graph_lib::Shape*> links_s;
    std::vector<Graph_lib::Widget*> links_w;

    static void cb_play_clicked(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<SDButton>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->clicked(bt);
    }

    static void cb_next(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->next_view();
    }

    static void cb_menu_toggle(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->menu_toggle();
    }

    static void cb_difficulty_toggle(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->difficulty_toggle(bt);
    }

    static void cb_quit(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<Graph_lib::Button>(addr);
        dynamic_cast<SubWindow&>(bt.window()).parent->quit();
    }

    void quit() {
        active_window->hide();
    }

    void clicked(SDButton& bt) {
        if (!selected) {
            selected = &bt;
            activated(*selected);
        } else {
            deactivated(*selected);
            if (selected == &bt) {
                selected = nullptr;
            } else {
                selected = &bt;
                activated(bt);
            }
        }
        play_win->redraw();   
    }

    void next_view() {
        if (active_window == menu) {
            play_win->show();
            active_window->hide();
            active_window = play_win;
            init_play();
        }
        else if (active_window == play_win) {
            end_win->show();
            active_window->hide();
            active_window = end_win;
            count_mistakes = 1;   // to next time don't re-create play_window, just change values
        }
        else if (active_window == end_win) {
            menu->show();
            active_window->hide();
            active_window = menu;
            if (difficulty_showed) menu_toggle();
        }
    }

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

    void difficulty_toggle(Graph_lib::Button& bt) {
        int move_coeff = 0;
        if (difficulty != bt.label[0]) {
            move_coeff = ((difficulty + bt.label[0] == 'E' + 'H') + 1) * 70 * ( 1 + 2 * -( (difficulty ==
                'M' || difficulty == 'E') && (bt.label[0] == 'H' || bt.label[0] == 'M') ) );
        }
        static_cast<Graph_lib::Circle*>(menu_obj[6])->move(0, -move_coeff);
        difficulty = bt.label[0];
        menu->redraw();
    }

    void generate_nums() {
        Grid* table = new Grid;
        table->init();
        table->mix();
        std::vector<std::vector<int>> st(9, std::vector<int>(9));
        std::vector<std::vector<int>> fin = table->gettable();
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (Graph_lib::randint(1)) {
                    st[i][j] = fin[i][j];
                } else {
                    st[i][j] = 0;
                }
            }
        }
        finish = fin;
        start = st;
    }

    void init_play() {
        selected = nullptr;
        generate_nums();
        if (count_mistakes) {
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

    void activated(SDButton& bt) {
        std::string lb = bt.text.label();
        int col = bt.getcolumn();
        int lin = bt.getline();

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if ( j == lin ) {
                    play_buttons[i][j]->setColor(Graph_lib::Color::green);
                }
                if ( i == col ) {
                    play_buttons[i][j]->setColor(Graph_lib::Color::green);
                }
                if ( j/3 == lin/3 && i/3 == col/3) {
                    play_buttons[i][j]->setColor(Graph_lib::Color::green);
                }
                if ( play_buttons[i][j]->text.label() == lb && lb != "") {
                    play_buttons[i][j]->setColor(Graph_lib::Color::cyan);
                }
            }
        }
        bt.setColor(Graph_lib::Color::dark_magenta);
        handle(Fl::event());


    }
    void deactivated(SDButton& bt) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (play_buttons[i][j]->getColor() == Graph_lib::Color::red) 
                    selected->setTextColor(Graph_lib::Color::red);
                play_buttons[i][j]->setColor(Graph_lib::Color::white);
            }
        }
    }

    bool is_corr(SDButton& bt) {
        int col = bt.getcolumn();
        int lin = bt.getline();
        if (bt.text.label() == std::to_string(finish[col][lin]) || bt.text.label() == "")
            return true;
        return false;
    }

    bool is_end() {
        return start == finish;
    }

    void mistake_occured() {
        selected->setColor(Graph_lib::Color::red);
        count_mistakes += 1;
        std::string str = "Mistakes: " + std::to_string(count_mistakes) + "/3";
        mistakes->set_label(str);
        if (count_mistakes == 3) game_over(False);
    }

    int handle(int event) {
        if (event == FL_KEYDOWN) {
            if (*Fl::e_text == ']') std::cout << "5d5" << std::endl;
            if (selected){
                int col = selected->getcolumn();
                int lin = selected->getline();

                if (selected->getColor() == Graph_lib::Color::red || selected->getTextColor() == Graph_lib::Color::red) {
                    deactivated(*selected);
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
                        }
                        if (is_end()) game_over(True);
                        play_win->redraw();
                    }
            }
            return 1;
        }
        return Graph_lib::Window::handle(event);
    }

    void game_over(bool winner) {
        next_view();
        Graph_lib::Text* t = new Graph_lib::Text{Graph_lib::Point{30, 50},
            winner ? "Congratulations:)" : "    You're lost:("};
        t->set_font_size(40);
        Graph_lib::Button* bt_next = new Graph_lib::Button{Graph_lib::Point{20, 150},
            140, 60, "Next game", cb_next};
        Graph_lib::Button* bt_quit = new Graph_lib::Button{Graph_lib::Point{240, 150},
            140, 60, "Quit", cb_quit};

        attach(*t);
        attach(*bt_next);
        attach(*bt_quit);
        std::cout << "End of the game" << std::endl;
    }

    void operator=(Sudoku&) = delete;
    Sudoku(Sudoku&) = delete;
    
};

#endif