
#include "Common.h"
#include "DisAllegrex.h"
#include "Allegrex.h"
#include "AllegrexDecoder.h"
#include "syscallsFirm15.h"
#include "Memory.h"

char *GPR_REG[32] = {
    "zero", "at", "v0", "v1", "a0", "a1", "a2", "a3",
    "t0", "t1", "t2", "t3", "t4", "t5", "t6", "t7",
    "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
    "t8", "t9", "k0", "k1", "gp", "sp", "fp", "ra"
};
char *COP0_REG[32] ={
        "Index","Random","EntryLo0","EntryLo1","Context","PageMask",
        "Wired","C0r7","BadVaddr","Count","EntryHi","Compare","Status",
        "Cause","EPC","PRId","Config","C0r17","C0r18","C0r19","C0r20",
        "C0r21","C0r22","C0r23","Debug","Perf","C0r26","C0r27","TagLo",
        "TagHi","ErrorPC","C0r31"
};
//floating point cop1 Floating point reg
char *COP1_REG_FP[32] ={
        "f00","f01","f02","f03","f04","f05","f06","f07",
        "f08","f09","f10","f11","f12","f13","f14","f15",
        "f16","f17","f18","f19","f20","f21","f21","f23",
        "f24","f25","f26","f27","f28","f29","f30","f31"
};
//floating point cop1 control registers
char *COP1_REG_FCR[32] ={
        "fcr00","fcr01","fcr02","fcr03","fcr04","fcr05","fcr06","fcr07",
        "fcr08","fcr09","fcr10","fcr11","fcr12","fcr13","fcr14","fcr15",
        "fcr16","fcr17","fcr18","fcr19","fcr20","fcr21","fcr21","fcr23",
        "fcr24","fcr25","fcr26","fcr27","fcr28","fcr29","fcr30","fcr31"
};

static char dbuf2[1024];
static char obuf2[1024];
unsigned long opcode_addr;


char* DisAllegrex::disAllegrex(u32 code, u32 pc) {
        u32 scode = pspRegs.code;
        opcode_addr = pc;
        pspRegs.code = code;
		const Allegrex::OPCODE& opcode = Allegrex::GetCurrentInstruction();
		opcode.disasm(dbuf2);
        sprintf(obuf2, "%s", dbuf2);
        pspRegs.code = scode;
        return obuf2;
}
char* DisAllegrex::jump_decode(void)
{
    static char buf[256];
    unsigned long addr;
    addr = (opcode_addr & 0xf0000000)|((pspRegs.code&0x3ffffff)<<2);
    sprintf(buf, "0x%08lX", addr);
    return buf;
}
char *DisAllegrex::offset_decode(void)
{
    static char buf[256];
    unsigned long addr;
    addr = ((((short)( pspRegs.code & 0xFFFF) * 4) + opcode_addr + 4));
    sprintf(buf, "0x%08lX", addr);
    return buf;
}

void DisAllegrex::Unknown( char *buf ){strcpy(buf, "????");}

