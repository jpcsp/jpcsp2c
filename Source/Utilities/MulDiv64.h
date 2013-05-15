/*
 *	Header file:		MulDiv64.h
 *	Author:				Richard van der Wal
 *	Contact:			R.vdWal@xs4all.nl
 *
 *	Description:
 *		Header file with prototype for 64 bit scaling functions
 *
 *	$Log: $
 * 
 */

/******************************************************************************/
#pragma once
#ifndef __FULL_MULDIV64_
#define __FULL_MULDIV64_


/*
 * MulDiv64
 * Multiplies an operant by a multiplier and divides the result by a divider
 * Used for scaling 64 bit integer values
 *     Xscaled = Xstart * Multiplier / Divider
 * Uses 128 bit intermediate result
 */
__int64 _stdcall MulDiv64(__int64 operant, __int64 multiplier, __int64 divider);


/*
 * MulShr64
 * Multiplies an operant by a multiplier and right shifts the result rshift times
 * Used for scaling 64 bit integer values
 *     Xscaled = (Xstart * Multiplier) SHR rshift
 * Uses 128 bit intermediate result
 */
__int64 _stdcall MulShr64(__int64 operant, __int64 multiplier, unsigned char rshift);


#endif //__FULL_MULDIV64_