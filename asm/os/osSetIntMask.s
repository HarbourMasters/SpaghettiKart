.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"

.set VERSION_EU_SH, 1

.section .text, "ax"

glabel osSetIntMask
.ifdef VERSION_EU_SH
    mfc0  $t4, C0_SR
    andi  $v0, $t4, OS_IM_CPU
    lui   $t0, %hi(__OSGlobalIntMask) # $t0, 0x8030
    addiu $t0, %lo(__OSGlobalIntMask) # addiu $t0, $t0, 0x208c
    lw    $t3, ($t0)
    li    $at, -1
    xor   $t0, $t3, $at
    andi  $t0, $t0, SR_IMASK
    or    $v0, $v0, $t0
.else
    mfc0  $t1, C0_SR
    andi  $v0, $t1, OS_IM_CPU
.endif
    lui   $t2, %hi(PHYS_TO_K1|MI_INTR_MASK_REG) # $t2, 0xa430
    lw    $t2, %lo(PHYS_TO_K1|MI_INTR_MASK_REG)($t2)
.ifdef VERSION_EU_SH
    beqz  $t2, .L80200074
    srl   $t1, $t3, 0x10
    li    $at, -1
    xor   $t1, $t1, $at
    andi  $t1, $t1, MI_INTR_MASK
    or    $t2, $t2, $t1
.L80200074:
.endif
    sll   $t2, $t2, 0x10
    or    $v0, $v0, $t2
    lui   $at, MI_INTR_MASK
    and   $t0, $a0, $at
.ifdef VERSION_EU_SH
    and   $t0, $t0, $t3
.endif
    srl   $t0, $t0, 0xf
    lui   $t2, %hi(__osRcpImTable)
    addu  $t2, $t2, $t0
    lhu   $t2, %lo(__osRcpImTable)($t2)
    lui   $at, %hi(PHYS_TO_K1|MI_INTR_MASK_REG) # $at, 0xa430
    sw    $t2, %lo(PHYS_TO_K1|MI_INTR_MASK_REG)($at)
    andi  $t0, $a0, OS_IM_CPU
.ifdef VERSION_EU_SH
    andi  $t1, $t3, SR_IMASK
    and   $t0, $t0, $t1
.endif
    lui   $at, %hi(~SR_IMASK) # lui $at, 0xffff
    ori   $at, %lo(~SR_IMASK) # ori $at, $at, 0xff
.ifdef VERSION_EU_SH
    and   $t4, $t4, $at
    or    $t4, $t4, $t0
    mtc0  $t4, C0_SR
.else
    and   $t1, $t1, $at
    or    $t1, $t1, $t0
    mtc0  $t1, C0_SR
.endif
    nop
    nop
    jr    $ra
     nop


.section .rodata

.set MI_INTR_MASK, 0x3f
.set CLR_SP, 0x0001
.set SET_SP, 0x0002
.set CLR_SI, 0x0004
.set SET_SI, 0x0008
.set CLR_AI, 0x0010
.set SET_AI, 0x0020
.set CLR_VI, 0x0040
.set SET_VI, 0x0080
.set CLR_PI, 0x0100
.set SET_PI, 0x0200
.set CLR_DP, 0x0400
.set SET_DP, 0x0800

