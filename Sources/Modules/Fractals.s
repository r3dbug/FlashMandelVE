
	XDEF	_BuddhaIterationsClassic
	XDEF	_BuddhaIterationsVampire
	XDEF 	_MultiMandelInASM
	XDEF 	_MultiJuliaInASM
	XDEF	_BurningShipInASM
    XDEF	_BurningShipInASMJulia
    XDEF	_MultiMandelFloatingPowerInASM
	XDEF	_MultiJuliaFloatingPowerInASM

	XDEF    _FindMaxCounters
    XDEF    _StoreIterationsCoreASM
    XDEF    _GetCyclePeriodASM
    
	XDEF 	_MultiTaskOff
	XDEF 	_MultiTaskOn
    
PI			EQU		3.14159265358979323846264338327
PIDIV2  	EQU     1.57079632679489661923132169163
PINEG   	EQU		-3.14159265358979323846264338327
PIDIV2NEG  	EQU     -1.57079632679489661923132169163
   
_BuddhaIterationsClassic:

	    fmovem  fp0-fp7,-(sp)
		fmove.x fp1,fp3  * Zi = Cim                ; fp1/3: y
        fmove.x fp0,fp2  * Zr = Cre                ; fp0/2: x
		fmove.d  _BAILOUT_VALUE,fp6
.loop:
		fmove.d fp2,(a0,d0.w*8) 				       
		fmove.d fp3,(a1,d0.w*8) 
		
        fmove.x fp3,fp4  * zi2  =  zi               ; fp1/3/4: y
        fmul.x  fp2,fp3  * zi   =  zr * zi          ; fp2: x*y
        fmul.x  fp2,fp2  * zr   =  zr * zr          
        fmul.x  fp4,fp4  * zi2 *=  zi
        fmove.x fp2,fp5  * dist =  zr
        fadd.x  fp4,fp5  * dist += zi2
        fcmp.x  fp6,fp5  * cmp  MaxDist  dist
        fbgt.w  .exit    * if   dist > MaxDist  exit
        fadd.x  fp3,fp3  * zi   += zi
        fsub.x  fp4,fp2  * zr   -= zi2
        fadd.x  fp1,fp3  * zi   += Cim
        fadd.x  fp0,fp2  * zr   += Cre

        dbra.w  d0,.loop * if --Iterations go to Loop
        moveq.l #0,d0    * Iterations = 0

.exit:
		fmovem  (sp)+,fp0-fp7
	rts
	
; test
_BuddhaIterationsVampire:
	fmove   fp1,e3		; i
	fmove   fp0,e2		; r
	fmove.d _BAILOUT_VALUE,e0
    
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
	;fcmp.s  #4,e6		; >=4?
	fcmp	e0,e6	
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
		

_MultiMandelInASM:
; fp0: cx=zx
; fp1: cy=zy
; d0: iterations
; d1: _MultiPower (global variable)

    fmove.s #0,e12	; ax
    fmove.s #0,e13	; ay
    sub.w   #1,d0
    fmove.d _BAILOUT_VALUE,e0
    
.i:
    move.l 	_MultiPower,d1
	sub.l  	#2,d1	
    
    fmove  	e12,e2		; ox=ax
    fmove 	e13,e3		; oy=ay

.p:
	fmul	e12,e2,e14	; ax*ox
    fmul    e13,e3,e15	; ay*oy
    fmul	e12,e3,e17	; ax*oy
    fsub    e15,e14,e12 ; xtemp (ax=xtemp)
	fmul	e13,e2,e18	; ay*ox
	fadd	e17,e18,e13	; ay
    
    dbra.w  d1,.p
    
    fadd    fp0,e12
    fadd    fp1,e13

.bailout:  
    fmul    e12,e12,e22	; ax*ax
    fmul    e13,e13,e23 ; ay*ay
    fadd    e22,e23,e7	; ax2+ay2
    ;fcmp.s  #4,e7
    fcmp	e0,e7
    fbgt    .exit
    dbra.w	d0,.i
	clr.l   d0
    
.exit:	
    fmove.d e12,_FinalXP1
    fmove.d e13,_FinalXP2
    
	rts


_MultiJuliaInASM:
; fp0: cx=zx
; fp1: cy=zy
; d0: iterations
; d1: _MultiPower (global variable)

    ;fmove.s #0,e12		; ax
    ;fmove.s #0,e13		; ay
    fmove 	fp0,e12		; ax=zx
    fmove 	fp1,e13		; ay=zy
    fmove.d _JKRE,fp0
    fmove.d _JKIM,fp1
    fmove.d _BAILOUT_VALUE,e0
    
    ;and.l   #$ffff,d0
    sub.w   #1,d0
    
