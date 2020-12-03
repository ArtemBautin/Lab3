/* 
Программа-заготовка для домашнего задания
*/

#ifndef HW_L3_DOMAIN_LAYER_H
#define HW_L3_DOMAIN_LAYER_H

#include <set>
#include <map>

#include "hw/l4_InfrastructureLayer.h"

const size_t MAX_LENGTH    = 50;

class Date
{
    int _year, _month, _day;

protected:
    bool invariant() const
    {
        return _year >= 1900 && _year <= 2100
            && _month >= 1 && _month <= 12
            && _day >= 1 && _day <= 31;
    }

public:
    Date() = delete;
    Date(int y, int m, int d);
    Date(std::string str);

    bool operator < (const Date & d2) const;

    int getYear()   const { return _year; }
    int getMonth()  const { return _month; }
    int getDay()    const { return _day; }

    std::string getDateString() const;

    int diffInMonths(const Date & d2) const;
};

class ShelfLife
{
    friend class Person;

    Date        _date;

protected:
    bool    invariant() const { return true; }

public:
    ShelfLife() = delete;
    ShelfLife(Date d);

    Date        getDate()       const { return _date; }

};

class AutoPart : public ICollectable
{
    std::string _auto_part_name;
    std::string _car_brand;
    std::string _car_model;
    uint16_t    _cost;
    std::string _availability;

    std::map<Date,ShelfLife> _shelf_life_story;

protected:
    bool invariant() const
    {
        return !_car_model.empty() && _car_model.size() <= MAX_LENGTH
            && !_auto_part_name.empty() && _auto_part_name.size() <= MAX_LENGTH
            && !_car_brand.empty() && _car_brand.size() <= MAX_LENGTH;
    }

    virtual bool   write(std::ostream& os) override;

public:
    AutoPart() = delete;
    AutoPart(const std::string & auto_part_name, const std::string & car_brand, std::string car_model, uint16_t cost, std::string availability);
    AutoPart(const std::string & auto_part_name, const std::string & car_brand, std::string car_model, uint16_t cost, std::string availability,
           std::map<Date,ShelfLife> shelf_life_story);

    std::string getAutoPartName()  const { return _auto_part_name; }
    std::string getCarBrand()   const { return _car_brand; }
    std::string getCarModel()      const { return _car_model; }
    uint16_t getCost()      const { return _cost; }
    std::string getAvailability()      const { return _availability; }

    const std::map<Date,ShelfLife> & getShelfLifeStory() const { return _shelf_life_story; }

    void setShelfLife(ShelfLife shelflife);
};


class ItemCollector: public ACollector
{
public:
    virtual std::shared_ptr<ICollectable> read(std::istream& is) override;
};

#endif // HW_L3_DOMAIN_LAYER_H
