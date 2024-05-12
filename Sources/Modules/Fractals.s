
	XDEF	_BuddhaIterationsClassic
	XDEF	_BuddhaIterationsVampire
	
	
_BuddhaIterationsClassic:
	    fmovem  fp0-fp7,-(sp)
		fmove.x fp1,fp3  * Zi = Cim                ; fp1/3: y
        fmove.x fp0,fp2  * Zr = Cre                ; fp0/2: x
        fmove.x #4,fp6   * MaxDist = 4
		
.loop:
		;moveq   #0,d1
		;move.w  d0,d1
		fmove.d fp2,(a0,d0.w*8) ;d1.l*8)    ; d0.l*8) ; slower with d0
		fmove.d fp3,(a1,d0.w*8) ; d1.l*8)    ; d0.l*8)
		
        fmove.x fp3,fp4  * zi2  =  zi               ; fp1/3/4: y
        fmul.x  fp2,fp3  * zi   =  zr * zi          ; fp2: x*y
        fmul.x  fp2,fp2  * zr   =  zr * zr          
        fmul.x  fp4,fp4  * zi2 *=  zi
        fmove.x fp2,fp5  * dist =  zr
        fadd.x  fp4,fp5  * dist += zi2
        fcmp.x  fp6,fp5  * cmp  MaxDist  dist
        fbgt.w  .exit     * if   dist > MaxDist  exit
        fadd.x  fp3,fp3  * zi   += zi
        fsub.x  fp4,fp2  * zr   -= zi2
        fadd.x  fp1,fp3  * zi   += Cim
        fadd.x  fp0,fp2  * zr   += Cre

        dbra.w  d0,.loop  * if --Iterations go to Loop
        moveq.l #0,d0    * Iterations = 0

.exit:
		fmovem  (sp)+,fp0-fp7
	rts
	
; test
_BuddhaIterationsVampire:
	fmove   fp1,e3		; i
	fmove   fp0,e2		; r

.loop:
	; (0)
	fmove.d e3,(a1,d0.w*8)
	;fmove.d e2,(a0,d0.w*8)
	; (1)
	fmul    e2,e2,e4	; r2=r*r
	fmul    e3,e3,e5	; i2=i*i
	fmove.d e2,(a0,d0.w*8)
	; (2)
	fadd    e4,e5,e6    ; r2+i2
	fmul    e2,e3		; do this instruction here (for free:)
	; (3)
	fcmp.s  #4,e6		; >=4?
	fbgt    .exit
	; (4)
	fsub    e5,e4,e2
	fadd    e3,e3
	; (5)
	fadd    fp0,e2		; r=r2-i2+x
	fadd    fp1,e3		; i=2*r*i+y
	
	dbra    d0,.loop
	clr.l   d0
	
.exit:
	; d0 = result
	rts
		

_BuddhaIterationsVampire1st:
		fmove 	fp1,e3  * i                ; fp1/3: y
        fmove 	fp0,e2  * r                 ; fp0/2: x
		
.loop:
		fmove.d e2,(a0,d0.w*8) ;d1.l*8)    ; d0.l*8) ; slower with d0
		fmove.d e3,(a1,d0.w*8) ; d1.l*8)    ; d0.l*8)
		
        fmul    e3,e3,e4
		fmul  	e2,e3 
        fmul  	e2,e2        
		fadd	e2,e4,e5
        fcmp.s  #4.0,e5 
        fbgt 	.exit  
        fadd	e3,e3 
        fsub  	e4,e2 
        fadd 	fp1,e3 
        fadd  	fp0,e2 

        dbra.w  d0,.loop  * if --Iterations go to Loop
        moveq.l #0,d0    * Iterations = 0

.exit:
	rts
	
	
	
