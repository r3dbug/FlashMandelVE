
#ifndef VERSION_H
#define VERSION_H

/* Version UWORD (from higher to lower):
 *
 * Bits 15-9: year (0-127) - add 2000 to get the actual year
 *       8-5: month (0-15)
 *       4-0: day (0-31)
 *
 * Numbering convention (for day value):
 *
 * 0		Alpha (= contains new experimental features, can crash the system or outcome uncertain)
 * odd  	Beta (= new, more or less tested features, "public" beta)
 * pair     Stable (= stable "as can be", but not yet an official public release)
 *
 * Official public releases will be marked as such directly in the source code "hardcoded".
 * The above numbering code is simply used as an "adhoc" marker for "work in progress" versions.
 * Official releases have two numbers separated by a point: Year.Month
 *
 */

#include <exec/types.h>
 
UWORD PackVersionWord(ULONG, ULONG, ULONG);
UBYTE GetYear(UWORD);
UBYTE GetMonth(UWORD);
UBYTE GetDay(UWORD);
BOOL IsBeta(UWORD);

#endif