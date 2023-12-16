#ifndef _AMOUNT_H_
#define _AMOUNT_H_ = 12

#include <iostream>
#include <vector>
#include <algorithm>

std::vector<int> check_cage(std::vector<std::vector<int>> const &grid,
    const int &cage_row, const int &cage_column);

int solutions(std::vector<std::vector<int>> const &grid);

#endif