.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"


.section .text, "ax"

glabel osWritebackDCache
    blez  $a1, .osWritebackDCacheReturn
     nop
    li    $t3, DCACHE_SIZE
    bgeu  $a1, $t3, .L80324E40
     nop
    move  $t0, $a0
    addu  $t1, $a0, $a1
    bgeu  $t0, $t1, .osWritebackDCacheReturn
     nop
    andi  $t2, $t0, DCACHE_LINEMASK
    addiu $t1, $t1, -DCACHE_LINESIZE
    subu  $t0, $t0, $t2
.L80324E28:
    cache C_HWB|CACH_PD, ($t0)
    bltu  $t0, $t1, .L80324E28
     addiu $t0, $t0, 0x10
.osWritebackDCacheReturn:
    jr    $ra
     nop

.L80324E40:
    li   $t0, KUSIZE
    addu  $t1, $t0, $t3
    addiu $t1, $t1, -DCACHE_LINESIZE
.L80324E4C:
    cache C_IWBINV|CACH_PD, ($t0)
    bltu  $t0, $t1, .L80324E4C
     addiu $t0, DCACHE_LINESIZE # addiu $t0, $t0, 0x10
    jr    $ra
     nop
