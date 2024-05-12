
; d0 = returns value
; 0 = no Vampire
; 1 = V600 (V2)
; 2 = V500 (V2)
; 3 = V4_500 (Firebird)
; 4 = V4_1200 (Icedrake)
; 5 = V4SA / V4SA+ (Standalone)
; 6 = V1200 (V2)
; 7 = V4_600 (Manticore)

    XDEF    _detectvamp
    
_detectvamp:
    move.l   #4,a0
    move.w   $128(a0),d0    ; ExecBase->AttnFlags
    cmp.w    #10,d0         ; AFB_68080 = 10
    beq.s    .no68080

.is68080:
    move.w   $dff3fc,d0
    asr.l    #8,d0
    rts
    
.no68080:
    clr.l    d0
    rts