glabel __osRcpImTable
/* LUT to convert between MI_INTR and MI_INTR_MASK */
/* MI_INTR is status for each interrupt whereas    */
/* MI_INTR_MASK has seperate bits for set/clr      */
.half CLR_SP | CLR_SI | CLR_AI | CLR_VI | CLR_PI | CLR_DP
.half SET_SP | CLR_SI | CLR_AI | CLR_VI | CLR_PI | CLR_DP
.half CLR_SP | SET_SI | CLR_AI | CLR_VI | CLR_PI | CLR_DP
.half SET_SP | SET_SI | CLR_AI | CLR_VI | CLR_PI | CLR_DP
.half CLR_SP | CLR_SI | SET_AI | CLR_VI | CLR_PI | CLR_DP
.half SET_SP | CLR_SI | SET_AI | CLR_VI | CLR_PI | CLR_DP
.half CLR_SP | SET_SI | SET_AI | CLR_VI | CLR_PI | CLR_DP
.half SET_SP | SET_SI | SET_AI | CLR_VI | CLR_PI | CLR_DP
.half CLR_SP | CLR_SI | CLR_AI | SET_VI | CLR_PI | CLR_DP
.half SET_SP | CLR_SI | CLR_AI | SET_VI | CLR_PI | CLR_DP
.half CLR_SP | SET_SI | CLR_AI | SET_VI | CLR_PI | CLR_DP
.half SET_SP | SET_SI | CLR_AI | SET_VI | CLR_PI | CLR_DP
.half CLR_SP | CLR_SI | SET_AI | SET_VI | CLR_PI | CLR_DP
.half SET_SP | CLR_SI | SET_AI | SET_VI | CLR_PI | CLR_DP
.half CLR_SP | SET_SI | SET_AI | SET_VI | CLR_PI | CLR_DP
.half SET_SP | SET_SI | SET_AI | SET_VI | CLR_PI | CLR_DP
.half CLR_SP | CLR_SI | CLR_AI | CLR_VI | SET_PI | CLR_DP
.half SET_SP | CLR_SI | CLR_AI | CLR_VI | SET_PI | CLR_DP
.half CLR_SP | SET_SI | CLR_AI | CLR_VI | SET_PI | CLR_DP
.half SET_SP | SET_SI | CLR_AI | CLR_VI | SET_PI | CLR_DP
.half CLR_SP | CLR_SI | SET_AI | CLR_VI | SET_PI | CLR_DP
.half SET_SP | CLR_SI | SET_AI | CLR_VI | SET_PI | CLR_DP
.half CLR_SP | SET_SI | SET_AI | CLR_VI | SET_PI | CLR_DP
.half SET_SP | SET_SI | SET_AI | CLR_VI | SET_PI | CLR_DP
.half CLR_SP | CLR_SI | CLR_AI | SET_VI | SET_PI | CLR_DP
.half SET_SP | CLR_SI | CLR_AI | SET_VI | SET_PI | CLR_DP
.half CLR_SP | SET_SI | CLR_AI | SET_VI | SET_PI | CLR_DP
.half SET_SP | SET_SI | CLR_AI | SET_VI | SET_PI | CLR_DP
.half CLR_SP | CLR_SI | SET_AI | SET_VI | SET_PI | CLR_DP
.half SET_SP | CLR_SI | SET_AI | SET_VI | SET_PI | CLR_DP
.half CLR_SP | SET_SI | SET_AI | SET_VI | SET_PI | CLR_DP
.half SET_SP | SET_SI | SET_AI | SET_VI | SET_PI | CLR_DP
.half CLR_SP | CLR_SI | CLR_AI | CLR_VI | CLR_PI | SET_DP
.half SET_SP | CLR_SI | CLR_AI | CLR_VI | CLR_PI | SET_DP
.half CLR_SP | SET_SI | CLR_AI | CLR_VI | CLR_PI | SET_DP
.half SET_SP | SET_SI | CLR_AI | CLR_VI | CLR_PI | SET_DP
.half CLR_SP | CLR_SI | SET_AI | CLR_VI | CLR_PI | SET_DP
.half SET_SP | CLR_SI | SET_AI | CLR_VI | CLR_PI | SET_DP
.half CLR_SP | SET_SI | SET_AI | CLR_VI | CLR_PI | SET_DP
.half SET_SP | SET_SI | SET_AI | CLR_VI | CLR_PI | SET_DP
.half CLR_SP | CLR_SI | CLR_AI | SET_VI | CLR_PI | SET_DP
.half SET_SP | CLR_SI | CLR_AI | SET_VI | CLR_PI | SET_DP
.half CLR_SP | SET_SI | CLR_AI | SET_VI | CLR_PI | SET_DP
.half SET_SP | SET_SI | CLR_AI | SET_VI | CLR_PI | SET_DP
.half CLR_SP | CLR_SI | SET_AI | SET_VI | CLR_PI | SET_DP
.half SET_SP | CLR_SI | SET_AI | SET_VI | CLR_PI | SET_DP
.half CLR_SP | SET_SI | SET_AI | SET_VI | CLR_PI | SET_DP
.half SET_SP | SET_SI | SET_AI | SET_VI | CLR_PI | SET_DP
.half CLR_SP | CLR_SI | CLR_AI | CLR_VI | SET_PI | SET_DP
.half SET_SP | CLR_SI | CLR_AI | CLR_VI | SET_PI | SET_DP
.half CLR_SP | SET_SI | CLR_AI | CLR_VI | SET_PI | SET_DP
.half SET_SP | SET_SI | CLR_AI | CLR_VI | SET_PI | SET_DP
.half CLR_SP | CLR_SI | SET_AI | CLR_VI | SET_PI | SET_DP
.half SET_SP | CLR_SI | SET_AI | CLR_VI | SET_PI | SET_DP
.half CLR_SP | SET_SI | SET_AI | CLR_VI | SET_PI | SET_DP
.half SET_SP | SET_SI | SET_AI | CLR_VI | SET_PI | SET_DP
.half CLR_SP | CLR_SI | CLR_AI | SET_VI | SET_PI | SET_DP
.half SET_SP | CLR_SI | CLR_AI | SET_VI | SET_PI | SET_DP
.half CLR_SP | SET_SI | CLR_AI | SET_VI | SET_PI | SET_DP
.half SET_SP | SET_SI | CLR_AI | SET_VI | SET_PI | SET_DP
.half CLR_SP | CLR_SI | SET_AI | SET_VI | SET_PI | SET_DP
.half SET_SP | CLR_SI | SET_AI | SET_VI | SET_PI | SET_DP
.half CLR_SP | SET_SI | SET_AI | SET_VI | SET_PI | SET_DP
.half SET_SP | SET_SI | SET_AI | SET_VI | SET_PI | SET_DP