.i:
    move.l 	_MultiPower,d1
	sub.l  	#2,d1	
    
    fmove  	e12,e2		; ox=ax
    fmove 	e13,e3		; oy=ay

.p:
	fmul	e12,e2,e14	; ax*ox
    fmul    e13,e3,e15	; ay*oy
    fmul	e12,e3,e17	; ax*oy
    fsub    e15,e14,e12 ; xtemp (ax=xtemp)
	fmul	e13,e2,e18	; ay*ox
	fadd	e17,e18,e13	; ay
    
    dbra.w  d1,.p
    
    fadd    fp0,e12
    fadd    fp1,e13

.bailout:  
    fmul    e12,e12,e22	; ax*ax
    fmul    e13,e13,e23 ; ay*ay
    fadd    e22,e23,e7	; ax2+ay2
    ;fcmp.s  #4,e7
    fcmp	e0,e7
    fbgt    .exit
    dbra.w	d0,.i
	clr.l   d0
    
.exit:	
    fmove.d e12,_FinalXP1
    fmove.d e13,_FinalXP2
    
	rts


_BurningShipInASM:
; fp0: cx=x
; fp1: y (-cy)
; d0: i

	;and.l   #$ffff,d0
	fmove.s	#0,e2	; xn=0
    fmove.s #0,e3	; yn=0
    fmove.d	_BAILOUT_VALUE,e0
    
    bra.s   .bailout
    
.i:
	fmul    e2,e3,e11	; xn*yn
	fsub    e5,e4,e10	; xn2-yn2
    fadd	e11,e11		; 2.0*xn*yn
    fabs    e11			; fabs(2.0*xn*yn)
    fadd    fp0,e10,e2  ; xn=xtemp=xn2-yn2+cx
    fsub    fp1,e11,e3	; yn=fabs(2.0*xn*yn)+(-cy)

.bailout:
	fmul	e2,e2,e4	; xn*xn
    fmul    e3,e3,e5	; yn*yn
    fadd    e4,e5,e6	; xn2+yn2
    ;fcmp.s	#4,e6
    fcmp	e0,e6
    fbgt	.exit
	dbra.w	d0,.i
    clr.l	d0
    
.exit:
	rts
    
_BurningShipInASMJulia:
; fp0: cx=x
; fp1: y (-cy)
; d0: i

	fmove	fp0,e2	; xn=x
    fmove 	fp1,e3	; yn=y
    fneg    e3		; yn=-y
    
    fmove.d _JKRE,fp0	; cx=JKRE;
    fmove.d _JKIM,fp1	; cy=JKIM
    fmove.d	_BAILOUT_VALUE,e0
    
    ;and.l   #$ffff,d0
    sub.w	#1,d0
    bra.s   .bailout
    
.i:
	fmul    e2,e3,e11	; xn*yn
	fsub    e5,e4,e10	; xn2-yn2
    fadd	e11,e11		; 2.0*xn*yn
    fabs    e11			; fabs(2.0*xn*yn)
    fadd    fp0,e10,e2  ; xn=xtemp=xn2-yn2+cx
    fsub    fp1,e11,e3	; yn=fabs(2.0*xn*yn)+(-cy)

.bailout:
	fmul	e2,e2,e4	; xn*xn
    fmul    e3,e3,e5	; yn*yn
    fadd    e4,e5,e6	; xn2+yn2
    ;fcmp.s	#4,e6
    fcmp	e0,e6
    fbgt	.exit
	dbra.w	d0,.i
    clr.l	d0
    
.exit:
	fmove.d	e2,_FinalXP1
    fmove.d e3,_FinalYP1
    	
    rts

_MultiMandelFloatingPowerInASM:
; fp0: cx=zx
; fp1: cy=zy
; d0: i
	fmovem		fp2-fp3,-(sp)
    	
    fmove		fp0,e12			; cx=zx
    fmove		fp1,e13			; cy=zy
	
    fmove.s 	#0,e2			; ax
    fmove.s     #0,e3			; ay
    
    fmove.d		_BAILOUT_VALUE,e14		; bailout
    fmove.d     _MultiPowerFloat,e15	; p
    fmove       e15,e16
    fdiv.s		#2,e16
    
    ;lea			_re,a0		; re[]
    ;lea			_im,a1		; im[]

	sub.l		#1,d0

