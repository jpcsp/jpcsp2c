/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Common.h"
/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Emulator.h"
#include "Loader.h"
#include "NIDMapper.h"
#include "Memory.h"
#include "HLE/Kernel/Managers/ModuleManager.h"
#include "HLE/pspSysMem.h"
#include "Allegrex/Allegrex.h"
#include "HLE/Kernel/Types/ThreadWaitInfo.h"
#include "HLE/Kernel/Types/SceKernelCallbackInfo.h"
#include "HLE/Kernel/Types/SceKernelThreadInfo.h"
#include "HLE/ThreadMan.h"
#include "HLE/Modules.h"
#include "HLE/Kernel/Types/PspGeList.h"
#include "HLE/pspge.h"
#include "HLE/modules/HLEModuleFunction.h"
#include "HLE/modules/HLEThread.h"
#include "HLE/modules/HLEModuleManager.h"

Log Emulator::log;
SceModule *module;
bool moduleLoaded;
bool Emulator::initialize()
{
 log = Log("Emulator");
 Modules::initialize();
 return true;
}
SceModule *Emulator::load(const char *pspFileName, ifstream &f)
{
	initNewPsp();
	module = Loader::loadModule(pspFileName,f, 0x08800000);
	if((module->fileFormat & FormatELF) != FormatELF)
	{
		log.error("File Format Not Supported!\n");
	}
	moduleLoaded = true;
    initCpu();
    return module;
}

void Emulator::initCpu() {
        pspRegs.pc = module->entry_addr; //set the pc register.
        pspRegs.npc = pspRegs.pc + 4;
        pspRegs.GPR[27] = 0; //k1 should probably be 0
        pspRegs.GPR[28] = module->gp_value; //gp reg    gp register should get the GlobalPointer!!!
		ThreadMan::InitialiseThreadman(pspRegs.pc,module->attribute,module->pspFileName.c_str());//for some reason string hits memstack so we call it with const char*
		pspge::Initialise();
		//TODO remove the following cause is fake and only for MINIFIRE!!!!!!!!!!!!!
		//pspRegs.GPR[29]=0x09FFC000;
		//pspRegs.pc=0x08900050; //fake the threadman :)
		

 }
void Emulator::initNewPsp()
{
	moduleLoaded = false;
	Allegrex::cpuReset();
	Memory::reset();
	NIDMapper::initialize();
	Loader::reset();
	ModuleManager::reset();//TODO reset the whole manager system
	HLEModuleManager::Initialise(150);//fw 1.5 for now!
	pspSysMem::initialise();
	
}


  
