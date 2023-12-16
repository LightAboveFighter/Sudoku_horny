#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Simple_window.h"

#include "Front.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    using namespace Graph_lib;
    std::cout << "ebat" << std::endl;

    Sudoku t{Point{0,0}, 1100, 600, "Sudoku"};
    // Grid d;
    // d.init();
    // std::cout << "nahui" << std::endl;
    // d.mix();
    // d.show();
    // std::cout << "suka" << std::endl;
    // sshow(d.erased('E'));
    // std::cout << "huinya" << std::endl;
    // d.show();
    // std::cout << "blya" << std::endl;

    return gui_main();
}