.i:
	fmul		e2,e2,e4		; ax*ax
    fmul		e3,e3,e5		; ay*ay
    fadd 		e4,e5,e6		; ax2ay2
    fcmp		e14,e6			; d>BAILOUT_VALUE?
    fbgt		.exit
    
    ;-----------------------------------------------------------------
    ;_atan2FPU
    fdiv    	e2,e3,fp0		; y/x
        
    fcmp.s  	#0,e2			; compare x
	fbgt    	.xgtzero		; x>0 !   (1x)
	fbeq    	.xiszero		; x==0 !  (2x)
		
	fcmp.s  	#0,e3			; compare y
	fbeq    	.yiszero		; y==0 !  (1x)
		
	; x<0
	fcmp.s  	#0,e3			; compare y
	fbgt    	.xltzeroygtzero ; x<0 && y>0  (1x)
	fblt    	.xltzeroyltzero ; x<0 && y<0  (1x)
		
	fmove.s 	#0,fp0
	bra.s		.endatan2

.xltzeroyltzero:
	fatan   	fp0
    fadd.d  	#PINEG,fp0
	bra.s		.endatan2
		
.xltzeroygtzero:
	fatan   	fp0
    fadd.d  	#PI,fp0
	bra.s		.endatan2
				
.yiszero:
	fcmp.s  	#0,e2			; compare x
	fblt    	.y0xltzero		; y==0 && x<0
	fmove.s 	#0,fp0
	bra.s		.endatan2

.y0xltzero:
	fmove.d  	#PINEG,fp0		; return -PI
	bra.s		.endatan2

.xiszero:
	fcmp.s  	#0,e3			; compare y
	fbgt		.ygtzero		; y>0
	fblt    	.yltzero		; y<0
	fmove.s 	#0,fp0
	bra.s		.endatan2
		
.yltzero:
	fmove.d  	#PIDIV2NEG,fp0
	bra.s		.endatan2

.ygtzero:
	fmove.d   	#PIDIV2,fp0
	bra.s		.endatan2
		
.xgtzero:
	fatan   	fp0				; arctan(y/x)	
        
;---------------------------------------------------------------------------
.endatan2:
    ; fp0: atan2(ax,ay)
    
    fmul	    e15,fp0,fp2		; patan2ayax=p*atan2FPU(ax,ay)
    fmove 		fp2,fp3
    
    ;-----------------------------------------------------------------------
    ;_powFPU
	fmove		e6,fp0			; ax2ay2
    flogn		fp0				; log(x)
	fmul    	e16,fp0			; b * log(x)
    fetox   	fp0				; result = e ^ (b * log(x))
	;-----------------------------------------------------------------------
   
   	; fp0: powvar 
    fcos		fp2				; cos(patan2ayax)
    fsin		fp3				; sin(patan2ayax)
    
    fmul		fp0,fp2			; powvar*cos(patan2ayax)
    fmul		fp0,fp3			; powvar*sin(patan2ayax)
    
	fadd        fp2,e12,e2		; ax (+cx)
    fadd        fp3,e13,e3		; ay (+cy)

	;fmove.d     e2,(a0)+
    ;fmove.d     e3,(a1)+

	dbra.w		d0,.i
    clr.l		d0    
    
	fmove.d		e2,_FinalXP1
    fmove.d		e3,_FinalXP2   
    
.exit:
    
    fmovem		(sp)+,fp2-fp3
    rts


; *****************************************  

_MultiJuliaFloatingPowerInASM:
; fp0: zx
; fp1: zy
; d0: i
	fmovem		fp2-fp3,-(sp)
    	
    fmove.d		_JKRE,e12		; cx
    fmove.d		_JKIM,e13		; cy
    
    fmove	 	fp0,e2			; ax=zx
    fmove	    fp1,e3			; ay=zy
    
    fmove.d		_BAILOUT_VALUE,e14		; bailout
    fmove.d     _MultiPowerFloat,e15	; p
    fmove       e15,e16
    fdiv.s		#2,e16
    
    ;lea			_re,a0		; re[]
    ;lea			_im,a1		; im[]

