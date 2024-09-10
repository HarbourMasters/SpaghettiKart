# handwritten (needs data/rodata too)
# assembler directives
.set noat      # allow manual use of $at
.set noreorder # don't insert nops after branches
.set gp=64

.include "macros.inc"

.section .rodata

.set REDISPATCH, 0x00
.set SW1, 0x04
.set SW2, 0x08
.set RCP, 0x0c
.set CART, 0x10
.set PRENMI, 0x14
.set IP6_HDLR, 0x18
.set IP7_HDLR, 0x1c
.set COUNTER, 0x20

glabel __osIntOffTable
.byte REDISPATCH
.byte PRENMI
.byte IP6_HDLR
.byte IP6_HDLR
.byte IP7_HDLR
.byte IP7_HDLR
.byte IP7_HDLR
.byte IP7_HDLR
.byte COUNTER
.byte COUNTER
.byte COUNTER
.byte COUNTER
.byte COUNTER
.byte COUNTER
.byte COUNTER
.byte COUNTER
.byte REDISPATCH
.byte SW1
.byte SW2
.byte SW2
.byte RCP
.byte RCP
.byte RCP
.byte RCP
.byte CART
.byte CART
.byte CART
.byte CART
.byte CART
.byte CART
.byte CART
.byte CART

glabel __osIntTable
.word redispatch, sw1, sw2, rcp, cart, prenmi, IP6_Hdlr, IP7_Hdlr, counter

.section .bss
glabel D_801976A0
.skip 432

.section .text, "ax"

