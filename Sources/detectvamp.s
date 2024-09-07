
; d0 = returns value
; 0 = no Vampire
; 1 = V600 (V2)
; 2 = V500 (V2)
; 3 = V4_500 (Firebird)
; 4 = V4_1200 (Icedrake)
; 5 = V4SA / V4SA+ (Standalone)
; 6 = V1200 (V2)
; 7 = V4_600 (Manticore)
; What the heck is Kraken? ;)

    XDEF    _detectvamp
	XDEF    _detectm68k
;	XDEF    _get_dcache_hits
;   XDEF    _get_dcache_miss
;   XDEF    _get_icache_hits
;   XDEF    _get_icache_miss
;   XDEF    _get_mips1
;   XDEF    _get_mips2
    XDEF 	_SetStart
	XDEF 	_SetStop
	XDEF 	_GetStart
	XDEF 	_GetStop
    XDEF 	_SetVampireTaskSwitching

    XDEF  _Has3OperantFPU
        		
_detectvamp:
    move.l   $4.w,a0
    move.w   $128(a0),d0    	; ExecBase->AttnFlags
    
    ; check for Vampire
    btst.l   #10,d0         	; AFB_68080 = bit 10
    beq.s    .no68080
    
.is68080:
    clr.l	 d0
    move.w   $dff3fc,d0
    asr.l    #8,d0
    rts
    
.no68080:
    clr.l    d0
    rts  

_detectm68k:
    move.l   $4.w,a0
    move.w   $128(a0),d0    	; ExecBase->AttnFlags

	btst     #7,d0				; 68060?
    beq      .68060	
    btst     #3,d0				; 68040?
    beq      .68040		  
    btst     #2,d0				; 68030?
    beq      .68030		  
    btst     #1,d0				; 68020?
    beq      .68020		  
    
	clr.l	 d0
    rts
    
.68020:
	move.l   #68020,d0
	rts
    
.68030:
	move.l   #68030,d0
	rts
    
.68040:
	move.l   #68040,d0
	rts
    
.68060:
	move.l   #68040,d0
	rts
    

; all cache hit/miss functions disabled

;_get_dcache_hits:
;	dc.w	$4e7a,$080e			; movec DCH,d0    ; $80e
;	rts
    
;_get_dcache_miss:
;	dc.w    $4e7a,$080f			; movec DCM,d0    ; $80f
;	rts
    
;_get_icache_hits:				; "ICH" does not exist => read mips1 instead!
;_get_mips1:
;	dc.w 	$4e7a,$080a			; mmovec $80a,d0
;	rts
    
;_get_icache_miss:
;	dc.w 	$4e7a,$0814			; movec ICM,d0    ; $814
;	rts
    
;_get_mips2:
;	dc.w 	$4e7a,$080b	
;	rts

; timing functions

_SetStart:
	movec   CCC,d0				; CCC = Clock Cycle Count register <=> dc.w $4e7a,$0809
	move.l  d0,time_start		; write CCC to time_start (via d0)
	rts
	
_SetStop:
	movec   CCC,d0
	move.l  d0,time_stop		; idem for time_stop
	rts

_GetStart:
	move.l  time_start,d0		; read variable time_start and return it in d0
	rts

_GetStop:
	move.l  time_stop,d0		; read variable time_stop and return it in d0
	rts

_SetVampireTaskSwitching:
; set bit 11 ("Apollo bit") in SR in order to signal
; exec that ALL register (= "classic" ones plus new
; b0-b7 and e0-e23 registers) should be saved during
; a task switch
    ori     #%100000000000,SR
    rts

; V2: cores <=2.17 do not support 3-operant instructions
; this code must be compiled with vasm19a to "work"
; "work" means: a) it doesn´t crash on V2 ("inoffensive code")
; and b) it gives correct results on V4
_Has3OperantFPU:
	dc.w	$F23C,$4400,$4000,$0000					; fmove.s #2,fp0
    dc.w	$71C1,$F23C,$4680,$0000,$0000			; fmove.s #0,e5 
    dc.w	$7141,$F200,$0180				 		; fmove   fp0,e3
	dc.w	$7D45,$F200,$0DA3						; fmul e3,e3,e5 (encoding with vasm 1.9a)
    dc.w    $71C1,$F23C,$46B8,$4080,$0000		    ; fcmp.s  #4,e5
    fbeq    .yes
    clr.l   d0
    rts
.yes
	move.l  #1,d0
	rts

time_start:	dc.l 0
time_stop:	dc.l 0


