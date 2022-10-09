***********************************************************************************************
**            Written by Dino Papararo            08-Sep-2000                                 *
**            Vampire optimizations by RedBug     September 2022							  *
**                                                                                			  *
**  FUNCTION                                                                                  *
**                                                                                            *
**    Julia -- perform Z = Z^2 + C iteration.                                                 *
**                                                                                            *
**  SYNOPSIS                                                                                  *
**                                                                                            *
**    WORD Julia68k_FPU (WORD Iterations,long double Cre,long double Cim)                     *
**                                                                                            *
**                                                                                            *
**  DESCRIPTION                                                                               *
**                                                                                            *
**  C equivalent function:                                                                    *
**                                                                                            *
**      ***************************************************************************************
**      *WORD Julia (WORD Iterazioni,double Cre,double Cim,const double Jim,cont double Jre)***
**      *{                                                                                  ***
**      *register long double zr,zi,zi2,dist,maxdist;                                       ***
**      *                                                                                   ***
**      *  zi = Cim;                                                                        ***
**      *                                                                                   ***
**      *  zr = Cre;                                                                        ***
**      *                                                                                   ***
**      *  maxdist = 4;                                                                     ***
**      *                                                                                   ***
**      *  do {                                                                             ***
**      *       zi2 = zi;                                                                   ***
**      *                                                                                   ***
**      *       zi  *= zr;                                                                  ***
**      *                                                                                   ***
**      *       zr  *= zr;                                                                  ***
**      *                                                                                   ***
**      *       zi2 *= zi2;                                                                 ***
**      *                                                                                   ***
**      *       dist = zr;                                                                  ***
**      *                                                                                   ***
**      *       dist += zi2;                                                                ***
**      *                                                                                   ***
**      *       if (dist > maxdist) return Iterazioni;                                      ***
**      *                                                                                   ***
**      *       zi += zi;                                                                   ***
**      *                                                                                   ***
**      *       zr -= zi2;                                                                  ***
**      *                                                                                   ***
**      *       zi += Jim;                                                                  ***
**      *                                                                                   ***
**      *       zr += Jre;                                                                  ***
**      *                                                                                   ***
**      *     } while (-- Iterazioni);                                                      ***
**      *                                                                                   ***
**      *  return 0;                                                                        ***
**      *}                                                                                  ***
**      ***************************************************************************************
**                                                                                            *
**  This function tests if a point belongs or not at Julia set                                *
**                                                                                            *
**  Optimized for pipelines of 68882+ coprocessors                                            *
**                                                                                            *
**  NOTICE: ALL VARIABLES ARE INTO REGISTERS FOR FULL SPEEEED                                 *
**                                                                                            *
**  d0:Iterations                                                                             *
**                                                                                            *
**  fp0:Cre/Jre fp1:Cim/Jim fp2:Zr fp3:Zi fp4:Zi2 fp5:Dist fp6:Tmp/MaxDist fp7:Tmp            *
***********************************************************************************************

** Compile with VASM: vasm -m68080 -Fhunk Julia.s -o Julia.o

*        MACHINE  68060 / Vampire 68080

		; single point calculations
        XDEF  _Julia68k_FPU_Vampire				; accelerated for Vampire
		XDEF  _Julia68k_FPU_Classic				; original _Julia68k_FPU_Classic (68881/2)
		
		; parallelized function (calculate 4 points in 1 loop)
		XDEF  _juliapar4iterateasmhoriz			; for JHLine_Vampire()
		XDEF  _juliapar4iterateasmvert			; for JVLine_Vampire()
		
_juliapar4iterateasmhoriz:
	; parameters
	; fp0/fp1: x1, y1 (point 1)	-- regs: e2-e6	(=0)
	; fp3/fp4/fp5: x2, x3, x4 (x-coordinate points 2-4)
	; fp6/fp7: x3, cx,cy (julia constant)
	; d1: MaxIter
	
	; prepare
	fmovem  fp0-fp7,-(sp)
	movem.l d0-d3,-(sp)
	clr.l   d0			; d0: mark points as "not done" (0)
	; start
	move.w  #0,_IterP1	; default value = 0
	fmove   fp1,e3		; i (y1)
	fmove   fp0,e2		; r (x1)
	; ------ p2
	move.w  #0,_IterP2
	fmove   fp1,e8		; y2
	fmove   fp2,e7		; x2
	; ------ p3
	move.w  #0,_IterP3
	fmove   fp1,e13		; y3
	fmove   fp3,e12		; x3
	; ------ p4
	fmove   fp1,e18		; y4
	fmove   fp4,e17		; x4
	move.w  #0,_IterP4
	
