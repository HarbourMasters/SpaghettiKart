.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"


.section .text, "ax"

glabel __osProbeTLB
    mfc0  $t0, C0_ENTRYHI
    andi  $t1, $t0, TLBHI_PIDMASK
    li    $at, TLBHI_VPN2MASK
    and   $t2, $a0, $at
    or    $t1, $t1, $t2
    mtc0  $t1, C0_ENTRYHI
    nop
    nop
    nop
    tlbp
    nop
    nop
    mfc0  $t3, C0_INX
    lui   $at, %hi(TLBINX_PROBE)
    and   $t3, $t3, $at
    bnez  $t3, .L8032A0D8
     nop
    tlbr
    nop
    nop
    nop
    mfc0  $t3, C0_PAGEMASK
    addi  $t3, $t3, DCACHE_SIZE
    srl   $t3, $t3, 1
    and   $t4, $t3, $a0
    bnez  $t4, .L8032A0A8
     addi  $t3, $t3, -1
    mfc0  $v0, C0_ENTRYLO0
    b     .L8032A0AC
     nop
.L8032A0A8:
    mfc0  $v0, C0_ENTRYLO1
.L8032A0AC:
    andi  $t5, $v0, TLBLO_V
    beqz  $t5, .L8032A0D8
     nop
    lui   $at, (TLBLO_PFNMASK >> 16) # lui $at, 0x3fff
    ori   $at, %lo(TLBLO_PFNMASK) # ori $at, $at, 0xffc0
    and   $v0, $v0, $at
    sll   $v0, $v0, TLBLO_PFNSHIFT
    and   $t5, $a0, $t3
    add   $v0, $v0, $t5
    b     .L8032A0DC
     nop
.L8032A0D8:
    li    $v0, -1
.L8032A0DC:
    mtc0  $t0, C0_ENTRYHI
    jr    $ra
     nop

    nop
    nop