.i:
	fmul		e2,e2,e4		; ax*ax
    fmul		e3,e3,e5		; ay*ay
    fadd 		e4,e5,e6		; ax2ay2
    fcmp		e14,e6			; d>BAILOUT_VALUE?
    fbgt		.exit
    
    ;-----------------------------------------------------------------
    ;_atan2FPU
    fdiv    	e2,e3,fp0		; y/x
        
    fcmp.s  	#0,e2			; compare x
	fbgt    	.xgtzero		; x>0 !   (1x)
	fbeq    	.xiszero		; x==0 !  (2x)
		
	fcmp.s  	#0,e3			; compare y
	fbeq    	.yiszero		; y==0 !  (1x)
		
	; x<0
	fcmp.s  	#0,e3			; compare y
	fbgt    	.xltzeroygtzero ; x<0 && y>0  (1x)
	fblt    	.xltzeroyltzero ; x<0 && y<0  (1x)
		
	fmove.s 	#0,fp0
	bra.s		.endatan2

.xltzeroyltzero:
	fatan   	fp0
    fadd.d  	#PINEG,fp0
	bra.s		.endatan2
		
.xltzeroygtzero:
	fatan   	fp0
    fadd.d  	#PI,fp0
	bra.s		.endatan2
				
.yiszero:
	fcmp.s  	#0,e2			; compare x
	fblt    	.y0xltzero		; y==0 && x<0
	fmove.s 	#0,fp0
	bra.s		.endatan2

.y0xltzero:
	fmove.d  	#PINEG,fp0		; return -PI
	bra.s		.endatan2

.xiszero:
	fcmp.s  	#0,e3			; compare y
	fbgt		.ygtzero		; y>0
	fblt    	.yltzero		; y<0
	fmove.s 	#0,fp0
	bra.s		.endatan2
		
.yltzero:
	fmove.d  	#PIDIV2NEG,fp0
	bra.s		.endatan2

.ygtzero:
	fmove.d   	#PIDIV2,fp0
	bra.s		.endatan2
		
.xgtzero:
	fatan   	fp0				; arctan(y/x)	
        
;---------------------------------------------------------------------------
.endatan2:
    ; fp0: atan2(ax,ay)
    
    fmul	    e15,fp0,fp2		; patan2ayax=p*atan2FPU(ax,ay)
    fmove 		fp2,fp3
    
    ;-----------------------------------------------------------------------
    ;_powFPU
	fmove		e6,fp0			; ax2ay2
    flogn		fp0				; log(x)
	fmul    	e16,fp0			; b * log(x)
    fetox   	fp0				; result = e ^ (b * log(x))
	;-----------------------------------------------------------------------
   
   	; fp0: powvar 
    fcos		fp2				; cos(patan2ayax)
    fsin		fp3				; sin(patan2ayax)
    
    fmul		fp0,fp2			; powvar*cos(patan2ayax)
    fmul		fp0,fp3			; powvar*sin(patan2ayax)
    
	fadd        fp2,e12,e2		; ax (+cx)
    fadd        fp3,e13,e3		; ay (+cy)

	;fmove.d     e2,(a0)+
    ;fmove.d     e3,(a1)+

	dbra.w		d0,.i
    clr.l		d0    
    
	fmove.d		e2,_FinalXP1
    fmove.d		e3,_FinalXP2   
    
.exit:
    
    fmovem		(sp)+,fp2-fp3
    rts

_FindMaxCounters:
; a0: BuddhaCounters (hist red)
; d0: Width
; d1: Height
; a1: max red
; a2: max green
; a3: max blue

	movem.l d2-d5,-(sp)
    
    subq    #1,d0
    subq    #1,d1
    
	; start with red

.init:
	moveq   #1,d4			; temp max
    move.l  d1,d3

.initw:
    move.l  d0,d2
    
.loop:
	move.l  (a0)+,d5
    cmp.l   d5,d4
    bge.s   .cont
	move.l  d5,d4

.cont:
	dbra.w  d2,.loop	; no dbra.l on non vampire => use 2x dbra.w instead
    dbra.w  d3,.initw

	move.l  d4,(a1)		; write max red / green / blue
    
	; prepare for green    
    cmpa.l   a1,a2
    beq      .blue
    cmpa.l   a1,a3
    beq 	 .done
    
    movea.l  a2,a1
    bra     .init

.blue:
    ; prepare for blue
    cmpa.l   a1,a3
    beq.s   .done
    
    movea.l a3,a1
    bra     .init
    
.done
    movem.l (sp)+,d2-d5
	rts
    
    
_StoreIterationsCoreASM:
; a0/1: re/im
; a2: bc
; d0/1: i,j
; d2: resx
; d3: dosymmetry
; fp0/1: fx/y

	movem.l	d4-d5,-(sp)
    fmovem  fp2-fp4,-(sp)

