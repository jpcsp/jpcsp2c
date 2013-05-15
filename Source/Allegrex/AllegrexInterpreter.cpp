/*
 * This file is part of pcsp.
 *
 * pcsp is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * pcsp is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY{assert(0);} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with pcsp.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "Common.h"
#include "Allegrex.h"
#include "AllegrexDecoder.h"
#include "AllegrexInterpreter.h"
#include "Memory.h"
#include "HLE/SyscallHandler.h"


#if defined(_M_IX86)
#include <intrin.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#pragma intrinsic(__emul)
#pragma intrinsic(__emulu)
#define __MULT(x, y) { *((__int64 *)&pspRegs.GPR[32]) = __emul(x, y); } 
#define __MULTU(x, y) { *((unsigned __int64 *)&pspRegs.GPR[32]) = __emulu(x, y); }
#define __MADD(x, y) { *((__int64 *)&pspRegs.GPR[32]) += __emul(x, y); } 
#define __MADDU(x, y) { *((unsigned __int64 *)&pspRegs.GPR[32]) += __emulu(x, y); }
#define __MSUB(x, y) { *((__int64 *)&pspRegs.GPR[32]) -= __emul(x, y); } 
#define __MSUBU(x, y) { *((unsigned __int64 *)&pspRegs.GPR[32]) -= __emulu(x, y); }
static __inline u32 __rotr(u32 rt, u32 sa) { return _lrotr(rt, sa); }
static __inline u32 __clz(u32 rt) { return rt ? __lzcnt(rt) : 32; }
static __inline u32 __clo(u32 rt) { return __clz(~rt); }
static __inline u32 __wsbw(u32 rt) { return _byteswap_ulong(rt); }
static __inline u32 __wsbh(u32 rt) { return  _lrotr(__wsbw(rt), 16); }
static __inline u32 __bitrev(u32 rt)
{
	rt = ((rt >> 1) & 0x55555555) | ((rt & 0x55555555) << 1);
	rt = ((rt >> 2) & 0x33333333) | ((rt & 0x33333333) << 2);
	rt = ((rt >> 4) & 0x0F0F0F0F) | ((rt & 0x0F0F0F0F) << 4);
	rt = ((rt >> 8) & 0x00FF00FF) | ((rt & 0x00FF00FF) << 8);
	rt = ( rt >> 16             ) | ( rt               << 16);
	return rt;
}
#else
#error need implementation
#endif

#include <assert.h>
Log IntAllegrex::log;

void IntAllegrex::Step()
{
   pspRegs.npc=pspRegs.pc+4;
   pspRegs.code=Memory::read32(pspRegs.pc);
   pspRegs.pc=pspRegs.npc;
   ++pspRegs.cycle;
   const Allegrex::OPCODE& opcode = Allegrex::GetCurrentInstruction();
   opcode.interpret();
}
void IntAllegrex::DelaySlot()
{
  pspRegs.code=Memory::read32(pspRegs.pc);
  pspRegs.pc+=4;
  ++pspRegs.cycle;
  const Allegrex::OPCODE& opcode = Allegrex::GetCurrentInstruction();
  opcode.interpret();
  pspRegs.pc=pspRegs.npc;
  pspRegs.npc=pspRegs.pc+4;
}



inline static u32 branch_target(u32 npc, s32 simm16)
{
	return npc + (simm16 << 2);
}

inline static u32 jump_target(u32 npc, u32 uimm26)
{
	return (npc & 0xf0000000) | (uimm26 << 2);
}

#define _GPR_(r) pspRegs.GPR[r]
#define _FPR_(r) pspRegs.FPR[r]

/*********************************************************
* Arithmetic with immediate operand                      *
* Format:  OP rt, rs, immediate                          *
*********************************************************/
void IntAllegrex::ADDI()  { if (!_Rt_) return; _rRt_ = u32(_rRs_) + _Imm_ ; }		// Rt = Rs + Im     (Exception on Integer Overflow)
void IntAllegrex::ADDIU() { if (!_Rt_) return; _rRt_ = u32(_rRs_) + _Imm_ ; }		// Rt = Rs + Im
void IntAllegrex::ANDI()  { if (!_Rt_) return; _rRt_ = u32(_rRs_) & _ImmU_; }       // Rt = Rs And Im
void IntAllegrex::ORI()   { if (!_Rt_) return; _rRt_ = u32(_rRs_) | _ImmU_; }		// Rt = Rs Or  Im
void IntAllegrex::SLTI()  { if (!_Rt_) return; _rRt_ = s32(_rRs_) < _Imm_; }		// Rt = Rs < Im		(Signed)
void IntAllegrex::SLTIU() { if (!_Rt_) return; _rRt_ = u32(_rRs_) < u32(_Imm_); }	// Rt = Rs < Im		(Unsigned)
void IntAllegrex::XORI()  { if (!_Rt_) return; _rRt_ = u32(_rRs_) ^ _ImmU_; }		// Rt = Rs Xor Im