glabel __osExceptionPreamble
/* 0D1DB0 800D11B0 3C1A800D */  lui   $k0, %hi(__osException) # $k0, 0x800d
/* 0D1DB4 800D11B4 275A11C0 */  addiu $k0, %lo(__osException) # addiu $k0, $k0, 0x11c0
/* 0D1DB8 800D11B8 03400008 */  jr    $k0
/* 0D1DBC 800D11BC 00000000 */   nop   
glabel __osException
/* 0D1DC0 800D11C0 3C1A8019 */  lui   $k0, %hi(D_801976A0) # $k0, 0x8019
/* 0D1DC4 800D11C4 275A76A0 */  addiu $k0, %lo(D_801976A0) # addiu $k0, $k0, 0x76a0
/* 0D1DC8 800D11C8 FF410020 */  sd    $at, 0x20($k0) # save $at
/* 0D1DCC 800D11CC 401B6000 */  mfc0  $k1, C0_SR # save sr
/* 0D1DD0 800D11D0 AF5B0118 */  sw    $k1, THREAD_SR($k0)
/* 0D1DD4 800D11D4 2401FFFC */  li    $at, -4
/* 0D1DD8 800D11D8 0361D824 */  and   $k1, $k1, $at
/* 0D1DDC 800D11DC 409B6000 */  mtc0  $k1, C0_SR
/* 0D1DE0 800D11E0 FF480058 */  sd    $t0, THREAD_T0($k0) # save other regs
/* 0D1DE4 800D11E4 FF490060 */  sd    $t1, THREAD_T1($k0)
/* 0D1DE8 800D11E8 FF4A0068 */  sd    $t2, THREAD_T2($k0)
/* 0D1DEC 800D11EC AF400018 */  sw    $zero, THREAD_FP($k0) # say fp has not been used
/* 0D1DF0 800D11F0 40086800 */  mfc0  $t0, C0_CAUSE
savecontext:
/* 0D1DF4 800D11F4 03404025 */  move  $t0, $k0
/* 0D1DF8 800D11F8 3C1A800F */  lui   $k0, %hi(__osRunningThread) # $k0, 0x800f
/* 0D1DFC 800D11FC 8F5AB3B0 */  lw    $k0, %lo(__osRunningThread)($k0)
/* 0D1E00 800D1200 DD090020 */  ld    $t1, THREAD_AT($t0)
/* 0D1E04 800D1204 FF490020 */  sd    $t1, THREAD_AT($k0)
/* 0D1E08 800D1208 DD090118 */  ld    $t1, THREAD_SR($t0)
/* 0D1E0C 800D120C FF490118 */  sd    $t1, THREAD_SR($k0)
/* 0D1E10 800D1210 DD090058 */  ld    $t1, THREAD_T0($t0)
/* 0D1E14 800D1214 FF490058 */  sd    $t1, THREAD_T0($k0)
/* 0D1E18 800D1218 DD090060 */  ld    $t1, THREAD_T1($t0)
/* 0D1E1C 800D121C FF490060 */  sd    $t1, THREAD_T1($k0)
/* 0D1E20 800D1220 DD090068 */  ld    $t1, THREAD_T2($t0)
/* 0D1E24 800D1224 FF490068 */  sd    $t1, THREAD_T2($k0)
3: /* this is probably left over from debug, not referenced but required for instruction ordering to match */
/* 0D1E28 800D1228 8F5B0118 */  lw    $k1, THREAD_SR($k0)
/* 0D1E2C 800D122C 00004012 */  mflo  $t0
/* 0D1E30 800D1230 FF480108 */  sd    $t0, THREAD_LO($k0)
/* 0D1E34 800D1234 00004010 */  mfhi  $t0
/* 0D1E38 800D1238 3369FF00 */  andi  $t1, $k1, 0xff00
/* 0D1E3C 800D123C FF420028 */  sd    $v0, THREAD_V0($k0)
/* 0D1E40 800D1240 FF430030 */  sd    $v1, THREAD_V1($k0)
/* 0D1E44 800D1244 FF440038 */  sd    $a0, THREAD_A0($k0)
/* 0D1E48 800D1248 FF450040 */  sd    $a1, THREAD_A1($k0)
/* 0D1E4C 800D124C FF460048 */  sd    $a2, THREAD_A2($k0)
/* 0D1E50 800D1250 FF470050 */  sd    $a3, THREAD_A3($k0)
/* 0D1E54 800D1254 FF4B0070 */  sd    $t3, THREAD_T3($k0)
/* 0D1E58 800D1258 FF4C0078 */  sd    $t4, THREAD_T4($k0)
/* 0D1E5C 800D125C FF4D0080 */  sd    $t5, THREAD_T5($k0)
/* 0D1E60 800D1260 FF4E0088 */  sd    $t6, THREAD_T6($k0)
/* 0D1E64 800D1264 FF4F0090 */  sd    $t7, THREAD_T7($k0)
/* 0D1E68 800D1268 FF500098 */  sd    $s0, THREAD_S0($k0)
/* 0D1E6C 800D126C FF5100A0 */  sd    $s1, THREAD_S1($k0)
/* 0D1E70 800D1270 FF5200A8 */  sd    $s2, THREAD_S2($k0)
/* 0D1E74 800D1274 FF5300B0 */  sd    $s3, THREAD_S3($k0)
/* 0D1E78 800D1278 FF5400B8 */  sd    $s4, THREAD_S4($k0)
/* 0D1E7C 800D127C FF5500C0 */  sd    $s5, THREAD_S5($k0)
/* 0D1E80 800D1280 FF5600C8 */  sd    $s6, THREAD_S6($k0)
/* 0D1E84 800D1284 FF5700D0 */  sd    $s7, THREAD_S7($k0)
/* 0D1E88 800D1288 FF5800D8 */  sd    $t8, THREAD_T8($k0)
/* 0D1E8C 800D128C FF5900E0 */  sd    $t9, THREAD_T9($k0)
/* 0D1E90 800D1290 FF5C00E8 */  sd    $gp, THREAD_GP($k0)
/* 0D1E94 800D1294 FF5D00F0 */  sd    $sp, THREAD_SP($k0)
/* 0D1E98 800D1298 FF5E00F8 */  sd    $fp, THREAD_S8($k0)
/* 0D1E9C 800D129C FF5F0100 */  sd    $ra, THREAD_RA($k0)
/* 0D1EA0 800D12A0 1120000D */  beqz  $t1, savercp
/* 0D1EA4 800D12A4 FF480110 */   sd    $t0, THREAD_HI($k0)
/* 0D1EA8 800D12A8 3C08800F */  lui   $t0, %hi(__OSGlobalIntMask) # $t0, 0x800f
/* 0D1EAC 800D12AC 2508A5EC */  addiu $t0, %lo(__OSGlobalIntMask) # addiu $t0, $t0, -0x5a14
/* 0D1EB0 800D12B0 8D080000 */  lw    $t0, ($t0)
/* 0D1EB4 800D12B4 2401FFFF */  li    $at, -1
/* 0D1EB8 800D12B8 01014026 */  xor   $t0, $t0, $at
/* 0D1EBC 800D12BC 3C01FFFF */  lui   $at, %hi(~SR_IMASK) # lui $at, 0xffff
/* 0D1EC0 800D12C0 3108FF00 */  andi  $t0, $t0, SR_IMASK
/* 0D1EC4 800D12C4 342100FF */  ori   $at, %lo(~SR_IMASK) # ori $at, $at, 0xff
/* 0D1EC8 800D12C8 01284825 */  or    $t1, $t1, $t0
/* 0D1ECC 800D12CC 0361D824 */  and   $k1, $k1, $at
/* 0D1ED0 800D12D0 0369D825 */  or    $k1, $k1, $t1
/* 0D1ED4 800D12D4 AF5B0118 */  sw    $k1, THREAD_SR($k0)
savercp:
/* 0D1ED8 800D12D8 3C09A430 */  lui   $t1, %hi(PHYS_TO_K1|MI_INTR_MASK_REG) # $t1, 0xa430
/* 0D1EDC 800D12DC 8D29000C */  lw    $t1, %lo(PHYS_TO_K1|MI_INTR_MASK_REG)($t1)
/* 0D1EE0 800D12E0 1120000B */  beqz  $t1, endrcp
/* 0D1EE4 800D12E4 00000000 */   nop   
/* 0D1EE8 800D12E8 3C08800F */  lui   $t0, %hi(__OSGlobalIntMask) # $t0, 0x800f
/* 0D1EEC 800D12EC 2508A5EC */  addiu $t0, %lo(__OSGlobalIntMask) # addiu $t0, $t0, -0x5a14
/* 0D1EF0 800D12F0 8D080000 */  lw    $t0, ($t0)
/* 0D1EF4 800D12F4 8F4C0128 */  lw    $t4, THREAD_RCP($k0)
/* 0D1EF8 800D12F8 2401FFFF */  li    $at, -1
/* 0D1EFC 800D12FC 00084402 */  srl   $t0, $t0, 0x10
/* 0D1F00 800D1300 01014026 */  xor   $t0, $t0, $at
/* 0D1F04 800D1304 3108003F */  andi  $t0, $t0, 0x3f
/* 0D1F08 800D1308 010C4024 */  and   $t0, $t0, $t4
/* 0D1F0C 800D130C 01284825 */  or    $t1, $t1, $t0
endrcp:
/* 0D1F10 800D1310 AF490128 */  sw    $t1, THREAD_RCP($k0)
/* 0D1F14 800D1314 40087000 */  mfc0  $t0, C0_EPC
/* 0D1F18 800D1318 AF48011C */  sw    $t0, THREAD_PC($k0)
/* 0D1F1C 800D131C 8F480018 */  lw    $t0, THREAD_FP($k0)
/* 0D1F20 800D1320 11000014 */  beqz  $t0, handle_interrupt
/* 0D1F24 800D1324 00000000 */   nop   
/* 0D1F28 800D1328 4448F800 */  cfc1  $t0, $31
/* 0D1F2C 800D132C 00000000 */  nop   
/* 0D1F30 800D1330 AF48012C */  sw    $t0, THREAD_FPCSR($k0)
/* 0D1F34 800D1334 F7400130 */  sdc1  $f0, THREAD_FP0($k0)
/* 0D1F38 800D1338 F7420138 */  sdc1  $f2, THREAD_FP2($k0)
/* 0D1F3C 800D133C F7440140 */  sdc1  $f4, THREAD_FP4($k0)
/* 0D1F40 800D1340 F7460148 */  sdc1  $f6, THREAD_FP6($k0)
/* 0D1F44 800D1344 F7480150 */  sdc1  $f8, THREAD_FP8($k0)
/* 0D1F48 800D1348 F74A0158 */  sdc1  $f10, THREAD_FP10($k0)
/* 0D1F4C 800D134C F74C0160 */  sdc1  $f12, THREAD_FP12($k0)
/* 0D1F50 800D1350 F74E0168 */  sdc1  $f14, THREAD_FP14($k0)
/* 0D1F54 800D1354 F7500170 */  sdc1  $f16, THREAD_FP16($k0)
/* 0D1F58 800D1358 F7520178 */  sdc1  $f18, THREAD_FP18($k0)
/* 0D1F5C 800D135C F7540180 */  sdc1  $f20, THREAD_FP20($k0)
/* 0D1F60 800D1360 F7560188 */  sdc1  $f22, THREAD_FP22($k0)
/* 0D1F64 800D1364 F7580190 */  sdc1  $f24, THREAD_FP24($k0)
/* 0D1F68 800D1368 F75A0198 */  sdc1  $f26, THREAD_FP26($k0)
/* 0D1F6C 800D136C F75C01A0 */  sdc1  $f28, THREAD_FP28($k0)
/* 0D1F70 800D1370 F75E01A8 */  sdc1  $f30, THREAD_FP30($k0)
handle_interrupt:
/* 0D1F74 800D1374 40086800 */  mfc0  $t0, C0_CAUSE
/* 0D1F78 800D1378 AF480120 */  sw    $t0, THREAD_CAUSE($k0)
/* 0D1F7C 800D137C 24090002 */  li    $t1, OS_STATE_RUNNABLE
/* 0D1F80 800D1380 A7490010 */  sh    $t1, THREAD_STATE($k0)
/* 0D1F84 800D1384 3109007C */  andi  $t1, $t0, CAUSE_EXCMASK
/* 0D1F88 800D1388 240A0024 */  li    $t2, EXC_BREAK
/* 0D1F8C 800D138C 112A00B0 */  beq   $t1, $t2, handle_break
/* 0D1F90 800D1390 00000000 */   nop   
/* 0D1F94 800D1394 240A002C */  li    $t2, EXC_CPU
/* 0D1F98 800D1398 112A00FF */  beq   $t1, $t2, handle_CpU
/* 0D1F9C 800D139C 00000000 */   nop   
/* 0D1FA0 800D13A0 240A0000 */  li    $t2, EXC_INT
/* 0D1FA4 800D13A4 152A00C3 */  bne   $t1, $t2, panic
/* 0D1FA8 800D13A8 00000000 */   nop   
/* 0D1FAC 800D13AC 03688024 */  and   $s0, $k1, $t0
next_interrupt:
/* 0D1FB0 800D13B0 3209FF00 */  andi  $t1, $s0, SR_IMASK
/* 0D1FB4 800D13B4 00095302 */  srl   $t2, $t1, 0xc
/* 0D1FB8 800D13B8 15400003 */  bnez  $t2, .L800D13C8
/* 0D1FBC 800D13BC 00000000 */   nop   
/* 0D1FC0 800D13C0 00095202 */  srl   $t2, $t1, 8
/* 0D1FC4 800D13C4 214A0010 */  addi  $t2, $t2, 0x10
.L800D13C8:
/* 0D1FC8 800D13C8 3C01800F */  lui   $at, %hi(__osIntOffTable) # 0x800f
/* 0D1FCC 800D13CC 002A0821 */  addu  $at, $at, $t2
/* 0D1FD0 800D13D0 902A3A00 */  lbu   $t2, %lo(__osIntOffTable)($at) # 0x3a00($at)
/* 0D1FD4 800D13D4 3C01800F */  lui   $at, %hi(__osIntTable)
/* 0D1FD8 800D13D8 002A0821 */  addu  $at, $at, $t2
/* 0D1FDC 800D13DC 8C2A3A20 */  lw    $t2, %lo(__osIntTable)($at)
/* 0D1FE0 800D13E0 01400008 */  jr    $t2
/* 0D1FE4 800D13E4 00000000 */   nop   
glabel IP6_Hdlr
/* 0D1FE8 800D13E8 2401DFFF */  li    $at, ~CAUSE_IP6
/* 0D1FEC 800D13EC 1000FFF0 */  b     next_interrupt
/* 0D1FF0 800D13F0 02018024 */   and   $s0, $s0, $at
glabel IP7_Hdlr
/* 0D1FF4 800D13F4 2401BFFF */  li    $at, ~CAUSE_IP7
/* 0D1FF8 800D13F8 1000FFED */  b     next_interrupt
/* 0D1FFC 800D13FC 02018024 */   and   $s0, $s0, $at
glabel counter
/* 0D2000 800D1400 40095800 */  mfc0  $t1, C0_COMPARE
/* 0D2004 800D1404 40895800 */  mtc0  $t1, C0_COMPARE
/* 0D2008 800D1408 0C0345B9 */  jal   send_mesg
/* 0D200C 800D140C 24040018 */   li    $a0, (OS_EVENT_COUNTER << 3)
/* 0D2010 800D1410 3C01FFFF */  lui   $at, %hi(~CAUSE_IP8) # lui $at, 0xffff
/* 0D2014 800D1414 34217FFF */  ori   $at, %lo(~CAUSE_IP8) # ori $at, $at, 0x7fff
/* 0D2018 800D1418 1000FFE5 */  b     next_interrupt
/* 0D201C 800D141C 02018024 */   and   $s0, $s0, $at
glabel cart
/* 0D2020 800D1420 2401F7FF */  li    $at, ~CAUSE_IP4
/* 0D2024 800D1424 02018024 */  and   $s0, $s0, $at
/* 0D2028 800D1428 240A0004 */  li    $t2, 4
/* 0D202C 800D142C 3C01800F */  lui   $at, %hi(__osHwIntTable) # 0x800f
/* 0D2030 800D1430 002A0821 */  addu  $at, $at, $t2
/* 0D2034 800D1434 8C2AB380 */  lw    $t2, %lo(__osHwIntTable)($at) # -0x4c80($at)
/* 0D2038 800D1438 3C1D8019 */  lui   $sp, %hi(leoDiskStack) # $sp, 0x8019
/* 0D203C 800D143C 27BD65E0 */  addiu $sp, %lo(leoDiskStack) # addiu $sp, $sp, 0x65e0
/* 0D2040 800D1440 24040010 */  li    $a0, (OS_EVENT_CART << 3)
/* 0D2044 800D1444 11400007 */  beqz  $t2, .L800D1464
/* 0D2048 800D1448 27BD0FF0 */   addiu $sp, $sp, OS_PIM_STACKSIZE-16 /* TODO: maybe make OS_LEO_STACKSIZE */
/* 0D204C 800D144C 0140F809 */  jalr  $t2
/* 0D2050 800D1450 00000000 */  nop   
/* 0D2054 800D1454 10400003 */  beqz  $v0, .L800D1464
/* 0D2058 800D1458 00000000 */   nop   
/* 0D205C 800D145C 10000082 */  b     .redispatch
/* 0D2060 800D1460 00000000 */   nop   
.L800D1464:
/* 0D2064 800D1464 0C0345B9 */  jal   send_mesg
/* 0D2068 800D1468 00000000 */   nop   
/* 0D206C 800D146C 1000FFD0 */  b     next_interrupt
/* 0D2070 800D1470 00000000 */   nop   
glabel rcp
/* 0D2074 800D1474 3C08800F */  lui   $t0, %hi(__OSGlobalIntMask) # $t0, 0x800f
/* 0D2078 800D1478 2508A5EC */  addiu $t0, %lo(__OSGlobalIntMask) # addiu $t0, $t0, -0x5a14
/* 0D207C 800D147C 8D080000 */  lw    $t0, ($t0)
/* 0D2080 800D1480 3C11A430 */  lui   $s1, %hi(PHYS_TO_K1|MI_INTR_REG) # $s1, 0xa430
/* 0D2084 800D1484 8E310008 */  lw    $s1, %lo(PHYS_TO_K1|MI_INTR_REG)($s1)
/* 0D2088 800D1488 00084402 */  srl   $t0, $t0, 0x10
/* 0D208C 800D148C 02288824 */  and   $s1, $s1, $t0
/* 0D2090 800D1490 32290001 */  andi  $t1, $s1, MI_INTR_SP
/* 0D2094 800D1494 11200013 */  beqz  $t1, vi
/* 0D2098 800D1498 00000000 */   nop   
/* 0D209C 800D149C 3C0CA404 */  lui   $t4, %hi(PHYS_TO_K1|SP_STATUS_REG) # $t4, 0xa404
/* 0D20A0 800D14A0 8D8C0010 */  lw    $t4, %lo(PHYS_TO_K1|SP_STATUS_REG)($t4)
/* 0D20A4 800D14A4 24090008 */  li    $t1, 8
/* 0D20A8 800D14A8 3C01A404 */  lui   $at, %hi(PHYS_TO_K1|SP_STATUS_REG) # $at, 0xa404
/* 0D20AC 800D14AC 318C0300 */  andi  $t4, $t4, 0x300
/* 0D20B0 800D14B0 3231003E */  andi  $s1, $s1, 0x3e
/* 0D20B4 800D14B4 11800007 */  beqz  $t4, sp_other_break
/* 0D20B8 800D14B8 AC290010 */   sw    $t1, %lo(PHYS_TO_K1|SP_STATUS_REG)($at)
/* 0D20BC 800D14BC 0C0345B9 */  jal   send_mesg
/* 0D20C0 800D14C0 24040020 */   li    $a0, (OS_EVENT_SP << 3)
/* 0D20C4 800D14C4 12200038 */  beqz  $s1, NoMoreRcpInts
/* 0D20C8 800D14C8 00000000 */   nop   
/* 0D20CC 800D14CC 10000005 */  b     vi
/* 0D20D0 800D14D0 00000000 */   nop   
sp_other_break:
/* 0D20D4 800D14D4 0C0345B9 */  jal   send_mesg
/* 0D20D8 800D14D8 24040058 */   li    $a0, (OS_EVENT_SP_BREAK << 3)
/* 0D20DC 800D14DC 12200032 */  beqz  $s1, NoMoreRcpInts
/* 0D20E0 800D14E0 00000000 */   nop   
vi:
/* 0D20E4 800D14E4 32290008 */  andi  $t1, $s1, 8
/* 0D20E8 800D14E8 11200007 */  beqz  $t1, ai
/* 0D20EC 800D14EC 3C01A440 */   lui   $at, %hi(PHYS_TO_K1|VI_CURRENT_REG) # $at, 0xa440
/* 0D20F0 800D14F0 32310037 */  andi  $s1, $s1, 0x37
/* 0D20F4 800D14F4 AC200010 */  sw    $zero, %lo(PHYS_TO_K1|VI_CURRENT_REG)($at)
/* 0D20F8 800D14F8 0C0345B9 */  jal   send_mesg
/* 0D20FC 800D14FC 24040038 */   li    $a0, (OS_EVENT_VI << 3)
/* 0D2100 800D1500 12200029 */  beqz  $s1, NoMoreRcpInts
/* 0D2104 800D1504 00000000 */   nop   
ai:
/* 0D2108 800D1508 32290004 */  andi  $t1, $s1, 4
/* 0D210C 800D150C 11200009 */  beqz  $t1, si
/* 0D2110 800D1510 00000000 */   nop   
/* 0D2114 800D1514 24090001 */  li    $t1, 1
/* 0D2118 800D1518 3C01A450 */  lui   $at, %hi(PHYS_TO_K1|AI_STATUS_REG) # $at, 0xa450
/* 0D211C 800D151C 3231003B */  andi  $s1, $s1, 0x3b
/* 0D2120 800D1520 AC29000C */  sw    $t1, %lo(PHYS_TO_K1|AI_STATUS_REG)($at)
/* 0D2124 800D1524 0C0345B9 */  jal   send_mesg
/* 0D2128 800D1528 24040030 */   li    $a0, (OS_EVENT_AI << 3)
/* 0D212C 800D152C 1220001E */  beqz  $s1, NoMoreRcpInts
/* 0D2130 800D1530 00000000 */   nop   
si:
/* 0D2134 800D1534 32290002 */  andi  $t1, $s1, 2
/* 0D2138 800D1538 11200007 */  beqz  $t1, pi
/* 0D213C 800D153C 3C01A480 */   lui   $at, %hi(PHYS_TO_K1|SI_STATUS_REG) # $at, 0xa480
/* 0D2140 800D1540 3231003D */  andi  $s1, $s1, 0x3d
/* 0D2144 800D1544 AC200018 */  sw    $zero, %lo(PHYS_TO_K1|SI_STATUS_REG)($at)
/* 0D2148 800D1548 0C0345B9 */  jal   send_mesg
/* 0D214C 800D154C 24040028 */   li    $a0, (OS_EVENT_SI << 3)
/* 0D2150 800D1550 12200015 */  beqz  $s1, NoMoreRcpInts
/* 0D2154 800D1554 00000000 */   nop   
pi:
/* 0D2158 800D1558 32290010 */  andi  $t1, $s1, 0x10
/* 0D215C 800D155C 11200009 */  beqz  $t1, dp
/* 0D2160 800D1560 00000000 */   nop   
/* 0D2164 800D1564 24090002 */  li    $t1, PI_STATUS_CLR_INTR
/* 0D2168 800D1568 3C01A460 */  lui   $at, %hi(PHYS_TO_K1|PI_STATUS_REG) # $at, 0xa460
/* 0D216C 800D156C 3231002F */  andi  $s1, $s1, 0x2f
/* 0D2170 800D1570 AC290010 */  sw    $t1, %lo(PHYS_TO_K1|PI_STATUS_REG)($at)
/* 0D2174 800D1574 0C0345B9 */  jal   send_mesg
/* 0D2178 800D1578 24040040 */   li    $a0, (OS_EVENT_PI << 3)
/* 0D217C 800D157C 1220000A */  beqz  $s1, NoMoreRcpInts
/* 0D2180 800D1580 00000000 */   nop   
dp:
/* 0D2184 800D1584 32290020 */  andi  $t1, $s1, 0x20
/* 0D2188 800D1588 11200007 */  beqz  $t1, NoMoreRcpInts
/* 0D218C 800D158C 00000000 */   nop   
/* 0D2190 800D1590 24090800 */  li    $t1, MI_CLR_DP_INTR
/* 0D2194 800D1594 3C01A430 */  lui   $at, %hi(PHYS_TO_K1|MI_INIT_MODE_REG) # $at, 0xa430
/* 0D2198 800D1598 3231001F */  andi  $s1, $s1, 0x1f
/* 0D219C 800D159C AC290000 */  sw    $t1, %lo(PHYS_TO_K1|MI_INIT_MODE_REG)($at)
/* 0D21A0 800D15A0 0C0345B9 */  jal   send_mesg
/* 0D21A4 800D15A4 24040048 */   li    $a0, (OS_EVENT_DP << 3)
NoMoreRcpInts:
/* 0D21A8 800D15A8 2401FBFF */  li    $at, ~CAUSE_IP3
/* 0D21AC 800D15AC 1000FF80 */  b     next_interrupt
/* 0D21B0 800D15B0 02018024 */   and   $s0, $s0, $at
glabel prenmi
/* 0D21B4 800D15B4 8F5B0118 */  lw    $k1, THREAD_SR($k0)
/* 0D21B8 800D15B8 2401EFFF */  li    $at, ~CAUSE_IP5
/* 0D21BC 800D15BC 3C09800F */  lui   $t1, %hi(__osShutdown) # $t1, 0x800f
/* 0D21C0 800D15C0 0361D824 */  and   $k1, $k1, $at
/* 0D21C4 800D15C4 AF5B0118 */  sw    $k1, THREAD_SR($k0)
/* 0D21C8 800D15C8 2529A5E8 */  addiu $t1, %lo(__osShutdown) # addiu $t1, $t1, -0x5a18
/* 0D21CC 800D15CC 8D2A0000 */  lw    $t2, ($t1)
/* 0D21D0 800D15D0 11400003 */  beqz  $t2, firstnmi
/* 0D21D4 800D15D4 2401EFFF */   li    $at, ~CAUSE_IP5
/* 0D21D8 800D15D8 10000023 */  b     .redispatch
/* 0D21DC 800D15DC 02018024 */   and   $s0, $s0, $at
firstnmi:
/* 0D21E0 800D15E0 240A0001 */  li    $t2, 1
/* 0D21E4 800D15E4 AD2A0000 */  sw    $t2, ($t1)
/* 0D21E8 800D15E8 0C0345B9 */  jal   send_mesg
/* 0D21EC 800D15EC 24040070 */   li    $a0, (OS_EVENT_PRENMI << 3)
/* 0D21F0 800D15F0 3C0A800F */  lui   $t2, %hi(__osRunQueue) # $t2, 0x800f
/* 0D21F4 800D15F4 8D4AB3A8 */  lw    $t2, %lo(__osRunQueue)($t2)
/* 0D21F8 800D15F8 2401EFFF */  li    $at, ~CAUSE_IP5
/* 0D21FC 800D15FC 02018024 */  and   $s0, $s0, $at
/* 0D2200 800D1600 8D5B0118 */  lw    $k1, THREAD_SR($t2)
/* 0D2204 800D1604 0361D824 */  and   $k1, $k1, $at
/* 0D2208 800D1608 10000017 */  b     .redispatch
/* 0D220C 800D160C AD5B0118 */   sw    $k1, THREAD_SR($t2)
glabel sw2
/* 0D2210 800D1610 2401FDFF */  li    $at, ~CAUSE_SW2
/* 0D2214 800D1614 01014024 */  and   $t0, $t0, $at
/* 0D2218 800D1618 40886800 */  mtc0  $t0, C0_CAUSE
/* 0D221C 800D161C 0C0345B9 */  jal   send_mesg
/* 0D2220 800D1620 24040008 */   li    $a0, (OS_EVENT_SW2 << 3)
/* 0D2224 800D1624 2401FDFF */  li    $at, ~CAUSE_SW2
/* 0D2228 800D1628 1000FF61 */  b     next_interrupt
/* 0D222C 800D162C 02018024 */   and   $s0, $s0, $at
glabel sw1
/* 0D2230 800D1630 2401FEFF */  li    $at, ~CAUSE_SW1
/* 0D2234 800D1634 01014024 */  and   $t0, $t0, $at
/* 0D2238 800D1638 40886800 */  mtc0  $t0, C0_CAUSE
/* 0D223C 800D163C 0C0345B9 */  jal   send_mesg
/* 0D2240 800D1640 24040000 */   li    $a0, (OS_EVENT_SW1 << 3)
/* 0D2244 800D1644 2401FEFF */  li    $at, ~CAUSE_SW1
/* 0D2248 800D1648 1000FF59 */  b     next_interrupt
/* 0D224C 800D164C 02018024 */   and   $s0, $s0, $at
handle_break:
/* 0D2250 800D1650 24090001 */  li    $t1, OS_FLAG_CPU_BREAK
/* 0D2254 800D1654 A7490012 */  sh    $t1, THREAD_FLAGS($k0)
/* 0D2258 800D1658 0C0345B9 */  jal   send_mesg
/* 0D225C 800D165C 24040050 */   li    $a0, (OS_EVENT_CPU_BREAK << 3)
/* 0D2260 800D1660 10000001 */  b     .redispatch
/* 0D2264 800D1664 00000000 */   nop   
.redispatch:
glabel redispatch
/* 0D2268 800D1668 3C0A800F */  lui   $t2, %hi(__osRunQueue) # $t2, 0x800f
/* 0D226C 800D166C 8D4AB3A8 */  lw    $t2, %lo(__osRunQueue)($t2)
/* 0D2270 800D1670 8F490004 */  lw    $t1, THREAD_PRI($k0)
/* 0D2274 800D1674 8D4B0004 */  lw    $t3, THREAD_PRI($t2)
/* 0D2278 800D1678 012B082A */  slt   $at, $t1, $t3
/* 0D227C 800D167C 10200007 */  beqz  $at, enqueueRunning
/* 0D2280 800D1680 00000000 */   nop   
/* 0D2284 800D1684 3C04800F */  lui   $a0, %hi(__osRunQueue) # $a0, 0x800f
/* 0D2288 800D1688 03402825 */  move  $a1, $k0
/* 0D228C 800D168C 0C034633 */  jal   __osEnqueueThread
/* 0D2290 800D1690 2484B3A8 */   addiu $a0, %lo(__osRunQueue) # addiu $a0, $a0, -0x4c58
/* 0D2294 800D1694 08034649 */  j     __osDispatchThread
/* 0D2298 800D1698 00000000 */   nop   

