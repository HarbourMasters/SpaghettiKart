.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"


.section .text, "ax"

glabel __osGetSR
    mfc0  $v0, C0_SR
    jr    $ra
     nop

    nop