.loop4:
	; (1)
	fmul    e2,e2,e4	; r2=r*r
	fmul    e3,e3,e5	; i2=i*i
	; ------ p2
	fmul    e7,e7,e9
	fmul    e8,e8,e10
	; ------ p3
	fmul    e12,e12,e14
	fmul    e13,e13,e15
	; ------ p4
	fmul    e17,e17,e19
	fmul    e18,e18,e20

	; (2)
	fadd    e4,e5,e6    ; r2+i2
	fmul    e2,e3		; r*i
	; ------ p2
	fadd	e9,e10,e11
	fmul    e7,e8    
	; ------ p3
	fadd	e14,e15,e16
	fmul    e12,e13    
	; ------ p4
	fadd	e19,e20,e21
	fmul    e17,e18    

	; (3)
	; check if points are done
	btst.l  #0,d0		; p1 done?
	bne   	.contp1		; yes => continue with p2
	fcmp.s 	#4,e6		; p1>=4?
	fblt    .contp1		; no => continue
	or.l    #%0001,d0		; mark p1 as done
	move.w  d1,_IterP1	; store iterations for p1
.contp1:
	; ------ p2
	btst.l  #1,d0		; p2 done?
	bne   	.contp2		; yes => continue with calculation
	fcmp.s 	#4,e11		; p2>4?
	fblt    .contp2		; no => continue
	or.l    #%0010,d0		; mark p2 as done
	move.w  d1,_IterP2	; store iterations for p2
.contp2:
	; ------ p3
	btst.l  #2,d0		; p3 done?
	bne   	.contp3		; yes => continue with calculation
	fcmp.s  #4,e16		; p3>4?
	fblt    .contp3		; no => continue
	or.l    #%0100,d0		; mark p4 as done
	move.w  d1,_IterP3	; store iterations for p3
.contp3:
	; ------ p4

	btst.l  #3,d0		; p4 done?
	bne   	.contp4		; yes => continue with calculation
	fcmp.s  #4,e21		; p4>4?
	fblt    .contp4		; no => continue
	or.l    #%1000,d0		; mark p4 as done
	move.w  d1,_IterP4	; store iterations for p4
.contp4:

	; (4) => squeeze these instruction in some latencies above!
	fsub    e5,e4,e2	; r2-i2
	fadd    e3,e3		; 2*r*i
	; ------ p2
	fsub    e10,e9,e7
	fadd    e8,e8
	; ------ p3
	fsub    e15,e14,e12
	fadd    e13,e13
	; ------ p4
	fsub    e20,e19,e17
	fadd    e18,e18

	; (5)
	fadd    fp6,e2		; r=r2-i2+cx
	fadd    fp7,e3		; i=2*r*i+cy
	; ------ p2
	fadd    fp6,e7
	fadd    fp7,e8
	; ------ p3
	fadd    fp6,e12
	fadd    fp7,e13
	; ------ p4
	fadd    fp6,e17
	fadd    fp7,e18
	
	; ------
	; [
	; "early" exit in between
	cmp.l   #%1111,d0
	beq     .4exit
	; ]
	dbra.w  d1,.loop4
	
.4exit:
	movem.l (sp)+,d0-d3
	fmovem  (sp)+,fp0-fp7
	rts


_juliapar4iterateasmvert:
	; parameters
	; fp0/fp1: x1, y1 (point 1)	-- regs: e2-e6	(=0)
	; fp3/fp4/fp5: y2, y3, y4 (x-coordinate points 2-4)
	; fp6/fp7: x3, cx,cy (julia constant)
	; d1: MaxIter
	
	; prepare
	fmovem  fp0-fp7,-(sp)
	movem.l d0-d3,-(sp)
	clr.l   d0			; d0: mark points as "not done" (0)
	; start
	move.w  #0,_IterP1	; default value = 0
	fmove   fp1,e3		; i (y1)
	fmove   fp0,e2		; r (x1)
	; ------ p2
	move.w  #0,_IterP2
	fmove   fp2,e8		; y2
	fmove   fp0,e7		; x2
	; ------ p3
	move.w  #0,_IterP3
	fmove   fp3,e13		; y3
	fmove   fp0,e12		; x3
	; ------ p4
	fmove   fp4,e18		; y4
	fmove   fp0,e17		; x4
	move.w  #0,_IterP4
	
.loop4:
	; (1)
	fmul    e2,e2,e4	; r2=r*r
	fmul    e3,e3,e5	; i2=i*i
	; ------ p2
	fmul    e7,e7,e9
	fmul    e8,e8,e10
	; ------ p3
	fmul    e12,e12,e14
	fmul    e13,e13,e15
	; ------ p4
	fmul    e17,e17,e19
	fmul    e18,e18,e20

	; (2)
	fadd    e4,e5,e6    ; r2+i2
	fmul    e2,e3		; r*i
	; ------ p2
	fadd	e9,e10,e11
	fmul    e7,e8    
	; ------ p3
	fadd	e14,e15,e16
	fmul    e12,e13    
	; ------ p4
	fadd	e19,e20,e21
	fmul    e17,e18    

	; (3)
	; check if points are done
	btst.l  #0,d0		; p1 done?
	bne   	.contp1		; yes => continue with p2
	fcmp.s 	#4,e6		; p1>=4?
	fblt    .contp1		; no => continue
	or.l    #%0001,d0		; mark p1 as done
	move.w  d1,_IterP1	; store iterations for p1
