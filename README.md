# Sudoku_horny
2nd task for programming on C++ at MIPT with FLTK Graph lib. 

Структура классов FLTK-версии: 

class SubWindow : public Graph_lib::Window {
public:
    SubWindow(int ww, int hh, const std::string& lb, Sudoku* par) :
        Window{ww, hh, lb}, parent{par} {};
    SubWindow(Graph_lib::Point p, int ww, int hh, const std::string& lb, Sudoku* par) :
        Window{p, ww, hh, lb}, parent{par} {};
    Sudoku* parent;
};

/* Судоку-окно наследуется от Window, в итоге создавая
три окна - меню начала, само Судоку, и меню завершения(конец).
К каждому окну прикреплено несколько кнопок, с помощью параметра parent
кнопки в этих окнах могут отправлять свое нажатие нашему классу.
*/
class Sudoku : public Window {
public:
    Sudoku(Graph_lib::Point p, int ww, int hh, const std::string& lb);

    void attach(Graph_lib::Shape& s);
    void attach(Graph_lib::Widget& w);
    void detach(Graph_lib::Shape& s);
    void detach(Graph_lib::Widget& w);

    ~Sudoku();

private:
    Window* menu;
    Window* play_win;
    Window* end_win;
    // Указатель на активное окно
    Window* active_window;

    // Параметр выбранной сложности
    char difficulty{'E'};

    // Булевая переменная, отвечаюшая за то, показано ли под-меню сложности
    // в первом окне.
    bool difficulty_showed{False};

    // Все объекты первого окна
    std::vector<void*> menu_obj{7, nullptr};
    // Все объекты конечного окна.
    std::vector<void*> end_obj{3, nullptr};

    // Параметр для деактивации изменений игрового поля после окончания игры
    // (Но с возможностью смотреть на это поле)
    bool is_playing{false};

    // При первом запуске окна создаются, при следующих - меняются.
    bool first_init{true};

    // Массив начальных чисел игры
    std::vector<std::vector<int>> start;
    // Массив конечных чисел игры
    std::vector<std::vector<int>> finish;
    // Таблица указателей на кнопки игры
    std::vector<std::vector<SDButton*>> play_buttons;
    // Указатель на выбранную кнопку
    SDButton* selected{nullptr};

    // Указатель на строку количества ошибок
    Graph_lib::Text* mistakes;

    // Счетчик количества ошибок.
    int count_mistakes;

    // Вектор всех фигур нашего класса. Нужны
    // для корректного удаления объекта, отсутствия утечек памяти.
    std::vector<Graph_lib::Shape*> links_s;
    // Вектор всех виджетов нашего класса. Нужны
    // для корректного удаления объекта, отсутствия утечек памяти.
    std::vector<Graph_lib::Widget*> links_w;

    // Callback функция нажатия кнопки - члена таблицы Судоку.
    static void cb_play_clicked(Graph_lib::Address, Graph_lib::Address addr);

    // Сallback функция нажатия кнопки Start или New game,
    // переключаящей активное окно.
    static void cb_next(Graph_lib::Address, Graph_lib::Address addr);

    // Сallback функция раскрытия подменю сложности в первом окне.
    static void cb_menu_toggle(Graph_lib::Address, Graph_lib::Address addr);

    // Сallback функция изменения уровня сложности в подменю первого окна.
    static void cb_difficulty_toggle(Graph_lib::Address, Graph_lib::Address addr);

    // Сallback функция выхода из программы, реализуется при нажатии Quit в последнем окне.
    static void cb_quit(Graph_lib::Address, Graph_lib::Address addr);

    // Завершает выполнение программы.
    void quit();

    // Реализует выделение и снятие выделения активной кнопки.
    void clicked(SDButton& bt);

    // Смена активного окна.
    void next_view();

    // Раскрытие подменю сложности в первом окне.
    void menu_toggle();

    // Изменение уровня сложности.
    void difficulty_toggle(Graph_lib::Button& bt);

    // Создание нового уровня - начальных и конечных чисел таблицы Судоку
    // в зависимости от выбранного уровня сложности.
    void generate_nums();

    // Подготовка главного игрового окна - либо создание, либо
    // изменение значений для следующей игры.
    void init_play();

    // Выделение кнопок в одной линии.
    void active_line(int line);

    // Выделение кнопок в одной колонке.
    void active_column(int col);

    // Выделение кнопок в одном квадрате.
    void active_square(int col, int line);

    // Выделение кнопок с одним значением.
    void active_labels(const std::string& lb);

    // Выделение активной линии, столбца, кнопок с такими же цифрами
    // и самой активной кнопки.
    void activated(SDButton& bt);

    // Установка всем кнопкам стандартного цвета.
    void deactivated();

    // Проверка введенного пользователем числа.
    bool is_corr(SDButton& bt);

    // Проверка успешного окончания игры.
    bool is_end();

    // При неверном введенном значении система увеличивает количество ошибок,
    // и обновляет строку с информацией об ошибках.
    // При 3х ошибках игра завершается.
    void mistake_occured();

    // Перехват клавиатуры - при нажатии кнопок 1-9 программа
    // фиксирует нажатие и изменяет значение активной кнопки.
    int handle(int event);

    // Завершение решения Судоку - вывод окна завершения со строкой выигрыша,
    // в случае его первого вызова - инициализация.
    void game_over(bool winner);

    // Создание конечного окна.
    void init_end(bool winner);

    void operator=(Sudoku&) = delete;
    Sudoku(Sudoku&) = delete;
    
};