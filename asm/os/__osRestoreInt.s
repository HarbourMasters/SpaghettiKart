.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"


.section .text, "ax"

glabel __osRestoreInt
    mfc0  $t0, C0_SR
    or    $t0, $t0, $a0
    mtc0  $t0, C0_SR
    nop
    nop
    jr    $ra
     nop

    nop

