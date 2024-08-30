
#ifndef DETECTVAMP_H
#define DETECTVAMP_H

/* Vampire detection */

IMPORT UWORD ASMCALL detectvamp(void);
IMPORT ULONG ASMCALL detectm68k(void);

/* Cache functions */

/*
IMPORT ULONG ASMCALL get_dcache_hits(void);
IMPORT ULONG ASMCALL get_dcache_miss(void);
IMPORT ULONG ASMCALL get_icache_hits(void);
IMPORT ULONG ASMCALL get_icache_miss(void);
IMPORT ULONG ASMCALL get_mips1(void);
IMPORT ULONG ASMCALL get_mips2(void);
*/

/* Timing functions */

IMPORT void ASMCALL SetStart(void);
IMPORT void ASMCALL SetStop(void);
IMPORT ULONG ASMCALL GetStart(void);
IMPORT ULONG ASMCALL GetStop(void);

/* Task switching for e-regs */

IMPORT UWORD ASMCALL SetVampireTaskSwitching(void);

#endif /* DETECTVAMP_H */