#ifndef LOG_H
#define LOG_H
#include <fstream>
//#include <string>
#include <ctime>
#include <string>

using namespace std;

class Log
{
private:
ofstream logger;
time_t session;
public:
Log();
~Log();
void AddNote(string note);
};

#endif // LOG_H