/*********************************************************
* Register arithmetic                                    *
* Format:  OP rd, rs, rt                                 *
*********************************************************/
void IntAllegrex::ADD()	 { if (!_Rd_) return; _rRd_ = u32(_rRs_) +  u32(_rRt_); }	// Rd = Rs + Rt		(Exception on Integer Overflow)
void IntAllegrex::ADDU() { if (!_Rd_) return; _rRd_ = u32(_rRs_) +  u32(_rRt_); }	// Rd = Rs + Rt
void IntAllegrex::SUB()  { if (!_Rd_) return; _rRd_ = u32(_rRs_) -  u32(_rRt_); }	// Rd = Rs - Rt		(Exception on Integer Overflow)
void IntAllegrex::SUBU() { if (!_Rd_) return; _rRd_ = u32(_rRs_) -  u32(_rRt_); }	// Rd = Rs - Rt
void IntAllegrex::AND()  { if (!_Rd_) return; _rRd_ = u32(_rRs_) &  u32(_rRt_); }	// Rd = Rs And Rt
void IntAllegrex::OR()   { if (!_Rd_) return; _rRd_ = u32(_rRs_) |  u32(_rRt_); }	// Rd = Rs Or Rt
void IntAllegrex::NOR()  { if (!_Rd_) return; _rRd_ = ~(u32(_rRs_) |  u32(_rRt_)); }	// Rd = ~(Rs Or Rt)
void IntAllegrex::XOR()  { if (!_Rd_) return; _rRd_ = u32(_rRs_) ^  u32(_rRt_); }	// Rd = Rs Xor Rt
void IntAllegrex::SLT()  { if (!_Rd_) return; _rRd_ = s32(_rRs_) <  s32(_rRt_); }	// Rd = Rs < Rt		(Signed)
void IntAllegrex::SLTU() { if (!_Rd_) return; _rRd_ = u32(_rRs_) <  u32(_rRt_); }	// Rd = Rs < Rt		(Unsigned)
void IntAllegrex::MAX()  { if (!_Rd_) return; s32 x = _rRs_, y = _rRt_; _rRd_ = x >  y ? x : y; }	// Rd = Max(Rs, Rt)
void IntAllegrex::MIN()  { if (!_Rd_) return; s32 x = _rRs_, y = _rRt_; _rRd_ = x <  y ? x : y; }	// Rd = Min(Rs, Rt)

