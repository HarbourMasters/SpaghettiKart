# handwritten
# assembler directives
.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"

.section .text, "ax"

glabel osWritebackDCacheAll
/* 0CE490 800CD890 3C088000 */  li    $t0, KUSIZE # $t0, 0x8000
/* 0CE494 800CD894 240A2000 */  li    $t2, DCACHE_SIZE
/* 0CE498 800CD898 010A4821 */  addu  $t1, $t0, $t2
/* 0CE49C 800CD89C 2529FFF0 */  addiu $t1, $t1, -DCACHE_LINESIZE
.L800CD8A0:
/* 0CE4A0 800CD8A0 BD010000 */  cache C_IWBINV | CACH_PD, ($t0)
/* 0CE4A4 800CD8A4 0109082B */  sltu  $at, $t0, $t1
/* 0CE4A8 800CD8A8 1420FFFD */  bnez  $at, .L800CD8A0
/* 0CE4AC 800CD8AC 25080010 */   addiu $t0, $t0, DCACHE_LINESIZE
/* 0CE4B0 800CD8B0 03E00008 */  jr    $ra
/* 0CE4B4 800CD8B4 00000000 */   nop   

/* 0CE4B8 800CD8B8 00000000 */  nop   
/* 0CE4BC 800CD8BC 00000000 */  nop   