enqueueRunning:
/* 0D229C 800D169C 3C09800F */  lui   $t1, %hi(__osRunQueue) # $t1, 0x800f
/* 0D22A0 800D16A0 2529B3A8 */  addiu $t1, %lo(__osRunQueue) # addiu $t1, $t1, -0x4c58
/* 0D22A4 800D16A4 8D2A0000 */  lw    $t2, MQ_MTQUEUE($t1)
/* 0D22A8 800D16A8 AF4A0000 */  sw    $t2, THREAD_NEXT($k0)
/* 0D22AC 800D16AC 08034649 */  j     __osDispatchThread
/* 0D22B0 800D16B0 AD3A0000 */   sw    $k0, MQ_MTQUEUE($t1)

panic:
/* 0D22B4 800D16B4 3C01800F */  lui   $at, %hi(__osFaultedThread) # $at, 0x800f
/* 0D22B8 800D16B8 AC3AB3B4 */  sw    $k0, %lo(__osFaultedThread)($at)
/* 0D22BC 800D16BC 24090001 */  li    $t1, OS_STATE_STOPPED
/* 0D22C0 800D16C0 A7490010 */  sh    $t1, THREAD_STATE($k0)
/* 0D22C4 800D16C4 24090002 */  li    $t1, OS_FLAG_FAULT
/* 0D22C8 800D16C8 A7490012 */  sh    $t1, THREAD_FLAGS($k0)
/* 0D22CC 800D16CC 400A4000 */  mfc0  $t2, C0_BADVADDR
/* 0D22D0 800D16D0 AF4A0124 */  sw    $t2, THREAD_BADVADDR($k0)
/* 0D22D4 800D16D4 0C0345B9 */  jal   send_mesg
/* 0D22D8 800D16D8 24040060 */   li    $a0, (OS_EVENT_FAULT << 3)
/* 0D22DC 800D16DC 08034649 */  j     __osDispatchThread
/* 0D22E0 800D16E0 00000000 */   nop   