/*********************************************************
* Shift arithmetic with constant shift                   *
* Format:  OP rd, rt, sa                                 *
*********************************************************/
void IntAllegrex::SLL() // Rd = Rt << sa
{
	if (!_Rd_) return;
	u32(_rRd_ ) = u32(_rRt_) << _Sa_;
}
void IntAllegrex::SLLV() // Rd = Rt << Rs
{
	if (!_Rd_) return;
	u32(_rRd_) = u32(_rRt_) << (_rRs_ & 31);
}
void IntAllegrex::SRL() // Rd = Rt >> sa (logical)
{
	if (!_Rd_) return;
	u32(_rRd_) = u32(_rRt_) >> _Sa_;
}
void IntAllegrex::SRLV() // Rd = Rt >> Rs (logical)
{
	if (!_Rd_) return;
	u32(_rRd_) = u32(_rRt_) >> (_rRs_ & 31);
}
void IntAllegrex::SRA() // Rd = Rt >> sa (arithmetic)
{
	if (!_Rd_) return;
	u32(_rRd_) = s32(_rRt_) >> _Sa_;
}
void IntAllegrex::SRAV() // Rd = Rt >> Rs (arithmetic)
{
	if (!_Rd_) return;
	u32(_rRd_) = s32(_rRt_) >> (_rRs_ & 31);
}
void IntAllegrex::ROTR()
{
	if (!_Rd_) return;
	u32 va = _rRt_;
	u32 sa = _Sa_;
	u32(_rRd_) = __rotr(va, sa); //(va >> sa) | (va << (32 - sa));
}
void IntAllegrex::ROTRV()
{
	if (!_Rd_) return;
	u32 va = _rRt_;
	u32 sa = _rRs_ & 31;
	u32(_rRd_) = __rotr(va, sa); //(va >> sa) | (va << (32 - sa));
}
/*********************************************************
* Bit manipulation                                       *
* Format:  OP rd, rt, msb, lsb                           *
*********************************************************/
void IntAllegrex::EXT()
{
    if (!_Rt_) return;
    _rRt_ = (_rRs_ >> _Sa_) & ~(~0 << (_Rd_ + 1));
}
void IntAllegrex::INS()
{
	if (!_Rt_) return;
    u32 mask = ~(~0 << (_Rd_ - _Sa_ + 1)) << _Sa_;
    _rRt_ = (_rRt_ & ~mask) | ((_rRs_ << _Sa_) & mask);
}
void IntAllegrex::SEH()
{
	if (!_Rd_) return;
	_rRd_ = s32(s16(_rRt_));
}
void IntAllegrex::BITREV()
{
	if (!_Rd_) return;
	_rRd_ = __bitrev(_rRt_);
}
void IntAllegrex::WSBH()
{
	if (!_Rd_) return;
	_rRd_ = __wsbh(_rRt_);
}
void IntAllegrex::WSBW()
{
	if (!_Rd_) return;
	_rRd_ = __wsbw(_rRt_);
}
void IntAllegrex::SEB()
{
	if (!_Rd_) return;
	_rRd_ = s32(s8(_rRt_));
}
void IntAllegrex::CLZ()
{
	if (!_Rd_) return;
	_rRd_ = __clz(_rRt_);
}
void IntAllegrex::CLO()
{
	if (!_Rd_) return;
	_rRd_ = __clo(_rRt_);
}


/*********************************************************
* Move from HI/LO to GPR                                 *
* Format:  OP rd                                         *
*********************************************************/
void IntAllegrex::MFHI() { if (!_Rd_) return; _rRd_ = _rHi_; } // Rd = Hi
void IntAllegrex::MFLO() { if (!_Rd_) return; _rRd_ = _rLo_; } // Rd = Lo

/*********************************************************
* Move to GPR to HI/LO & Register jump                   *
* Format:  OP rs                                         *
*********************************************************/
void IntAllegrex::MTHI() { _rHi_ = _rRs_; } // Hi = Rs
void IntAllegrex::MTLO() { _rLo_ = _rRs_; } // Lo = Rs

/*********************************************************
* Register mult/div & Register trap logic                *
* Format:  OP rs, rt                                     *
*********************************************************/

void IntAllegrex::DIV()
{
    s32 hi, lo, x, y = _rRt_;
    if (y != 0)
    {
        x = _rRs_;
        lo = x / y;
        hi = x % y;
        _rLo_ = lo;
        _rHi_ = hi;
    }
}
void IntAllegrex::DIVU()
{
    u32 hi, lo, x, y = _rRt_;
    if (y != 0)
    {
        x = _rRs_;
        lo = x / y;
        hi = x % y;
        _rLo_ = lo;
        _rHi_ = hi;
    }
}
void IntAllegrex::MULT()
{
	__MULT(_rRs_, _rRt_);
}
void IntAllegrex::MULTU()
{
	__MULTU(_rRs_, _rRt_);
}
void IntAllegrex::MADD()
{
	__MADD(_rRs_, _rRt_);
}
void IntAllegrex::MADDU()
{
	__MADDU(_rRs_, _rRt_);
}
void IntAllegrex::MSUB()
{
	__MSUB(_rRs_, _rRt_);
}
void IntAllegrex::MSUBU()
{
	__MSUBU(_rRs_, _rRt_);
}

/*********************************************************
* Load higher 16 bits of the first word in GPR with imm  *
* Format:  OP rt, immediate                              *
*********************************************************/
void IntAllegrex::LUI() { if (!_Rt_) return; u32(_rRt_) = pspRegs.code << 16; } // Upper halfword of Rt = Im

