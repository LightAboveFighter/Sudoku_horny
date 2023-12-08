#ifndef _FRONT_H_
#define _FRONT_H_

#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/fltk.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Simple_window.h"

#include <string>
#include <vector>
#include <iostream>

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
private:
    int column{0};
    int line{0};
    int color;
    using Graph_lib::Button::label;
};

class Sudoku : public Graph_lib::Window {
public:
    Sudoku(Graph_lib::Point p, int ww, int hh, const std::string& lb) :
        Graph_lib::Window{p, ww, hh, lb}, start{9, std::vector<int>(9)},
        finish{9, std::vector<int>(9)}, buttons{9, std::vector<SDButton*>(9)},
        inputs{9, std::vector<Graph_lib::In_box*>(9)} {};
    
    void init_start_finish_pos(std::vector<std::vector<int>> st,
        std::vector<std::vector<int>> fin);

    ~Sudoku();

private:
    std::vector<std::vector<int>> start;
    std::vector<std::vector<int>> finish;
    std::vector<std::vector<SDButton*>> buttons;
    std::vector<std::vector<Graph_lib::In_box*>> inputs;
    SDButton* selected{nullptr};
    Graph_lib::Text* mistakes;
    int count_mistakes;

    static void cb_clicked(Graph_lib::Address, Graph_lib::Address addr) {
        auto& bt = Graph_lib::reference_to<SDButton>(addr);
        dynamic_cast<Sudoku&>(bt.window()).clicked(bt);
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
        redraw();   
    }

    void activated(SDButton& bt) {
        std::string lb = bt.text.label();
        int col = bt.getcolumn();
        int lin = bt.getline();

        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if ( j == lin ) {
                    buttons[i][j]->setColor(Graph_lib::Color::green);
                }
                if ( i == col ) {
                    buttons[i][j]->setColor(Graph_lib::Color::green);
                }
                if ( j/3 == lin/3 && i/3 == col/3) {
                    buttons[i][j]->setColor(Graph_lib::Color::green);
                }
                if ( buttons[i][j]->text.label() == lb && lb != "") {
                    buttons[i][j]->setColor(Graph_lib::Color::cyan);
                }
            }
        }
        bt.setColor(Graph_lib::Color::dark_magenta);
        handle(Fl::event());


    }
    void deactivated(SDButton& bt) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (buttons[i][j]->getColor() == Graph_lib::Color::red) 
                    selected->setTextColor(Graph_lib::Color::red);
                buttons[i][j]->setColor(Graph_lib::Color::white);
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
    }

    int handle(int event) {
        if (event == FL_KEYDOWN) {
            if (selected){
                int col = selected->getcolumn();
                int lin = selected->getline();

                if (selected->getColor() == Graph_lib::Color::red || selected->getTextColor() == Graph_lib::Color::red) {
                    deactivated(*selected);
                    selected->text.set_label("");
                    start[col][lin] = 0;
                    selected->resetTextColor();
                    selected = nullptr;
                    redraw();
                    return 1;
                }

                char val = *Fl::e_text;
                if ( val >= '1' && val <= '9')
                    if (selected->text.label() == "") {
                        selected->text.set_label(Fl::e_text);
                        start[lin][col] = *Fl::e_text;
                        selected->text.set_color(FL_INACTIVE_COLOR);
                        activated(*selected);
                        if (!is_corr(*selected)) {
                            mistake_occured();
                        }
                        // if (is_end()) game_over();
                        redraw();
                    }
            }
            return 1;
        }
        return Graph_lib::Window::handle(event);
    }

    // void game_over() {
    //     clear();
    //     Graph_lib::Text* t = new Graph_lib::Text{Graph_lib::Point{580, 300}, "Congratulations!"};
    //     t->set_font_size(40);
    //     attach(*t);
    //     std::cout << "end of the game" << std::endl;
    // }
    
};

#endif