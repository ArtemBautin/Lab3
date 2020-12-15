/* Программа-заготовка для домашнего задания
*/

#include "hw/l4_InfrastructureLayer.h"

#include <iomanip>

using namespace std;

string readString(istream& is, size_t max_string_length)
{
    uint16_t len = readNumber<uint16_t>(is);

    if(len > max_string_length)
        throw Exception("readString","len > max_string_length");

    char b[max_string_length+1];

    if (len > 0)
        is.read(b, len);

    b[len] = 0;

    return std::string(b);
}

void writeString(ostream& os, const string& s)
{
    uint16_t len = s.length();

    writeNumber(os, len);

    os.write(s.c_str(), len);
}


shared_ptr<ICollectable> ACollector::getItem(size_t index) const
{
    if(index >= _items.size())
        throw Exception("ACollector::getItem","index >= _items.size()");

    return _items[index];
}

bool ACollector::isRemoved(size_t index) const
{
    if(index >= _removed_signs.size())
        throw Exception("ACollector::isRemoved","index >= _removed_signs.size()");

    return _removed_signs[index];
}

void ACollector::addItem(shared_ptr<ICollectable> item)
{
    _items.emplace_back(item);
    _removed_signs.emplace_back(false);
}

void ACollector::removeItem(size_t index)
{
    if(index >= _items.size())
        throw Exception("ACollector::removeItem","index >= _items.size()");
    if(index >= _removed_signs.size())
        throw Exception("ACollector::removeItem","index >= _removed_signs.size()");

    if (!_removed_signs[index])
    {
        _removed_signs[index] = true;
        _removed_count ++;
    }
}

void ACollector::updateItem(size_t index, const shared_ptr<ICollectable> & item)
{
    if(index >= _items.size())
        throw Exception("ACollector::updateItem","index >= _items.size()");

    _items[index] = item;
}

void ACollector::clean()
{
    _items.clear();
    _removed_signs.clear();
}

bool ACollector::loadCollection(const string file_name)
{
    ifstream ifs (file_name, ios_base::binary);

    if (!ifs)
        return false;

    size_t count = readNumber<size_t>(ifs);

    _items.reserve(count);

    for(size_t i=0; i < count; ++i)
        addItem(read(ifs));

    if (!invariant())
        throw Exception("ACollector::loadCollection","invariant");

    return ifs.good();
}

bool ACollector::saveCollection(const string file_name) const
{
    if (!invariant())
        throw Exception("ACollector::saveCollection","invariant");

    ofstream ofs (file_name, ios_base::binary);

    if (!ofs)
        return false;

    if (_items.size() < _removed_count)
        throw Exception("ACollector::saveCollection","_items.size() < _removed_count");

    size_t count = _items.size() - _removed_count;

    writeNumber(ofs, count);

    for(size_t i=0; i < _items.size(); ++i)
        if (!_removed_signs[i])
            _items[i]->write(ofs);

    return ofs.good();
}

const string LOG_FILE_NAME {"tmp/hw.log"};

void LogSession::write(string level, string text)
{
    if (!good())
        return;

    auto    now     = chrono::high_resolution_clock::now();
    time_t  time    = std::chrono::system_clock::to_time_t(now);

    _log_file << std::put_time(std::localtime(&time), "%F %T")  << " "
              << to_string(_session_id) << " "
              << level << " "
              << text
              << std::endl;
}

LogSession::LogSession(int session_id, string session_name)
    : _session_id(session_id)
    , _log_file(LOG_FILE_NAME, ios_base::app)
{
    if (!good())
        throw Exception("LogSession::LogSession", "Не удалось открыть файл журнала '" + LOG_FILE_NAME + "'");

    _session_start = chrono::high_resolution_clock::now();
    write("START", session_name);
}

LogSession::~LogSession()
{
    auto session_end = chrono::high_resolution_clock::now();

    auto elapsed = chrono::duration_cast<chrono::microseconds>(session_end-_session_start);

    write("END", to_string(elapsed.count()));

    _log_file.close();
}

void LogSession::information(string text)
{
    write("INFO", text);
}

void LogSession::error(string text)
{
    write("ERROR", text);
}

int LogSession::generateSessionID()
{
    return chrono::high_resolution_clock::now().time_since_epoch().count() % 100000;
}
