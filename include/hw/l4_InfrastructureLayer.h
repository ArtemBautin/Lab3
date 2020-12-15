/* 
Программа-заготовка для домашнего задания
*/

#ifndef HW_L4_INFRASTRUCTURE_LAYER_H
#define HW_L4_INFRASTRUCTURE_LAYER_H

#include <string>
#include <vector>
#include <memory>
#include <exception>
#include <chrono>

#include <fstream>

template<typename T>
T readNumber(std::istream& is)
{   
	T result;
    is.read(reinterpret_cast<char *>(&result), sizeof(result));
    return result;
}

std::string readString(std::istream& is, size_t max_string_length);

template<typename T>
void writeNumber(std::ostream& os, T i)
{
    os.write(reinterpret_cast<char *>(&i),sizeof(i));
}

void writeString(std::ostream& os, const std::string& s);

class ICollectable
{
public:
    virtual ~ICollectable() = default;

    virtual bool write(std::ostream& os) = 0;
};

class ACollector
{
    std::vector<std::shared_ptr<ICollectable>> _items;
    std::vector<bool>                          _removed_signs;
    size_t                                     _removed_count = 0;

    bool    invariant() const
    {
        return _items.size() == _removed_signs.size() && _removed_count <= _items.size();
    }

public:
    virtual ~ACollector() = default;

    virtual std::shared_ptr<ICollectable> read(std::istream& is) = 0;

    size_t getSize() const { return _items.size(); }

    std::shared_ptr<ICollectable> getItem(size_t index) const;

    bool isRemoved(size_t index) const;

    void addItem(std::shared_ptr<ICollectable> item);

    void removeItem(size_t index);

    void updateItem(size_t index, const std::shared_ptr<ICollectable> & item);

    void clean();

    bool loadCollection(const std::string file_name);

    bool saveCollection(const std::string file_name) const;
};

class Exception : public std::exception
{
    std::string _where;
    std::string _what;

public:
    Exception(std::string where, std::string what)
        : std::exception()
        , _where(where)
        , _what(what)
    {}

    virtual ~Exception() override {}

    const std::string & where() const noexcept { return _where; }

    virtual const char* what() const noexcept override { return _what.c_str(); }
};

class LogSession
{
    int                                                         _session_id;
    std::ofstream                                               _log_file;
    std::chrono::time_point<std::chrono::high_resolution_clock> _session_start;

    void write(std::string level, std::string text);

public:
    LogSession() = delete;

    LogSession(int session_id, std::string session_name);

    virtual ~LogSession();

    bool good() const { return _log_file.good(); }

    void information(std::string text);
    void error(std::string text);

    static int generateSessionID();
};

#endif // HW_L4_INFRASTRUCTURE_LAYER_H
