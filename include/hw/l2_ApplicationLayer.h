/* 
Программа-заготовка для домашнего задания
*/

#ifndef HW_L2_APPLICATION_LAYER_H
#define HW_L2_APPLICATION_LAYER_H

#include <string>

#include "hw/l3_DomainLayer.h"

class IOutput
{
public:
    virtual ~IOutput() = default;

    virtual void Output(std::string s) const = 0;
};

class Application
{
public:
    Application() = delete;
    Application(const Application &) = delete;

    Application & operator=(const Application &) = delete;

    Application(const IOutput & out)
        : _out(out)
    {}

    bool performCommand(int session_id, const std::vector<std::string> & args);
protected:
    void error(LogSession &log, std::string text);
    
private:
    const IOutput & _out;
    ItemCollector   _col;
};

#endif // HW_L2_APPLICATION_LAYER_H
