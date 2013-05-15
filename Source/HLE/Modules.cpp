

#include "Common.h"
#include "Modules.h"

Log Modules::log;
bool Modules::initialize()
{
 log = Log("Modules");
 return true;
}