void DisAllegrex::J(char *buf){sprintf(buf, "%-10s %s","j",                    jump_decode()); }
void DisAllegrex::JAL(char *buf){
	int addr = (opcode_addr & 0xf0000000)|((pspRegs.code&0x3ffffff)<<2);
	if(addr !=0)
	{
		int read=Memory::read32(addr+4);
		if(!Memory::isValid(read))
		{
          sprintf(buf, "%-10s %s","jal",                  jump_decode());
		}
		else
		{
		 if(read !=0)
		 {
            char *nextInstruction =disAllegrex(read,addr+4);
			string nextinst (nextInstruction);
			if(nextinst.find("syscall") ==string::npos) //we didn't find a syscall there return default..
			{
               sprintf(buf, "%-10s %s","jal",                  jump_decode());
			}
			else
			{
				int start =nextinst.find("[");
				int end = nextinst.find("]");
				string syscall = nextinst.substr(start,end);
				sprintf(buf, "%-10s %s %s","jal",                  jump_decode(),syscall.c_str());
			}
		 } 
		 else
		 {
           sprintf(buf, "%-10s %s","jal",                  jump_decode());
		 }
		}
	}
	
}
void DisAllegrex::BEQ(char *buf){  sprintf(buf, "%-10s %s, %s, %s","beq",          GPR_REG[DECODE_RS], GPR_REG[DECODE_RT], offset_decode()); } 
void DisAllegrex::BNE(char *buf){  sprintf(buf, "%-10s %s, %s, %s","bne",          GPR_REG[DECODE_RS], GPR_REG[DECODE_RT], offset_decode()); } 
void DisAllegrex::BLEZ(char *buf){ sprintf(buf, "%-10s %s, %s","blez",             GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BGTZ(char *buf){ sprintf(buf, "%-10s %s, %s","bgtz",             GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::ADDI(char *buf){ sprintf(buf, "%-10s %s, %s, 0x%04lX","addi",    GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED);}   
void DisAllegrex::ADDIU(char *buf){sprintf(buf, "%-10s %s, %s, 0x%04lX","addiu",   GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED);} 
void DisAllegrex::SLTI(char *buf){ sprintf(buf, "%-10s %s, %s, 0x%04lX","slti",    GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED);}
void DisAllegrex::SLTIU(char *buf){sprintf(buf, "%-10s %s, %s, 0x%04lX","sltiu",   GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED);}
void DisAllegrex::ANDI(char *buf){ sprintf(buf, "%-10s %s, %s, 0x%04lX","andi",    GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED);}
void DisAllegrex::ORI(char *buf){  sprintf(buf, "%-10s %s, %s, 0x%04lX","ori",     GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED); }
void DisAllegrex::XORI(char *buf){ sprintf(buf, "%-10s %s, %s, 0x%04lX","xori",    GPR_REG[DECODE_RT], GPR_REG[DECODE_RS], DECODE_IMMED); }
void DisAllegrex::LUI(char *buf){  sprintf(buf, "%-10s %s, 0x%04lX","lui",         GPR_REG[DECODE_RT], DECODE_IMMED); }
void DisAllegrex::BEQL(char *buf)   { sprintf(buf, "%-10s %s, %s, %s","beql",       GPR_REG[DECODE_RS], GPR_REG[DECODE_RT], offset_decode()); }
void DisAllegrex::BNEL(char *buf)   { sprintf(buf, "%-10s %s, %s, %s","bnel",       GPR_REG[DECODE_RS], GPR_REG[DECODE_RT], offset_decode()); }
void DisAllegrex::BLEZL(char *buf)  { sprintf(buf, "%-10s %s, %s","blezl",          GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BGTZL(char *buf)  { sprintf(buf, "%-10s %s, %s","bgtzl",          GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::LB(char *buf){sprintf(buf,  "%-10s %s, 0x%04lX(%s)","lb",     GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); }    
void DisAllegrex::LH(char *buf){sprintf(buf,  "%-10s %s, 0x%04lX(%s)","lh",     GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); }    
void DisAllegrex::LWL(char *buf){sprintf(buf, "%-10s %s, 0x%04lX(%s)","lwl",    GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); } 
void DisAllegrex::LW(char *buf){sprintf(buf,  "%-10s %s, 0x%04lX(%s)","lw",     GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); }   
void DisAllegrex::LBU(char *buf){sprintf(buf, "%-10s %s, 0x%04lX(%s)","lbu",    GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); } 
void DisAllegrex::LHU(char *buf){sprintf(buf, "%-10s %s, 0x%04lX(%s)","lhu",    GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); } 
void DisAllegrex::LWR(char *buf){sprintf(buf, "%-10s %s, 0x%04lX(%s)","lwr",    GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]);} 
void DisAllegrex::SB(char *buf){sprintf(buf,  "%-10s %s, 0x%04lX(%s)","sb",     GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]);}    
void DisAllegrex::SH(char *buf){sprintf(buf,  "%-10s %s, 0x%04lX(%s)","sh",     GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); }   
void DisAllegrex::SWL(char *buf){sprintf(buf, "%-10s %s, 0x%04lX(%s)","swl",    GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); } 
void DisAllegrex::SW(char *buf){sprintf(buf,  "%-10s %s, 0x%04lX(%s)","sw",     GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]); }  
void DisAllegrex::SWR(char *buf){sprintf(buf, "%-10s %s, 0x%04lX(%s)","swr",    GPR_REG[DECODE_RT], DECODE_IMMED, GPR_REG[DECODE_RS]);} 
/*TODO*/void DisAllegrex::LL(char *buf){strcpy(buf,"notsupported");}
void DisAllegrex::LWC1(char *buf){sprintf(buf, "%-10s %s, 0x%04X(%s)","lwc1",   COP1_REG_FP[DECODE_FT], DECODE_IMMED, GPR_REG[DECODE_RS]); }


/*TODO*/void DisAllegrex::SC(char *buf){strcpy(buf,"notsupported");}
void DisAllegrex::SWC1(char *buf){sprintf(buf, "%-10s %s, 0x%04X(%s)","swc1",   COP1_REG_FP[DECODE_FT], DECODE_IMMED, GPR_REG[DECODE_RS]); }


void DisAllegrex::SLL(char *buf)
{
   if (pspRegs.code == 0x00000000)
        strcpy(buf, "nop");
    else
		sprintf(buf, "%-10s %s, %s, 0x%02lX","sll", GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], DECODE_SA);

}
void DisAllegrex::SRL(char *buf){sprintf(buf, "%-10s %s, %s, 0x%02lX","srl", GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], DECODE_SA);}
void DisAllegrex::ROTR(char *buf){ sprintf(buf, "%-10s %s, %s, 0x%02lX","rotr", GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], DECODE_SA);}
void DisAllegrex::SRA(char *buf){sprintf(buf,  "%-10s %s, %s, 0x%02lX","sra", GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], DECODE_SA);}
void DisAllegrex::SLLV(char *buf){sprintf(buf, "%-10s %s, %s, %s","sllv",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], GPR_REG[DECODE_RS]); } 
void DisAllegrex::SRLV(char *buf){sprintf(buf, "%-10s %s, %s, %s","srlv",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], GPR_REG[DECODE_RS]);}
void DisAllegrex::ROTRV(char *buf){sprintf(buf, "%-10s %s, %s, %s","rotrv",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], GPR_REG[DECODE_RS]);}
void DisAllegrex::SRAV(char *buf){sprintf(buf, "%-10s %s, %s, %s","srav",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RT], GPR_REG[DECODE_RS]); }
void DisAllegrex::JR(char *buf){sprintf(buf, "%-10s %s","jr",               GPR_REG[DECODE_RS]);}  
void DisAllegrex::JALR(char *buf)
{
    int rd = DECODE_RD;

    if (rd == 31)
        sprintf(buf, "%-10s %s","jalr", GPR_REG[DECODE_RS]);
    else
        sprintf(buf, "%-10s %s, %s","jalr", GPR_REG[rd], GPR_REG[DECODE_RS]);
}
void DisAllegrex::MOVZ(char *buf){sprintf(buf, "%-10s %s, %s, %s","movz", GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);}
void DisAllegrex::MOVN(char *buf){sprintf(buf, "%-10s %s, %s, %s","movn", GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);}
void DisAllegrex::SYSCALL(char *buf)
{
	int decodesyscall =(pspRegs.code>>6)&1048575;
	for (int i = 0; i < sizeof(calls); i++)
	{
		if(calls[i].syscall ==decodesyscall)
		{
			sprintf(buf, "%-10s 0x%05x [%s]","syscall",decodesyscall,calls[i].name);
			break;
		}
		else
		{
           	sprintf(buf, "%-10s 0x%05x [unknown]","syscall",decodesyscall);
		}
			
	}

}
void DisAllegrex::BREAK(char *buf){strcpy(buf, "break");}
void DisAllegrex::SYNC(char *buf){strcpy(buf, "sync");}
void DisAllegrex::MFHI(char *buf){sprintf(buf, "%-10s %s","mfhi",          GPR_REG[DECODE_RD]); }
void DisAllegrex::MTHI(char *buf){sprintf(buf, "%-10s %s","mthi",          GPR_REG[DECODE_RS]); } 
void DisAllegrex::MFLO(char *buf){sprintf(buf, "%-10s %s","mflo",          GPR_REG[DECODE_RD]); }
void DisAllegrex::MTLO(char *buf){sprintf(buf, "%-10s %s","mtlo",          GPR_REG[DECODE_RS]); }
void DisAllegrex::CLZ(char *buf){	sprintf(buf, "%-10s %s, %s, ","clz",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RS]);}
void DisAllegrex::CLO(char *buf){sprintf(buf, "%-10s %s, %s, ","clo",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RS]);}
void DisAllegrex::MULT(char *buf)    { sprintf(buf, "%-10s %s, %s","mult",      GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);}
void DisAllegrex::MULTU(char *buf)   { sprintf(buf, "%-10s %s, %s","multu",     GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);}
void DisAllegrex::DIV(char *buf)     { sprintf(buf, "%-10s %s, %s","div",       GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);} 
void DisAllegrex::DIVU(char *buf)    { sprintf(buf, "%-10s %s, %s","divu",      GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]); }
void DisAllegrex::MADD(char *buf)    { sprintf(buf, "%-10s %s, %s, %s","madd",  GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::MADDU(char *buf)   { sprintf(buf, "%-10s %s, %s, %s","maddu", GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::ADD(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","add",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::ADDU(char *buf)    { sprintf(buf, "%-10s %s, %s, %s","addu",  GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::SUB(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","sub",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::SUBU(char *buf)    { sprintf(buf, "%-10s %s, %s, %s","subu",  GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::AND(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","and",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::OR(char *buf)      { sprintf(buf, "%-10s %s, %s, %s","or",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::XOR(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","xor",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::NOR(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","nor",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::SLT(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","slt",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::SLTU(char *buf)    { sprintf(buf, "%-10s %s, %s, %s","sltu",  GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::MAX(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","max",  GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]); }
void DisAllegrex::MIN(char *buf)     { sprintf(buf, "%-10s %s, %s, %s","min",  GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]); }
void DisAllegrex::MSUB(char *buf){     sprintf(buf, "%-10s %s, %s, %s","msub",    GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::MSUBU(char *buf){    sprintf(buf, "%-10s %s, %s, %s","msubu",   GPR_REG[DECODE_RD], GPR_REG[DECODE_RS], GPR_REG[DECODE_RT]);  }
void DisAllegrex::BLTZ(char *buf)    { sprintf(buf, "%-10s %s, %s","bltz",        GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BGEZ(char *buf)    { sprintf(buf, "%-10s %s, %s","bgez",        GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BLTZL(char *buf)   { sprintf(buf, "%-10s %s, %s","bltzl",       GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BGEZL(char *buf)   { sprintf(buf, "%-10s %s, %s","bgezl",       GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BLTZAL(char *buf)  { sprintf(buf, "%-10s %s, %s","bltzal",      GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BGEZAL(char *buf)  { sprintf(buf, "%-10s %s, %s","bgezal",      GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BLTZALL(char *buf){  sprintf(buf, "%-10s %s, %s","bltzall",     GPR_REG[DECODE_RS], offset_decode()); }
void DisAllegrex::BGEZALL(char *buf){  sprintf(buf, "%-10s %s, %s","bgezall",     GPR_REG[DECODE_RS], offset_decode()); }

/****************
SPECIAL 2 opcodes
*****************/
/*TODO*/void DisAllegrex::MFIC(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::MTIC(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::HALT(char *buf){strcpy(buf,"notsupported");}
/****************
SPECIAL 3 opcodes
*****************/
/*TODO*/void DisAllegrex::SEH(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::BITREV(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::WSBH(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::WSBW(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::EXT(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::INS(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::SEB(char *buf){strcpy(buf,"notsupported");}


/****************
COP0 opcodes
*****************/
/*TODO*/void DisAllegrex::MFC0(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::ERET(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::CFC0(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::MTC0(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::CTC0(char *buf){strcpy(buf,"notsupported");}

/****************
COP1 opcodes
*****************/
void DisAllegrex::MFC1(char *buf){   sprintf(buf, "%-10s %s, %s","mfc1",      GPR_REG[DECODE_RT], COP1_REG_FP[DECODE_FS]);  }
void DisAllegrex::CFC1(char *buf){   sprintf(buf, "%-10s %s, %s","cfc1",      GPR_REG[DECODE_RT], COP1_REG_FCR[DECODE_FS]); }
void DisAllegrex::MTC1(char *buf){   sprintf(buf, "%-10s %s, %s","mtc1",      GPR_REG[DECODE_RT], COP1_REG_FP[DECODE_FS]);  }
void DisAllegrex::CTC1(char *buf){   sprintf(buf, "%-10s %s, %s","ctc1",      GPR_REG[DECODE_RT], COP1_REG_FCR[DECODE_FS]); }
void DisAllegrex::COP1_Unknown(char *buf){strcpy(buf,"COP1 ??"); }
void DisAllegrex::CVT_S_W(char *buf){sprintf(buf, "%-10s %s, %s","cvt.s.w",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::BC1F(char *buf){   sprintf(buf, "%-10s %s","bc1f",     offset_decode()); }
void DisAllegrex::BC1T(char *buf){   sprintf(buf, "%-10s %s","bc1t",     offset_decode()); }
void DisAllegrex::BC1FL(char *buf){  sprintf(buf, "%-10s %s","bc1fl",    offset_decode()); }
void DisAllegrex::BC1TL(char *buf){  sprintf(buf, "%-10s %s","bc1tl",    offset_decode()); }
void DisAllegrex::CVT_W_S(char *buf){sprintf(buf, "%-10s %s, %s","cvt.w.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}   
void DisAllegrex::ADD_S(char *buf){sprintf(buf, "%-10s %s, %s, %s","add.s", COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS], COP1_REG_FP[DECODE_FT]);}
void DisAllegrex::SUB_S(char *buf){sprintf(buf, "%-10s %s, %s, %s","sub.s", COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS], COP1_REG_FP[DECODE_FT]);}
void DisAllegrex::MUL_S(char *buf){sprintf(buf, "%-10s %s, %s, %s","mul.s", COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS], COP1_REG_FP[DECODE_FT]);}
void DisAllegrex::DIV_S(char *buf){sprintf(buf, "%-10s %s, %s, %s","div.s", COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS], COP1_REG_FP[DECODE_FT]);}
void DisAllegrex::SQRT_S(char *buf){sprintf(buf, "%-10s %s, %s","sqrt.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::ABS_S(char *buf){sprintf(buf, "%-10s %s, %s","abs.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::MOV_S(char *buf){sprintf(buf, "%-10s %s, %s","mov.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::NEG_S(char *buf){sprintf(buf, "%-10s %s, %s","neg.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::ROUND_W_S(char *buf){sprintf(buf, "%-10s %s, %s","round.w.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::TRUNC_W_S(char *buf){sprintf(buf, "%-10s %s, %s","trunc.w.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::CEIL_W_S(char *buf){sprintf(buf, "%-10s %s, %s","ceil.w.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);}
void DisAllegrex::FLOOR_W_S(char *buf){sprintf(buf, "%-10s %s, %s","floor.w.s",   COP1_REG_FP[DECODE_FD], COP1_REG_FP[DECODE_FS]);} 
/*TODO*/void DisAllegrex::C_COND_S(char *buf){strcpy(buf,"notsupported");}  

/****************
VFPU opcodes
*****************/
/*TODO*/void DisAllegrex::BVF(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::BVT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::BVFL(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::BVTL(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::MFV(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::MTV(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::MFVC(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::MTVC(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VADD(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSUB(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSBN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VDIV(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMUL(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VDOT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSCL(char *buf){strcpy(buf,"notsupported");} 
void DisAllegrex::VFPU_Unknown(char *buf){strcpy(buf,"VFPU ???");} 
/*TODO*/void DisAllegrex::VHDP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VDET(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VCRS(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VCMP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMIN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMAX(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSLT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSCMP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSGE(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMOV(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VNEG(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VABS(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VIDT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VF2IN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VI2F(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VWBN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSAT0(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VZERO(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VONE(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSAT1(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VRCP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSIN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VRSQ(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VCOS(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VEXP2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSQRT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VLOG2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VASIN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VNRCP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VNSIN(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VREXP2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VRNDS(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VRNDF1(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VRNDI(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VRNDF2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VCMOVT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VF2IZ(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VCMOVF(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VF2H(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VH2F(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSBZ(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VLGB(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VUC2I(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VC2I(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VI2UC(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VI2US(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VI2C(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VI2S(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSRT1(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VBFY1(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSRT2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VBFY2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VF2IU(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VOCP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VFAD(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSOCP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VAVG(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSRT3(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSRT4(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VUS2I(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VS2I(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMFVC(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMTVC(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VT4444(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VT5650(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VT5551(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VCST(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VF2ID(char *buf){strcpy(buf,"notsupported");} 	 
/*TODO*/void DisAllegrex::VIIM(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VFIM(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VPFXS(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VPFXT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VPFXD(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::LVLQ(char *buf){strcpy(buf,"notsupported");}  
/*TODO*/void DisAllegrex::LVRQ(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VHTFM2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VTFM2(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VTFM3(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VHTFM3(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VHTFM4(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VTFM4(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMMOV(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMZERO(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMIDT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMONE(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VROT(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMMUL(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VMSCL(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VQMUL(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::SVLQ(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::SVRQ(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::SVQ(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::SWB(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VNOP(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VSYNC(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::VFLUSH(char *buf){strcpy(buf,"notsupported");} 
/*TODO*/void DisAllegrex::LVS(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::SVS(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::LVQ(char *buf){strcpy(buf,"notsupported");}

/*******
CACHE instructions
********/
/*TODO*/void DisAllegrex::ICACHE_FILL(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::ICACHE_FILL_WITH_LOCK(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_CREATE_DIRTY_EXCLUSIVE(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_HIT_INVALIDATE(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_HIT_WRITEBACK(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_HIT_WRITEBACK_INVALIDATE(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::CACHE_Unknown(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::ICACHE_INDEX_INVALIDATE(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::ICACHE_INDEX_UNLOCK(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_FILL(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_FILL_WITH_LOCK(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::ICACHE_HIT_INVALIDATE(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_INDEX_WRITEBACK_INVALIDATE(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_INDEX_UNLOCK(char *buf){strcpy(buf,"notsupported");}
/*TODO*/void DisAllegrex::DCACHE_CREATE_DIRTY_EXCLUSIVE_WITH_LOCK(char *buf){strcpy(buf,"notsupported");}