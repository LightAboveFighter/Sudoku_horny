#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Simple_window.h"

#include "Front.h"
#include "generator.cpp"

#include <iostream>
#include <vector>
#include <random>
#include <string>

    // int glob_size = 9;
    // int glob_s_size = 3;
    // // w.color(Color::black);

    // Button* r;

    // std::vector<std::vector<Text*>> str_boxes(9, std::vector<Text*>(9));

    // for (int i = 0; i < glob_size; ++i) {
    //     for (int j = 0; j < glob_size; ++j) {
    //         r = new Button{Point{300 + 63*i, 20 + 63*j}, 60, 60, "", change_color()};
    //         w.attach(*r);
        
    //         Text* t = new Text{Point{300 + 63*i + 13, 20 + 63*j + 50}, ""};
    //         t->set_font_size(50);
    //         w.attach(*t);
    //         str_boxes[i][j] = t;
    //     }
    // }


int main() {
    using namespace Graph_lib;
    Grid table;
    table.init();
    table.mix();
    std::vector<std::vector<int>> start(9, std::vector<int>(9));
    std::vector<std::vector<int>> finish = table.gettable();
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (randint(1)) {
                start[i][j] = finish[i][j];
            } else {
                start[i][j] = 0;
            }
        }
    }
    Sudoku t{Point{0,0}, 1100, 600, "Sudoku"};
    t.init_start_finish_pos(start, finish);
    return gui_main();
}