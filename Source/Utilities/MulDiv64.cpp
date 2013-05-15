/*
 *	Source file:		MulDiv64.cpp
 *	Author:				Richard van der Wal
 *	Contact:			R.vdWal@xs4all.nl
 *
 *	Description:
 *		Implementation for MulDiv64 and MulShr64
 *
 *	$Log: $
 */

/*
 * MulDiv64
 * Multiplies an operant by a multiplier and divides the result by a divider
 * Used for scaling 64 bit integer values
 *     Xscaled = Xstart * Multiplier / Divider
 * Uses 128 bit intermediate result
 */
#define ABS64(num) (num >=0 ? num : -num)
//
__int64 _stdcall MulDiv64(__int64 operant, __int64 multiplier, __int64 divider)
{
	// Declare 128bit storage
	struct{
		unsigned long DW[4];
	}var128, quotient;
	// Change semantics for intermediate results for Full Div
	// by renaming the vars
	#define REMAINDER quotient
	#define QUOTIENT edi

	// Save combined sign on stack
	_asm{
		mov		eax, dword ptr[operant+4]
		xor		eax, dword ptr[multiplier+4]
		xor		eax, dword ptr[divider+4]
		pushfd
	}

	// Take absolute values because algorithm is for unsigned only
	operant		= ABS64(operant);
	multiplier	= ABS64(multiplier);
	divider		= ABS64(divider);

	_asm{
		// First check divider for 0
		mov		eax, dword ptr[divider+4]
		or		eax, dword ptr[divider]
		jnz		dividerOK
		div		eax
dividerOK:
		lea		edi,[var128]					// edi = &var128
		// Check multiplier for 1 or 0
		xor		eax, eax
		cmp		eax, dword ptr[multiplier+4]
		jnz		startMUL
		cmp		eax, dword ptr[multiplier]
		jnz		multiNotNUL
		xor		edx, edx
		popfd									// cleanup stack
		jmp		done
multiNotNUL:
		// Set result HI part to 0
		xor		eax,eax
		mov		dword ptr[edi+12], eax
		mov		dword ptr[edi+8], eax
		mov		eax, 1
		cmp		eax, dword ptr[multiplier]
		jnz		smallMUL
		// Multiplier is 1 so just copy operant to result
		mov		eax, dword ptr[operant+4]
		mov		dword ptr[edi+4], eax
		mov		eax, dword ptr[operant]
		mov		dword ptr[edi], eax
		jmp		startDIV
smallMUL:
		// Test for 32/32 bit multiplication
        xor		eax, eax
        mov		ecx, dword ptr[operant+4]
        or      ecx, eax         ;test for both hiwords zero.
		jnz		startMUL
		// Do 32/32 bit multiplication
        mov		ecx, dword ptr[multiplier]
		mov		eax, dword ptr[operant]
		mul		ecx
		mov		dword ptr[edi+4], edx
		mov		dword ptr[edi], eax
		jmp		startDIV
startMUL:
		// Check signs
		// Multiply: var128 = operant * multiplier
		mov		eax, dword ptr[multiplier]		// eax = LO(multiplier)
		mul		dword ptr[operant]				// edx:eax = eax * LO(operant)
		mov		dword ptr[edi], eax				// var128.DW0 = eax
		mov		ecx, edx						// ecx = edx

		mov		eax, dword ptr[multiplier]		// eax = LO(multiplier)
		mul		dword ptr[operant+4]			// edx:eax = eax * HI(operant)
		add		eax, ecx						// eax = eax + ecx
		adc		edx, 0							// edx = edx + 0 + carry
		mov		ebx, eax
		mov		ecx, edx

		mov		eax, dword ptr[multiplier+4]
		mul		dword ptr[operant]
		add		eax, ebx
		mov		dword ptr[edi+4], eax
		adc		ecx, edx
		pushfd

		mov		eax, dword ptr[multiplier+4]
		mul		dword ptr[operant+4]
		popfd
		adc		eax, ecx
		adc		edx, 0
		mov		dword ptr[edi+8], eax
		mov		dword ptr[edi+12], edx
startDIV:
		// Divide: var128 = var128 / divider
		//
		// Test divider = 32bit value
		mov		eax, dword ptr[divider+4]
		cmp		eax, 0
		jnz		fullDIV
		mov		ecx, dword ptr[divider]
		cmp		ecx, 1
		jz		applySign

		// Start 128/32 bit division
		mov		eax, dword ptr[edi+12]
		xor		edx, edx
		div		ecx
		mov		dword ptr[quotient+12], eax

		mov		eax, dword ptr[edi+8]
		div		ecx
		mov		dword ptr[quotient+8], eax

		mov		eax, dword ptr[edi+4]
		div		ecx
		mov		dword ptr[quotient+4], eax

		mov		eax, dword ptr[edi]
		div		ecx
		mov		dword ptr[quotient], eax
		
		// Copy the quotient to the result storage (var128)
		mov		eax, dword ptr[quotient+12]
		mov		dword ptr[edi+12], eax
		mov		eax, dword ptr[quotient+8]
		mov		dword ptr[edi+8], eax
		mov		eax, dword ptr[quotient+4]
		mov		dword ptr[edi+4], eax
		mov		eax, dword ptr[quotient]
		mov		dword ptr[edi], eax
		// To sign correction and return
		jmp		applySign

fullDIV:
		// Full 128/64 bit division
		xor		eax, eax
		mov		dword ptr[REMAINDER+12], eax
		mov		dword ptr[REMAINDER+8], eax
		mov		dword ptr[REMAINDER+4], eax
		mov		dword ptr[REMAINDER], eax

		mov		ecx, 128
loop1:
		// Compute REMAINDER:QUOTIENT = REMAINDER:QUOTIENT shl 1
		shl		dword ptr[QUOTIENT], 1
		rcl		dword ptr[QUOTIENT+4], 1
		rcl		dword ptr[QUOTIENT+8], 1
		rcl		dword ptr[QUOTIENT+12], 1
		rcl		dword ptr[REMAINDER], 1
		rcl		dword ptr[REMAINDER+4], 1
		rcl		dword ptr[REMAINDER+8], 1
		rcl		dword ptr[REMAINDER+12], 1

		// Test (REMAINDER >= Divider)
		xor		eax, eax
		cmp		dword ptr[REMAINDER+12], eax
		ja		iftrue
		jb		iffalse

		cmp		dword ptr[REMAINDER+8], eax
		ja		iftrue
		jb		iffalse

		mov		eax, dword ptr[REMAINDER+4]
		cmp		eax, dword ptr[divider+4]
		ja		iftrue
		jb		iffalse

		mov		eax, dword ptr[REMAINDER]
		cmp		eax, dword ptr[divider]
		jb		iffalse
iftrue:
		// Remainder = remainder - divider
		mov		eax, dword ptr[divider]
		sub		dword ptr[REMAINDER], eax
		mov		eax, dword ptr[divider+4]
		sbb		dword ptr[REMAINDER+4], eax
		xor		eax, eax
		sbb		dword ptr[REMAINDER+8], eax
		sbb		dword ptr[REMAINDER+12], eax
		// Quotient = quotient +1
		add		dword ptr[QUOTIENT], 1
		adc		dword ptr[QUOTIENT+4], 0
		adc		dword ptr[QUOTIENT+8], 0
		adc		dword ptr[QUOTIENT+12], 0
iffalse:
		// Loop size = 101 bytes, is less than 127 so loop is possible
		loop	loop1

applySign:
		// Correct the sign of the result based on the stored combined sign
		popfd
		jns		storeRes
		not		dword ptr[edi+12]
		not		dword ptr[edi+ 8]
		not		dword ptr[edi+ 4]
		not		dword ptr[edi]
		add		dword ptr[edi], 1
		adc		dword ptr[edi+ 4], 0
		adc		dword ptr[edi+ 8], 0
		adc		dword ptr[edi+12], 0

storeRES:
		// Get low order qword from var128
		mov		edx, dword ptr[edi+4]
		mov		eax, dword ptr[edi]
done:
	}
	// result is returned in edx:eax
}

