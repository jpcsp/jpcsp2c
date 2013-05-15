
#include "Common.h"
#include "Memory.h"
#include "Allegrex/Allegrex.h"
#include "psputils.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



typedef struct SceMt19937Context {
  unsigned int count;
  unsigned int state[624];
} SceMt19937Context;

void psputils::sceKernelUtilsMt19937Init(u32 ctx_addr, u32 seed)
{
  		srand(pspRegs.GPR[4]);
		pspRegs.GPR[2] = 0;		
		
}
void psputils::sceKernelUtilsMt19937UInt(u32 ctx_addr)
{
       pspRegs.GPR[2] = (u32)rand() * (u32)rand();
}