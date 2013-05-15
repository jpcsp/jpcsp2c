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
#include "DisAllegrex.h"
#include "AllegrexInterpreter.h"
#include "AllegrexDecoder.h"
#include "Allegrex.h"


namespace Allegrex{
	namespace Opcodes
	{
       MakeOpcode(Unknown);
	   MakeOpcodeClass(SPECIAL);
	   MakeOpcodeClass(REGIMM);
       MakeOpcodeClass(COP0);
	   MakeOpcodeClass(COP1);
	   MakeOpcodeClass(BC1TABLE);
	   MakeOpcodeClass(COP1STABLE);

	   MakeOpcode(J);
       MakeOpcode(JAL);
       MakeOpcode(BEQ);
       MakeOpcode(BNE);
       MakeOpcode(BLEZ);
       MakeOpcode(BGTZ );
	   MakeOpcode(ADDI);
       MakeOpcode(ADDIU);
       MakeOpcode(SLTI);
       MakeOpcode(SLTIU);
       MakeOpcode(ANDI);
       MakeOpcode(ORI);
	   MakeOpcode(XORI);
	   MakeOpcode(LUI);
	   MakeOpcodeClass(VFPU2);
	   MakeOpcode(BVF);
	   MakeOpcode(BVT);
	   MakeOpcode(BVFL);
	   MakeOpcode(BVTL);
	   MakeOpcode(MFV);
	   MakeOpcode(MTV);
	   MakeOpcode(MFVC);
	   MakeOpcode(MTVC);
	   MakeOpcode(BEQL);
	   MakeOpcode(BNEL);
	   MakeOpcode(BLEZL);
	   MakeOpcode(BGTZL );
	   MakeOpcodeClass(VFPU0);
	   MakeOpcode(VADD);
	   MakeOpcode(VSUB);
	   MakeOpcode(VSBN);
	   MakeOpcode(VDIV);
	   MakeOpcodeClass(VFPU1);
	   MakeOpcode(VMUL);
	   MakeOpcode(VDOT);
	   MakeOpcode(VSCL);
	   MakeOpcode(VFPU_Unknown);
	   MakeOpcode(VHDP);
	   MakeOpcode(VDET);
	   MakeOpcode(VCRS);
	   MakeOpcodeClass(VFPU3);
	   MakeOpcode(VCMP);
	   MakeOpcode(VMIN);
	   MakeOpcode(VMAX);
	   MakeOpcode(VSLT);
	   MakeOpcode(VSCMP);
	   MakeOpcode(VSGE);
	   MakeOpcodeClass(SPECIAL2);
	   MakeOpcode(MFIC);
	   MakeOpcode(MTIC);
	   MakeOpcode(HALT);
	   MakeOpcodeClass(SPECIAL3); 
	   MakeOpcode(SEH);
	   MakeOpcode(BITREV);
	   MakeOpcode(WSBH);
	   MakeOpcode(WSBW);
	   MakeOpcode(EXT);
	   MakeOpcode(INS);
	   MakeOpcode(SEB);
	   MakeOpcode(LB);
	   MakeOpcode(LH);
	   MakeOpcode(LWL);
	   MakeOpcode(LW);
	   MakeOpcode(LBU);
	   MakeOpcode(LHU);
	   MakeOpcode(LWR);
	   MakeOpcode(SB);
	   MakeOpcode(SH);
	   MakeOpcode(SWL);
	   MakeOpcode(SW);
	   MakeOpcode(SWR);
	   MakeOpcodeClass(CACHE);
	   MakeOpcodeClass(CACHE1);
	   MakeOpcodeClass(CACHE2);
	   MakeOpcodeClass(CACHE3);
	   MakeOpcodeClass(CACHE4);
	   MakeOpcode(ICACHE_FILL);
	   MakeOpcode(ICACHE_FILL_WITH_LOCK);
	   MakeOpcode(DCACHE_CREATE_DIRTY_EXCLUSIVE);
	   MakeOpcode(DCACHE_HIT_INVALIDATE);
	   MakeOpcode(DCACHE_HIT_WRITEBACK);
	   MakeOpcode(DCACHE_HIT_WRITEBACK_INVALIDATE);
	   MakeOpcode(CACHE_Unknown);
	   MakeOpcode(ICACHE_INDEX_INVALIDATE);
	   MakeOpcode(ICACHE_INDEX_UNLOCK);
	   MakeOpcode(DCACHE_FILL);
	   MakeOpcode(DCACHE_FILL_WITH_LOCK);
	   MakeOpcode(ICACHE_HIT_INVALIDATE);
	   MakeOpcode(DCACHE_INDEX_WRITEBACK_INVALIDATE);
	   MakeOpcode(DCACHE_INDEX_UNLOCK);
	   MakeOpcode(DCACHE_CREATE_DIRTY_EXCLUSIVE_WITH_LOCK);
	   MakeOpcode(LL);
	   MakeOpcode(LWC1);
	   MakeOpcode(LVS);
	   MakeOpcodeClass(VFPU4);
	   MakeOpcodeClass(vfpu5op0);
	   MakeOpcodeClass(vfpu5op1);
	   MakeOpcodeClass(vfpu5op2);
	   MakeOpcodeClass(vfpu5op3);
	   MakeOpcodeClass(vfpu5op4);
	   MakeOpcodeClass(vfpu5op5);
	   MakeOpcodeClass(vfpu5op6);
	   MakeOpcodeClass(vfpu5op7);
	   MakeOpcodeClass(vfpu5op8);
	   MakeOpcodeClass(vfpu5op9);
	   MakeOpcodeClass(vfpu5op10);
	   MakeOpcodeClass(vfpu5op11);
	   MakeOpcodeClass(vfpu5op12);
	   MakeOpcodeClass(vfpu5op13);
	   MakeOpcodeClass(vfpu5op14);
	   MakeOpcodeClass(vfpu5op15);
	   MakeOpcodeClass(vfpu5op16);
	   MakeOpcodeClass(vfpu5op17);
	   MakeOpcodeClass(vfpu5op18);
	   MakeOpcodeClass(vfpu5op19);
	   MakeOpcodeClass(vfpu5op20);
	   MakeOpcodeClass(vfpu5op21);
	   MakeOpcodeClass(vfpu5op22);
	   MakeOpcodeClass(vfpu5op23);
	   MakeOpcodeClass(vfpu5op24);
	   MakeOpcodeClass(vfpu5op25);
	   MakeOpcodeClass(vfpu5op26);
	   MakeOpcodeClass(vfpu5op27);
	   MakeOpcodeClass(vfpu5op28);
	   MakeOpcodeClass(vfpu5op29);
	   MakeOpcodeClass( vfpu5op30);
	   MakeOpcodeClass( vfpu5op31);
	   MakeOpcode(VMOV);
       MakeOpcode(VNEG);
       MakeOpcode(VABS);
       MakeOpcode(VIDT);
       MakeOpcode(VF2IN);
       MakeOpcode(VI2F);
       MakeOpcode(VWBN);
	   MakeOpcode(VSAT0);
	   MakeOpcode(VZERO);
	   MakeOpcode(VONE);
	   MakeOpcode(VSAT1);
       MakeOpcode(VRCP);
	   MakeOpcode(VSIN);
	   MakeOpcode(VRSQ);
	   MakeOpcode(VCOS);
	   MakeOpcode(VEXP2);
	   MakeOpcode(VSQRT);
	   MakeOpcode(VLOG2);
	   MakeOpcode(VASIN);
	   MakeOpcode(VNRCP);
	   MakeOpcode(VNSIN);
	   MakeOpcode(VREXP2);
	   MakeOpcode(VRNDS);
	   MakeOpcode(VRNDF1);
	   MakeOpcode(VRNDI);
	   MakeOpcode(VRNDF2);
	   MakeOpcode(VCMOVT);
	   MakeOpcode(VF2IZ);
	   MakeOpcode(VCMOVF);
	   MakeOpcode(VF2H);
	   MakeOpcode(VH2F);
	   MakeOpcode(VSBZ);
	   MakeOpcode(VLGB);
	   MakeOpcode(VUC2I);
	   MakeOpcode(VC2I);
	   MakeOpcode(VI2UC);
	   MakeOpcode(VI2US);
	   MakeOpcode(VI2C);
	   MakeOpcode(VI2S);
	   MakeOpcode(VSRT1);
	   MakeOpcode(VBFY1);
	   MakeOpcode(VSRT2);
	   MakeOpcode(VBFY2);
	   MakeOpcode(VF2IU);
	   MakeOpcode(VOCP);
	   MakeOpcode(VFAD);
	   MakeOpcode(VSOCP);
	   MakeOpcode(VAVG);
	   MakeOpcode(VSRT3);
	   MakeOpcode(VSRT4);
	   MakeOpcode(VUS2I);
	   MakeOpcode(VS2I);
	   MakeOpcode(VMFVC);
	   MakeOpcode(VMTVC);
	   MakeOpcode(VT4444);
	   MakeOpcode(VT5650);
	   MakeOpcode(VT5551);
	   MakeOpcode(VCST);
	   MakeOpcode(VF2ID);
	   MakeOpcodeClass(ULVQ);
	   MakeOpcode(LVLQ);
	   MakeOpcode(LVRQ);
	   MakeOpcode(LVQ);
	   MakeOpcodeClass(VFPU5);
	   MakeOpcodeClass(VIIM_VFIM);
	   MakeOpcode(VIIM);
	   MakeOpcode(VFIM);
	   MakeOpcode(VPFXS);
	   MakeOpcode(VPFXT);
	   MakeOpcode(VPFXD);
	   MakeOpcode(SC);
	   MakeOpcode(SWC1);
	   MakeOpcode(SVS);
	   MakeOpcodeClass(VFPU6);
	   MakeOpcodeClass(VHTFM2_VTFM2);
	   MakeOpcodeClass(VTFM3_VHTFM3);
	   MakeOpcodeClass(VHTFM4_VTFM4);
	   MakeOpcodeClass(VFPUI);
	   MakeOpcode(VHTFM2);
	   MakeOpcode(VTFM2);
	   MakeOpcode(VTFM3);
	   MakeOpcode(VHTFM3);
	   MakeOpcode(VHTFM4);
	   MakeOpcode(VTFM4);
	   MakeOpcode(VMMOV);
	   MakeOpcode(VMZERO);
	   MakeOpcode(VMIDT);
	   MakeOpcode(VMONE);
	   MakeOpcode(VROT);
	   MakeOpcode(VMMUL);
	   MakeOpcode(VMSCL);
	   MakeOpcode(VQMUL);
	   MakeOpcodeClass(USVQ);
	   MakeOpcode(SVLQ);
	   MakeOpcode(SVRQ);
	   MakeOpcodeClass(SVQT);
	   MakeOpcode(SVQ);
	   MakeOpcode(SWB);
	   MakeOpcodeClass(VFPU7);
       MakeOpcode(VNOP);
	   MakeOpcode(VSYNC);
	   MakeOpcode(VFLUSH);
	   MakeOpcode(SLL);
	   MakeOpcodeClass(SRLROTR);
       MakeOpcode(SRL);
	   MakeOpcode(ROTR);
	   MakeOpcode(SRA);
	   MakeOpcode(SLLV);
	   MakeOpcodeClass(SRLVROTRV);
	   MakeOpcode(SRLV);
	   MakeOpcode(ROTRV);
	   MakeOpcode(SRAV);
	   MakeOpcode(JR);
	   MakeOpcode(JALR);
	   MakeOpcode(MOVZ);
	   MakeOpcode(MOVN);
	   MakeOpcode(SYSCALL);
	   MakeOpcode(BREAK);
	   MakeOpcode(SYNC);
	   MakeOpcode(MFHI);
	   MakeOpcode(MTHI);
	   MakeOpcode(MFLO);
	   MakeOpcode(MTLO);
	   MakeOpcode(CLZ);
	   MakeOpcode(CLO);
	   MakeOpcode(MULT);
	   MakeOpcode(MULTU);
	   MakeOpcode(DIV);
	   MakeOpcode(DIVU);
	   MakeOpcode(MADD);
	   MakeOpcode(MADDU);
	   MakeOpcode(ADD);
	   MakeOpcode(ADDU);
	   MakeOpcode(SUB);
	   MakeOpcode(SUBU);
	   MakeOpcode(AND);
	   MakeOpcode(OR);
	   MakeOpcode(XOR);
	   MakeOpcode(NOR); 
	   MakeOpcode(SLT);
	   MakeOpcode(SLTU);
	   MakeOpcode(MAX);
	   MakeOpcode(MIN);
	   MakeOpcode(MSUB);
	   MakeOpcode(MSUBU);
	   MakeOpcode(BLTZ);
	   MakeOpcode(BGEZ);
	   MakeOpcode(BLTZL);
	   MakeOpcode(BGEZL);
	   MakeOpcode(BLTZAL);
	   MakeOpcode(BGEZAL);
	   MakeOpcode(BLTZALL);
	   MakeOpcode(BGEZALL);
	   MakeOpcodeClass(MFC0_ERET);
	   MakeOpcode(MFC0);
	   MakeOpcode(ERET);
	   MakeOpcode(CFC0);
	   MakeOpcode(MTC0);
	   MakeOpcode(CTC0);
	   MakeOpcodeClass(MFC1_CFC1);
	   MakeOpcode(MFC1);
	   MakeOpcode(CFC1);
	   MakeOpcodeClass(MTC1_CTC1);
	   MakeOpcode(MTC1);
	   MakeOpcode(CTC1);
	   MakeOpcode(CVT_S_W);
	   MakeOpcode(BC1F);
	   MakeOpcode(BC1T);
	   MakeOpcode(BC1FL);
	   MakeOpcode(BC1TL);
	   MakeOpcode(ADD_S);
       MakeOpcode(SUB_S);   
	   MakeOpcode(MUL_S);   
	   MakeOpcode(DIV_S);    
	   MakeOpcodeClass(SQRT_S_CVT_W_S); 
	   MakeOpcode(SQRT_S);
	   MakeOpcode(CVT_W_S);
	   MakeOpcode(ABS_S);    
	   MakeOpcode(MOV_S);    
	   MakeOpcode(NEG_S);   
	   MakeOpcode(ROUND_W_S);  
	   MakeOpcode(TRUNC_W_S);  
	   MakeOpcode(CEIL_W_S);  
	   MakeOpcode(FLOOR_W_S);  
	   MakeOpcode(C_COND_S); 
	   MakeOpcode(COP1_Unknown); 

