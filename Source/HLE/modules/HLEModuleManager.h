/*
*
*
*   
*    5/5/2011 -  synced with 913
*/

namespace HLEModuleManager
{
     int psfFirmwareVersionToInt(string firmwareVersion);
	 void Initialise(int firmwareVersion); 
	 void installDefaultModules();
	 void initialiseFlash0PRXMap();
	 void addFunction(HLEModuleFunction *func, int nid);
	 void removeFunction(HLEModuleFunction *func);
	 void addThread(HLEThread *thread);
	 void removeThread(HLEThread *thread);
	 void step();
	 bool handleSyscall(int code);
};