glabel send_mesg
/* 0D22E4 800D16E4 3C0A8019 */  lui   $t2, %hi(__osEventStateTab) # $t2, 0x8019
/* 0D22E8 800D16E8 254A6440 */  addiu $t2, %lo(__osEventStateTab) # addiu $t2, $t2, 0x6440
/* 0D22EC 800D16EC 01445021 */  addu  $t2, $t2, $a0
/* 0D22F0 800D16F0 8D490000 */  lw    $t1, OS_EVENTSTATE_MESSAGE_QUEUE($t2)
/* 0D22F4 800D16F4 03E09025 */  move  $s2, $ra
/* 0D22F8 800D16F8 11200025 */  beqz  $t1, send_done
/* 0D22FC 800D16FC 00000000 */   nop   
/* 0D2300 800D1700 8D2B0008 */  lw    $t3, MQ_VALIDCOUNT($t1)
/* 0D2304 800D1704 8D2C0010 */  lw    $t4, MQ_MSGCOUNT($t1)
/* 0D2308 800D1708 016C082A */  slt   $at, $t3, $t4
/* 0D230C 800D170C 10200020 */  beqz  $at, send_done
/* 0D2310 800D1710 00000000 */   nop   
/* 0D2314 800D1714 8D2D000C */  lw    $t5, MQ_FIRST($t1)
/* 0D2318 800D1718 01AB6821 */  addu  $t5, $t5, $t3
/* 0D231C 800D171C 01AC001A */  div   $zero, $t5, $t4
/* 0D2320 800D1720 15800002 */  bnez  $t4, .L800D172C
/* 0D2324 800D1724 00000000 */   nop   
/* 0D2328 800D1728 0007000D */  break 7
.L800D172C:
/* 0D232C 800D172C 2401FFFF */  li    $at, -1
/* 0D2330 800D1730 15810004 */  bne   $t4, $at, .L800D1744
/* 0D2334 800D1734 3C018000 */   lui   $at, 0x8000
/* 0D2338 800D1738 15A10002 */  bne   $t5, $at, .L800D1744
/* 0D233C 800D173C 00000000 */   nop   
/* 0D2340 800D1740 0006000D */  break 6
.L800D1744:
/* 0D2344 800D1744 8D2C0014 */  lw    $t4, MQ_MSG($t1)
/* 0D2348 800D1748 00006810 */  mfhi  $t5
/* 0D234C 800D174C 000D6880 */  sll   $t5, $t5, 2 /* pointer arithmetic for msg[t5] */
/* 0D2350 800D1750 018D6021 */  addu  $t4, $t4, $t5
/* 0D2354 800D1754 8D4D0004 */  lw    $t5, OS_EVENTSTATE_MESSAGE($t2)
/* 0D2358 800D1758 256A0001 */  addiu $t2, $t3, 1
/* 0D235C 800D175C AD8D0000 */  sw    $t5, ($t4) /* msg[t5] = */
/* 0D2360 800D1760 AD2A0008 */  sw    $t2, MQ_VALIDCOUNT($t1)
/* 0D2364 800D1764 8D2A0000 */  lw    $t2, MQ_MTQUEUE($t1)
/* 0D2368 800D1768 8D4B0000 */  lw    $t3, THREAD_NEXT($t2)
/* 0D236C 800D176C 11600008 */  beqz  $t3, send_done
/* 0D2370 800D1770 00000000 */   nop   
/* 0D2374 800D1774 0C034645 */  jal   __osPopThread
/* 0D2378 800D1778 01202025 */   move  $a0, $t1
/* 0D237C 800D177C 00405025 */  move  $t2, $v0
/* 0D2380 800D1780 3C04800F */  lui   $a0, %hi(__osRunQueue) # $a0, 0x800f
/* 0D2384 800D1784 01402825 */  move  $a1, $t2
/* 0D2388 800D1788 0C034633 */  jal   __osEnqueueThread
/* 0D238C 800D178C 2484B3A8 */   addiu $a0, %lo(__osRunQueue) # addiu $a0, $a0, -0x4c58
send_done:
/* 0D2390 800D1790 02400008 */  jr    $s2
/* 0D2394 800D1794 00000000 */   nop   
handle_CpU:
/* 0D2398 800D1798 3C013000 */  lui   $at, 0x3000 # CAUSE_CEMASK
/* 0D239C 800D179C 01014824 */  and   $t1, $t0, $at
/* 0D23A0 800D17A0 00094F02 */  srl   $t1, $t1, CAUSE_CESHIFT
/* 0D23A4 800D17A4 240A0001 */  li    $t2, 1 /* cp1 error */
/* 0D23A8 800D17A8 152AFFC2 */  bne   $t1, $t2, panic
/* 0D23AC 800D17AC 00000000 */   nop   
/* 0D23B0 800D17B0 8F5B0118 */  lw    $k1, THREAD_SR($k0) /* enable cp1 (fpu) for this thread */
/* 0D23B4 800D17B4 3C012000 */  lui   $at, 0x2000 # SR_CU1
/* 0D23B8 800D17B8 24090001 */  li    $t1, 1
/* 0D23BC 800D17BC 0361D825 */  or    $k1, $k1, $at
/* 0D23C0 800D17C0 AF490018 */  sw    $t1, THREAD_FP($k0)
/* 0D23C4 800D17C4 1000FFB5 */  b     enqueueRunning
/* 0D23C8 800D17C8 AF5B0118 */   sw    $k1, THREAD_SR($k0)
glabel __osEnqueueAndYield
/* 0D23CC 800D17CC 3C05800F */  lui   $a1, %hi(__osRunningThread) # $a1, 0x800f
/* 0D23D0 800D17D0 8CA5B3B0 */  lw    $a1, %lo(__osRunningThread)($a1)
/* 0D23D4 800D17D4 40086000 */  mfc0  $t0, C0_SR
/* 0D23D8 800D17D8 8CBB0018 */  lw    $k1, THREAD_FP($a1)
/* 0D23DC 800D17DC 35080002 */  ori   $t0, $t0, SR_EXL
/* 0D23E0 800D17E0 ACA80118 */  sw    $t0, THREAD_SR($a1)
/* 0D23E4 800D17E4 FCB00098 */  sd    $s0, THREAD_S0($a1)
/* 0D23E8 800D17E8 FCB100A0 */  sd    $s1, THREAD_S1($a1)
/* 0D23EC 800D17EC FCB200A8 */  sd    $s2, THREAD_S2($a1)
/* 0D23F0 800D17F0 FCB300B0 */  sd    $s3, THREAD_S3($a1)
/* 0D23F4 800D17F4 FCB400B8 */  sd    $s4, THREAD_S4($a1)
/* 0D23F8 800D17F8 FCB500C0 */  sd    $s5, THREAD_S5($a1)
/* 0D23FC 800D17FC FCB600C8 */  sd    $s6, THREAD_S6($a1)
/* 0D2400 800D1800 FCB700D0 */  sd    $s7, THREAD_S7($a1)
/* 0D2404 800D1804 FCBC00E8 */  sd    $gp, THREAD_GP($a1)
/* 0D2408 800D1808 FCBD00F0 */  sd    $sp, THREAD_SP($a1)
/* 0D240C 800D180C FCBE00F8 */  sd    $fp, THREAD_S8($a1)
/* 0D2410 800D1810 FCBF0100 */  sd    $ra, THREAD_RA($a1)
/* 0D2414 800D1814 13600009 */  beqz  $k1, .L800D183C
/* 0D2418 800D1818 ACBF011C */   sw    $ra, THREAD_PC($a1)
/* 0D241C 800D181C 445BF800 */  cfc1  $k1, $31
/* 0D2420 800D1820 F4B40180 */  sdc1  $f20, THREAD_FP20($a1)
/* 0D2424 800D1824 F4B60188 */  sdc1  $f22, THREAD_FP22($a1)
/* 0D2428 800D1828 F4B80190 */  sdc1  $f24, THREAD_FP24($a1)
/* 0D242C 800D182C F4BA0198 */  sdc1  $f26, THREAD_FP26($a1)
/* 0D2430 800D1830 F4BC01A0 */  sdc1  $f28, THREAD_FP28($a1)
/* 0D2434 800D1834 F4BE01A8 */  sdc1  $f30, THREAD_FP30($a1)
/* 0D2438 800D1838 ACBB012C */  sw    $k1, THREAD_FPCSR($a1)
.L800D183C:
/* 0D243C 800D183C 8CBB0118 */  lw    $k1, THREAD_SR($a1)
/* 0D2440 800D1840 3369FF00 */  andi  $t1, $k1, SR_IMASK
/* 0D2444 800D1844 1120000D */  beqz  $t1, .L800D187C
/* 0D2448 800D1848 00000000 */   nop   
/* 0D244C 800D184C 3C08800F */  lui   $t0, %hi(__OSGlobalIntMask) # $t0, 0x800f
/* 0D2450 800D1850 2508A5EC */  addiu $t0, %lo(__OSGlobalIntMask) # addiu $t0, $t0, -0x5a14
/* 0D2454 800D1854 8D080000 */  lw    $t0, ($t0)
/* 0D2458 800D1858 2401FFFF */  li    $at, -1
/* 0D245C 800D185C 01014026 */  xor   $t0, $t0, $at
/* 0D2460 800D1860 3C01FFFF */  lui   $at, %hi(~SR_IMASK) # lui $at, 0xffff
/* 0D2464 800D1864 3108FF00 */  andi  $t0, $t0, SR_IMASK
/* 0D2468 800D1868 342100FF */  ori   $at, %lo(~SR_IMASK) # ori $at, $at, 0xff
/* 0D246C 800D186C 01284825 */  or    $t1, $t1, $t0
/* 0D2470 800D1870 0361D824 */  and   $k1, $k1, $at
/* 0D2474 800D1874 0369D825 */  or    $k1, $k1, $t1
/* 0D2478 800D1878 ACBB0118 */  sw    $k1, THREAD_SR($a1)
.L800D187C:
/* 0D247C 800D187C 3C1BA430 */  lui   $k1, %hi(PHYS_TO_K1|MI_INTR_MASK_REG) # $k1, 0xa430
/* 0D2480 800D1880 8F7B000C */  lw    $k1, %lo(PHYS_TO_K1|MI_INTR_MASK_REG)($k1)
/* 0D2484 800D1884 1360000B */  beqz  $k1, .L800D18B4
/* 0D2488 800D1888 00000000 */   nop   
/* 0D248C 800D188C 3C1A800F */  lui   $k0, %hi(__OSGlobalIntMask) # $k0, 0x800f
/* 0D2490 800D1890 275AA5EC */  addiu $k0, %lo(__OSGlobalIntMask) # addiu $k0, $k0, -0x5a14
/* 0D2494 800D1894 8F5A0000 */  lw    $k0, ($k0)
/* 0D2498 800D1898 8CA80128 */  lw    $t0, THREAD_RCP($a1)
/* 0D249C 800D189C 2401FFFF */  li    $at, -1
/* 0D24A0 800D18A0 001AD402 */  srl   $k0, $k0, 0x10
/* 0D24A4 800D18A4 0341D026 */  xor   $k0, $k0, $at
/* 0D24A8 800D18A8 335A003F */  andi  $k0, $k0, 0x3f
/* 0D24AC 800D18AC 0348D024 */  and   $k0, $k0, $t0
/* 0D24B0 800D18B0 037AD825 */  or    $k1, $k1, $k0
.L800D18B4:
/* 0D24B4 800D18B4 10800003 */  beqz  $a0, .L800D18C4
/* 0D24B8 800D18B8 ACBB0128 */   sw    $k1, THREAD_RCP($a1)
/* 0D24BC 800D18BC 0C034633 */  jal   __osEnqueueThread
/* 0D24C0 800D18C0 00000000 */   nop   
.L800D18C4:
/* 0D24C4 800D18C4 08034649 */  j     __osDispatchThread
/* 0D24C8 800D18C8 00000000 */   nop   