/*
 * MulShr64
 * Multiplies an operant by a multiplier and right shifts the result rshift times
 * Used for scaling 64 bit integer values
 *     Xscaled = (Xstart * Multiplier) SHR rshift
 * Uses 128 bit intermediate result
 */
__int64 _stdcall MulShr64(__int64 operant, __int64 multiplier, unsigned char rshift)
{
	// Declare 128bit storage
	struct{
		unsigned long DW[4];
	}var128;

	// Save combined sign on stack
	_asm{
		mov		eax, dword ptr[operant+4]
		xor		eax, dword ptr[multiplier+4]
		pushfd
	}

	// Take absolute values because algorithm is for unsigned only
	operant		= ABS64(operant);
	multiplier	= ABS64(multiplier);

	_asm{
		// Test rshift for >128
		mov		al, byte ptr[rshift]
		cmp		al, 80
		jl		shiftOK
		popfd									// cleanup stack
		xor		edx, edx
		xor		eax, eax
		jmp		done
shiftOK:
		lea		edi,[var128]					// edi = &var128
		// Check multiplier for 1 or 0
		xor		eax, eax
		cmp		eax, dword ptr[multiplier+4]
		jnz		startMUL
		cmp		eax, dword ptr[multiplier]
		jnz		multiNotNUL
		xor		edx, edx
		popfd									// cleanup stack
		jmp		done
multiNotNUL:
		// Set result HI part to 0
		xor		eax,eax
		mov		dword ptr[edi+12], eax
		mov		dword ptr[edi+8], eax
		mov		eax, 1
		cmp		eax, dword ptr[multiplier]
		jnz		smallMUL
		// Multiplier is 1 so just copy operant to result
		mov		eax, dword ptr[operant+4]
		mov		dword ptr[edi+4], eax
		mov		eax, dword ptr[operant]
		mov		dword ptr[edi], eax
		jmp		startDIV
smallMUL:
		// Test for 32/32 bit multiplication
        xor		eax, eax
        mov		ecx, dword ptr[operant+4]
        or      ecx, eax         ;test for both hiwords zero.
		jnz		startMUL
		// Do 32/32 bit multiplication
        mov		ecx, dword ptr[multiplier]
		mov		eax, dword ptr[operant]
		mul		ecx
		mov		dword ptr[edi+4], edx
		mov		dword ptr[edi], eax
		jmp		startDIV
startMUL:
		// Check signs
		// Multiply: var128 = operant * multiplier
		mov		eax, dword ptr[multiplier]		// eax = LO(multiplier)
		mul		dword ptr[operant]				// edx:eax = eax * LO(operant)
		mov		dword ptr[edi], eax				// var128.DW0 = eax
		mov		ecx, edx						// ecx = edx

		mov		eax, dword ptr[multiplier]		// eax = LO(multiplier)
		mul		dword ptr[operant+4]			// edx:eax = eax * HI(operant)
		add		eax, ecx						// eax = eax + ecx
		adc		edx, 0							// edx = edx + 0 + carry
		mov		ebx, eax
		mov		ecx, edx

		mov		eax, dword ptr[multiplier+4]
		mul		dword ptr[operant]
		add		eax, ebx
		mov		dword ptr[edi+4], eax
		adc		ecx, edx
		pushfd

		mov		eax, dword ptr[multiplier+4]
		mul		dword ptr[operant+4]
		popfd
		adc		eax, ecx
		adc		edx, 0
		mov		dword ptr[edi+8], eax
		mov		dword ptr[edi+12], edx
startDIV:
		// Divide: var128 = var128 / (2^rshift)
		//
		xor		eax, eax
		mov		al, byte ptr[rshift]
		cmp		al, 0
		jz		applySign

		// Start 128bit right shift
		//
		// Test shift for multiples of 32
		mov		cl, 0x20
		div		cl
		mov		cl, al						// Store number of 32 blocks in counter
		mov		char ptr[rshift], ah		// Store remaining number of shifts
		// Test shift not equal or larger than 4*32 already done at the begining
		// Do dword shift cl times (max = 3)
		xor		ch, ch
		xor		edx, edx
		jcxz	bitShift
		mov		eax, dword ptr[edi+4]
		mov		dword ptr[edi], eax
		mov		eax, dword ptr[edi+8]
		mov		dword ptr[edi+4], eax
		mov		eax, dword ptr[edi+12]
		mov		dword ptr[edi+8], eax
		mov		dword ptr[edi+12], edx
		dec		cx
		jcxz	bitShift
		mov		eax, dword ptr[edi+4]
		mov		dword ptr[edi], eax
		mov		eax, dword ptr[edi+8]
		mov		dword ptr[edi+4], eax
		mov		dword ptr[edi+8], edx
		dec		cx
		jcxz	bitShift
		mov		eax, dword ptr[edi+4]
		mov		dword ptr[edi], eax
		mov		dword ptr[edi+4], edx

bitShift:
		// Do multiple precision bitshift
		mov		cl, byte ptr[rshift]
		mov		eax, dword ptr[edi+4]
		shrd	dword ptr[edi], eax, cl
		mov		eax, dword ptr[edi+8]
		shrd	dword ptr[edi+4], eax, cl
		mov		eax, dword ptr[edi+12]
		shrd	dword ptr[edi+8], eax, cl
		// To sign correction and return

applySign:
		// Correct the sign of the result based on the stored combined sign
		popfd
		jns		storeRes
		not		dword ptr[edi+12]
		not		dword ptr[edi+ 8]
		not		dword ptr[edi+ 4]
		not		dword ptr[edi]
		add		dword ptr[edi], 1
		adc		dword ptr[edi+ 4], 0
		adc		dword ptr[edi+ 8], 0
		adc		dword ptr[edi+12], 0

storeRES:
		// Get low order qword from var128
		mov		edx, dword ptr[edi+4]
		mov		eax, dword ptr[edi]
done:
	}
	// result is returned in edx:eax
}
