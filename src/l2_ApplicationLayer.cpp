/* Программа-заготовка для домашнего задания
*/

#include "hw/l2_ApplicationLayer.h"
#include <ctime>
using namespace std;

bool Application::performCommand(const vector<string> & args)
{
    if (args.empty())
        return false;

    if (args[0] == "l" || args[0] == "load")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!_col.loadCollection(filename))
        {
            _out.Output("Ошибка при загрузке файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "s" || args[0] == "save")
    {
        string filename = (args.size() == 1) ? "hw.data" : args[1];

        if (!_col.saveCollection(filename))
        {
            _out.Output("Ошибка при сохранении файла '" + filename + "'");
            return false;
        }

        return true;
    }

    if (args[0] == "c" || args[0] == "clean")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды clean");
            return false;
        }

        _col.clean();

        return true;
    }

    if (args[0] == "a" || args[0] == "add")
    {
        if (args.size() != 6)
        {
            _out.Output("Некорректное количество аргументов команды add");
            return false;
        }

        _col.addItem(make_shared<AutoPart>(args[1].c_str(), args[2].c_str(), args[3].c_str(), stoul(args[4]), args[5].c_str()));
        return true;
    }

    if (args[0] == "r" || args[0] == "remove")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды remove");
            return false;
        }

        _col.removeItem(stoul(args[1]));
        return true;
    }

    if (args[0] == "u" || args[0] == "update")
    {
        if (args.size() != 7)
        {
            _out.Output("Некорректное количество аргументов команды update");
            return false;
        }

        AutoPart & a = static_cast<AutoPart &>(*_col.getItem(stoul(args[1])));

        _col.updateItem(stoul(args[1]), make_shared<AutoPart>(args[2].c_str(), args[3].c_str(), args[4].c_str(), stoul(args[5]), args[6].c_str(),
                                                            a.getShelfLifeStory()));
        return true;
    }

    if (args[0] == "ad" || args[0] == "addinfo")
    {
        if (args.size() != 3)
        {
            _out.Output("Некорректное количество аргументов команды addinfo");
            return false;
        }

        AutoPart & a = static_cast<AutoPart &>(*_col.getItem(stoul(args[1])));

        Date date(args[2]);

        a.setShelfLife(date);

        return true;
    }

    if (args[0] == "v" || args[0] == "view")
    {
        if (args.size() != 1)
        {
            _out.Output("Некорректное количество аргументов команды view");
            return false;
        }

        size_t count = 0;
        for(size_t i=0; i < _col.getSize(); ++i)
        {
            const AutoPart & autopart = static_cast<AutoPart &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                _out.Output("[" + to_string(i) + "] "
                        + autopart.getAutoPartName() + " "
                        + autopart.getCarBrand() + " "
                        + autopart.getCarModel() + " "
                        + to_string(autopart.getCost()) + " "
                        + autopart.getAvailability());

                const map<Date,ShelfLife> & shelf_life_story = autopart.getShelfLifeStory();
                if (!shelf_life_story.empty())
                {

                    for(const auto [date,shelflife] : shelf_life_story)
                        _out.Output("\tСрок годности до " + date.getDateString() + ". "
                                                                  
                                  );
                }

                count ++;
            }
        }

        _out.Output("Количество элементов в коллекции: " + to_string(count));
        return true;
    }

    if (args[0] == "vd" || args[0] == "viewdata")
    {
        if (args.size() != 2)
        {
            _out.Output("Некорректное количество аргументов команды viewdata");
            return false;
        }

        Date today(args[1]);

        size_t count = 0;
        for(size_t i=0; i < _col.getSize(); ++i)
        {
            const AutoPart &    autopart         = static_cast<AutoPart &>(*_col.getItem(i));

            if (!_col.isRemoved(i))
            {
                uint64_t shelflife_sum = 0;

                time_t t;
                time(&t);
 
                int month=localtime(&t)->tm_mon;    

                    if (month>= today.diffInMonths(today)){
                        _out.Output("[" + to_string(i) + "] "
                                + autopart.getAutoPartName() + " "
                                + autopart.getCarBrand() + " "
                                + autopart.getCarModel() + " "
                                + to_string(autopart.getCost()) + " "
                                + autopart.getAvailability() + " "
                                + to_string(shelflife_sum));

                        count ++;}
            }
        }

        _out.Output("Товары к списанию в этом месяце " + today.getDateString()
                + " : " + to_string(count));
        return true;
    }

    _out.Output("Недопустимая команда '" + args[0] + "'");
    return false;
}