.contp1:
	; ------ p2
	btst.l  #1,d0		; p2 done?
	bne   	.contp2		; yes => continue with calculation
	fcmp.s 	#4,e11		; p2>4?
	fblt    .contp2		; no => continue
	or.l    #%0010,d0		; mark p2 as done
	move.w  d1,_IterP2	; store iterations for p2
.contp2:
	; ------ p3
	btst.l  #2,d0		; p3 done?
	bne   	.contp3		; yes => continue with calculation
	fcmp.s  #4,e16		; p3>4?
	fblt    .contp3		; no => continue
	or.l    #%0100,d0		; mark p4 as done
	move.w  d1,_IterP3	; store iterations for p3
.contp3:
	; ------ p4

	btst.l  #3,d0		; p4 done?
	bne   	.contp4		; yes => continue with calculation
	fcmp.s  #4,e21		; p4>4?
	fblt    .contp4		; no => continue
	or.l    #%1000,d0		; mark p4 as done
	move.w  d1,_IterP4	; store iterations for p4
.contp4:

	; (4) => squeeze these instruction in some latencies above!
	fsub    e5,e4,e2	; r2-i2
	fadd    e3,e3		; 2*r*i
	; ------ p2
	fsub    e10,e9,e7
	fadd    e8,e8
	; ------ p3
	fsub    e15,e14,e12
	fadd    e13,e13
	; ------ p4
	fsub    e20,e19,e17
	fadd    e18,e18

	; (5)
	fadd    fp6,e2		; r=r2-i2+cx
	fadd    fp7,e3		; i=2*r*i+cy
	; ------ p2
	fadd    fp6,e7
	fadd    fp7,e8
	; ------ p3
	fadd    fp6,e12
	fadd    fp7,e13
	; ------ p4
	fadd    fp6,e17
	fadd    fp7,e18
	
	; ------
	; [
	; "early" exit in between
	cmp.l   #%1111,d0
	beq     .4exit
	; ]
	dbra.w  d1,.loop4
	
.4exit:
	movem.l (sp)+,d0-d3
	fmovem  (sp)+,fp0-fp7
	rts
		
; ********************************
; ********************************
; ********************************

_Julia68k_FPU_Vampire:
	fmove   fp1,e3		; i
	fmove   fp0,e2		; r

.loop9:
	; (1)
	fmul    e2,e2,e4	; r2=r*r
	fmul    e3,e3,e5	; i2=i*i
	; (2)
	fadd    e4,e5,e6    ; r2+i2
	fmul    e2,e3		; do this instruction here (for free:)
	; (3)
	fcmp.s  #4,e6		; >=4?
	fbgt    .exit9
	; (4)
	fsub    e5,e4,e2
	fadd    e3,e3
	; (5)
	fadd    fp6,e2		; r=r2-i2+x
	fadd    fp7,e3		; i=2*r*i+y
	
	dbra    d0,.loop9
	clr.l   d0
	rts
	
.exit9:
.end9	
	; d0 = result
	rts
	
_Julia68k_FPU_tested:

.loop:
        fmul.x  fp1,fp1,fp4
        fmul.x  fp0,fp1         * zi   =  zr * zi
        fmul.x  fp0,fp0         * zr   =  zr * zr
        
        fadd.x  fp0,fp4,fp5
        fcmp.s  #4,fp5          * cmp  MaxDist  dist
        fbgt.w  .exit           * if   dist > MaxDist  exit
        fadd.x  fp1,fp1         * zi   += zi
        fsub.x  fp4,fp0         * zr   -= zi2
        fadd.x  fp7,fp1         * zi   += Jim
        fadd.x  fp6,fp0         * zr   += Jre

        dbra.w  d0,.loop        * if --Iterations go to Loop
        ;moveq.l #0,d0           * Iterations = 0
        clr.l   d0
.exit:
        rts                     * return Iterations

; ***************** original routine
_Julia68k_FPU_Classic:
        
        fmove.x fp0,fp2  * Zr = Cre
        fmove.x fp1,fp3  * Zi = Cim
        fmove.x fp6,fp0  * Moving Julia real constant to fp0
        fmove.x fp7,fp1  * Moving Julia imag constant to fp1
        fmove.x #4,fp6   * MaxDist = 4
Loop:

        fmove.x fp3,fp4  * zi2  =  zi
        fmul.x  fp2,fp3  * zi   =  zr * zi
        fmul.x  fp2,fp2  * zr   =  zr * zr
        fmul.x  fp4,fp4  * zi2 *=  zi
        fmove.x fp2,fp5  * dist =  zr
        fadd.x  fp4,fp5  * dist += zi2
        fcmp.x  fp6,fp5  * cmp  MaxDist  dist
        fbgt.w  Exit     * if   dist > MaxDist  exit
        fadd.x  fp3,fp3  * zi   += zi
        fsub.x  fp4,fp2  * zr   -= zi2
        fadd.x  fp1,fp3  * zi   += Jim
        fadd.x  fp0,fp2  * zr   += Jre

        dbra.w  d0,Loop  * if --Iterations go to Loop
        moveq.l #0,d0    * Iterations = 0

Exit:

        rts  * return Iterations

        end
