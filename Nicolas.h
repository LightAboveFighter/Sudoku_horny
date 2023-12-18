#ifndef _NICOLAS_H_
#define _NICOLAS_H_

#include <vector>
#include <algorithm>

class Nicolas
{
private:
    std::vector<int> available;
    int cage_value;
public:
    Nicolas(int cage_v, std::vector<int> availab);        // конструкторы
    Nicolas(int cage_v);
    Nicolas();

    int cage_get() const;                                 // методы получения данных
    std::vector<int> available_get() const;

    void cage_put(int num);                               // методы записи данных
    void available_put(std::vector<int> available_new);

    void available_remove(int num);                       // удаление всех элементов 
                                                          // со значением num из available
};

#endif