/*********************************************************
* Register branch logic                                  *
* Format:  OP rs, offset                                 *
*********************************************************/
void IntAllegrex::BGEZ()
{
        pspRegs.npc = (s32(_rRs_) >= 0) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
        DelaySlot();
}
void IntAllegrex::BGEZL()
{
    if (s32(_rRs_) >= 0)
    {
        pspRegs.npc = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
void IntAllegrex::BGTZ ()
{
        pspRegs.npc = (s32(_rRs_) > 0) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
        DelaySlot();
}
void IntAllegrex::BGTZL ()
{
    if (s32(_rRs_) > 0)
    {
        pspRegs.npc = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
/*********************************************************
* Register branch logic                                  *
* Format:  OP rs, rt, offset                             *
*********************************************************/
void IntAllegrex::BNE()
{
   pspRegs.npc = (_rRs_ != _rRt_) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
   DelaySlot();
}
void IntAllegrex::BLEZ()
{
   pspRegs.npc = (s32(_rRs_) <= 0) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
   DelaySlot();
}
void IntAllegrex::BLEZL()
{
    if (s32(_rRs_) <= 0)
    {
        pspRegs.npc = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
void IntAllegrex::BLTZ()
{
   pspRegs.npc = (s32(_rRs_) < 0) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
   DelaySlot();
}
void IntAllegrex::BLTZL()
{
    if (s32(_rRs_) < 0)
    {
        pspRegs.npc = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
void IntAllegrex::BNEL()
{
    if (_rRs_ != _rRt_)
    {
        pspRegs.npc = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
void IntAllegrex::BEQ()
{
    pspRegs.npc = (_rRs_ == _rRt_) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
    if (pspRegs.npc == pspRegs.pc - 4 && _Rs_ == _Rt_)
    {
        assert(0);//self jump..
    }
    DelaySlot();
}
void IntAllegrex::BEQL()
{       
    if (_rRs_ == _rRt_)
    {
         pspRegs.npc  = branch_target(pspRegs.pc, _Imm_);
         DelaySlot();
    }
    else
    {
       pspRegs.pc += 4;
    }
}
/*********************************************************
* Jump to target                                         *
* Format:  OP target                                     *
*********************************************************/
void IntAllegrex::J()
{
    pspRegs.npc = jump_target(pspRegs.pc, _Target_);
    if (pspRegs.npc == pspRegs.pc - 4)
    {
        assert(0);//self jump..
    }
    DelaySlot();
}
void IntAllegrex::JAL()
{
    _rRa_ = pspRegs.pc + 4;
    pspRegs.npc= jump_target(pspRegs.pc, _Target_);
    DelaySlot();
}
void IntAllegrex::JR()
{
    pspRegs.npc = _rRs_;
    DelaySlot();

}
void IntAllegrex::JALR()
{
        if (_Rd_ != 0)
        {
           _rRd_ = pspRegs.pc + 4;
        }
        pspRegs.npc = _rRs_;
        DelaySlot();
}

void IntAllegrex::BC1F()
{
    pspRegs.npc = ((_rFC31_ & (1<<23)) == 0) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
    DelaySlot();
}
void IntAllegrex::BC1T()
{
    pspRegs.npc = ((_rFC31_ & (1<<23)) != 0) ? branch_target(pspRegs.pc, _Imm_) : (pspRegs.pc + 4);
    DelaySlot();
}
void IntAllegrex::BC1FL()
{       
    if ((_rFC31_ & (1<<23)) == 0)
    {
        pspRegs.npc  = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
void IntAllegrex::BC1TL()
{       
    if ((_rFC31_ & (1<<23)) != 0)
    {
        pspRegs.npc  = branch_target(pspRegs.pc, _Imm_);
        DelaySlot();
    }
    else
    {
        pspRegs.pc += 4;
    }
}
/*********************************************************
* Load and store for GPR                                 *
* Format:  OP rt, offset(base)                           *
*********************************************************/
void IntAllegrex::LW()
{
    if (_Rt_)
    {
        u32(_rRt_) = Memory::read32(u32(_rRs_) + _Imm_);
    }
    else
    {
        Memory::read32(u32(_rRs_) + _Imm_);
    }
}
void IntAllegrex::LB()
{
    if (_Rt_)
    {
        _rRt_ = (s8)Memory::read8(u32(_rRs_) + _Imm_);
    }
    else
    {
        Memory::read8(u32(_rRs_) + _Imm_);
    }
}
void IntAllegrex::LBU()
{
    if (_Rt_)
    {
        u32(_rRt_) = Memory::read8(u32(_rRs_) + _Imm_);
    }
    else
    {
        Memory::read8(u32(_rRs_) + _Imm_);
    }
}
void IntAllegrex::LH()
{
    if (_Rt_) {
        _rRt_ = (s16)Memory::read16(u32(_rRs_) + _Imm_);
    }
    else
    {
        Memory::read16(u32(_rRs_) + _Imm_);
    }
}
void IntAllegrex::LHU()
{
    if (_Rt_)
    {
        u32(_rRt_) = Memory::read16(u32(_rRs_) + _Imm_);
    }
    else
    {
        Memory::read16(u32(_rRs_) + _Imm_);
    }
}

void IntAllegrex::LWL()
{

    static u32 const lwl_mask[4] = { 0xffffff, 0xffff, 0xff, 0 };
    static u32 const lwl_shift[4] = { 24, 16, 8, 0 };
    u32 rt = _Rt_;
    u32 &value = pspRegs.GPR[rt];
    u32 address = _rRs_ + _Imm_;
    u32 offset = address & 0x3;
    if (rt)
    {
        value = (Memory::read32(address & 0xfffffffc) << lwl_shift[offset]) | (value & lwl_mask[offset]);
    }
}

void IntAllegrex::LWR()
{

    static u32 const lwr_mask[4] = { 0, 0xff000000, 0xffff0000, 0xffffff00 };
    static u32 const lwr_shift[4] = { 0, 8, 16, 24 };
    u32 rt = _Rt_;
    u32 &value = pspRegs.GPR[rt];
    u32 address = _rRs_ + _Imm_;
    u32 offset = address & 0x3;
    if (rt)
    {
        value = (Memory::read32(address & 0xfffffffc) >> lwr_shift[offset]) | (value & lwr_mask[offset]);
    }
}

void IntAllegrex::LWC1()
{
    if (_Rt_)
    {
        *((u32 *)&_rFt_) = Memory::read32(u32(_rRs_) + _Imm_);
    }
    else
    {
        Memory::read32(u32(_rRs_) + _Imm_);
    }
}

void IntAllegrex::SB(){ Memory::write8 (u32(_rRs_) + _Imm_, u8(_rRt_));}
void IntAllegrex::SH(){ Memory::write16(u32(_rRs_) + _Imm_, u16(_rRt_));}
void IntAllegrex::SW(){ Memory::write32(u32(_rRs_) + _Imm_, u32(_rRt_));}

void IntAllegrex::SWL()
{

    static u32 const swl_mask[4] = { 0xffffff00, 0xffff0000, 0xff000000, 0 };
    static u32 const swl_shift[4] = { 24, 16, 8, 0 };
    u32 address = _rRs_ + _Imm_;
    u32 offset = address & 0x3;
    address &= 0xfffffffc;
    Memory::write32(address, (_rRt_ >> swl_shift[offset]) | (Memory::read32(address) & swl_mask[offset]));
}
void IntAllegrex::SWR()
{

    static u32 const swl_mask[4] = { 0, 0xff, 0xffff, 0xffffff };
    static u32 const swl_shift[4] = { 0, 8, 16, 24 };
    u32 address = _rRs_ + _Imm_;
    u32 offset = address & 0x3;
    address &= 0xfffffffc;
    Memory::write32(address, (_rRt_ << swl_shift[offset]) | (Memory::read32(address) & swl_mask[offset]));
}

void IntAllegrex::SWC1() { Memory::write32(u32(_rRs_) + _Imm_, *((u32 *)&_rFt_));}

/*********************************************************
* Conditional move                                       *
* Format:  OP rd, rs, rt                                 *
*********************************************************/
void IntAllegrex::MOVZ(){ if (_Rd_ && !_rRt_) _rRd_ = _rRs_; } // if(Rt == 0) Rd = Rs
void IntAllegrex::MOVN(){ if (_Rd_ &&  _rRt_) _rRd_ = _rRs_; } // if(Rt != 0) Rd = Rs


/*********************************************************
* Special Allegrex instructions.                         *
* Format:                                                *
*********************************************************/
void IntAllegrex::MFIC(){}
void IntAllegrex::MTIC(){}
void IntAllegrex::HALT(){}


void IntAllegrex::SYSCALL()
{
	SyscallHandler::syscall((pspRegs.code >> 6) & 0x000FFFFF);
}
///////////////////////////////////////////

static int intInit() {
	IntAllegrex::log = Log("Interpreter");
	return 0;
}

static void intReset() {
}

static void intExecute() {
   IntAllegrex::Step();
}

static void intExecuteBlock() {

}

static void intClear(u32 Addr, u32 Size) {
}

static void intShutdown() {
}

AllegrexCPU pspInterpreter =
{
    intInit,
    intReset,
    intExecute,
    intExecuteBlock,
    intClear,
    intShutdown
};

/*TODO opcodes */
void IntAllegrex::Unknown()
{
    IntAllegrex::log.fatal("Unknown opcode called!");
}

void IntAllegrex::COP0(){IntAllegrex::log.fatal("COP0 Unsupported");}
void IntAllegrex::COP1(){IntAllegrex::log.fatal("COP1 Unsupported");}
void IntAllegrex::LL(){IntAllegrex::log.fatal("LL Unsupported");}
void IntAllegrex::LVS(){IntAllegrex::log.fatal("LVS Unsupported");}
void IntAllegrex::LVQ(){IntAllegrex::log.fatal("LVQ Unsupported");}
void IntAllegrex::SC(){IntAllegrex::log.fatal("SC Unsupported");}
void IntAllegrex::SVS(){IntAllegrex::log.fatal("SVS Unsupported");}
void IntAllegrex::BREAK(){IntAllegrex::log.fatal("BREAK Unsupported");}
void IntAllegrex::SYNC(){IntAllegrex::log.fatal("SYNC Unsupported");}
void IntAllegrex::BLTZAL(){IntAllegrex::log.fatal("BLTZAL Unsupported");}
void IntAllegrex::BGEZAL(){IntAllegrex::log.fatal("BGEZAL Unsupported");}
void IntAllegrex::BLTZALL(){IntAllegrex::log.fatal("BLTZALL Unsupported");}
void IntAllegrex::BGEZALL(){IntAllegrex::log.fatal("BGEZALL Unsupported");}
void IntAllegrex::MFC0(){assert(0);}
void IntAllegrex::ERET(){assert(0);}
void IntAllegrex::CFC0(){assert(0);}
void IntAllegrex::MTC0(){assert(0);}
void IntAllegrex::CTC0(){assert(0);}		  


void IntAllegrex::MFC1()
{
    _rRt_ = *((u32 *)&_rFs_);
}
void IntAllegrex::CFC1()
{
    if (_Rt_ != 0)
    {
        switch (_Fs_)
        {
        case 0:
            _rRt_ = _rFC0_;
            break;

        case 31:
            _rRt_ = _rFC31_;
            break;

        default:
            IntAllegrex::log.fatal("CFC1 Unsupported for fcr%d", _Fs_);
        }
    }
}
void IntAllegrex::MTC1()
{
    *((u32 *)&_rFs_) = _rRt_;
}
void IntAllegrex::CTC1()
{
    switch (_Fs_)
    {
    case 31:
        _rFC31_ = _rRt_ & 0x01800003;
        break;

    default:
        IntAllegrex::log.fatal("CTC1 Unsupported for fcr%d", _Fs_);
    }
}

void IntAllegrex::CVT_S_W() { _rFd_ = float(*((s32 *)&_rFs_)); }

void IntAllegrex::ADD_S() { _rFd_ = _rFs_ + _rFt_; }
void IntAllegrex::SUB_S() { _rFd_ = _rFs_ - _rFt_; }
void IntAllegrex::MUL_S() { _rFd_ = _rFs_ * _rFt_; }   
void IntAllegrex::DIV_S() { _rFd_ = _rFs_ / _rFt_; }
void IntAllegrex::SQRT_S() { _rFd_ = sqrtf(_rFs_); }
void IntAllegrex::CVT_W_S()
{
    switch (_rFC31_ & 3)
    {
    case 1:
        *((s32 *)&_rFd_) = s32(_rFs_);
        break;
    case 2:
        *((s32 *)&_rFd_) = s32(ceilf(_rFs_));
        break;
    case 3:
        *((s32 *)&_rFd_) = s32(floorf(_rFs_));
        break;
    default:
        *((s32 *)&_rFd_) = s32(_rFs_ + 0.5);
        break;
    }
}
void IntAllegrex::ABS_S() { _rFd_ = fabsf(_rFs_); }
void IntAllegrex::MOV_S() { _rFd_ = _rFs_; }    
void IntAllegrex::NEG_S() { _rFd_ = 0.0f - _rFs_; }
void IntAllegrex::ROUND_W_S() { *((s32 *)&_rFd_) = s32(_rFs_ + 0.5); }
void IntAllegrex::TRUNC_W_S() { *((s32 *)&_rFd_) = s32(_rFs_); }
void IntAllegrex::CEIL_W_S() { *((s32 *)&_rFd_) = s32(ceilf(_rFs_)); }
void IntAllegrex::FLOOR_W_S() { *((s32 *)&_rFd_) = s32(floorf(_rFs_)); } 

void IntAllegrex::C_COND_S()
{
    int cond = DECODE_FCOND;
    float x = _rFs_;
    float y = _rFt_;
    bool unordered = ((cond & 1) != 0) && (_isnan(x) || _isnan(y));

    if (unordered)
    {
        _rFC31_ |= 1<<23;
    }
    else
    {
        bool equal = ((cond & 2) != 0) && (x == y);
        bool less = ((cond & 4) != 0) && (x < y);

        _rFC31_ |= (less || equal)<<23;
    }
}

void IntAllegrex::COP1_Unknown() { assert(0); }

void IntAllegrex::BVF(){assert(0);}
void IntAllegrex::BVT(){assert(0);}
void IntAllegrex::BVFL(){assert(0);}
void IntAllegrex::BVTL(){assert(0);}
void IntAllegrex::MFV(){assert(0);}
void IntAllegrex::MTV(){assert(0);}
void IntAllegrex::MFVC(){assert(0);}
void IntAllegrex::MTVC(){assert(0);}
void IntAllegrex::VADD(){assert(0);}
void IntAllegrex::VSUB(){assert(0);}
void IntAllegrex::VSBN(){assert(0);}
void IntAllegrex::VDIV(){assert(0);}
void IntAllegrex::VMUL(){assert(0);}
void IntAllegrex::VDOT(){assert(0);}
void IntAllegrex::VSCL(){assert(0);}
void IntAllegrex::VFPU_Unknown(){assert(0);}
void IntAllegrex::VHDP(){assert(0);}
void IntAllegrex::VDET(){assert(0);}
void IntAllegrex::VCRS(){assert(0);}
void IntAllegrex::VCMP(){assert(0);}
void IntAllegrex::VMIN(){assert(0);}
void IntAllegrex::VMAX(){assert(0);}
void IntAllegrex::VSLT(){assert(0);}
void IntAllegrex::VSCMP(){assert(0);}
void IntAllegrex::VSGE(){assert(0);}
void IntAllegrex::VMOV(){assert(0);}
void IntAllegrex::VNEG(){assert(0);}
void IntAllegrex::VABS(){assert(0);}
void IntAllegrex::VIDT(){assert(0);}
void IntAllegrex::VF2IN(){assert(0);}
void IntAllegrex::VI2F(){assert(0);}
void IntAllegrex::VWBN(){assert(0);}
void IntAllegrex::VSAT0(){assert(0);}
void IntAllegrex::VZERO(){assert(0);}
void IntAllegrex::VONE(){assert(0);}
void IntAllegrex::VSAT1(){assert(0);}
void IntAllegrex::VRCP(){assert(0);}
void IntAllegrex::VSIN(){assert(0);}
void IntAllegrex::VRSQ(){assert(0);}
void IntAllegrex::VCOS(){assert(0);}
void IntAllegrex::VEXP2(){assert(0);}
void IntAllegrex::VSQRT(){assert(0);}
void IntAllegrex::VLOG2(){assert(0);}
void IntAllegrex::VASIN(){assert(0);}
void IntAllegrex::VNRCP(){assert(0);}
void IntAllegrex::VNSIN(){assert(0);}
void IntAllegrex::VREXP2(){assert(0);}
void IntAllegrex::VRNDS(){assert(0);}
void IntAllegrex::VRNDF1(){assert(0);}
void IntAllegrex::VRNDI(){assert(0);}
void IntAllegrex::VRNDF2(){assert(0);}
void IntAllegrex::VCMOVT(){assert(0);}
void IntAllegrex::VF2IZ(){assert(0);}
void IntAllegrex::VCMOVF(){assert(0);}
void IntAllegrex::VF2H(){assert(0);}
void IntAllegrex::VH2F(){assert(0);}
void IntAllegrex::VSBZ(){assert(0);}
void IntAllegrex::VLGB(){assert(0);}
void IntAllegrex::VUC2I(){assert(0);}
void IntAllegrex::VC2I(){assert(0);}
void IntAllegrex::VI2UC(){assert(0);}
void IntAllegrex::VI2US(){assert(0);}
void IntAllegrex::VI2C(){assert(0);}
void IntAllegrex::VI2S(){assert(0);}
void IntAllegrex::VSRT1(){assert(0);}
void IntAllegrex::VBFY1(){assert(0);}
void IntAllegrex::VSRT2(){assert(0);}
void IntAllegrex::VBFY2(){assert(0);}
void IntAllegrex::VF2IU(){assert(0);}
void IntAllegrex::VOCP(){assert(0);}
void IntAllegrex::VFAD(){assert(0);}
void IntAllegrex::VSOCP(){assert(0);}
void IntAllegrex::VAVG(){assert(0);}
void IntAllegrex::VSRT3(){assert(0);}
void IntAllegrex::VSRT4(){assert(0);}
void IntAllegrex::VUS2I(){assert(0);}
void IntAllegrex::VS2I(){assert(0);}
void IntAllegrex::VMFVC(){assert(0);}
void IntAllegrex::VMTVC(){assert(0);}
void IntAllegrex::VT4444(){assert(0);}
void IntAllegrex::VT5650(){assert(0);}
void IntAllegrex::VT5551(){assert(0);}
void IntAllegrex::VCST(){assert(0);}
void IntAllegrex::VF2ID(){assert(0);}
void IntAllegrex::VIIM(){assert(0);}
void IntAllegrex::VFIM(){assert(0);}
void IntAllegrex::VPFXS(){assert(0);}
void IntAllegrex::VPFXT(){assert(0);}
void IntAllegrex::VPFXD(){assert(0);}
void IntAllegrex::LVLQ(){assert(0);} 
void IntAllegrex::LVRQ(){assert(0);}
void IntAllegrex::VHTFM2(){assert(0);}
void IntAllegrex::VTFM2(){assert(0);}
void IntAllegrex::VTFM3(){assert(0);}
void IntAllegrex::VHTFM3(){assert(0);}
void IntAllegrex::VHTFM4(){assert(0);}
void IntAllegrex::VTFM4(){assert(0);}
void IntAllegrex::VMMOV(){assert(0);}
void IntAllegrex::VMZERO(){assert(0);}
void IntAllegrex::VMIDT(){assert(0);}
void IntAllegrex::VMONE(){assert(0);}
void IntAllegrex::VROT(){assert(0);}
void IntAllegrex::VMMUL(){assert(0);}
void IntAllegrex::VMSCL(){assert(0);}
void IntAllegrex::VQMUL(){assert(0);}
void IntAllegrex::SVLQ(){assert(0);}
void IntAllegrex::SVRQ(){assert(0);}
void IntAllegrex::SVQ(){assert(0);}
void IntAllegrex::SWB(){assert(0);}
void IntAllegrex::VNOP(){assert(0);}
void IntAllegrex::VSYNC(){assert(0);}
void IntAllegrex::VFLUSH(){assert(0);}
void IntAllegrex::ICACHE_FILL(){assert(0);}
void IntAllegrex::ICACHE_FILL_WITH_LOCK(){assert(0);}
void IntAllegrex::DCACHE_CREATE_DIRTY_EXCLUSIVE(){assert(0);}
void IntAllegrex::DCACHE_HIT_INVALIDATE(){assert(0);}
void IntAllegrex::DCACHE_HIT_WRITEBACK(){assert(0);}
void IntAllegrex::DCACHE_HIT_WRITEBACK_INVALIDATE(){assert(0);}
void IntAllegrex::CACHE_Unknown(){assert(0);}
void IntAllegrex::ICACHE_INDEX_INVALIDATE(){assert(0);}
void IntAllegrex::ICACHE_INDEX_UNLOCK(){assert(0);}
void IntAllegrex::DCACHE_FILL(){assert(0);}
void IntAllegrex::DCACHE_FILL_WITH_LOCK(){assert(0);}
void IntAllegrex::ICACHE_HIT_INVALIDATE(){assert(0);}
void IntAllegrex::DCACHE_INDEX_WRITEBACK_INVALIDATE(){assert(0);}
void IntAllegrex::DCACHE_INDEX_UNLOCK(){assert(0);}
void IntAllegrex::DCACHE_CREATE_DIRTY_EXCLUSIVE_WITH_LOCK(){assert(0);}