.loop:
	cmp.l   d0,d1
    beq.s   .exit
    
	fmove.d (a1,d1.l*8),fp3		; tim=im[j]
    fmove.d (a0,d1.l*8),fp2		; tre=re[j]
    
    fcmp.d  _IMIN,fp3			; tim>IMIN?
    fblt    .out
    fcmp.d  _IMAX,fp3			; tim<IMAX?
    fbgt    .out
    fcmp.d  _RMIN,fp2			; tre>RMIN?
    fblt    .out
    fcmp.d  _RMAX,fp2			; tre<RMAX?
    fbgt    .out
    
    ; tim/tre within MAX values
    fmove   fp3,fp4				; sim
   
    fsub.d  _RMIN,fp2
    fsub.d  _IMIN,fp3			
    fmul    fp1,fp2				; rpy = (tre-RMIN)*fy (sic!)
    ;fmul    fp0,fp3				; rpx = (tim-IMIN)*fx (sic!)
    ;fsub.d  _RMIN,fp2			
    fmul    fp0,fp3	
    ;fmul    fp1,fp2				; rpy = (tre-RMIN)*fy (sic!)
    
    fmove.l  fp2,d4
    cmp.l    _DD_HEIGHT,d4		; ADDITIONAL CHECK
    bge.s    .out
    
    mulu.l   d2,d4				; yoffset=rpy*resx
    
 	fmove.l fp3,d5
    cmp.l   _DD_WIDTH,d5		; ADDITIONAL CHECK
    bge.s   .out
    
    add.l   d4,d5				; offset=yoffset + rpx

	add.l   #1,(a2,d5.l*4)		; bc[offset]++

	; symmetry
 	tst.b   d3
    beq.s   .out
    
	fneg    fp4					; sim=-tim
    fsub.d  _IMIN,fp4
    fmul    fp0,fp4				; rpx=(sim-IMIN)*fx
    
    fmove.l fp4,d5
    add.l   d4,d5				; offset=yoffset + rpx
    
    add.l   #1,(a2,d5.l*4)
    
.out:
	dbra.l d1,.loop	

.exit:
    fmovem  (sp)+,fp2-fp4
	movem.l (sp)+,d4-d5
    
	rts

_GetCyclePeriodASM:
; a0/1: re/im
; d2: maxi
; fp0: epsilon
; d1 and fp1 not saved (scratch registers)

	fmovem	fp2-fp4,-(sp)    

	clr.l	d1					; tortoise
    move.l  #1,d0				; hare
    
.loop:
	fmove.d	(a0,d1.l*8),fp1		; tr
    fmove.d (a0,d0.l*8),fp2		; hr
    fmove.d (a1,d1.l*8),fp3		; ti
    fmove.d (a1,d0.l*8),fp4		; hi
    
    fsub	fp2,fp1
    fsub    fp4,fp3
    fabs	fp1					; act_eps_re=fabs(tr-hr)
    fabs    fp3					; act_eps_im=fabs(ti-hi)
    
    fcmp	fp0,fp1
    fbgt    .cont
    fcmp    fp0,fp3
    fbgt    .cont
    
    ; actual epsilon < epsilon
    sub.l   d1,d0				; r=hare-tortoise
    bra.s   .exit
    
.cont:
    addq.l  #2,d0				; hare+=2
    addq.l  #1,d1				; tortoise+=2
    
    cmp.l   d2,d0
    blt     .loop
    
    ; no periodicity found
    clr.l	d0
    
.exit:
	fmovem  (sp)+,fp2-fp4    

	rts
    
         
_MultiTaskOff:
	movem.l d0,-(sp)			; save d0 on stack
	move.w	$dff002,d0			; save dmacon
	or.w	d0,dmacon			; dmacon | $8000 ($8000 = bit 15 set => this is the value MultiTaskOn can write back)
	move.w	#$7fff,$dff096		; all dma off
	move.w	$dff01c,d0			; save intena
	or.w	d0,intena			; intena | $8000 (again: set bit 15 to write value back in MultiTaskOn)
	move.w	#$7fff,$dff09a		; all intena off
	movem.l (sp)+,d0			; restore d0 from stack
	rts
	
_MultiTaskOn:
	move.w	#$7fff,$dff096
	move.w	dmacon,$dff096		; restore old dmacon
	move.w	intena,$dff09a		; restore old intena
	rts

dmacon:		dc.w $8000
intena:		dc.w $8000