glabel __osEnqueueThread
/* 0D24CC 800D18CC 8C980000 */  lw    $t8, ($a0)
/* 0D24D0 800D18D0 8CAF0004 */  lw    $t7, THREAD_PRI($a1)
/* 0D24D4 800D18D4 0080C825 */  move  $t9, $a0
/* 0D24D8 800D18D8 8F0E0004 */  lw    $t6, THREAD_PRI($t8)
/* 0D24DC 800D18DC 01CF082A */  slt   $at, $t6, $t7
/* 0D24E0 800D18E0 14200007 */  bnez  $at, .L800D1900
/* 0D24E4 800D18E4 00000000 */   nop   
.L800D18E8:
/* 0D24E8 800D18E8 0300C825 */  move  $t9, $t8
/* 0D24EC 800D18EC 8F180000 */  lw    $t8, THREAD_NEXT($t8)
/* 0D24F0 800D18F0 8F0E0004 */  lw    $t6, THREAD_PRI($t8)
/* 0D24F4 800D18F4 01CF082A */  slt   $at, $t6, $t7
/* 0D24F8 800D18F8 1020FFFB */  beqz  $at, .L800D18E8
/* 0D24FC 800D18FC 00000000 */   nop   
.L800D1900:
/* 0D2500 800D1900 8F380000 */  lw    $t8, THREAD_NEXT($t9)
/* 0D2504 800D1904 ACB80000 */  sw    $t8, THREAD_NEXT($a1)
/* 0D2508 800D1908 AF250000 */  sw    $a1, THREAD_NEXT($t9)
/* 0D250C 800D190C 03E00008 */  jr    $ra
/* 0D2510 800D1910 ACA40008 */   sw    $a0, THREAD_QUEUE($a1)

