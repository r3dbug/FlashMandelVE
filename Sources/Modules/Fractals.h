
#ifndef FRACTALS_H
#define FRACTALS_H

IMPORT UWORD ASMCALL BuddhaIterationsVampire(REG(d0,UWORD),REG(fp0,double),REG(fp1,double),REG(a0,double*),REG(a1,double*));
IMPORT ULONG ASMCALL MultiMandelInASM(REG(d0,ULONG),REG(fp0,double),REG(d1,double));
IMPORT ULONG ASMCALL MultiJuliaInASM(REG(d0,ULONG),REG(fp0,volatile double),REG(d1,volatile double));
IMPORT ULONG ASMCALL BurningShipInASM(REG(d0,ULONG),REG(fp0,double),REG(d1,double));
IMPORT ULONG ASMCALL BurningShipInASMJulia(REG(d0,ULONG),REG(fp0,double),REG(fp1,double)); /* d1 ?!? as last argument */
IMPORT ULONG ASMCALL MultiMandelFloatingPowerInASM(REG(d0,ULONG),REG(fp0,double),REG(fp1,double));
IMPORT ULONG ASMCALL MultiJuliaFloatingPowerInASM(REG(d0,ULONG),REG(fp0,double),REG(fp1,double));

IMPORT ULONG ASMCALL FindMaxCounters(REG(a0,ULONG*),REG(d0,ULONG),REG(d1,ULONG),REG(a1,ULONG*),REG(a2,ULONG*),REG(a3,ULONG*));
IMPORT ULONG ASMCALL StoreIterationsCoreASM(REG(a0,double*),REG(a1,double*),REG(a2,ULONG*),REG(d0,ULONG),REG(d1,ULONG),REG(fp0,double),REG(fp1,double),REG(d2,ULONG),REG(d3,ULONG));
IMPORT ULONG ASMCALL GetCyclePeriodASM(REG(a0,double*),REG(a1,double*),REG(d2,ULONG),REG(fp0,double));

IMPORT void ASMCALL MultiTaskOff(void);
IMPORT void ASMCALL MultiTaskOn(void);

#endif /* FRACTALS_H */