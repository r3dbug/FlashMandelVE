
; d0 = returns value
; 1 = V600
; 2 = V500
; 3 = V4_500 (Firebird)
; 4 = V4_1200 (Icedrake)
; 5 = V4SA
; 6 = V1200

    XREF    _detectvamp
    
_detectvamp:

    movem.l d1,-(sp)
    clr.l   d1
    movec   pcr,d1
    tst.l   d1
    bne     .pcr        ; pcr present = 68060 or 68080 processor
    movem.l (sp)+,d1
    clr.l   0
    rts
    
.pcr:
    move.w  $dff3fc,d0
    asr.l   #8,d0
    movem.l (sp)+,d1
    rts