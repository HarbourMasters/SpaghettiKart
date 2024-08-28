# handwritten
# assembler directives
.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"

.section .text, "ax"


glabel osInvalICache
/* 0CE700 800CDB00 18A00011 */  blez  $a1, .L800CDB48
/* 0CE704 800CDB04 00000000 */   nop   
/* 0CE708 800CDB08 240B4000 */  li    $t3, ICACHE_SIZE
/* 0CE70C 800CDB0C 00AB082B */  sltu  $at, $a1, $t3
/* 0CE710 800CDB10 1020000F */  beqz  $at, .L800CDB50
/* 0CE714 800CDB14 00000000 */   nop   
/* 0CE718 800CDB18 00804025 */  move  $t0, $a0
/* 0CE71C 800CDB1C 00854821 */  addu  $t1, $a0, $a1
/* 0CE720 800CDB20 0109082B */  sltu  $at, $t0, $t1
/* 0CE724 800CDB24 10200008 */  beqz  $at, .L800CDB48
/* 0CE728 800CDB28 00000000 */   nop   
/* 0CE72C 800CDB2C 310A001F */  andi  $t2, $t0, ICACHE_LINEMASK
/* 0CE730 800CDB30 2529FFE0 */  addiu $t1, $t1, -ICACHE_LINESIZE
/* 0CE734 800CDB34 010A4023 */  subu  $t0, $t0, $t2
.L800CDB38:
/* 0CE738 800CDB38 BD100000 */  cache C_HINV|CACH_PI, ($t0)
/* 0CE73C 800CDB3C 0109082B */  sltu  $at, $t0, $t1
/* 0CE740 800CDB40 1420FFFD */  bnez  $at, .L800CDB38
/* 0CE744 800CDB44 25080020 */   addiu $t0, $t0, ICACHE_LINESIZE
.L800CDB48:
/* 0CE748 800CDB48 03E00008 */  jr    $ra
/* 0CE74C 800CDB4C 00000000 */   nop   

.L800CDB50:
/* 0CE750 800CDB50 3C088000 */  li    $t0, KUSIZE
/* 0CE754 800CDB54 010B4821 */  addu  $t1, $t0, $t3
/* 0CE758 800CDB58 2529FFE0 */  addiu $t1, $t1, -ICACHE_LINESIZE
.L800CDB5C:
/* 0CE75C 800CDB5C BD000000 */  cache C_IINV|CACH_PI, ($t0)
/* 0CE760 800CDB60 0109082B */  sltu  $at, $t0, $t1
/* 0CE764 800CDB64 1420FFFD */  bnez  $at, .L800CDB5C
/* 0CE768 800CDB68 25080020 */   addiu $t0, $t0, ICACHE_LINESIZE
/* 0CE76C 800CDB6C 03E00008 */  jr    $ra
/* 0CE770 800CDB70 00000000 */   nop   

/* 0CE774 800CDB74 00000000 */  nop   
/* 0CE778 800CDB78 00000000 */  nop   
/* 0CE77C 800CDB7C 00000000 */  nop   
