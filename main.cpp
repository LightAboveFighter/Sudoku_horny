#include "Graph_lib/Graph.h"

#include "Front.h"
#include <iostream>
#include <vector>
#include <string>

int main() {
    Sudoku t{Graph_lib::Point{0,0}, 1100, 600, "Sudoku"};
    return Graph_lib::gui_main();
}