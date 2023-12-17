#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Graph.h"
#include "/home/alexunder/Documents/MIPT/C_plus_plus/add_libraries/Graph_lib/Simple_window.h"

#include "Front.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    Sudoku t{Graph_lib::Point{0,0}, 1100, 600, "Sudoku"};
    return Graph_lib::gui_main();
}