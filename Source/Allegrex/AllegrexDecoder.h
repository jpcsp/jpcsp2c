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

namespace Allegrex
{
  	struct OPCODE
	{
      const char Name[45];
	  const OPCODE& (*getsubclass)();
	  void (*interpret)();
	  //void (*recompile)();
	  void (*disasm)( char *buf );
	};
	const OPCODE& GetCurrentInstruction();


#define MakeOpcode(name) \
		static const OPCODE name = { \
			#name, \
			0,\
			IntAllegrex::name, \
			DisAllegrex::name \
		}
#define MakeOpcodeClass( name ) \
		static const OPCODE name = { \
			#name, \
			Allegrex::Opcodes::Class_##name, \
			0, \
			0, \
		}
	namespace OpcodeTables
	{
		extern const OPCODE BASIC[64];
		extern const OPCODE Special[64];
		extern const OPCODE RegImm[32];
		extern const OPCODE Cop0[4];
		extern const OPCODE Cop1[8];
		extern const OPCODE Bc1Table[4];
		extern const OPCODE Cop1sTable[32];
		extern const OPCODE vfpu1[4];
		extern const OPCODE vfpu2[4];
		extern const OPCODE vfpu3[8];
		extern const OPCODE vfpu4[8];
		extern const OPCODE vfpu5[32];
		extern const OPCODE vfpu6[4];
		extern const OPCODE vfpu7[8];
		extern const OPCODE cache[16];

	}
	namespace Opcodes
	{
		const OPCODE& Class_SPECIAL();
		const OPCODE& Class_REGIMM();
		const OPCODE& Class_COP0();
        const OPCODE& Class_COP1();
		const OPCODE& Class_BC1TABLE();
		const OPCODE& Class_COP1STABLE();
		const OPCODE& Class_MFC1_CFC1();
		const OPCODE& Class_MTC1_CTC1();
		const OPCODE& Class_MFC0_ERET();
		const OPCODE& Class_SQRT_S_CVT_W_S();
		const OPCODE& Class_SRLROTR();
		const OPCODE& Class_SRLVROTRV();
		const OPCODE& Class_SPECIAL2();
		const OPCODE& Class_SPECIAL3();
		const OPCODE& Class_VFPU2();
		const OPCODE& Class_VFPU0();
        const OPCODE& Class_VFPU1();
		const OPCODE& Class_VFPU3();
        const OPCODE& Class_VFPU4();
		const OPCODE& Class_vfpu5op0();
	    const OPCODE& Class_vfpu5op1();
	    const OPCODE& Class_vfpu5op2();
	    const OPCODE& Class_vfpu5op3();
	    const OPCODE& Class_vfpu5op4();
	    const OPCODE& Class_vfpu5op5();
	    const OPCODE& Class_vfpu5op6();
	    const OPCODE& Class_vfpu5op7();
	    const OPCODE& Class_vfpu5op8();
	    const OPCODE& Class_vfpu5op9();
	    const OPCODE& Class_vfpu5op10();
	    const OPCODE& Class_vfpu5op11();
	    const OPCODE& Class_vfpu5op12();
	    const OPCODE& Class_vfpu5op13();
	    const OPCODE& Class_vfpu5op14();
	    const OPCODE& Class_vfpu5op15();
	    const OPCODE& Class_vfpu5op16();
	    const OPCODE& Class_vfpu5op17();
	    const OPCODE& Class_vfpu5op18();
	    const OPCODE& Class_vfpu5op19();
	    const OPCODE& Class_vfpu5op20();
	    const OPCODE& Class_vfpu5op21();
	    const OPCODE& Class_vfpu5op22();
	    const OPCODE& Class_vfpu5op23();
	    const OPCODE& Class_vfpu5op24();
	    const OPCODE& Class_vfpu5op25();
	    const OPCODE& Class_vfpu5op26();
	    const OPCODE& Class_vfpu5op27();
	    const OPCODE& Class_vfpu5op28();
	    const OPCODE& Class_vfpu5op29();
	    const OPCODE& Class_vfpu5op30();
	    const OPCODE& Class_vfpu5op31();
		const OPCODE& Class_VFPU5();
		const OPCODE& Class_VIIM_VFIM();
		const OPCODE& Class_ULVQ();
		const OPCODE& Class_VFPU6();
		const OPCODE& Class_VHTFM2_VTFM2();
		const OPCODE& Class_VTFM3_VHTFM3();
		const OPCODE& Class_VHTFM4_VTFM4();
		const OPCODE& Class_VFPUI();
		const OPCODE& Class_USVQ();
		const OPCODE& Class_SVQT();
        const OPCODE& Class_VFPU7();
        const OPCODE& Class_CACHE();
		const OPCODE& Class_CACHE1();
		const OPCODE& Class_CACHE2();
		const OPCODE& Class_CACHE3();
		const OPCODE& Class_CACHE4();

		
	}
}