	   const OPCODE& Class_SPECIAL(){ return Allegrex::OpcodeTables::Special[((pspRegs.code) & 0x3F)];}
	   const OPCODE& Class_REGIMM(){ return Allegrex::OpcodeTables::RegImm[DECODE_RT];}
	   const OPCODE& Class_COP0(){ return Allegrex::OpcodeTables::Cop0[(pspRegs.code >> 22) & 0x00000003];}
	   const OPCODE& Class_COP1(){ return Allegrex::OpcodeTables::Cop1[(pspRegs.code >> 23) & 0x00000007];}
	   const OPCODE& Class_BC1TABLE(){ return Allegrex::OpcodeTables::Bc1Table[(pspRegs.code >> 16) & 0x00000003];}
	   const OPCODE& Class_COP1STABLE(){ return Allegrex::OpcodeTables::Cop1sTable[pspRegs.code  & 0x0000001f];}
	   const OPCODE& Class_SRLROTR()
	   {
		   if((pspRegs.code & 0x00200000) == 0x00000000)
            return Allegrex::Opcodes::SRL;
		   else
		    return Allegrex::Opcodes::ROTR;
	   }
	   const OPCODE& Class_SRLVROTRV()
	   {
          if((pspRegs.code & 0x00000040)==0x00000000)
            return Allegrex::Opcodes::SRLV;
		  else
		    return Allegrex::Opcodes::ROTRV;
	   }
	   const OPCODE& Class_MFC1_CFC1()
	   {
		      if ((pspRegs.code & 0x00400000) == 0x00000000) 
				  return Allegrex::Opcodes::MFC1;
             else
	            return Allegrex::Opcodes::CFC1;
	   }
	   const OPCODE& Class_MTC1_CTC1()
	   {
           if ((pspRegs.code & 0x00400000) == 0x00000000) 
	             return Allegrex::Opcodes::MTC1;
           else
	             return Allegrex::Opcodes::CTC1;
	   }
	   const OPCODE& Class_MFC0_ERET()
	   {
          	if ((pspRegs.code & 0x00000008) == 0x00000000)
               return Allegrex::Opcodes::MFC0;
	        else
               return Allegrex::Opcodes::ERET;
	   }
	   const OPCODE& Class_SQRT_S_CVT_W_S()
	   {
		   	if ((pspRegs.code & 0x00000020) == 0x00000000)
              return Allegrex::Opcodes::SQRT_S;
	        else
              return Allegrex::Opcodes::CVT_W_S;
	   }
	   const OPCODE& Class_SPECIAL2()
	   {
           if ((pspRegs.code & 0x00000002) == 0x00000000) 
		   {
               if ((pspRegs.code & 0x00000004) == 0x00000000) 
                  return Allegrex::Opcodes::HALT;
			   else 
				   return Allegrex::Opcodes::MFIC;
            }
            else 
               return Allegrex::Opcodes::MTIC;

	   }
	   const OPCODE& Class_SPECIAL3()
	   {
           if ((pspRegs.code & 0x00000021) == 0x00000020) {
                    if ((pspRegs.code & 0x00000080) == 0x00000000) 
					{
                        if ((pspRegs.code & 0x00000100) == 0x00000000) 
                            return Allegrex::Opcodes::SEH;
                         else 
                            return Allegrex::Opcodes::BITREV;
                    } 
					else 
					{
                        if ((pspRegs.code & 0x00000040) == 0x00000000) 
                            return Allegrex::Opcodes::WSBH;
						else 
                            return Allegrex::Opcodes::WSBW;
                    }
           } 
		   else 
		   {
                    if ((pspRegs.code & 0x00000001) == 0x00000000) 
					{
                        if ((pspRegs.code & 0x00000004) == 0x00000000) 
                            return Allegrex::Opcodes::EXT;
                         else 
                            return Allegrex::Opcodes::INS;
                    } 
					else 
					{
                        return Allegrex::Opcodes::SEB;
                    }
          }
	   }
	   const OPCODE& Class_VFPU2()
	   {
            if ((pspRegs.code & 0x00200000) == 0x00000000) 
				return Allegrex::OpcodeTables::vfpu1[(pspRegs.code >> 16) & 0x00000003];
			else 
			{
                    if ((pspRegs.code & 0x00000080) == 0x00000000) 
					{
                        if ((pspRegs.code & 0x00800000) == 0x00000000) 
                            return Allegrex::Opcodes::MFV;
                         else 
                            return Allegrex::Opcodes::MTV;
                    } 
					else 
					{
                        if ((pspRegs.code & 0x00800000) == 0x00000000) 
                            return Allegrex::Opcodes::MFVC;
                         else 
                            return Allegrex::Opcodes::MTVC;                     
                    }
             }
	   }
	   const OPCODE& Class_VFPU0()
	   {
		   return Allegrex::OpcodeTables::vfpu2[(pspRegs.code >> 23) & 0x00000003];
	   }
	   const OPCODE& Class_VFPU1()
	   {
           return Allegrex::OpcodeTables::vfpu3[(pspRegs.code >> 23) & 0x00000007];
	   }
	   const OPCODE& Class_VFPU3()
	   {
          return Allegrex::OpcodeTables::vfpu4[(pspRegs.code >> 23) & 0x00000007];
	   }
	   const OPCODE& Class_VFPU4()
	   {
          return Allegrex::OpcodeTables::vfpu5[(pspRegs.code >> 18) & 0x0000001f];
	   }
	   const OPCODE& Class_vfpu5op0()
	   {
	      if ((pspRegs.code & 0x02010000) == 0x00000000) 
		  {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) 
                        return Allegrex::Opcodes::VMOV;
                      else 
                        return Allegrex::Opcodes::VNEG;
           } 
		  else 
		  {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) 
                        return Allegrex::Opcodes::VABS;
                     else 
					 {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) 
                            return Allegrex::Opcodes::VIDT;
                        else 
						{
                            if ((pspRegs.code & 0x01800000) == 0x00000000) 
                                return Allegrex::Opcodes::VF2IN;
                             else 
							 {
                                if ((pspRegs.code & 0x01000000) == 0x00000000) 
                                    return Allegrex::Opcodes::VI2F;
                                 else 
                                    return Allegrex::Opcodes::VWBN;
                            }
                        }
                    }
            }
	   }
	   const OPCODE& Class_vfpu5op1()
	   { 
		   if ((pspRegs.code & 0x02010000) == 0x00000000) 
		   {
               if ((pspRegs.code & 0x00020000) == 0x00000000) 
                        return Allegrex::Opcodes::VSAT0;
                     else 
                        return Allegrex::Opcodes::VZERO;
                    
			}
		    else 
			{
                    if ((pspRegs.code & 0x02020000) == 0x00000000) 
                        return Allegrex::Opcodes::VSAT1;
                     else 
					 {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) 
                            return Allegrex::Opcodes::VONE;
                         else 
						 {
                            if ((pspRegs.code & 0x01800000) == 0x00000000) 
                                return Allegrex::Opcodes::VF2IN;
                            else 
							{
                                if ((pspRegs.code & 0x01000000) == 0x00000000) 
                                    return Allegrex::Opcodes::VI2F;
                                 else 
                                    return Allegrex::Opcodes::VWBN;   
                            }
                        }
                     }
               }
	   }
	   const OPCODE& Class_vfpu5op2()
	   { 
		       if ((pspRegs.code & 0x01800000) == 0x00000000) {
                    return Allegrex::Opcodes::VF2IN;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VI2F;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op3()
	   { 
                if ((pspRegs.code & 0x01800000) == 0x00000000) {
                    return Allegrex::Opcodes::VF2IN;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VI2F;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op4()
	   { 
		   if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VRCP;
                    } else {
                        return Allegrex::Opcodes::VSIN;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VRSQ;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VCOS;
                        } else {
                            if ((pspRegs.code & 0x01800000) == 0x00000000) {
                                return Allegrex::Opcodes::VF2IN;
                            } else {
                                if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                    return Allegrex::Opcodes::VI2F;
                                } else {
                                    return Allegrex::Opcodes::VWBN;
                                }
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op5()
	   { 
	           if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VEXP2;
                    } else {
                        return Allegrex::Opcodes::VSQRT;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VLOG2;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VASIN;
                        } else {
                            if ((pspRegs.code & 0x01800000) == 0x00000000) {
                                return Allegrex::Opcodes::VF2IN;
                            } else {
                                if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                    return Allegrex::Opcodes::VI2F;
                                } else {
                                    return Allegrex::Opcodes::VWBN;
                                }
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op6()
	   { 
	           if ((pspRegs.code & 0x02020000) == 0x00000000) {
                    return Allegrex::Opcodes::VNRCP;
                } else {
                    if ((pspRegs.code & 0x02000000) == 0x00000000) {
                        return Allegrex::Opcodes::VNSIN;
                    } else {
                        if ((pspRegs.code & 0x01800000) == 0x00000000) {
                            return Allegrex::Opcodes::VF2IN;
                        } else {
                            if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                return Allegrex::Opcodes::VI2F;
                            } else {
                                return Allegrex::Opcodes::VWBN;
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op7()
	   {
               if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VREXP2;
                } else {
                    if ((pspRegs.code & 0x01800000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2IN;
                    } else {
                        if ((pspRegs.code & 0x01000000) == 0x00000000) {
                            return Allegrex::Opcodes::VI2F;
                        } else {
                            return Allegrex::Opcodes::VWBN;
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op8()
	   {
	          if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VRNDS;
                    } else {
                        return Allegrex::Opcodes::VRNDF1;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VRNDI;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VRNDF2;
                        } else {
                            if ((pspRegs.code & 0x01800000) == 0x00800000) {
                                return Allegrex::Opcodes::VCMOVT;
                            } else {
                                if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                    return Allegrex::Opcodes::VF2IZ;
                                } else {
                                    return Allegrex::Opcodes::VWBN;
                                }
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op9()
	   { 
	         if ((pspRegs.code & 0x01800000) == 0x00800000) {
                    return Allegrex::Opcodes::VCMOVT;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2IZ;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op10()
	   { 
	              if ((pspRegs.code & 0x01800000) == 0x00800000) {
                    return Allegrex::Opcodes::VCMOVF;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2IZ;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op11()
	   { 
	            if ((pspRegs.code & 0x01800000) == 0x00800000) {
                    return Allegrex::Opcodes::VCMOVF;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2IZ;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op12()
	   { 
	            if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    return Allegrex::Opcodes::VF2H;
                } else {
                    if ((pspRegs.code & 0x02000000) == 0x00000000) {
                        return Allegrex::Opcodes::VH2F;
                    } else {
                        if ((pspRegs.code & 0x01000000) == 0x00000000) {
                            return Allegrex::Opcodes::VF2IZ;
                        } else {
                            return Allegrex::Opcodes::VWBN;
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op13()
	   { 
	            if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    return Allegrex::Opcodes::VSBZ;
                } else {
                    if ((pspRegs.code & 0x02000000) == 0x00000000) {
                        return Allegrex::Opcodes::VLGB;
                    } else {
                        if ((pspRegs.code & 0x01000000) == 0x00000000) {
                            return Allegrex::Opcodes::VF2IZ;
                        } else {
                            return Allegrex::Opcodes::VWBN;
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op14()
	   { 
	            if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VUC2I;
                    } else {
                        return Allegrex::Opcodes::VUS2I;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VC2I;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VS2I;
                        } else {
                            if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                return Allegrex::Opcodes::VF2IZ;
                            } else {
                                return Allegrex::Opcodes::VWBN;
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op15()
	   { 
	           if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VI2UC;
                    } else {
                        return Allegrex::Opcodes::VI2US;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VI2C;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VI2S;
                        } else {
                            if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                return Allegrex::Opcodes::VF2IZ;
                            } else {
                                return Allegrex::Opcodes::VWBN;
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op16()
	   { 
	            if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VSRT1;
                    } else {
                        return Allegrex::Opcodes::VBFY1;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VSRT2;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VBFY2;
                        } else {
                            if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                return Allegrex::Opcodes::VF2IU;
                            } else {
                                return Allegrex::Opcodes::VWBN;
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op17()
	   { 
	            if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VOCP;
                    } else {
                        return Allegrex::Opcodes::VFAD;
                    }
                } else {
                    if ((pspRegs.code & 0x02020000) == 0x00000000) {
                        return Allegrex::Opcodes::VSOCP;
                    } else {
                        if ((pspRegs.code & 0x02000000) == 0x00000000) {
                            return Allegrex::Opcodes::VAVG;
                        } else {
                            if ((pspRegs.code & 0x01000000) == 0x00000000) {
                                return Allegrex::Opcodes::VF2IU;
                            } else {
                                return Allegrex::Opcodes::VWBN;
                            }
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op18()
	   { 
	            if ((pspRegs.code & 0x02010000) == 0x00000000) {
                    return Allegrex::Opcodes::VSRT3;
                } else {
                    if ((pspRegs.code & 0x02000000) == 0x00000000) {
                        return Allegrex::Opcodes::VSRT4;
                    } else {
                        if ((pspRegs.code & 0x01000000) == 0x00000000) {
                            return Allegrex::Opcodes::VF2IU;
                        } else {
                            return Allegrex::Opcodes::VWBN;
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op19()
	   {
	           if ((pspRegs.code & 0x01000000) == 0x00000000) {
                    return Allegrex::Opcodes::VF2IU;
                } else {
                    return Allegrex::Opcodes::VWBN;
                }
	   }
	   const OPCODE& Class_vfpu5op20()
	   { 
	            if ((pspRegs.code & 0x02000080) == 0x00000000) {
                    return Allegrex::Opcodes::VMFVC;
                } else {
                    if ((pspRegs.code & 0x02000000) == 0x00000000) {
                        return Allegrex::Opcodes::VMTVC;
                    } else {
                        if ((pspRegs.code & 0x01000000) == 0x00000000) {
                            return Allegrex::Opcodes::VF2IU;
                        } else {
                            return Allegrex::Opcodes::VWBN;
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op21()
	   { 
                if ((pspRegs.code & 0x01000000) == 0x00000000) {
                    return Allegrex::Opcodes::VF2IU;
                } else {
                    return Allegrex::Opcodes::VWBN;
                }
	   }
	   const OPCODE& Class_vfpu5op22()
	   { 
                if ((pspRegs.code & 0x02010000) == 0x00010000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VT4444;
                    } else {
                        return Allegrex::Opcodes::VT5650;
                    }
                } else {
                    if ((pspRegs.code & 0x02000000) == 0x00000000) {
                        return Allegrex::Opcodes::VT5551;
                    } else {
                        if ((pspRegs.code & 0x01000000) == 0x00000000) {
                            return Allegrex::Opcodes::VF2IU;
                        } else {
                            return Allegrex::Opcodes::VWBN;
                        }
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op23()
	   { 
               if ((pspRegs.code & 0x01000000) == 0x00000000) {
                    return Allegrex::Opcodes::VF2IU;
                } else {
                    return Allegrex::Opcodes::VWBN;
                }
	   }
	   const OPCODE& Class_vfpu5op24()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op25()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op26()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op27()
	   {
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op28()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op29()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op30()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_vfpu5op31()
	   { 
                if ((pspRegs.code & 0x02000000) == 0x00000000) {
                    return Allegrex::Opcodes::VCST;
                } else {
                    if ((pspRegs.code & 0x01000000) == 0x00000000) {
                        return Allegrex::Opcodes::VF2ID;
                    } else {
                        return Allegrex::Opcodes::VWBN;
                    }
                }
	   }
	   const OPCODE& Class_VFPU5()
	   {
		   return Allegrex::OpcodeTables::vfpu6[(pspRegs.code >> 24) & 0x00000003];
	   }
	   const OPCODE& Class_VIIM_VFIM()
	   {
                if ((pspRegs.code& 0x00800000) == 0x00000000) {
                    return Allegrex::Opcodes::VIIM;
                } else {
                    return Allegrex::Opcodes::VFIM;
                }
	   }
	   const OPCODE& Class_ULVQ()
	   {
             if ((pspRegs.code& 0x00000002) == 0x00000000) 
                    return Allegrex::Opcodes::LVLQ;
               else 
                    return Allegrex::Opcodes::LVRQ;
	   }
	   const OPCODE& Class_VFPU6()
	   {
		   return Allegrex::OpcodeTables::vfpu7[(pspRegs.code >> 23) & 0x00000007];
	   }
	   	const OPCODE& Class_VHTFM2_VTFM2()
		{
			  if ((pspRegs.code & 0x00000080) == 0x00000000) {
                    return Allegrex::Opcodes::VHTFM2;
                } else {
                    return Allegrex::Opcodes::VTFM2;
					}
		}
		const OPCODE& Class_VTFM3_VHTFM3()
		{
			  if ((pspRegs.code & 0x00000080) == 0x00000000) {
                    return  Allegrex::Opcodes::VTFM3;
                } else {
                    return  Allegrex::Opcodes::VHTFM3;
                }
		}
		const OPCODE& Class_VHTFM4_VTFM4()
		{
			   if ((pspRegs.code & 0x00000080) == 0x00000000) {
                    return Allegrex::Opcodes::VHTFM4;
                } else {
                    return Allegrex::Opcodes::VTFM4;
                }

		}
		const OPCODE& Class_VFPUI()
		{
                if ((pspRegs.code& 0x00210000) == 0x00000000) {
                    if ((pspRegs.code & 0x00020000) == 0x00000000) {
                        return Allegrex::Opcodes::VMMOV;
                    } else {
                        return Allegrex::Opcodes::VMZERO;
                    }
                } else {
                    if ((pspRegs.code & 0x00200000) == 0x00000000) {
                        if ((pspRegs.code & 0x00040000) == 0x00000000) {
                            return Allegrex::Opcodes::VMIDT;
                        } else {
                            return Allegrex::Opcodes::VMONE;
                        }
                    } else {
                        return Allegrex::Opcodes::VROT;
                    }
                }
		}
		const OPCODE& Class_USVQ()
		{
              if ((pspRegs.code & 0x00000002) == 0x00000000) {
                    return Allegrex::Opcodes::SVLQ;
                } else {
                    return Allegrex::Opcodes::SVRQ;
                }
		}
		const OPCODE& Class_SVQT()
		{
               if ((pspRegs.code & 0x00000002) == 0x00000000) {
                    return Allegrex::Opcodes::SVQ;
                } else {
                    return Allegrex::Opcodes::SWB;
                }
		}
		const OPCODE& Class_VFPU7()
		{
              if ((pspRegs.code & 0x00000001) == 0x00000000) {
                    if ((pspRegs.code & 0x00000020) == 0x00000000) {
                        return Allegrex::Opcodes::VNOP;
                    } else {
                        return Allegrex::Opcodes::VSYNC;
                    }
                } else {
                    return Allegrex::Opcodes::VFLUSH;
                }
		}
		const OPCODE& Class_CACHE()
		{
			return Allegrex::OpcodeTables::cache[(pspRegs.code >> 17) & 0x0000000f];
		}
		const OPCODE& Class_CACHE1()
		{
            if ((pspRegs.code & 0x00010000) == 0x00000000) {
                    return Allegrex::Opcodes::ICACHE_FILL;
                } else {
                    return Allegrex::Opcodes::ICACHE_FILL_WITH_LOCK;
                }
		}
		const OPCODE& Class_CACHE2()
		{
               if ((pspRegs.code & 0x00010000) == 0x00000000) {
                    return Allegrex::Opcodes::DCACHE_CREATE_DIRTY_EXCLUSIVE;
                } else {
                    return Allegrex::Opcodes::DCACHE_HIT_INVALIDATE;
                }
		}
		const OPCODE& Class_CACHE3()
		{
               if ((pspRegs.code & 0x00010000) == 0x00000000) {
                    return Allegrex::Opcodes::DCACHE_HIT_WRITEBACK;
                } else {
                    return Allegrex::Opcodes::DCACHE_HIT_WRITEBACK_INVALIDATE;
                }
		}
		const OPCODE& Class_CACHE4()
		{
                if ((pspRegs.code & 0x00010000) == 0x00000000) {
                    return Allegrex::Opcodes::DCACHE_FILL;
                } else {
                    return Allegrex::Opcodes::DCACHE_FILL_WITH_LOCK;
                }
		}

	}
	const OPCODE& GetCurrentInstruction()
	{
		const Allegrex::OPCODE* opcode = &Allegrex::OpcodeTables::BASIC[(pspRegs.code) >> 26];
	
		while( opcode->getsubclass != NULL )
			opcode = &opcode->getsubclass();

		return *opcode;
	}
	
 	namespace OpcodeTables
	{
		using namespace Opcodes;
        // CPU: encoded by opcode field.

		const OPCODE BASIC[64] = 
		{
			SPECIAL,  REGIMM,   J,       JAL,     BEQ,     BNE,     BLEZ,   BGTZ,
			ADDI,     ADDIU,    SLTI,    SLTIU,   ANDI,    ORI,     XORI,   LUI,
			COP0,     COP1,     VFPU2,   Unknown, BEQL,    BNEL,    BLEZL,  BGTZL,
			VFPU0,    VFPU1,    Unknown, VFPU3,   SPECIAL2,Unknown, Unknown,SPECIAL3,
			LB,       LH,       LWL,     LW,      LBU,     LHU,     LWR,    Unknown,
			SB,       SH,       SWL,     SW,      Unknown, Unknown, SWR,    CACHE,
			LL,       LWC1,     LVS ,    Unknown, VFPU4,   ULVQ,    LVQ,    VFPU5,
			SC,       SWC1,     SVS,     Unknown, VFPU6,   USVQ,    SVQT,    VFPU7
		};
		const OPCODE Special[64] = 
		{
			SLL,      Unknown,  SRLROTR,  SRA,      SLLV,    Unknown, SRLVROTRV ,SRAV,
			JR,       JALR,     MOVZ,     MOVN,     SYSCALL, BREAK,   Unknown,   SYNC,
			MFHI,     MTHI,     MFLO,     MTLO,     Unknown, Unknown, CLZ ,      CLO,
			MULT,     MULTU,    DIV,      DIVU,     MADD,    MADDU,   Unknown,   Unknown,
			ADD,      ADDU,     SUB,      SUBU,     AND,     OR,      XOR,       NOR,
			Unknown,  Unknown,  SLT,      SLTU,     MAX ,    MIN,     MSUB,      MSUBU,
			Unknown,  Unknown,  Unknown,  Unknown,  Unknown, Unknown, Unknown,   Unknown,
			Unknown , Unknown,  Unknown,  Unknown,  Unknown, Unknown, Unknown,   Unknown
		};
		const OPCODE RegImm[32] = {
			BLTZ,    BGEZ,     BLTZL,     BGEZL,   Unknown, Unknown, Unknown, Unknown,
			Unknown, Unknown,  Unknown,   Unknown, Unknown, Unknown, Unknown, Unknown,
			BLTZAL,  BGEZAL,   BLTZALL,   BGEZALL, Unknown, Unknown, Unknown, Unknown,
			Unknown, Unknown , Unknown,   Unknown, Unknown, Unknown, Unknown, Unknown,
		};
		const OPCODE Cop0[4] = {
			MFC0_ERET,CFC0,MTC0,CTC0,
		};
		const OPCODE Cop1[8] = {
            MFC1_CFC1,MTC1_CTC1,BC1TABLE,COP1_Unknown,COP1STABLE,CVT_S_W,COP1_Unknown,COP1_Unknown,
		};
		const OPCODE Bc1Table[4] = {
			BC1F,BC1T,BC1FL,BC1TL
		};
		const OPCODE Cop1sTable[32] = {
			ADD_S,        SUB_S,        MUL_S,        DIV_S,         SQRT_S_CVT_W_S, ABS_S,    MOV_S,    NEG_S,    
			COP1_Unknown, COP1_Unknown, COP1_Unknown, COP1_Unknown,  ROUND_W_S,      TRUNC_W_S,CEIL_W_S, FLOOR_W_S,
			C_COND_S,     C_COND_S,     C_COND_S,     C_COND_S,      C_COND_S,       C_COND_S, C_COND_S, C_COND_S, 
			C_COND_S,     C_COND_S,     C_COND_S,     C_COND_S,      C_COND_S       ,C_COND_S, C_COND_S, C_COND_S, 
		};
		const OPCODE vfpu1[4] = {
              BVF,BVT,BVFL,BVTL,
		};
		const OPCODE vfpu2[4] = {
           VADD,VSUB,VSBN,VDIV,
		};
		const OPCODE vfpu3[8]= {
		  VMUL,VDOT,VSCL,VFPU_Unknown,VHDP,VDET,VCRS,VFPU_Unknown,
		};
		const OPCODE vfpu4[8]= {
		  VCMP,VFPU_Unknown,VMIN,VMAX,VSLT,VSCMP,VSGE,VFPU_Unknown,
		};
		const OPCODE vfpu5[32] = {
			vfpu5op0 ,vfpu5op1 ,vfpu5op2 ,vfpu5op3 ,vfpu5op4 ,vfpu5op5 ,vfpu5op6 ,vfpu5op7,
			vfpu5op8 ,vfpu5op9 ,vfpu5op10,vfpu5op11,vfpu5op12,vfpu5op13,vfpu5op14,vfpu5op15,
			vfpu5op16,vfpu5op17,vfpu5op18,vfpu5op19,vfpu5op20,vfpu5op21,vfpu5op22,vfpu5op23,
			vfpu5op24,vfpu5op25,vfpu5op26,vfpu5op27,vfpu5op28,vfpu5op29,vfpu5op30,vfpu5op31,
		};
		const OPCODE vfpu6[4] = {
			VPFXS,VPFXT,VPFXD,VIIM_VFIM,
		};
		const OPCODE vfpu7[8] = {
            VMMUL,VHTFM2_VTFM2,VTFM3_VHTFM3,VHTFM4_VTFM4,VMSCL,VQMUL,VFPU_Unknown,VFPUI,
		};
		const OPCODE cache[16] = {
        CACHE_Unknown        ,CACHE_Unknown,ICACHE_INDEX_INVALIDATE           ,ICACHE_INDEX_UNLOCK,
        ICACHE_HIT_INVALIDATE,CACHE1       ,CACHE_Unknown                     ,CACHE_Unknown,
		CACHE_Unknown        ,CACHE_Unknown,DCACHE_INDEX_WRITEBACK_INVALIDATE,DCACHE_INDEX_UNLOCK
		,CACHE2              ,CACHE3       ,DCACHE_CREATE_DIRTY_EXCLUSIVE_WITH_LOCK    ,CACHE4
		};


	}
  
}
  


        



