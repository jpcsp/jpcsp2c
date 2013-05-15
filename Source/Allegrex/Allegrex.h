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
#pragma once
namespace Allegrex{
  int  cpuIntialaze();
  void cpuReset();
}
#pragma once
namespace Allegrex{
  int  cpuIntialaze();
  void cpuReset();
}
typedef struct {
	u32 GPR[34];		/* General Purpose Registers */ /* Lo, Hi in r[32] and r[33] */
	u32 COP0[32];		/* Coprocessor0 Registers */
	float FPR[32];      /* FPU registers */
    u32 FPRc0;
	u32 FPRc31;
	u32 pc;				/* Program counter */
    u32 code;			/* The instruction */
	u32 cycle;
	u32 npc;
} pspRegisters;

extern pspRegisters pspRegs;

typedef struct {
	int  (*Init)();
	void (*Reset)();
	void (*Execute)();		/* executes up to a break */
	void (*ExecuteBlock)();	/* executes up to a jump */
	void (*Clear)(u32 Addr, u32 Size);
	void (*Shutdown)();
} AllegrexCPU;

extern AllegrexCPU *pspCpu;

extern AllegrexCPU pspInterpreter;
//extern AllegrexCPU pspRecompiler;//TODO

#define DECODE_RD     ((pspRegs.code >> 11) & 0x1F) 
#define DECODE_RT     ((pspRegs.code >> 16) & 0x1F) 
#define DECODE_RS     ((pspRegs.code >> 21) & 0x1F) 
#define DECODE_FS           (DECODE_RD)
#define DECODE_FT           (DECODE_RT)
#define DECODE_FD           (DECODE_SA)
#define DECODE_IMMED   (pspRegs.code & 0xFFFF)  
#define DECODE_SA      ((pspRegs.code >>  6) & 0x1F)
#define DECODE_FS      (DECODE_RD)
#define DECODE_SYSCALL      ((opcode_addr & 0x03FFFFFF) >> 6)
#define DECODE_FCOND   (pspRegs.code & 0xF)  

#define DECODE_C0BC         ((pspRegs.code >> 16) & 0x03)
#define DECODE_C1BC         ((pspRegs.code >> 16) & 0x03)

//for interpreter (and probably rec use)
#define _Rd_ DECODE_RD
#define _Rt_ DECODE_RT 
#define _Rs_ DECODE_RS
#define _Sa_ DECODE_SA
#define _Fd_ DECODE_FD
#define _Ft_ DECODE_FT 
#define _Fs_ DECODE_FS


#define _rRs_   pspRegs.GPR[DECODE_RS]   // Rs register
#define _rRt_   pspRegs.GPR[DECODE_RT]   // Rt register
#define _rRd_   pspRegs.GPR[DECODE_RD]   // Rd register

#define _rFs_   pspRegs.FPR[DECODE_FS]   // Fs register
#define _rFt_   pspRegs.FPR[DECODE_FT]   // Ft register
#define _rFd_   pspRegs.FPR[DECODE_FD]   // Fd register

#define _rHi_   pspRegs.GPR[33]   // The HI register
#define _rLo_   pspRegs.GPR[32]  // The LO register
#define _rRa_   pspRegs.GPR[31]

#define _Imm_	 ((s16)pspRegs.code)
#define _ImmU_	 (pspRegs.code&0xffff)
#define _Target_ (pspRegs.code & 0x03ffffff) 

#define _rFC0_   pspRegs.FPRc0
#define _rFC31_  pspRegs.FPRc31




