/* Программа-заготовка для домашнего задания
*/

#include "hw/l3_DomainLayer.h"

using namespace std;

Date::Date(int y, int m, int d)
    : _year(y), _month(m), _day(d)
{
    assert(invariant());
}

Date::Date(string str)
{
    string  ys, ms, ds;
    int     stage = 0;

    for(auto ch : str)
    {
        if (ch == '/')
            stage ++;
        else
        {
            if (stage == 0)
                ys += ch;
            else if (stage == 1)
                ms += ch;
            else
                ds += ch;
        }
    }

    _year = stoi(ys);
    _month = stoi(ms);
    _day = stoi(ds);

    assert(invariant());
}

bool Date::operator <(const Date &d2) const
{
    return (_year > d2.getYear())
        || (_year == d2.getYear() && _month > d2.getMonth())
        || (_year == d2.getYear() && _month == d2.getMonth() && _day > d2.getDay());
}

string Date::getDateString() const
{
    return to_string(_year) + "/" + to_string(_month) + "/" + to_string(_day);
}

int Date::diffInMonths(const Date &d2) const
{
    int diff = (getYear() - d2.getYear())*12;
    diff += getMonth() - d2.getMonth();
    return diff;
}


ShelfLife::ShelfLife(Date d)
    : _date(d)
{
    assert(invariant());
}


AutoPart::AutoPart(const std::string & auto_part_name, const std::string & car_brand, string car_model, uint16_t cost, std::string availability)
    : _auto_part_name(auto_part_name)
    , _car_brand(car_brand)
    , _car_model(car_model)
    , _cost(cost)
    , _availability(availability)
{
    assert(invariant());
}

AutoPart::AutoPart(const string &auto_part_name, const string &car_brand, string car_model, uint16_t cost, std::string availability,
               std::map<Date, ShelfLife> shelf_life_story)
    : _auto_part_name(auto_part_name)
    , _car_brand(car_brand)
    , _car_model(car_model)
    , _cost(cost)
    , _availability(availability)
    , _shelf_life_story(shelf_life_story)
{
    assert(invariant());
}

bool   AutoPart::write(ostream& os)
{
    writeString(os, _auto_part_name);
    writeString(os, _car_brand);
    writeString(os, _car_model);
    writeNumber(os, _cost);
    writeString(os, _availability);

    size_t shelf_life_story_quantity = _shelf_life_story.size();
    writeNumber(os, shelf_life_story_quantity);

    for(const auto & [date,debt] : _shelf_life_story)
    {
        writeNumber(os, date.getYear());
        writeNumber(os, date.getMonth());
        writeNumber(os, date.getDay());
    }

    return os.good();
}

void AutoPart::setShelfLife(ShelfLife shelflife)
{
    auto it = _shelf_life_story.find(shelflife.getDate());

    if (it != _shelf_life_story.end())
        _shelf_life_story.erase(it);

    _shelf_life_story.insert({shelflife.getDate(), shelflife});
}


shared_ptr<ICollectable> ItemCollector::read(istream& is)
{
    string   auto_part_name = readString(is, MAX_LENGTH);
    string   car_brand  = readString(is, MAX_LENGTH);
    string   car_model      = readString(is, MAX_LENGTH);
    uint16_t   cost      = readNumber<uint16_t>(is);
    string   availability      = readString(is, MAX_LENGTH);

    map<Date, ShelfLife> shelf_life_story;
    size_t shelf_life_story_quantity = readNumber<size_t>(is);
    for(size_t i=0; i < shelf_life_story_quantity; ++i)
    {
        int      year   = readNumber<int>(is);
        int      month  = readNumber<int>(is);
        int      day    = readNumber<int>(is);

        shelf_life_story.insert({Date(year,month,day), ShelfLife(Date(year,month,day))});
    }

    return std::make_shared<AutoPart>(auto_part_name, car_brand, car_model, cost, availability, shelf_life_story);
}


