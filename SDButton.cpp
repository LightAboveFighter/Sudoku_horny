#include "SDButton.h"

using namespace Graph_lib;

void SDButton::attach(Graph_lib::Window& win) {
    Button::attach(win);
    win.attach(text);
    resetColor();
}

// Изменение цвета кнопки
void SDButton::setColor(Color c) {
    pw->color(c.as_int());
    color = c.as_int();
}

// Изменение цвета значения кнопки
void SDButton::setTextColor(Color c) {
    text.set_color(c);
}

// Сброс цвета кнопки
void SDButton::resetColor() {
    pw->color(Color::white);
    color = Color::white;
}

// Сброс цвета значения кнопки
void SDButton::resetTextColor() {
    text.set_color(Color::black);
}