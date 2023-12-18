#include "Nicolas.h"

Nicolas::Nicolas(int cage_v, std::vector<int> availab)
{
    cage_value = cage_v;
    available = availab;
}
Nicolas::Nicolas(int cage_v)
{
    cage_value = cage_v;
    available = {};
}
Nicolas::Nicolas()
{
    cage_value = 0;
    available = {};
}

int Nicolas::cage_get() const
{
    return cage_value;
}
void Nicolas::cage_put(int num)
{
    cage_value = num;
}

std::vector<int> Nicolas::available_get() const
{
    return available;
}
void Nicolas::available_put(std::vector<int> available_new)
{
    available = available_new;
}
void Nicolas::available_remove(int num)
{
    available.erase(std::remove(available.begin(), available.end(), num), available.end());
}


