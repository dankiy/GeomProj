#include "log.h"

Log::Log()
{
logger.open("log.txt");
session = time(0);
logger << ctime(&session);
logger << '\n';
}

Log::~Log()
{
logger.close();
}

void Log::AddNote(string note)
{
logger << note;
logger << '\n';
}
