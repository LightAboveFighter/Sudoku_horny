#ifndef _SDBUTTON_H_
#define _SDBUTTON_H_
#include "Graph_lib/Simple_window.h"

/* Класс Судоку-кнопки, наследумый от стандартной кнопки Graph_lib
Стандартной кнопке блокируется label - вместо него над ней рисуется
Graph_lib::Text - так мы можем регулировать размер, шрифт и цвет значения кнопки.
Также Судоку-кнопка запоминает свое расположение в таблице - колонка и строка,
так как она будет находиться в Судоку-таблице.
*/
class SDButton : public Graph_lib::Button {
public:
    Graph_lib::Text text;
    // p - положение кнопки, ww - ширина, hh - высота, lb - значение кнопки,
    // cb - callback функция, привязанная к кнопке, c - столбец положения
    // в таблице, l - линия положения в таблице
    SDButton(Graph_lib::Point p, int ww, int hh,
        const std::string& lb, Graph_lib::Callback cb, int c, int l) :
        Graph_lib::Button{p, ww, hh, "", cb}, column{c}, line{l},
        text{Graph_lib::Point{p.x + 18, p.y + 42}, lb} {};
    
    void attach(Graph_lib::Window& win) override; //добавляем в attach присоединение
                                                  //текста - Graph_lib::Text
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
    using Graph_lib::Button::label;  //блокируем стандартное значение кнопки
};

#endif