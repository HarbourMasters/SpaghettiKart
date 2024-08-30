.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"


.section .text, "ax"


glabel __osDisableInt
  mfc0  $t0, C0_SR
  and   $t1, $t0, ~SR_IE
  mtc0  $t1, C0_SR
  andi  $v0, $t0, SR_IE
  nop
  jr    $ra
   nop

