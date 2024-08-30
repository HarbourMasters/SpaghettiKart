# handwritten
# assembler directives
.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"

.section .text, "ax"


glabel bzero
/* 0CE660 800CDA60 28A1000C */  slti  $at, $a1, 0xc
/* 0CE664 800CDA64 1420001D */  bnez  $at, bytezero
/* 0CE668 800CDA68 00041823 */   negu  $v1, $a0
/* 0CE66C 800CDA6C 30630003 */  andi  $v1, $v1, 3
/* 0CE670 800CDA70 10600003 */  beqz  $v1, blkzero
/* 0CE674 800CDA74 00A32823 */   subu  $a1, $a1, $v1
/* 0CE678 800CDA78 A8800000 */  swl   $zero, ($a0)
/* 0CE67C 800CDA7C 00832021 */  addu  $a0, $a0, $v1
blkzero:
/* 0CE680 800CDA80 2401FFE0 */  li    $at, -32
/* 0CE684 800CDA84 00A13824 */  and   $a3, $a1, $at
/* 0CE688 800CDA88 10E0000C */  beqz  $a3, wordzero
/* 0CE68C 800CDA8C 00A72823 */   subu  $a1, $a1, $a3
/* 0CE690 800CDA90 00E43821 */  addu  $a3, $a3, $a0
.L800CDA94:
/* 0CE694 800CDA94 24840020 */  addiu $a0, $a0, 0x20
/* 0CE698 800CDA98 AC80FFE0 */  sw    $zero, -0x20($a0)
/* 0CE69C 800CDA9C AC80FFE4 */  sw    $zero, -0x1c($a0)
/* 0CE6A0 800CDAA0 AC80FFE8 */  sw    $zero, -0x18($a0)
/* 0CE6A4 800CDAA4 AC80FFEC */  sw    $zero, -0x14($a0)
/* 0CE6A8 800CDAA8 AC80FFF0 */  sw    $zero, -0x10($a0)
/* 0CE6AC 800CDAAC AC80FFF4 */  sw    $zero, -0xc($a0)
/* 0CE6B0 800CDAB0 AC80FFF8 */  sw    $zero, -8($a0)
/* 0CE6B4 800CDAB4 1487FFF7 */  bne   $a0, $a3, .L800CDA94
/* 0CE6B8 800CDAB8 AC80FFFC */   sw    $zero, -4($a0)
wordzero:
/* 0CE6BC 800CDABC 2401FFFC */  li    $at, -4
/* 0CE6C0 800CDAC0 00A13824 */  and   $a3, $a1, $at
/* 0CE6C4 800CDAC4 10E00005 */  beqz  $a3, bytezero
/* 0CE6C8 800CDAC8 00A72823 */   subu  $a1, $a1, $a3
/* 0CE6CC 800CDACC 00E43821 */  addu  $a3, $a3, $a0
.L800CDAD0:
/* 0CE6D0 800CDAD0 24840004 */  addiu $a0, $a0, 4
/* 0CE6D4 800CDAD4 1487FFFE */  bne   $a0, $a3, .L800CDAD0
/* 0CE6D8 800CDAD8 AC80FFFC */   sw    $zero, -4($a0)
bytezero:
/* 0CE6DC 800CDADC 18A00005 */  blez  $a1, zerodone
/* 0CE6E0 800CDAE0 00000000 */   nop   
/* 0CE6E4 800CDAE4 00A42821 */  addu  $a1, $a1, $a0
.L800CDAE8:
/* 0CE6E8 800CDAE8 24840001 */  addiu $a0, $a0, 1
/* 0CE6EC 800CDAEC 1485FFFE */  bne   $a0, $a1, .L800CDAE8
/* 0CE6F0 800CDAF0 A080FFFF */   sb    $zero, -1($a0)
zerodone:
/* 0CE6F4 800CDAF4 03E00008 */  jr    $ra
/* 0CE6F8 800CDAF8 00000000 */   nop   

/* 0CE6FC 800CDAFC 00000000 */  nop   