glabel __osPopThread
/* 0D2514 800D1914 8C820000 */  lw    $v0, ($a0)
/* 0D2518 800D1918 8C590000 */  lw    $t9, THREAD_NEXT($v0)
/* 0D251C 800D191C 03E00008 */  jr    $ra
/* 0D2520 800D1920 AC990000 */   sw    $t9, ($a0)

glabel __osDispatchThread
/* 0D2524 800D1924 3C04800F */  lui   $a0, %hi(__osRunQueue) # $a0, 0x800f
/* 0D2528 800D1928 0C034645 */  jal   __osPopThread
/* 0D252C 800D192C 2484B3A8 */   addiu $a0, %lo(__osRunQueue) # addiu $a0, $a0, -0x4c58
/* 0D2530 800D1930 3C01800F */  lui   $at, %hi(__osRunningThread) # $at, 0x800f
/* 0D2534 800D1934 AC22B3B0 */  sw    $v0, %lo(__osRunningThread)($at)
/* 0D2538 800D1938 24080004 */  li    $t0, OS_STATE_RUNNING
/* 0D253C 800D193C A4480010 */  sh    $t0, THREAD_STATE($v0)
/* 0D2540 800D1940 0040D025 */  move  $k0, $v0
__osDispatchThreadSave:
/* 0D2544 800D1944 3C08800F */  lui   $t0, %hi(__OSGlobalIntMask) # $t0, 0x800f
/* 0D2548 800D1948 8F5B0118 */  lw    $k1, THREAD_SR($k0)
/* 0D254C 800D194C 2508A5EC */  addiu $t0, %lo(__OSGlobalIntMask) # addiu $t0, $t0, -0x5a14
/* 0D2550 800D1950 8D080000 */  lw    $t0, ($t0)
/* 0D2554 800D1954 3C01FFFF */  lui   $at, %hi(~SR_IMASK) # lui $at, 0xffff
/* 0D2558 800D1958 3369FF00 */  andi  $t1, $k1, SR_IMASK
/* 0D255C 800D195C 342100FF */  ori   $at, %lo(~SR_IMASK) # ori $at, $at, 0xff
/* 0D2560 800D1960 3108FF00 */  andi  $t0, $t0, SR_IMASK
/* 0D2564 800D1964 01284824 */  and   $t1, $t1, $t0
/* 0D2568 800D1968 0361D824 */  and   $k1, $k1, $at
/* 0D256C 800D196C 0369D825 */  or    $k1, $k1, $t1
/* 0D2570 800D1970 409B6000 */  mtc0  $k1, C0_SR
/* 0D2574 800D1974 DF5B0108 */  ld    $k1, THREAD_LO($k0)
/* 0D2578 800D1978 DF410020 */  ld    $at, THREAD_AT($k0)
/* 0D257C 800D197C DF420028 */  ld    $v0, THREAD_V0($k0)
/* 0D2580 800D1980 03600013 */  mtlo  $k1
/* 0D2584 800D1984 DF5B0110 */  ld    $k1, THREAD_HI($k0)
/* 0D2588 800D1988 DF430030 */  ld    $v1, THREAD_V1($k0)
/* 0D258C 800D198C DF440038 */  ld    $a0, THREAD_A0($k0)
/* 0D2590 800D1990 DF450040 */  ld    $a1, THREAD_A1($k0)
/* 0D2594 800D1994 DF460048 */  ld    $a2, THREAD_A2($k0)
/* 0D2598 800D1998 DF470050 */  ld    $a3, THREAD_A3($k0)
/* 0D259C 800D199C DF480058 */  ld    $t0, THREAD_T0($k0)
/* 0D25A0 800D19A0 DF490060 */  ld    $t1, THREAD_T1($k0)
/* 0D25A4 800D19A4 DF4A0068 */  ld    $t2, THREAD_T2($k0)
/* 0D25A8 800D19A8 DF4B0070 */  ld    $t3, THREAD_T3($k0)
/* 0D25AC 800D19AC DF4C0078 */  ld    $t4, THREAD_T4($k0)
/* 0D25B0 800D19B0 DF4D0080 */  ld    $t5, THREAD_T5($k0)
/* 0D25B4 800D19B4 DF4E0088 */  ld    $t6, THREAD_T6($k0)
/* 0D25B8 800D19B8 DF4F0090 */  ld    $t7, THREAD_T7($k0)
/* 0D25BC 800D19BC DF500098 */  ld    $s0, THREAD_S0($k0)
/* 0D25C0 800D19C0 DF5100A0 */  ld    $s1, THREAD_S1($k0)
/* 0D25C4 800D19C4 DF5200A8 */  ld    $s2, THREAD_S2($k0)
/* 0D25C8 800D19C8 DF5300B0 */  ld    $s3, THREAD_S3($k0)
/* 0D25CC 800D19CC DF5400B8 */  ld    $s4, THREAD_S4($k0)
/* 0D25D0 800D19D0 DF5500C0 */  ld    $s5, THREAD_S5($k0)
/* 0D25D4 800D19D4 DF5600C8 */  ld    $s6, THREAD_S6($k0)
/* 0D25D8 800D19D8 DF5700D0 */  ld    $s7, THREAD_S7($k0)
/* 0D25DC 800D19DC DF5800D8 */  ld    $t8, THREAD_T8($k0)
/* 0D25E0 800D19E0 DF5900E0 */  ld    $t9, THREAD_T9($k0)
/* 0D25E4 800D19E4 DF5C00E8 */  ld    $gp, THREAD_GP($k0)
/* 0D25E8 800D19E8 03600011 */  mthi  $k1
/* 0D25EC 800D19EC DF5D00F0 */  ld    $sp, THREAD_SP($k0)
/* 0D25F0 800D19F0 DF5E00F8 */  ld    $fp, THREAD_S8($k0)
/* 0D25F4 800D19F4 DF5F0100 */  ld    $ra, THREAD_RA($k0)
/* 0D25F8 800D19F8 8F5B011C */  lw    $k1, THREAD_PC($k0)
/* 0D25FC 800D19FC 409B7000 */  mtc0  $k1, C0_EPC
/* 0D2600 800D1A00 8F5B0018 */  lw    $k1, 0x18($k0)
/* 0D2604 800D1A04 13600013 */  beqz  $k1, .L800D1A54
/* 0D2608 800D1A08 00000000 */   nop   
/* 0D260C 800D1A0C 8F5B012C */  lw    $k1, THREAD_FPCSR($k0)
/* 0D2610 800D1A10 44DBF800 */  ctc1  $k1, $31
/* 0D2614 800D1A14 D7400130 */  ldc1  $f0, THREAD_FP0($k0)
/* 0D2618 800D1A18 D7420138 */  ldc1  $f2, THREAD_FP2($k0)
/* 0D261C 800D1A1C D7440140 */  ldc1  $f4, THREAD_FP4($k0)
/* 0D2620 800D1A20 D7460148 */  ldc1  $f6, THREAD_FP6($k0)
/* 0D2624 800D1A24 D7480150 */  ldc1  $f8, THREAD_FP8($k0)
/* 0D2628 800D1A28 D74A0158 */  ldc1  $f10, THREAD_FP10($k0)
/* 0D262C 800D1A2C D74C0160 */  ldc1  $f12, THREAD_FP12($k0)
/* 0D2630 800D1A30 D74E0168 */  ldc1  $f14, THREAD_FP14($k0)
/* 0D2634 800D1A34 D7500170 */  ldc1  $f16, THREAD_FP16($k0)
/* 0D2638 800D1A38 D7520178 */  ldc1  $f18, THREAD_FP18($k0)
/* 0D263C 800D1A3C D7540180 */  ldc1  $f20, THREAD_FP20($k0)
/* 0D2640 800D1A40 D7560188 */  ldc1  $f22, THREAD_FP22($k0)
/* 0D2644 800D1A44 D7580190 */  ldc1  $f24, THREAD_FP24($k0)
/* 0D2648 800D1A48 D75A0198 */  ldc1  $f26, THREAD_FP26($k0)
/* 0D264C 800D1A4C D75C01A0 */  ldc1  $f28, THREAD_FP28($k0)
/* 0D2650 800D1A50 D75E01A8 */  ldc1  $f30, THREAD_FP30($k0)
.L800D1A54:
/* 0D2654 800D1A54 8F5B0128 */  lw    $k1, THREAD_RCP($k0)
/* 0D2658 800D1A58 3C1A800F */  lui   $k0, %hi(__OSGlobalIntMask) # $k0, 0x800f
/* 0D265C 800D1A5C 275AA5EC */  addiu $k0, %lo(__OSGlobalIntMask) # addiu $k0, $k0, -0x5a14
/* 0D2660 800D1A60 8F5A0000 */  lw    $k0, ($k0)
/* 0D2664 800D1A64 001AD402 */  srl   $k0, $k0, 0x10
/* 0D2668 800D1A68 037AD824 */  and   $k1, $k1, $k0
/* 0D266C 800D1A6C 001BD840 */  sll   $k1, $k1, 1
/* 0D2670 800D1A70 3C1A800F */  lui   $k0, %hi(__osRcpImTable) # $k0, 0x800f
/* 0D2674 800D1A74 275A3C10 */  addiu $k0, %lo(__osRcpImTable) # addiu $k0, $k0, 0x3c10
/* 0D2678 800D1A78 037AD821 */  addu  $k1, $k1, $k0
/* 0D267C 800D1A7C 977B0000 */  lhu   $k1, ($k1)
/* 0D2680 800D1A80 3C1AA430 */  lui   $k0, %hi(PHYS_TO_K1|MI_INTR_MASK_REG) # $k0, 0xa430
/* 0D2684 800D1A84 275A000C */  addiu $k0, %lo(PHYS_TO_K1|MI_INTR_MASK_REG) # addiu $k0, $k0, 0xc
/* 0D2688 800D1A88 AF5B0000 */  sw    $k1, ($k0)
/* 0D268C 800D1A8C 00000000 */  nop   
/* 0D2690 800D1A90 00000000 */  nop   
/* 0D2694 800D1A94 00000000 */  nop   
/* 0D2698 800D1A98 00000000 */  nop   
/* 0D269C 800D1A9C 42000018 */  eret  
glabel __osCleanupThread
/* 0D26A0 800D1AA0 0C035E9C */  jal   osDestroyThread
/* 0D26A4 800D1AA4 00002025 */   move  $a0, $zero
/* 0D26A8 800D1AA8 00000000 */  nop   
/* 0D26AC 800D1AAC 00000000 */  nop   
