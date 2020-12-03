/* Программа-заготовка для домашнего задания
*/

#include <iostream>
#include <string>
#include <cassert>

#include <sstream>

#include "hw/l2_ApplicationLayer.h"

using namespace std;

class TerminalOutput : public IOutput
{
public:
    virtual void Output(string s) const override final;
};


void TerminalOutput::Output(string s) const
{
    cout << s << endl;
}


int main(int , char **)
{
    TerminalOutput out;
    Application    app(out);

    cout << "Введите команду для работы с карточкой 'Автозапчасть': " << endl;
    cout << "l - Загрузить данные из хранилища;" << endl;
    cout << "s - Сохранить введенные данные;" << endl;
    cout << "c - Очистить текущие данные;" << endl;
    cout << "a - Добавить данные (a <Название> <Марка> <Модель> <Стоимость> <Наличие>);" << endl;
    cout << "r - Удалить данные (r <id>);" << endl;
    cout << "u - Изменить данные (u <id> <Новое название> <Новая марка> <Новая модель> <Новая стоимость> <Новое наличие>);" << endl;
    cout << "v - Показать текущие данные;" << endl;
    cout << "ad - Добавить данные о сроке годности (ad <id> <Дата>);" << endl;
    cout << "vd - Отчет (vd <Дата>);" << endl;

    for (string line; getline(cin, line); )
    {
        if (line.empty())
            break;

        istringstream  iss(line);
        vector<string> args;
    
        for(string str; iss.good();)
        {
            iss >> str;
            args.emplace_back(str);
        }

        if (!app.performCommand(args))
            return 1;
    }

    cout << "Выполнение завершено успешно" << endl;
    return 0;
}
