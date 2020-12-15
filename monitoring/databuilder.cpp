#include <iostream>
#include <vector>

using namespace std;

struct RawData
{
    string  date_time;
    string  session_id;
    string  type;
    string  duration;
};

void readData(vector<RawData> & input_data);
void processingData(const vector<RawData> & in, vector<RawData> & out);
void writeData(const vector<RawData> & data);

int main(int , char **)
{
    vector<RawData> input_data;
    vector<RawData> output_data;

    readData(input_data);
    processingData(input_data,output_data);
    writeData(output_data);
}

void readData(vector<RawData> & data)
{
    for (string line; getline(cin, line); )
    {
        string::size_type time_pos = line.find(' ');
        if (string::npos == time_pos)
            continue;

        string::size_type session_pos = line.find(' ', ++time_pos);
        if (string::npos == session_pos)
            continue;

        string::size_type type_pos = line.find(' ', ++session_pos);
        if (string::npos == type_pos)
            continue;

        string::size_type text_pos = line.find(' ', ++type_pos);
        if (string::npos == text_pos)
            continue;

        string  date_time   = line.substr(0, session_pos-1);
        string  session_id  = line.substr(session_pos, type_pos-session_pos-1);
        string  type        = line.substr(type_pos, text_pos-type_pos);
        string  duration    = (type != "END") ? "" : line.substr(text_pos);

        data.push_back({date_time, session_id, type, duration});
    }
}

void processingData(const vector<RawData> & in, vector<RawData> & out)
{
    string  last_date_time = "";
    int     duration_sum    = 0;

    string  date_time;
    string  session_id;
    string  type;
    string  duration;

    for(const auto & row : in)
    {
        if (last_date_time != row.date_time)
        {
            if (!last_date_time.empty())
                out.push_back({date_time, session_id, type, to_string(duration_sum)});

            last_date_time = row.date_time;
            duration_sum = 0;
            type = "";
        }

        date_time   = row.date_time;
        session_id  = row.session_id;
        type        = (type == "ERROR") ? type : row.type;
        duration    = row.duration;

        if (!duration.empty())
            duration_sum += stoi(duration);
    }

    if (!last_date_time.empty())
        out.push_back({date_time, session_id, type, to_string(duration_sum)});
}

void writeData(const vector<RawData> & data)
{
    for(auto r : data)
        cout << r.date_time << " "
             << r.session_id << " "
             << r.duration << " "
             << r.type << endl;
}
