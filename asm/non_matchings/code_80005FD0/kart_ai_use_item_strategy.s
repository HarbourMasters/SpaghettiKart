.section .late_rodata

glabel jpt_800ED404
.word L8001ACAC, L8001AD64, L8001AF34, L8001AFD8
.word L8001B4AC, L8001B528, L8001B5B8, L8001B630
.word L8001B6AC, L8001B738, L8001B7BC, L8001B838
.word L8001B90C, L8001BA38, L8001BA98, L8001BB30
.word L8001BE30, L8001BE30, L8001BE30, L8001BE30
.word L8001BE30, L8001BE30, L8001BC28, L8001BC64
.word L8001BE30, L8001BC8C, L8001BCBC, L8001BCE0
.word L8001BD10, L8001BD34, L8001BD60, L8001BD94
.word L8001BDC8, L8001BDE4, L8001B0EC, L8001B2C4
.word L8001B3B4

glabel D_800ED498
.double 0.4

glabel jpt_800ED4A0
.word L8001B970, L8001B988, L8001B9A0, L8001B9B8
.word L8001B9D0

.section .text

glabel kart_ai_use_item_strategy
/* 01B810 8001AC10 000478C0 */  sll   $t7, $a0, 3
/* 01B814 8001AC14 01E47823 */  subu  $t7, $t7, $a0
/* 01B818 8001AC18 000F7900 */  sll   $t7, $t7, 4
/* 01B81C 8001AC1C 01E47823 */  subu  $t7, $t7, $a0
/* 01B820 8001AC20 3C18800E */  lui   $t8, %hi(gPlayerOne)
/* 01B824 8001AC24 8F18C4DC */  lw    $t8, %lo(gPlayerOne)($t8)
/* 01B828 8001AC28 000F7880 */  sll   $t7, $t7, 2
/* 01B82C 8001AC2C 3C0B800E */  lui   $t3, %hi(gModeSelection)
/* 01B830 8001AC30 8D6BC53C */  lw    $t3, %lo(gModeSelection)($t3)
/* 01B834 8001AC34 27BDFFD0 */  addiu $sp, $sp, -0x30
/* 01B838 8001AC38 01E47823 */  subu  $t7, $t7, $a0
/* 01B83C 8001AC3C 000F78C0 */  sll   $t7, $t7, 3
/* 01B840 8001AC40 24080001 */  li    $t0, 1
/* 01B844 8001AC44 01F8C821 */  addu  $t9, $t7, $t8
/* 01B848 8001AC48 AFBF0024 */  sw    $ra, 0x24($sp)
/* 01B84C 8001AC4C AFB00020 */  sw    $s0, 0x20($sp)
/* 01B850 8001AC50 AFA40030 */  sw    $a0, 0x30($sp)
/* 01B854 8001AC54 110B0483 */  beq   $t0, $t3, .L8001BE64
/* 01B858 8001AC58 AFB9002C */   sw    $t9, 0x2c($sp)
/* 01B85C 8001AC5C 3C0C8016 */  lui   $t4, %hi(D_801646CC) # $t4, 0x8016
/* 01B860 8001AC60 958C46CC */  lhu   $t4, %lo(D_801646CC)($t4)
/* 01B864 8001AC64 510C0480 */  beql  $t0, $t4, .L8001BE68
/* 01B868 8001AC68 8FBF0024 */   lw    $ra, 0x24($sp)
/* 01B86C 8001AC6C 972E0000 */  lhu   $t6, ($t9)
/* 01B870 8001AC70 3C0B8016 */  lui   $t3, %hi(D_801642D8) # $t3, 0x8016
/* 01B874 8001AC74 256B42D8 */  addiu $t3, %lo(D_801642D8) # addiu $t3, $t3, 0x42d8
/* 01B878 8001AC78 31CF0800 */  andi  $t7, $t6, 0x800
/* 01B87C 8001AC7C 15E00479 */  bnez  $t7, .L8001BE64
/* 01B880 8001AC80 0004C900 */   sll   $t9, $a0, 4
/* 01B884 8001AC84 032B8021 */  addu  $s0, $t9, $t3
/* 01B888 8001AC88 86050000 */  lh    $a1, ($s0)
/* 01B88C 8001AC8C 2CA10025 */  sltiu $at, $a1, 0x25
/* 01B890 8001AC90 10200467 */  beqz  $at, .L8001BE30
/* 01B894 8001AC94 00056080 */   sll   $t4, $a1, 2
/* 01B898 8001AC98 3C01800F */  lui   $at, %hi(jpt_800ED404) # 0x800f
/* 01B89C 8001AC9C 002C0821 */  addu  $at, $at, $t4
/* 01B8A0 8001ACA0 8C2CD404 */  lw    $t4, %lo(jpt_800ED404)($at) # -0x2bfc($at)
/* 01B8A4 8001ACA4 01800008 */  jr    $t4
/* 01B8A8 8001ACA8 00000000 */   nop
glabel L8001ACAC
/* 01B8AC 8001ACAC 2405FFFF */  li    $a1, -1
/* 01B8B0 8001ACB0 A6050002 */  sh    $a1, 2($s0)
/* 01B8B4 8001ACB4 8FAD0030 */  lw    $t5, 0x30($sp)
/* 01B8B8 8001ACB8 3C0E8016 */  lui   $t6, %hi(D_80164450) # 0x8016
/* 01B8BC 8001ACBC 8FA40030 */  lw    $a0, 0x30($sp)
/* 01B8C0 8001ACC0 000D1080 */  sll   $v0, $t5, 2
/* 01B8C4 8001ACC4 01C27021 */  addu  $t6, $t6, $v0
/* 01B8C8 8001ACC8 000D7880 */  sll   $t7, $t5, 2
/* 01B8CC 8001ACCC 8DCE4450 */  lw    $t6, %lo(D_80164450)($t6) # 0x4450($t6)
/* 01B8D0 8001ACD0 01ED7821 */  addu  $t7, $t7, $t5
/* 01B8D4 8001ACD4 000F7880 */  sll   $t7, $t7, 2
/* 01B8D8 8001ACD8 25F80064 */  addiu $t8, $t7, 0x64
/* 01B8DC 8001ACDC 030E082A */  slt   $at, $t8, $t6
/* 01B8E0 8001ACE0 1020001C */  beqz  $at, .L8001AD54
/* 01B8E4 8001ACE4 00000000 */   nop
/* 01B8E8 8001ACE8 86190004 */  lh    $t9, 4($s0)
/* 01B8EC 8001ACEC 2B210259 */  slti  $at, $t9, 0x259
/* 01B8F0 8001ACF0 14200018 */  bnez  $at, .L8001AD54
/* 01B8F4 8001ACF4 00000000 */   nop
/* 01B8F8 8001ACF8 860B0006 */  lh    $t3, 6($s0)
/* 01B8FC 8001ACFC 3C038016 */  lui   $v1, %hi(gLapCountByPlayerId) # 0x8016
/* 01B900 8001AD00 00621821 */  addu  $v1, $v1, $v0
/* 01B904 8001AD04 29610003 */  slti  $at, $t3, 3
/* 01B908 8001AD08 10200012 */  beqz  $at, .L8001AD54
/* 01B90C 8001AD0C 00000000 */   nop
/* 01B910 8001AD10 8C634390 */  lw    $v1, %lo(gLapCountByPlayerId)($v1) # 0x4390($v1)
/* 01B914 8001AD14 3C068016 */  lui   $a2, %hi(gGPCurrentRaceRankByPlayerId) # $a2, 0x8016
/* 01B918 8001AD18 24C643B8 */  addiu $a2, %lo(gGPCurrentRaceRankByPlayerId) # addiu $a2, $a2, 0x43b8
/* 01B91C 8001AD1C 28610003 */  slti  $at, $v1, 3
/* 01B920 8001AD20 1020000C */  beqz  $at, .L8001AD54
/* 01B924 8001AD24 00C26821 */   addu  $t5, $a2, $v0
/* 01B928 8001AD28 00032400 */  sll   $a0, $v1, 0x10
/* 01B92C 8001AD2C 00046403 */  sra   $t4, $a0, 0x10
/* 01B930 8001AD30 01802025 */  move  $a0, $t4
/* 01B934 8001AD34 0C01EBDE */  jal   kart_ai_gen_random_item
/* 01B938 8001AD38 85A50002 */   lh    $a1, 2($t5)
/* 01B93C 8001AD3C 8FA40030 */  lw    $a0, 0x30($sp)
/* 01B940 8001AD40 02002825 */  move  $a1, $s0
/* 01B944 8001AD44 0C006ADD */  jal   kart_ai_decisions_branch_item
/* 01B948 8001AD48 00403025 */   move  $a2, $v0
/* 01B94C 8001AD4C 10000003 */  b     .L8001AD5C
/* 01B950 8001AD50 00000000 */   nop
.L8001AD54:
/* 01B954 8001AD54 0C006AF8 */  jal   func_8001ABE0
/* 01B958 8001AD58 02002825 */   move  $a1, $s0
.L8001AD5C:
/* 01B95C 8001AD5C 10000435 */  b     .L8001BE34
/* 01B960 8001AD60 86020004 */   lh    $v0, 4($s0)
glabel L8001AD64
/* 01B964 8001AD64 8FA20030 */  lw    $v0, 0x30($sp)
/* 01B968 8001AD68 3C0E8016 */  lui   $t6, %hi(gLapCountByPlayerId) # 0x8016
/* 01B96C 8001AD6C 3C068016 */  lui   $a2, %hi(gGPCurrentRaceRankByPlayerId) # $a2, 0x8016
/* 01B970 8001AD70 00027880 */  sll   $t7, $v0, 2
/* 01B974 8001AD74 01CF7021 */  addu  $t6, $t6, $t7
/* 01B978 8001AD78 8DCE4390 */  lw    $t6, %lo(gLapCountByPlayerId)($t6) # 0x4390($t6)
/* 01B97C 8001AD7C 24C643B8 */  addiu $a2, %lo(gGPCurrentRaceRankByPlayerId) # addiu $a2, $a2, 0x43b8
/* 01B980 8001AD80 3C038016 */  lui   $v1, %hi(D_80163478) # $v1, 0x8016
/* 01B984 8001AD84 19C0004B */  blez  $t6, .L8001AEB4
/* 01B988 8001AD88 00000000 */   nop
/* 01B98C 8001AD8C 84633478 */  lh    $v1, %lo(D_80163478)($v1)
/* 01B990 8001AD90 00CF5821 */  addu  $t3, $a2, $t7
/* 01B994 8001AD94 8D6C0000 */  lw    $t4, ($t3)
/* 01B998 8001AD98 0003C080 */  sll   $t8, $v1, 2
/* 01B99C 8001AD9C 00D8C821 */  addu  $t9, $a2, $t8
/* 01B9A0 8001ADA0 8F240000 */  lw    $a0, ($t9)
/* 01B9A4 8001ADA4 008C082A */  slt   $at, $a0, $t4
/* 01B9A8 8001ADA8 10200042 */  beqz  $at, .L8001AEB4
/* 01B9AC 8001ADAC 00000000 */   nop
/* 01B9B0 8001ADB0 14800040 */  bnez  $a0, .L8001AEB4
/* 01B9B4 8001ADB4 8FAD002C */   lw    $t5, 0x2c($sp)
/* 01B9B8 8001ADB8 95A20254 */  lhu   $v0, 0x254($t5)
/* 01B9BC 8001ADBC 24010004 */  li    $at, 4
/* 01B9C0 8001ADC0 3C088016 */  lui   $t0, %hi(gNearestWaypointByPlayerId) # $t0, 0x8016
/* 01B9C4 8001ADC4 10410006 */  beq   $v0, $at, .L8001ADE0
/* 01B9C8 8001ADC8 25084438 */   addiu $t0, %lo(gNearestWaypointByPlayerId) # addiu $t0, $t0, 0x4438
/* 01B9CC 8001ADCC 24060006 */  li    $a2, 6
/* 01B9D0 8001ADD0 10460014 */  beq   $v0, $a2, .L8001AE24
/* 01B9D4 8001ADD4 3C088016 */   lui   $t0, %hi(gNearestWaypointByPlayerId) # 0x8016
/* 01B9D8 8001ADD8 10000024 */  b     .L8001AE6C
/* 01B9DC 8001ADDC 3C088016 */   lui   $t0, %hi(gNearestWaypointByPlayerId) # $t0, 0x8016
.L8001ADE0:
/* 01B9E0 8001ADE0 8FAF0030 */  lw    $t7, 0x30($sp)
/* 01B9E4 8001ADE4 0003C840 */  sll   $t9, $v1, 1
/* 01B9E8 8001ADE8 3C0C8016 */  lui   $t4, %hi(D_80164430) # $t4, 0x8016
/* 01B9EC 8001ADEC 000F7040 */  sll   $t6, $t7, 1
/* 01B9F0 8001ADF0 958C4430 */  lhu   $t4, %lo(D_80164430)($t4)
/* 01B9F4 8001ADF4 010EC021 */  addu  $t8, $t0, $t6
/* 01B9F8 8001ADF8 01195821 */  addu  $t3, $t0, $t9
/* 01B9FC 8001ADFC 95650000 */  lhu   $a1, ($t3)
/* 01BA00 8001AE00 97040000 */  lhu   $a0, ($t8)
/* 01BA04 8001AE04 24060028 */  li    $a2, 40
/* 01BA08 8001AE08 24070002 */  li    $a3, 2
/* 01BA0C 8001AE0C 0C001EFE */  jal   func_80007BF8
/* 01BA10 8001AE10 AFAC0010 */   sw    $t4, 0x10($sp)
/* 01BA14 8001AE14 18400045 */  blez  $v0, .L8001AF2C
/* 01BA18 8001AE18 240D0022 */   li    $t5, 34
/* 01BA1C 8001AE1C 10000043 */  b     .L8001AF2C
/* 01BA20 8001AE20 A60D0000 */   sh    $t5, ($s0)
.L8001AE24:
/* 01BA24 8001AE24 8FAF0030 */  lw    $t7, 0x30($sp)
/* 01BA28 8001AE28 25084438 */  addiu $t0, %lo(gNearestWaypointByPlayerId) # addiu $t0, $t0, 0x4438
/* 01BA2C 8001AE2C 0003C840 */  sll   $t9, $v1, 1
/* 01BA30 8001AE30 3C0C8016 */  lui   $t4, %hi(D_80164430) # $t4, 0x8016
/* 01BA34 8001AE34 000F7040 */  sll   $t6, $t7, 1
/* 01BA38 8001AE38 958C4430 */  lhu   $t4, %lo(D_80164430)($t4)
/* 01BA3C 8001AE3C 010EC021 */  addu  $t8, $t0, $t6
/* 01BA40 8001AE40 01195821 */  addu  $t3, $t0, $t9
/* 01BA44 8001AE44 95650000 */  lhu   $a1, ($t3)
/* 01BA48 8001AE48 97040000 */  lhu   $a0, ($t8)
/* 01BA4C 8001AE4C 24060004 */  li    $a2, 4
/* 01BA50 8001AE50 24070002 */  li    $a3, 2
/* 01BA54 8001AE54 0C001EFE */  jal   func_80007BF8
/* 01BA58 8001AE58 AFAC0010 */   sw    $t4, 0x10($sp)
/* 01BA5C 8001AE5C 18400033 */  blez  $v0, .L8001AF2C
/* 01BA60 8001AE60 240D0022 */   li    $t5, 34
/* 01BA64 8001AE64 10000031 */  b     .L8001AF2C
/* 01BA68 8001AE68 A60D0000 */   sh    $t5, ($s0)
.L8001AE6C:
/* 01BA6C 8001AE6C 8FAF0030 */  lw    $t7, 0x30($sp)
/* 01BA70 8001AE70 25084438 */  addiu $t0, $t0, %lo(gNearestWaypointByPlayerId) # 0x4438
/* 01BA74 8001AE74 0003C840 */  sll   $t9, $v1, 1
/* 01BA78 8001AE78 3C0C8016 */  lui   $t4, %hi(D_80164430) # $t4, 0x8016
/* 01BA7C 8001AE7C 000F7040 */  sll   $t6, $t7, 1
/* 01BA80 8001AE80 958C4430 */  lhu   $t4, %lo(D_80164430)($t4)
/* 01BA84 8001AE84 010EC021 */  addu  $t8, $t0, $t6
/* 01BA88 8001AE88 01195821 */  addu  $t3, $t0, $t9
/* 01BA8C 8001AE8C 95650000 */  lhu   $a1, ($t3)
/* 01BA90 8001AE90 97040000 */  lhu   $a0, ($t8)
/* 01BA94 8001AE94 2406000A */  li    $a2, 10
/* 01BA98 8001AE98 24070002 */  li    $a3, 2
/* 01BA9C 8001AE9C 0C001EFE */  jal   func_80007BF8
/* 01BAA0 8001AEA0 AFAC0010 */   sw    $t4, 0x10($sp)
/* 01BAA4 8001AEA4 18400021 */  blez  $v0, .L8001AF2C
/* 01BAA8 8001AEA8 240D0022 */   li    $t5, 34
/* 01BAAC 8001AEAC 1000001F */  b     .L8001AF2C
/* 01BAB0 8001AEB0 A60D0000 */   sh    $t5, ($s0)
.L8001AEB4:
/* 01BAB4 8001AEB4 1505001D */  bne   $t0, $a1, .L8001AF2C
/* 01BAB8 8001AEB8 00000000 */   nop
/* 01BABC 8001AEBC 0C0ACB5C */  jal   use_banana_item
/* 01BAC0 8001AEC0 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01BAC4 8001AEC4 A6020002 */  sh    $v0, 2($s0)
/* 01BAC8 8001AEC8 86030002 */  lh    $v1, 2($s0)
/* 01BACC 8001AECC 04600015 */  bltz  $v1, .L8001AF24
/* 01BAD0 8001AED0 28610064 */   slti  $at, $v1, 0x64
/* 01BAD4 8001AED4 10200013 */  beqz  $at, .L8001AF24
/* 01BAD8 8001AED8 8FAF002C */   lw    $t7, 0x2c($sp)
/* 01BADC 8001AEDC 8DEE000C */  lw    $t6, 0xc($t7)
/* 01BAE0 8001AEE0 3C010004 */  lui   $at, 4
/* 01BAE4 8001AEE4 24190002 */  li    $t9, 2
/* 01BAE8 8001AEE8 01C1C025 */  or    $t8, $t6, $at
/* 01BAEC 8001AEEC ADF8000C */  sw    $t8, 0xc($t7)
/* 01BAF0 8001AEF0 860B0006 */  lh    $t3, 6($s0)
/* 01BAF4 8001AEF4 A6190000 */  sh    $t9, ($s0)
/* 01BAF8 8001AEF8 A6000004 */  sh    $zero, 4($s0)
/* 01BAFC 8001AEFC 256C0001 */  addiu $t4, $t3, 1
/* 01BB00 8001AF00 A60C0006 */  sh    $t4, 6($s0)
/* 01BB04 8001AF04 0C0ADF8D */  jal   random_int
/* 01BB08 8001AF08 24040003 */   li    $a0, 3
/* 01BB0C 8001AF0C 00026880 */  sll   $t5, $v0, 2
/* 01BB10 8001AF10 01A26821 */  addu  $t5, $t5, $v0
/* 01BB14 8001AF14 000D6880 */  sll   $t5, $t5, 2
/* 01BB18 8001AF18 25AE000A */  addiu $t6, $t5, 0xa
/* 01BB1C 8001AF1C 10000003 */  b     .L8001AF2C
/* 01BB20 8001AF20 A60E000E */   sh    $t6, 0xe($s0)
.L8001AF24:
/* 01BB24 8001AF24 A6000000 */  sh    $zero, ($s0)
/* 01BB28 8001AF28 A6000004 */  sh    $zero, 4($s0)
.L8001AF2C:
/* 01BB2C 8001AF2C 100003C1 */  b     .L8001BE34
/* 01BB30 8001AF30 86020004 */   lh    $v0, 4($s0)
glabel L8001AF34
/* 01BB34 8001AF34 86180002 */  lh    $t8, 2($s0)
/* 01BB38 8001AF38 3C198016 */  lui   $t9, %hi(gActorList) # $t9, 0x8016
/* 01BB3C 8001AF3C 2739F9B8 */  addiu $t9, %lo(gActorList) # addiu $t9, $t9, -0x648
/* 01BB40 8001AF40 001878C0 */  sll   $t7, $t8, 3
/* 01BB44 8001AF44 01F87823 */  subu  $t7, $t7, $t8
/* 01BB48 8001AF48 000F7900 */  sll   $t7, $t7, 4
/* 01BB4C 8001AF4C 01F93821 */  addu  $a3, $t7, $t9
/* 01BB50 8001AF50 84E30002 */  lh    $v1, 2($a3)
/* 01BB54 8001AF54 306B8000 */  andi  $t3, $v1, 0x8000
/* 01BB58 8001AF58 5160000D */  beql  $t3, $zero, .L8001AF90
/* 01BB5C 8001AF5C A6000000 */   sh    $zero, ($s0)
/* 01BB60 8001AF60 84EC0000 */  lh    $t4, ($a3)
/* 01BB64 8001AF64 24060006 */  li    $a2, 6
/* 01BB68 8001AF68 54CC0009 */  bnel  $a2, $t4, .L8001AF90
/* 01BB6C 8001AF6C A6000000 */   sh    $zero, ($s0)
/* 01BB70 8001AF70 84ED0006 */  lh    $t5, 6($a3)
/* 01BB74 8001AF74 8FAE0030 */  lw    $t6, 0x30($sp)
/* 01BB78 8001AF78 55A00005 */  bnel  $t5, $zero, .L8001AF90
/* 01BB7C 8001AF7C A6000000 */   sh    $zero, ($s0)
/* 01BB80 8001AF80 84F80010 */  lh    $t8, 0x10($a3)
/* 01BB84 8001AF84 51D8000C */  beql  $t6, $t8, .L8001AFB8
/* 01BB88 8001AF88 86020004 */   lh    $v0, 4($s0)
/* 01BB8C 8001AF8C A6000000 */  sh    $zero, ($s0)
.L8001AF90:
/* 01BB90 8001AF90 A6000004 */  sh    $zero, 4($s0)
/* 01BB94 8001AF94 8FAF002C */  lw    $t7, 0x2c($sp)
/* 01BB98 8001AF98 3C01FFFB */  lui   $at, (0xFFFBFFFF >> 16) # lui $at, 0xfffb
/* 01BB9C 8001AF9C 3421FFFF */  ori   $at, (0xFFFBFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 01BBA0 8001AFA0 8DF9000C */  lw    $t9, 0xc($t7)
/* 01BBA4 8001AFA4 03215824 */  and   $t3, $t9, $at
/* 01BBA8 8001AFA8 ADEB000C */  sw    $t3, 0xc($t7)
/* 01BBAC 8001AFAC 100003A1 */  b     .L8001BE34
/* 01BBB0 8001AFB0 86020004 */   lh    $v0, 4($s0)
/* 01BBB4 8001AFB4 86020004 */  lh    $v0, 4($s0)
.L8001AFB8:
/* 01BBB8 8001AFB8 860C000E */  lh    $t4, 0xe($s0)
/* 01BBBC 8001AFBC 240D0003 */  li    $t5, 3
/* 01BBC0 8001AFC0 0182082A */  slt   $at, $t4, $v0
/* 01BBC4 8001AFC4 5020039C */  beql  $at, $zero, .L8001BE38
/* 01BBC8 8001AFC8 28412710 */   slti  $at, $v0, 0x2710
/* 01BBCC 8001AFCC A60D0000 */  sh    $t5, ($s0)
/* 01BBD0 8001AFD0 10000398 */  b     .L8001BE34
/* 01BBD4 8001AFD4 86020004 */   lh    $v0, 4($s0)
glabel L8001AFD8
/* 01BBD8 8001AFD8 860E0002 */  lh    $t6, 2($s0)
/* 01BBDC 8001AFDC 3C198016 */  lui   $t9, %hi(gActorList) # $t9, 0x8016
/* 01BBE0 8001AFE0 2739F9B8 */  addiu $t9, %lo(gActorList) # addiu $t9, $t9, -0x648
/* 01BBE4 8001AFE4 000EC0C0 */  sll   $t8, $t6, 3
/* 01BBE8 8001AFE8 030EC023 */  subu  $t8, $t8, $t6
/* 01BBEC 8001AFEC 0018C100 */  sll   $t8, $t8, 4
/* 01BBF0 8001AFF0 03193821 */  addu  $a3, $t8, $t9
/* 01BBF4 8001AFF4 84E30002 */  lh    $v1, 2($a3)
/* 01BBF8 8001AFF8 8FB80030 */  lw    $t8, 0x30($sp)
/* 01BBFC 8001AFFC 306B8000 */  andi  $t3, $v1, 0x8000
/* 01BC00 8001B000 5160000D */  beql  $t3, $zero, .L8001B038
/* 01BC04 8001B004 84E50010 */   lh    $a1, 0x10($a3)
/* 01BC08 8001B008 84EF0000 */  lh    $t7, ($a3)
/* 01BC0C 8001B00C 24060006 */  li    $a2, 6
/* 01BC10 8001B010 54CF0009 */  bnel  $a2, $t7, .L8001B038
/* 01BC14 8001B014 84E50010 */   lh    $a1, 0x10($a3)
/* 01BC18 8001B018 84EC0006 */  lh    $t4, 6($a3)
/* 01BC1C 8001B01C 8FAD0030 */  lw    $t5, 0x30($sp)
/* 01BC20 8001B020 55800005 */  bnel  $t4, $zero, .L8001B038
/* 01BC24 8001B024 84E50010 */   lh    $a1, 0x10($a3)
/* 01BC28 8001B028 84EE0010 */  lh    $t6, 0x10($a3)
/* 01BC2C 8001B02C 51AE0007 */  beql  $t5, $t6, .L8001B04C
/* 01BC30 8001B030 44800000 */   mtc1  $zero, $f0
/* 01BC34 8001B034 84E50010 */  lh    $a1, 0x10($a3)
.L8001B038:
/* 01BC38 8001B038 53050023 */  beql  $t8, $a1, .L8001B0C8
/* 01BC3C 8001B03C 8FAD002C */   lw    $t5, 0x2c($sp)
/* 01BC40 8001B040 10000021 */  b     .L8001B0C8
/* 01BC44 8001B044 8FAD002C */   lw    $t5, 0x2c($sp)
/* 01BC48 8001B048 44800000 */  mtc1  $zero, $f0
.L8001B04C:
/* 01BC4C 8001B04C A4E80006 */  sh    $t0, 6($a3)
/* 01BC50 8001B050 3C0F8016 */  lui   $t7, %hi(D_801631E0) # 0x8016
/* 01BC54 8001B054 E4E00024 */  swc1  $f0, 0x24($a3)
/* 01BC58 8001B058 E4E00028 */  swc1  $f0, 0x28($a3)
/* 01BC5C 8001B05C E4E0002C */  swc1  $f0, 0x2c($a3)
/* 01BC60 8001B060 8FB90030 */  lw    $t9, 0x30($sp)
/* 01BC64 8001B064 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01BC68 8001B068 3C01403E */  li    $at, 0x403E0000 # 2.968750
/* 01BC6C 8001B06C 00195840 */  sll   $t3, $t9, 1
/* 01BC70 8001B070 01EB7821 */  addu  $t7, $t7, $t3
/* 01BC74 8001B074 95EF31E0 */  lhu   $t7, %lo(D_801631E0)($t7) # 0x31e0($t7)
/* 01BC78 8001B078 550F0013 */  bnel  $t0, $t7, .L8001B0C8
/* 01BC7C 8001B07C 8FAD002C */   lw    $t5, 0x2c($sp)
/* 01BC80 8001B080 C5840018 */  lwc1  $f4, 0x18($t4)
/* 01BC84 8001B084 44814800 */  mtc1  $at, $f9
/* 01BC88 8001B088 44804000 */  mtc1  $zero, $f8
/* 01BC8C 8001B08C 460021A1 */  cvt.d.s $f6, $f4
/* 01BC90 8001B090 C58C0014 */  lwc1  $f12, 0x14($t4)
/* 01BC94 8001B094 46283280 */  add.d $f10, $f6, $f8
/* 01BC98 8001B098 8D86001C */  lw    $a2, 0x1c($t4)
/* 01BC9C 8001B09C AFA70028 */  sw    $a3, 0x28($sp)
/* 01BCA0 8001B0A0 0C0AB870 */  jal   spawn_actor_on_surface
/* 01BCA4 8001B0A4 462053A0 */   cvt.s.d $f14, $f10
/* 01BCA8 8001B0A8 8FA70028 */  lw    $a3, 0x28($sp)
/* 01BCAC 8001B0AC 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 01BCB0 8001B0B0 44819000 */  mtc1  $at, $f18
/* 01BCB4 8001B0B4 C4F0000C */  lwc1  $f16, 0xc($a3)
/* 01BCB8 8001B0B8 46128100 */  add.s $f4, $f16, $f18
/* 01BCBC 8001B0BC 46040180 */  add.s $f6, $f0, $f4
/* 01BCC0 8001B0C0 E4E6001C */  swc1  $f6, 0x1c($a3)
/* 01BCC4 8001B0C4 8FAD002C */  lw    $t5, 0x2c($sp)
.L8001B0C8:
/* 01BCC8 8001B0C8 3C01FFFB */  lui   $at, (0xFFFBFFFF >> 16) # lui $at, 0xfffb
/* 01BCCC 8001B0CC 3421FFFF */  ori   $at, (0xFFFBFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 01BCD0 8001B0D0 8DAE000C */  lw    $t6, 0xc($t5)
/* 01BCD4 8001B0D4 01C1C024 */  and   $t8, $t6, $at
/* 01BCD8 8001B0D8 ADB8000C */  sw    $t8, 0xc($t5)
/* 01BCDC 8001B0DC A6000004 */  sh    $zero, 4($s0)
/* 01BCE0 8001B0E0 A6000000 */  sh    $zero, ($s0)
/* 01BCE4 8001B0E4 10000353 */  b     .L8001BE34
/* 01BCE8 8001B0E8 86020004 */   lh    $v0, 4($s0)
glabel L8001B0EC
/* 01BCEC 8001B0EC 0C0ACB5C */  jal   use_banana_item
/* 01BCF0 8001B0F0 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01BCF4 8001B0F4 8FAA0030 */  lw    $t2, 0x30($sp)
/* 01BCF8 8001B0F8 A6020002 */  sh    $v0, 2($s0)
/* 01BCFC 8001B0FC 86030002 */  lh    $v1, 2($s0)
/* 01BD00 8001B100 0460006C */  bltz  $v1, .L8001B2B4
/* 01BD04 8001B104 28610064 */   slti  $at, $v1, 0x64
/* 01BD08 8001B108 1020006A */  beqz  $at, .L8001B2B4
/* 01BD0C 8001B10C 3C088016 */   lui   $t0, %hi(gNearestWaypointByPlayerId) # $t0, 0x8016
/* 01BD10 8001B110 0003C8C0 */  sll   $t9, $v1, 3
/* 01BD14 8001B114 0323C823 */  subu  $t9, $t9, $v1
/* 01BD18 8001B118 3C0B8016 */  lui   $t3, %hi(gActorList) # $t3, 0x8016
/* 01BD1C 8001B11C 256BF9B8 */  addiu $t3, %lo(gActorList) # addiu $t3, $t3, -0x648
/* 01BD20 8001B120 0019C900 */  sll   $t9, $t9, 4
/* 01BD24 8001B124 032B3821 */  addu  $a3, $t9, $t3
/* 01BD28 8001B128 240F0004 */  li    $t7, 4
/* 01BD2C 8001B12C A4EF0006 */  sh    $t7, 6($a3)
/* 01BD30 8001B130 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01BD34 8001B134 3C014034 */  li    $at, 0x40340000 # 2.812500
/* 01BD38 8001B138 44810800 */  mtc1  $at, $f1
/* 01BD3C 8001B13C 8D8E000C */  lw    $t6, 0xc($t4)
/* 01BD40 8001B140 3C010004 */  lui   $at, 4
/* 01BD44 8001B144 240D0023 */  li    $t5, 35
/* 01BD48 8001B148 01C1C025 */  or    $t8, $t6, $at
/* 01BD4C 8001B14C AD98000C */  sw    $t8, 0xc($t4)
/* 01BD50 8001B150 86190006 */  lh    $t9, 6($s0)
/* 01BD54 8001B154 A60D0000 */  sh    $t5, ($s0)
/* 01BD58 8001B158 A6000004 */  sh    $zero, 4($s0)
/* 01BD5C 8001B15C 272B0001 */  addiu $t3, $t9, 1
/* 01BD60 8001B160 A60B0006 */  sh    $t3, 6($s0)
/* 01BD64 8001B164 3C068016 */  lui   $a2, %hi(D_80163478) # $a2, 0x8016
/* 01BD68 8001B168 84C63478 */  lh    $a2, %lo(D_80163478)($a2)
/* 01BD6C 8001B16C 25084438 */  addiu $t0, %lo(gNearestWaypointByPlayerId) # addiu $t0, $t0, 0x4438
/* 01BD70 8001B170 3C098016 */  lui   $t1, %hi(gPathIndexByPlayerId) # $t1, 0x8016
/* 01BD74 8001B174 00067840 */  sll   $t7, $a2, 1
/* 01BD78 8001B178 010F7021 */  addu  $t6, $t0, $t7
/* 01BD7C 8001B17C 95D80000 */  lhu   $t8, ($t6)
/* 01BD80 8001B180 252945B0 */  addiu $t1, %lo(gPathIndexByPlayerId) # addiu $t1, $t1, 0x45b0
/* 01BD84 8001B184 012F6821 */  addu  $t5, $t1, $t7
/* 01BD88 8001B188 95B90000 */  lhu   $t9, ($t5)
/* 01BD8C 8001B18C 270C001E */  addiu $t4, $t8, 0x1e
/* 01BD90 8001B190 01E03025 */  move  $a2, $t7
/* 01BD94 8001B194 3C0F8016 */  lui   $t7, %hi(gWaypointCountByPathIndex) # 0x8016
/* 01BD98 8001B198 00195840 */  sll   $t3, $t9, 1
/* 01BD9C 8001B19C 01EB7821 */  addu  $t7, $t7, $t3
/* 01BDA0 8001B1A0 95EF45C8 */  lhu   $t7, %lo(gWaypointCountByPathIndex)($t7) # 0x45c8($t7)
/* 01BDA4 8001B1A4 952D0000 */  lhu   $t5, ($t1)
/* 01BDA8 8001B1A8 3C0B8016 */  lui   $t3, %hi(D_80164550) # 0x8016
/* 01BDAC 8001B1AC 018F001A */  div   $zero, $t4, $t7
/* 01BDB0 8001B1B0 000DC880 */  sll   $t9, $t5, 2
/* 01BDB4 8001B1B4 01795821 */  addu  $t3, $t3, $t9
/* 01BDB8 8001B1B8 8D6B4550 */  lw    $t3, %lo(D_80164550)($t3) # 0x4550($t3)
/* 01BDBC 8001B1BC 00007010 */  mfhi  $t6
/* 01BDC0 8001B1C0 000EC0C0 */  sll   $t8, $t6, 3
/* 01BDC4 8001B1C4 030B1021 */  addu  $v0, $t8, $t3
/* 01BDC8 8001B1C8 44800000 */  mtc1  $zero, $f0
/* 01BDCC 8001B1CC 15E00002 */  bnez  $t7, .L8001B1D8
/* 01BDD0 8001B1D0 00000000 */   nop
/* 01BDD4 8001B1D4 0007000D */  break 7
.L8001B1D8:
/* 01BDD8 8001B1D8 2401FFFF */  li    $at, -1
/* 01BDDC 8001B1DC 15E10004 */  bne   $t7, $at, .L8001B1F0
/* 01BDE0 8001B1E0 3C018000 */   lui   $at, 0x8000
/* 01BDE4 8001B1E4 15810002 */  bne   $t4, $at, .L8001B1F0
/* 01BDE8 8001B1E8 00000000 */   nop
/* 01BDEC 8001B1EC 0006000D */  break 6
.L8001B1F0:
/* 01BDF0 8001B1F0 844C0000 */  lh    $t4, ($v0)
/* 01BDF4 8001B1F4 8FAF002C */  lw    $t7, 0x2c($sp)
/* 01BDF8 8001B1F8 3C014010 */  li    $at, 0x40100000 # 2.250000
/* 01BDFC 8001B1FC 448C4000 */  mtc1  $t4, $f8
/* 01BE00 8001B200 C5F00014 */  lwc1  $f16, 0x14($t7)
/* 01BE04 8001B204 3C052900 */  lui   $a1, (0x29008009 >> 16) # lui $a1, 0x2900
/* 01BE08 8001B208 468042A0 */  cvt.s.w $f10, $f8
/* 01BE0C 8001B20C 44815800 */  mtc1  $at, $f11
/* 01BE10 8001B210 34A58009 */  ori   $a1, (0x29008009 & 0xFFFF) # ori $a1, $a1, 0x8009
/* 01BE14 8001B214 314400FF */  andi  $a0, $t2, 0xff
/* 01BE18 8001B218 46105481 */  sub.s $f18, $f10, $f16
/* 01BE1C 8001B21C 46009121 */  cvt.d.s $f4, $f18
/* 01BE20 8001B220 46202183 */  div.d $f6, $f4, $f0
/* 01BE24 8001B224 46203220 */  cvt.s.d $f8, $f6
/* 01BE28 8001B228 E4E80024 */  swc1  $f8, 0x24($a3)
/* 01BE2C 8001B22C 844E0002 */  lh    $t6, 2($v0)
/* 01BE30 8001B230 8FAD002C */  lw    $t5, 0x2c($sp)
/* 01BE34 8001B234 448E5000 */  mtc1  $t6, $f10
/* 01BE38 8001B238 C5B20018 */  lwc1  $f18, 0x18($t5)
/* 01BE3C 8001B23C 46805420 */  cvt.s.w $f16, $f10
/* 01BE40 8001B240 44805000 */  mtc1  $zero, $f10
/* 01BE44 8001B244 46128101 */  sub.s $f4, $f16, $f18
/* 01BE48 8001B248 460021A1 */  cvt.d.s $f6, $f4
/* 01BE4C 8001B24C 46203203 */  div.d $f8, $f6, $f0
/* 01BE50 8001B250 462A4400 */  add.d $f16, $f8, $f10
/* 01BE54 8001B254 462084A0 */  cvt.s.d $f18, $f16
/* 01BE58 8001B258 E4F20028 */  swc1  $f18, 0x28($a3)
/* 01BE5C 8001B25C 84590004 */  lh    $t9, 4($v0)
/* 01BE60 8001B260 8FB8002C */  lw    $t8, 0x2c($sp)
/* 01BE64 8001B264 44992000 */  mtc1  $t9, $f4
/* 01BE68 8001B268 C708001C */  lwc1  $f8, 0x1c($t8)
/* 01BE6C 8001B26C 468021A0 */  cvt.s.w $f6, $f4
/* 01BE70 8001B270 46083281 */  sub.s $f10, $f6, $f8
/* 01BE74 8001B274 46005421 */  cvt.d.s $f16, $f10
/* 01BE78 8001B278 46208483 */  div.d $f18, $f16, $f0
/* 01BE7C 8001B27C 46209120 */  cvt.s.d $f4, $f18
/* 01BE80 8001B280 E4E4002C */  swc1  $f4, 0x2c($a3)
/* 01BE84 8001B284 8FAB002C */  lw    $t3, 0x2c($sp)
/* 01BE88 8001B288 C5660018 */  lwc1  $f6, 0x18($t3)
/* 01BE8C 8001B28C 0C0324B3 */  jal   func_800C92CC
/* 01BE90 8001B290 E4E6001C */   swc1  $f6, 0x1c($a3)
/* 01BE94 8001B294 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01BE98 8001B298 3C061901 */  lui   $a2, (0x19018014 >> 16) # lui $a2, 0x1901
/* 01BE9C 8001B29C 34C68014 */  ori   $a2, (0x19018014 & 0xFFFF) # ori $a2, $a2, 0x8014
/* 01BEA0 8001B2A0 25840014 */  addiu $a0, $t4, 0x14
/* 01BEA4 8001B2A4 0C03262E */  jal   func_800C98B8
/* 01BEA8 8001B2A8 25850034 */   addiu $a1, $t4, 0x34
/* 01BEAC 8001B2AC 10000003 */  b     .L8001B2BC
/* 01BEB0 8001B2B0 00000000 */   nop
.L8001B2B4:
/* 01BEB4 8001B2B4 A6000004 */  sh    $zero, 4($s0)
/* 01BEB8 8001B2B8 A6000000 */  sh    $zero, ($s0)
.L8001B2BC:
/* 01BEBC 8001B2BC 100002DD */  b     .L8001BE34
/* 01BEC0 8001B2C0 86020004 */   lh    $v0, 4($s0)
glabel L8001B2C4
/* 01BEC4 8001B2C4 860F0002 */  lh    $t7, 2($s0)
/* 01BEC8 8001B2C8 3C0D8016 */  lui   $t5, %hi(gActorList) # $t5, 0x8016
/* 01BECC 8001B2CC 25ADF9B8 */  addiu $t5, %lo(gActorList) # addiu $t5, $t5, -0x648
/* 01BED0 8001B2D0 000F70C0 */  sll   $t6, $t7, 3
/* 01BED4 8001B2D4 01CF7023 */  subu  $t6, $t6, $t7
/* 01BED8 8001B2D8 000E7100 */  sll   $t6, $t6, 4
/* 01BEDC 8001B2DC 01CD3821 */  addu  $a3, $t6, $t5
/* 01BEE0 8001B2E0 84E30002 */  lh    $v1, 2($a3)
/* 01BEE4 8001B2E4 30798000 */  andi  $t9, $v1, 0x8000
/* 01BEE8 8001B2E8 5320000F */  beql  $t9, $zero, .L8001B328
/* 01BEEC 8001B2EC A6000004 */   sh    $zero, 4($s0)
/* 01BEF0 8001B2F0 84F80000 */  lh    $t8, ($a3)
/* 01BEF4 8001B2F4 24060006 */  li    $a2, 6
/* 01BEF8 8001B2F8 54D8000B */  bnel  $a2, $t8, .L8001B328
/* 01BEFC 8001B2FC A6000004 */   sh    $zero, 4($s0)
/* 01BF00 8001B300 84EB0006 */  lh    $t3, 6($a3)
/* 01BF04 8001B304 24010004 */  li    $at, 4
/* 01BF08 8001B308 8FAC0030 */  lw    $t4, 0x30($sp)
/* 01BF0C 8001B30C 55610006 */  bnel  $t3, $at, .L8001B328
/* 01BF10 8001B310 A6000004 */   sh    $zero, 4($s0)
/* 01BF14 8001B314 84EF0010 */  lh    $t7, 0x10($a3)
/* 01BF18 8001B318 3C01800F */  lui   $at, %hi(D_800ED498)
/* 01BF1C 8001B31C 518F000C */  beql  $t4, $t7, .L8001B350
/* 01BF20 8001B320 C4E80028 */   lwc1  $f8, 0x28($a3)
/* 01BF24 8001B324 A6000004 */  sh    $zero, 4($s0)
.L8001B328:
/* 01BF28 8001B328 A6000000 */  sh    $zero, ($s0)
/* 01BF2C 8001B32C 8FAE002C */  lw    $t6, 0x2c($sp)
/* 01BF30 8001B330 3C01FFFB */  lui   $at, (0xFFFBFFFF >> 16) # lui $at, 0xfffb
/* 01BF34 8001B334 3421FFFF */  ori   $at, (0xFFFBFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 01BF38 8001B338 8DCD000C */  lw    $t5, 0xc($t6)
/* 01BF3C 8001B33C 01A1C824 */  and   $t9, $t5, $at
/* 01BF40 8001B340 ADD9000C */  sw    $t9, 0xc($t6)
/* 01BF44 8001B344 100002BB */  b     .L8001BE34
/* 01BF48 8001B348 86020004 */   lh    $v0, 4($s0)
/* 01BF4C 8001B34C C4E80028 */  lwc1  $f8, 0x28($a3)
.L8001B350:
/* 01BF50 8001B350 D430D498 */  ldc1  $f16, %lo(D_800ED498)($at)
/* 01BF54 8001B354 C4E60018 */  lwc1  $f6, 0x18($a3)
/* 01BF58 8001B358 460042A1 */  cvt.d.s $f10, $f8
/* 01BF5C 8001B35C C4E80024 */  lwc1  $f8, 0x24($a3)
/* 01BF60 8001B360 46305481 */  sub.d $f18, $f10, $f16
/* 01BF64 8001B364 C4F0001C */  lwc1  $f16, 0x1c($a3)
/* 01BF68 8001B368 24180024 */  li    $t8, 36
/* 01BF6C 8001B36C 46083280 */  add.s $f10, $f6, $f8
/* 01BF70 8001B370 C4E8002C */  lwc1  $f8, 0x2c($a3)
/* 01BF74 8001B374 C4E60020 */  lwc1  $f6, 0x20($a3)
/* 01BF78 8001B378 46209120 */  cvt.s.d $f4, $f18
/* 01BF7C 8001B37C E4EA0018 */  swc1  $f10, 0x18($a3)
/* 01BF80 8001B380 46083280 */  add.s $f10, $f6, $f8
/* 01BF84 8001B384 E4E40028 */  swc1  $f4, 0x28($a3)
/* 01BF88 8001B388 C4F20028 */  lwc1  $f18, 0x28($a3)
/* 01BF8C 8001B38C E4EA0020 */  swc1  $f10, 0x20($a3)
/* 01BF90 8001B390 46128100 */  add.s $f4, $f16, $f18
/* 01BF94 8001B394 E4E4001C */  swc1  $f4, 0x1c($a3)
/* 01BF98 8001B398 86020004 */  lh    $v0, 4($s0)
/* 01BF9C 8001B39C 28410015 */  slti  $at, $v0, 0x15
/* 01BFA0 8001B3A0 542002A5 */  bnel  $at, $zero, .L8001BE38
/* 01BFA4 8001B3A4 28412710 */   slti  $at, $v0, 0x2710
/* 01BFA8 8001B3A8 A6180000 */  sh    $t8, ($s0)
/* 01BFAC 8001B3AC 100002A1 */  b     .L8001BE34
/* 01BFB0 8001B3B0 86020004 */   lh    $v0, 4($s0)
glabel L8001B3B4
/* 01BFB4 8001B3B4 860B0002 */  lh    $t3, 2($s0)
/* 01BFB8 8001B3B8 3C0F8016 */  lui   $t7, %hi(gActorList) # $t7, 0x8016
/* 01BFBC 8001B3BC 25EFF9B8 */  addiu $t7, %lo(gActorList) # addiu $t7, $t7, -0x648
/* 01BFC0 8001B3C0 000B60C0 */  sll   $t4, $t3, 3
/* 01BFC4 8001B3C4 018B6023 */  subu  $t4, $t4, $t3
/* 01BFC8 8001B3C8 000C6100 */  sll   $t4, $t4, 4
/* 01BFCC 8001B3CC 018F3821 */  addu  $a3, $t4, $t7
/* 01BFD0 8001B3D0 84E30002 */  lh    $v1, 2($a3)
/* 01BFD4 8001B3D4 8FAC0030 */  lw    $t4, 0x30($sp)
/* 01BFD8 8001B3D8 306D8000 */  andi  $t5, $v1, 0x8000
/* 01BFDC 8001B3DC 51A0000F */  beql  $t5, $zero, .L8001B41C
/* 01BFE0 8001B3E0 84E50010 */   lh    $a1, 0x10($a3)
/* 01BFE4 8001B3E4 84F90000 */  lh    $t9, ($a3)
/* 01BFE8 8001B3E8 24060006 */  li    $a2, 6
/* 01BFEC 8001B3EC 54D9000B */  bnel  $a2, $t9, .L8001B41C
/* 01BFF0 8001B3F0 84E50010 */   lh    $a1, 0x10($a3)
/* 01BFF4 8001B3F4 84EE0006 */  lh    $t6, 6($a3)
/* 01BFF8 8001B3F8 24010004 */  li    $at, 4
/* 01BFFC 8001B3FC 8FB80030 */  lw    $t8, 0x30($sp)
/* 01C000 8001B400 55C10006 */  bnel  $t6, $at, .L8001B41C
/* 01C004 8001B404 84E50010 */   lh    $a1, 0x10($a3)
/* 01C008 8001B408 84EB0010 */  lh    $t3, 0x10($a3)
/* 01C00C 8001B40C 3C01403E */  li    $at, 0x403E0000 # 2.968750
/* 01C010 8001B410 530B0007 */  beql  $t8, $t3, .L8001B430
/* 01C014 8001B414 C4F0001C */   lwc1  $f16, 0x1c($a3)
/* 01C018 8001B418 84E50010 */  lh    $a1, 0x10($a3)
.L8001B41C:
/* 01C01C 8001B41C 5185001A */  beql  $t4, $a1, .L8001B488
/* 01C020 8001B420 8FAF002C */   lw    $t7, 0x2c($sp)
/* 01C024 8001B424 10000018 */  b     .L8001B488
/* 01C028 8001B428 8FAF002C */   lw    $t7, 0x2c($sp)
/* 01C02C 8001B42C C4F0001C */  lwc1  $f16, 0x1c($a3)
.L8001B430:
/* 01C030 8001B430 44800000 */  mtc1  $zero, $f0
/* 01C034 8001B434 44812800 */  mtc1  $at, $f5
/* 01C038 8001B438 44802000 */  mtc1  $zero, $f4
/* 01C03C 8001B43C 460084A1 */  cvt.d.s $f18, $f16
/* 01C040 8001B440 A4E80006 */  sh    $t0, 6($a3)
/* 01C044 8001B444 46249180 */  add.d $f6, $f18, $f4
/* 01C048 8001B448 C4EC0018 */  lwc1  $f12, 0x18($a3)
/* 01C04C 8001B44C 8CE60020 */  lw    $a2, 0x20($a3)
/* 01C050 8001B450 E4E00024 */  swc1  $f0, 0x24($a3)
/* 01C054 8001B454 E4E00028 */  swc1  $f0, 0x28($a3)
/* 01C058 8001B458 E4E0002C */  swc1  $f0, 0x2c($a3)
/* 01C05C 8001B45C AFA70028 */  sw    $a3, 0x28($sp)
/* 01C060 8001B460 0C0AB870 */  jal   spawn_actor_on_surface
/* 01C064 8001B464 462033A0 */   cvt.s.d $f14, $f6
/* 01C068 8001B468 8FA70028 */  lw    $a3, 0x28($sp)
/* 01C06C 8001B46C 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 01C070 8001B470 44815000 */  mtc1  $at, $f10
/* 01C074 8001B474 C4E8000C */  lwc1  $f8, 0xc($a3)
/* 01C078 8001B478 460A4400 */  add.s $f16, $f8, $f10
/* 01C07C 8001B47C 46100480 */  add.s $f18, $f0, $f16
/* 01C080 8001B480 E4F2001C */  swc1  $f18, 0x1c($a3)
/* 01C084 8001B484 8FAF002C */  lw    $t7, 0x2c($sp)
.L8001B488:
/* 01C088 8001B488 3C01FFFB */  lui   $at, (0xFFFBFFFF >> 16) # lui $at, 0xfffb
/* 01C08C 8001B48C 3421FFFF */  ori   $at, (0xFFFBFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 01C090 8001B490 8DED000C */  lw    $t5, 0xc($t7)
/* 01C094 8001B494 01A1C824 */  and   $t9, $t5, $at
/* 01C098 8001B498 ADF9000C */  sw    $t9, 0xc($t7)
/* 01C09C 8001B49C A6000004 */  sh    $zero, 4($s0)
/* 01C0A0 8001B4A0 A6000000 */  sh    $zero, ($s0)
/* 01C0A4 8001B4A4 10000263 */  b     .L8001BE34
/* 01C0A8 8001B4A8 86020004 */   lh    $v0, 4($s0)
glabel L8001B4AC
/* 01C0AC 8001B4AC 3C0E8015 */  lui   $t6, %hi(gNumActors) # $t6, 0x8015
/* 01C0B0 8001B4B0 95CE0110 */  lhu   $t6, %lo(gNumActors)($t6)
/* 01C0B4 8001B4B4 29C10050 */  slti  $at, $t6, 0x50
/* 01C0B8 8001B4B8 50200019 */  beql  $at, $zero, .L8001B520
/* 01C0BC 8001B4BC A6000000 */   sh    $zero, ($s0)
/* 01C0C0 8001B4C0 0C0AC727 */  jal   use_green_shell_item
/* 01C0C4 8001B4C4 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01C0C8 8001B4C8 A6020002 */  sh    $v0, 2($s0)
/* 01C0CC 8001B4CC 86030002 */  lh    $v1, 2($s0)
/* 01C0D0 8001B4D0 04600010 */  bltz  $v1, .L8001B514
/* 01C0D4 8001B4D4 28610064 */   slti  $at, $v1, 0x64
/* 01C0D8 8001B4D8 1020000E */  beqz  $at, .L8001B514
/* 01C0DC 8001B4DC 24180005 */   li    $t8, 5
/* 01C0E0 8001B4E0 860B0006 */  lh    $t3, 6($s0)
/* 01C0E4 8001B4E4 A6180000 */  sh    $t8, ($s0)
/* 01C0E8 8001B4E8 A6000004 */  sh    $zero, 4($s0)
/* 01C0EC 8001B4EC 256C0001 */  addiu $t4, $t3, 1
/* 01C0F0 8001B4F0 A60C0006 */  sh    $t4, 6($s0)
/* 01C0F4 8001B4F4 0C0ADF8D */  jal   random_int
/* 01C0F8 8001B4F8 24040003 */   li    $a0, 3
/* 01C0FC 8001B4FC 00026880 */  sll   $t5, $v0, 2
/* 01C100 8001B500 01A26821 */  addu  $t5, $t5, $v0
/* 01C104 8001B504 000D6880 */  sll   $t5, $t5, 2
/* 01C108 8001B508 25B9000A */  addiu $t9, $t5, 0xa
/* 01C10C 8001B50C 10000004 */  b     .L8001B520
/* 01C110 8001B510 A619000E */   sh    $t9, 0xe($s0)
.L8001B514:
/* 01C114 8001B514 10000002 */  b     .L8001B520
/* 01C118 8001B518 A6000000 */   sh    $zero, ($s0)
/* 01C11C 8001B51C A6000000 */  sh    $zero, ($s0)
.L8001B520:
/* 01C120 8001B520 10000244 */  b     .L8001BE34
/* 01C124 8001B524 86020004 */   lh    $v0, 4($s0)
glabel L8001B528
/* 01C128 8001B528 860F0002 */  lh    $t7, 2($s0)
/* 01C12C 8001B52C 3C188016 */  lui   $t8, %hi(gActorList) # $t8, 0x8016
/* 01C130 8001B530 2718F9B8 */  addiu $t8, %lo(gActorList) # addiu $t8, $t8, -0x648
/* 01C134 8001B534 000F70C0 */  sll   $t6, $t7, 3
/* 01C138 8001B538 01CF7023 */  subu  $t6, $t6, $t7
/* 01C13C 8001B53C 000E7100 */  sll   $t6, $t6, 4
/* 01C140 8001B540 01D83821 */  addu  $a3, $t6, $t8
/* 01C144 8001B544 84E30002 */  lh    $v1, 2($a3)
/* 01C148 8001B548 306B8000 */  andi  $t3, $v1, 0x8000
/* 01C14C 8001B54C 5160000D */  beql  $t3, $zero, .L8001B584
/* 01C150 8001B550 A6000004 */   sh    $zero, 4($s0)
/* 01C154 8001B554 84EC0000 */  lh    $t4, ($a3)
/* 01C158 8001B558 24010007 */  li    $at, 7
/* 01C15C 8001B55C 55810009 */  bnel  $t4, $at, .L8001B584
/* 01C160 8001B560 A6000004 */   sh    $zero, 4($s0)
/* 01C164 8001B564 84ED0006 */  lh    $t5, 6($a3)
/* 01C168 8001B568 8FB90030 */  lw    $t9, 0x30($sp)
/* 01C16C 8001B56C 55A00005 */  bnel  $t5, $zero, .L8001B584
/* 01C170 8001B570 A6000004 */   sh    $zero, 4($s0)
/* 01C174 8001B574 84EF0014 */  lh    $t7, 0x14($a3)
/* 01C178 8001B578 532F0006 */  beql  $t9, $t7, .L8001B594
/* 01C17C 8001B57C 86020004 */   lh    $v0, 4($s0)
/* 01C180 8001B580 A6000004 */  sh    $zero, 4($s0)
.L8001B584:
/* 01C184 8001B584 A6000000 */  sh    $zero, ($s0)
/* 01C188 8001B588 1000022A */  b     .L8001BE34
/* 01C18C 8001B58C 86020004 */   lh    $v0, 4($s0)
/* 01C190 8001B590 86020004 */  lh    $v0, 4($s0)
.L8001B594:
/* 01C194 8001B594 860E000E */  lh    $t6, 0xe($s0)
/* 01C198 8001B598 24060006 */  li    $a2, 6
/* 01C19C 8001B59C 01C2082A */  slt   $at, $t6, $v0
/* 01C1A0 8001B5A0 50200225 */  beql  $at, $zero, .L8001BE38
/* 01C1A4 8001B5A4 28412710 */   slti  $at, $v0, 0x2710
/* 01C1A8 8001B5A8 A6000004 */  sh    $zero, 4($s0)
/* 01C1AC 8001B5AC A6060000 */  sh    $a2, ($s0)
/* 01C1B0 8001B5B0 10000220 */  b     .L8001BE34
/* 01C1B4 8001B5B4 86020004 */   lh    $v0, 4($s0)
glabel L8001B5B8
/* 01C1B8 8001B5B8 86180002 */  lh    $t8, 2($s0)
/* 01C1BC 8001B5BC 3C0C8016 */  lui   $t4, %hi(gActorList) # $t4, 0x8016
/* 01C1C0 8001B5C0 258CF9B8 */  addiu $t4, %lo(gActorList) # addiu $t4, $t4, -0x648
/* 01C1C4 8001B5C4 001858C0 */  sll   $t3, $t8, 3
/* 01C1C8 8001B5C8 01785823 */  subu  $t3, $t3, $t8
/* 01C1CC 8001B5CC 000B5900 */  sll   $t3, $t3, 4
/* 01C1D0 8001B5D0 016C3821 */  addu  $a3, $t3, $t4
/* 01C1D4 8001B5D4 84E30002 */  lh    $v1, 2($a3)
/* 01C1D8 8001B5D8 306D8000 */  andi  $t5, $v1, 0x8000
/* 01C1DC 8001B5DC 51A0000D */  beql  $t5, $zero, .L8001B614
/* 01C1E0 8001B5E0 A6000000 */   sh    $zero, ($s0)
/* 01C1E4 8001B5E4 84F90000 */  lh    $t9, ($a3)
/* 01C1E8 8001B5E8 24010007 */  li    $at, 7
/* 01C1EC 8001B5EC 57210009 */  bnel  $t9, $at, .L8001B614
/* 01C1F0 8001B5F0 A6000000 */   sh    $zero, ($s0)
/* 01C1F4 8001B5F4 84EF0006 */  lh    $t7, 6($a3)
/* 01C1F8 8001B5F8 8FAE0030 */  lw    $t6, 0x30($sp)
/* 01C1FC 8001B5FC 55E00005 */  bnel  $t7, $zero, .L8001B614
/* 01C200 8001B600 A6000000 */   sh    $zero, ($s0)
/* 01C204 8001B604 84F80014 */  lh    $t8, 0x14($a3)
/* 01C208 8001B608 51D80005 */  beql  $t6, $t8, .L8001B620
/* 01C20C 8001B60C A4E80006 */   sh    $t0, 6($a3)
/* 01C210 8001B610 A6000000 */  sh    $zero, ($s0)
.L8001B614:
/* 01C214 8001B614 10000004 */  b     .L8001B628
/* 01C218 8001B618 A6000004 */   sh    $zero, 4($s0)
/* 01C21C 8001B61C A4E80006 */  sh    $t0, 6($a3)
.L8001B620:
/* 01C220 8001B620 A6000004 */  sh    $zero, 4($s0)
/* 01C224 8001B624 A6000000 */  sh    $zero, ($s0)
.L8001B628:
/* 01C228 8001B628 10000202 */  b     .L8001BE34
/* 01C22C 8001B62C 86020004 */   lh    $v0, 4($s0)
glabel L8001B630
/* 01C230 8001B630 3C0B8015 */  lui   $t3, %hi(gNumActors) # $t3, 0x8015
/* 01C234 8001B634 956B0110 */  lhu   $t3, %lo(gNumActors)($t3)
/* 01C238 8001B638 29610050 */  slti  $at, $t3, 0x50
/* 01C23C 8001B63C 50200019 */  beql  $at, $zero, .L8001B6A4
/* 01C240 8001B640 A6000000 */   sh    $zero, ($s0)
/* 01C244 8001B644 0C0AC792 */  jal   use_red_shell_item
/* 01C248 8001B648 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01C24C 8001B64C A6020002 */  sh    $v0, 2($s0)
/* 01C250 8001B650 86030002 */  lh    $v1, 2($s0)
/* 01C254 8001B654 04600010 */  bltz  $v1, .L8001B698
/* 01C258 8001B658 28610064 */   slti  $at, $v1, 0x64
/* 01C25C 8001B65C 1020000E */  beqz  $at, .L8001B698
/* 01C260 8001B660 240C0008 */   li    $t4, 8
/* 01C264 8001B664 860D0006 */  lh    $t5, 6($s0)
/* 01C268 8001B668 A60C0000 */  sh    $t4, ($s0)
/* 01C26C 8001B66C A6000004 */  sh    $zero, 4($s0)
/* 01C270 8001B670 25B90001 */  addiu $t9, $t5, 1
/* 01C274 8001B674 A6190006 */  sh    $t9, 6($s0)
/* 01C278 8001B678 0C0ADF8D */  jal   random_int
/* 01C27C 8001B67C 24040003 */   li    $a0, 3
/* 01C280 8001B680 00027880 */  sll   $t7, $v0, 2
/* 01C284 8001B684 01E27821 */  addu  $t7, $t7, $v0
/* 01C288 8001B688 000F7880 */  sll   $t7, $t7, 2
/* 01C28C 8001B68C 25EE000A */  addiu $t6, $t7, 0xa
/* 01C290 8001B690 10000004 */  b     .L8001B6A4
/* 01C294 8001B694 A60E000E */   sh    $t6, 0xe($s0)
.L8001B698:
/* 01C298 8001B698 10000002 */  b     .L8001B6A4
/* 01C29C 8001B69C A6000000 */   sh    $zero, ($s0)
/* 01C2A0 8001B6A0 A6000000 */  sh    $zero, ($s0)
.L8001B6A4:
/* 01C2A4 8001B6A4 100001E3 */  b     .L8001BE34
/* 01C2A8 8001B6A8 86020004 */   lh    $v0, 4($s0)
glabel L8001B6AC
/* 01C2AC 8001B6AC 86180002 */  lh    $t8, 2($s0)
/* 01C2B0 8001B6B0 3C0C8016 */  lui   $t4, %hi(gActorList) # $t4, 0x8016
/* 01C2B4 8001B6B4 258CF9B8 */  addiu $t4, %lo(gActorList) # addiu $t4, $t4, -0x648
/* 01C2B8 8001B6B8 001858C0 */  sll   $t3, $t8, 3
/* 01C2BC 8001B6BC 01785823 */  subu  $t3, $t3, $t8
/* 01C2C0 8001B6C0 000B5900 */  sll   $t3, $t3, 4
/* 01C2C4 8001B6C4 016C3821 */  addu  $a3, $t3, $t4
/* 01C2C8 8001B6C8 84E30002 */  lh    $v1, 2($a3)
/* 01C2CC 8001B6CC 306D8000 */  andi  $t5, $v1, 0x8000
/* 01C2D0 8001B6D0 51A0000D */  beql  $t5, $zero, .L8001B708
/* 01C2D4 8001B6D4 A6000004 */   sh    $zero, 4($s0)
/* 01C2D8 8001B6D8 84F90000 */  lh    $t9, ($a3)
/* 01C2DC 8001B6DC 24010008 */  li    $at, 8
/* 01C2E0 8001B6E0 57210009 */  bnel  $t9, $at, .L8001B708
/* 01C2E4 8001B6E4 A6000004 */   sh    $zero, 4($s0)
/* 01C2E8 8001B6E8 84EF0006 */  lh    $t7, 6($a3)
/* 01C2EC 8001B6EC 8FAE0030 */  lw    $t6, 0x30($sp)
/* 01C2F0 8001B6F0 55E00005 */  bnel  $t7, $zero, .L8001B708
/* 01C2F4 8001B6F4 A6000004 */   sh    $zero, 4($s0)
/* 01C2F8 8001B6F8 84F80014 */  lh    $t8, 0x14($a3)
/* 01C2FC 8001B6FC 51D80006 */  beql  $t6, $t8, .L8001B718
/* 01C300 8001B700 86020004 */   lh    $v0, 4($s0)
/* 01C304 8001B704 A6000004 */  sh    $zero, 4($s0)
.L8001B708:
/* 01C308 8001B708 A6000000 */  sh    $zero, ($s0)
/* 01C30C 8001B70C 100001C9 */  b     .L8001BE34
/* 01C310 8001B710 86020004 */   lh    $v0, 4($s0)
/* 01C314 8001B714 86020004 */  lh    $v0, 4($s0)
.L8001B718:
/* 01C318 8001B718 860B000E */  lh    $t3, 0xe($s0)
/* 01C31C 8001B71C 240C0009 */  li    $t4, 9
/* 01C320 8001B720 0162082A */  slt   $at, $t3, $v0
/* 01C324 8001B724 502001C4 */  beql  $at, $zero, .L8001BE38
/* 01C328 8001B728 28412710 */   slti  $at, $v0, 0x2710
/* 01C32C 8001B72C A60C0000 */  sh    $t4, ($s0)
/* 01C330 8001B730 100001C0 */  b     .L8001BE34
/* 01C334 8001B734 86020004 */   lh    $v0, 4($s0)
glabel L8001B738
/* 01C338 8001B738 0C006AFB */  jal   func_8001ABEC
/* 01C33C 8001B73C 02002025 */   move  $a0, $s0
/* 01C340 8001B740 860D0002 */  lh    $t5, 2($s0)
/* 01C344 8001B744 3C0F8016 */  lui   $t7, %hi(gActorList) # $t7, 0x8016
/* 01C348 8001B748 25EFF9B8 */  addiu $t7, %lo(gActorList) # addiu $t7, $t7, -0x648
/* 01C34C 8001B74C 000DC8C0 */  sll   $t9, $t5, 3
/* 01C350 8001B750 032DC823 */  subu  $t9, $t9, $t5
/* 01C354 8001B754 0019C900 */  sll   $t9, $t9, 4
/* 01C358 8001B758 032F3821 */  addu  $a3, $t9, $t7
/* 01C35C 8001B75C 84E30002 */  lh    $v1, 2($a3)
/* 01C360 8001B760 24080001 */  li    $t0, 1
/* 01C364 8001B764 8FAA0030 */  lw    $t2, 0x30($sp)
/* 01C368 8001B768 306E8000 */  andi  $t6, $v1, 0x8000
/* 01C36C 8001B76C 51C0000C */  beql  $t6, $zero, .L8001B7A0
/* 01C370 8001B770 A6000000 */   sh    $zero, ($s0)
/* 01C374 8001B774 84F80000 */  lh    $t8, ($a3)
/* 01C378 8001B778 24010008 */  li    $at, 8
/* 01C37C 8001B77C 57010008 */  bnel  $t8, $at, .L8001B7A0
/* 01C380 8001B780 A6000000 */   sh    $zero, ($s0)
/* 01C384 8001B784 84EB0006 */  lh    $t3, 6($a3)
/* 01C388 8001B788 55600005 */  bnel  $t3, $zero, .L8001B7A0
/* 01C38C 8001B78C A6000000 */   sh    $zero, ($s0)
/* 01C390 8001B790 84EC0014 */  lh    $t4, 0x14($a3)
/* 01C394 8001B794 514C0005 */  beql  $t2, $t4, .L8001B7AC
/* 01C398 8001B798 A4E80006 */   sh    $t0, 6($a3)
/* 01C39C 8001B79C A6000000 */  sh    $zero, ($s0)
.L8001B7A0:
/* 01C3A0 8001B7A0 10000004 */  b     .L8001B7B4
/* 01C3A4 8001B7A4 A6000004 */   sh    $zero, 4($s0)
/* 01C3A8 8001B7A8 A4E80006 */  sh    $t0, 6($a3)
.L8001B7AC:
/* 01C3AC 8001B7AC A6000004 */  sh    $zero, 4($s0)
/* 01C3B0 8001B7B0 A6000000 */  sh    $zero, ($s0)
.L8001B7B4:
/* 01C3B4 8001B7B4 1000019F */  b     .L8001BE34
/* 01C3B8 8001B7B8 86020004 */   lh    $v0, 4($s0)
glabel L8001B7BC
/* 01C3BC 8001B7BC 3C0D8015 */  lui   $t5, %hi(gNumActors) # $t5, 0x8015
/* 01C3C0 8001B7C0 95AD0110 */  lhu   $t5, %lo(gNumActors)($t5)
/* 01C3C4 8001B7C4 29A10050 */  slti  $at, $t5, 0x50
/* 01C3C8 8001B7C8 50200019 */  beql  $at, $zero, .L8001B830
/* 01C3CC 8001B7CC A6000000 */   sh    $zero, ($s0)
/* 01C3D0 8001B7D0 0C0AC5FD */  jal   use_banana_bunch_item
/* 01C3D4 8001B7D4 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01C3D8 8001B7D8 A6020002 */  sh    $v0, 2($s0)
/* 01C3DC 8001B7DC 86030002 */  lh    $v1, 2($s0)
/* 01C3E0 8001B7E0 04600010 */  bltz  $v1, .L8001B824
/* 01C3E4 8001B7E4 28610064 */   slti  $at, $v1, 0x64
/* 01C3E8 8001B7E8 1020000E */  beqz  $at, .L8001B824
/* 01C3EC 8001B7EC 2419000B */   li    $t9, 11
/* 01C3F0 8001B7F0 860F0006 */  lh    $t7, 6($s0)
/* 01C3F4 8001B7F4 A6190000 */  sh    $t9, ($s0)
/* 01C3F8 8001B7F8 A6000004 */  sh    $zero, 4($s0)
/* 01C3FC 8001B7FC 25EE0001 */  addiu $t6, $t7, 1
/* 01C400 8001B800 A60E0006 */  sh    $t6, 6($s0)
/* 01C404 8001B804 0C0ADF8D */  jal   random_int
/* 01C408 8001B808 24040003 */   li    $a0, 3
/* 01C40C 8001B80C 0002C080 */  sll   $t8, $v0, 2
/* 01C410 8001B810 0302C021 */  addu  $t8, $t8, $v0
/* 01C414 8001B814 0018C080 */  sll   $t8, $t8, 2
/* 01C418 8001B818 270B003C */  addiu $t3, $t8, 0x3c
/* 01C41C 8001B81C 10000004 */  b     .L8001B830
/* 01C420 8001B820 A60B000E */   sh    $t3, 0xe($s0)
.L8001B824:
/* 01C424 8001B824 10000002 */  b     .L8001B830
/* 01C428 8001B828 A6000000 */   sh    $zero, ($s0)
/* 01C42C 8001B82C A6000000 */  sh    $zero, ($s0)
.L8001B830:
/* 01C430 8001B830 10000180 */  b     .L8001BE34
/* 01C434 8001B834 86020004 */   lh    $v0, 4($s0)
glabel L8001B838
/* 01C438 8001B838 860C0002 */  lh    $t4, 2($s0)
/* 01C43C 8001B83C 3C198016 */  lui   $t9, %hi(gActorList) # $t9, 0x8016
/* 01C440 8001B840 2739F9B8 */  addiu $t9, %lo(gActorList) # addiu $t9, $t9, -0x648
/* 01C444 8001B844 000C68C0 */  sll   $t5, $t4, 3
/* 01C448 8001B848 01AC6823 */  subu  $t5, $t5, $t4
/* 01C44C 8001B84C 000D6900 */  sll   $t5, $t5, 4
/* 01C450 8001B850 01B93821 */  addu  $a3, $t5, $t9
/* 01C454 8001B854 84E40006 */  lh    $a0, 6($a3)
/* 01C458 8001B858 24060006 */  li    $a2, 6
/* 01C45C 8001B85C 14C40029 */  bne   $a2, $a0, .L8001B904
/* 01C460 8001B860 00000000 */   nop
/* 01C464 8001B864 84EF001A */  lh    $t7, 0x1a($a3)
/* 01C468 8001B868 2405FFFF */  li    $a1, -1
/* 01C46C 8001B86C 00001825 */  move  $v1, $zero
/* 01C470 8001B870 50AF0003 */  beql  $a1, $t7, .L8001B880
/* 01C474 8001B874 84EE0018 */   lh    $t6, 0x18($a3)
/* 01C478 8001B878 24030001 */  li    $v1, 1
/* 01C47C 8001B87C 84EE0018 */  lh    $t6, 0x18($a3)
.L8001B880:
/* 01C480 8001B880 50AE0003 */  beql  $a1, $t6, .L8001B890
/* 01C484 8001B884 84F80016 */   lh    $t8, 0x16($a3)
/* 01C488 8001B888 24030001 */  li    $v1, 1
/* 01C48C 8001B88C 84F80016 */  lh    $t8, 0x16($a3)
.L8001B890:
/* 01C490 8001B890 50B80003 */  beql  $a1, $t8, .L8001B8A0
/* 01C494 8001B894 84EB0014 */   lh    $t3, 0x14($a3)
/* 01C498 8001B898 24030001 */  li    $v1, 1
/* 01C49C 8001B89C 84EB0014 */  lh    $t3, 0x14($a3)
.L8001B8A0:
/* 01C4A0 8001B8A0 50AB0003 */  beql  $a1, $t3, .L8001B8B0
/* 01C4A4 8001B8A4 84EC0012 */   lh    $t4, 0x12($a3)
/* 01C4A8 8001B8A8 24030001 */  li    $v1, 1
/* 01C4AC 8001B8AC 84EC0012 */  lh    $t4, 0x12($a3)
.L8001B8B0:
/* 01C4B0 8001B8B0 50AC0003 */  beql  $a1, $t4, .L8001B8C0
/* 01C4B4 8001B8B4 84E20000 */   lh    $v0, ($a3)
/* 01C4B8 8001B8B8 24030001 */  li    $v1, 1
/* 01C4BC 8001B8BC 84E20000 */  lh    $v0, ($a3)
.L8001B8C0:
/* 01C4C0 8001B8C0 2405000E */  li    $a1, 14
/* 01C4C4 8001B8C4 54A20004 */  bnel  $a1, $v0, .L8001B8D8
/* 01C4C8 8001B8C8 A6000000 */   sh    $zero, ($s0)
/* 01C4CC 8001B8CC 54600005 */  bnel  $v1, $zero, .L8001B8E4
/* 01C4D0 8001B8D0 860D000E */   lh    $t5, 0xe($s0)
/* 01C4D4 8001B8D4 A6000000 */  sh    $zero, ($s0)
.L8001B8D8:
/* 01C4D8 8001B8D8 1000000A */  b     .L8001B904
/* 01C4DC 8001B8DC A6000004 */   sh    $zero, 4($s0)
/* 01C4E0 8001B8E0 860D000E */  lh    $t5, 0xe($s0)
.L8001B8E4:
/* 01C4E4 8001B8E4 86190004 */  lh    $t9, 4($s0)
/* 01C4E8 8001B8E8 240F000C */  li    $t7, 12
/* 01C4EC 8001B8EC 01B9082A */  slt   $at, $t5, $t9
/* 01C4F0 8001B8F0 10200004 */  beqz  $at, .L8001B904
/* 01C4F4 8001B8F4 00000000 */   nop
/* 01C4F8 8001B8F8 A60F0000 */  sh    $t7, ($s0)
/* 01C4FC 8001B8FC A6000008 */  sh    $zero, 8($s0)
/* 01C500 8001B900 A6000004 */  sh    $zero, 4($s0)
.L8001B904:
/* 01C504 8001B904 1000014B */  b     .L8001BE34
/* 01C508 8001B908 86020004 */   lh    $v0, 4($s0)
glabel L8001B90C
/* 01C50C 8001B90C 86020004 */  lh    $v0, 4($s0)
/* 01C510 8001B910 2401000A */  li    $at, 10
/* 01C514 8001B914 3C0C8016 */  lui   $t4, %hi(gActorList) # $t4, 0x8016
/* 01C518 8001B918 0041001A */  div   $zero, $v0, $at
/* 01C51C 8001B91C 00007010 */  mfhi  $t6
/* 01C520 8001B920 258CF9B8 */  addiu $t4, %lo(gActorList) # addiu $t4, $t4, -0x648
/* 01C524 8001B924 55C00144 */  bnel  $t6, $zero, .L8001BE38
/* 01C528 8001B928 28412710 */   slti  $at, $v0, 0x2710
/* 01C52C 8001B92C 86030008 */  lh    $v1, 8($s0)
/* 01C530 8001B930 28610005 */  slti  $at, $v1, 5
/* 01C534 8001B934 1020003C */  beqz  $at, .L8001BA28
/* 01C538 8001B938 00036880 */   sll   $t5, $v1, 2
/* 01C53C 8001B93C 86180002 */  lh    $t8, 2($s0)
/* 01C540 8001B940 2C610005 */  sltiu $at, $v1, 5
/* 01C544 8001B944 00001025 */  move  $v0, $zero
/* 01C548 8001B948 001858C0 */  sll   $t3, $t8, 3
/* 01C54C 8001B94C 01785823 */  subu  $t3, $t3, $t8
/* 01C550 8001B950 000B5900 */  sll   $t3, $t3, 4
/* 01C554 8001B954 10200023 */  beqz  $at, .L8001B9E4
/* 01C558 8001B958 016C3821 */   addu  $a3, $t3, $t4
/* 01C55C 8001B95C 3C01800F */  lui   $at, %hi(jpt_800ED4A0)
/* 01C560 8001B960 002D0821 */  addu  $at, $at, $t5
/* 01C564 8001B964 8C2DD4A0 */  lw    $t5, %lo(jpt_800ED4A0)($at)
/* 01C568 8001B968 01A00008 */  jr    $t5
/* 01C56C 8001B96C 00000000 */   nop
glabel L8001B970
/* 01C570 8001B970 84F9001A */  lh    $t9, 0x1a($a3)
/* 01C574 8001B974 2405FFFF */  li    $a1, -1
/* 01C578 8001B978 50B9001B */  beql  $a1, $t9, .L8001B9E8
/* 01C57C 8001B97C 84EC0000 */   lh    $t4, ($a3)
/* 01C580 8001B980 10000018 */  b     .L8001B9E4
/* 01C584 8001B984 01001025 */   move  $v0, $t0
glabel L8001B988
/* 01C588 8001B988 84EF0018 */  lh    $t7, 0x18($a3)
/* 01C58C 8001B98C 2405FFFF */  li    $a1, -1
/* 01C590 8001B990 50AF0015 */  beql  $a1, $t7, .L8001B9E8
/* 01C594 8001B994 84EC0000 */   lh    $t4, ($a3)
/* 01C598 8001B998 10000012 */  b     .L8001B9E4
/* 01C59C 8001B99C 01001025 */   move  $v0, $t0
glabel L8001B9A0
/* 01C5A0 8001B9A0 84EE0016 */  lh    $t6, 0x16($a3)
/* 01C5A4 8001B9A4 2405FFFF */  li    $a1, -1
/* 01C5A8 8001B9A8 50AE000F */  beql  $a1, $t6, .L8001B9E8
/* 01C5AC 8001B9AC 84EC0000 */   lh    $t4, ($a3)
/* 01C5B0 8001B9B0 1000000C */  b     .L8001B9E4
/* 01C5B4 8001B9B4 01001025 */   move  $v0, $t0
glabel L8001B9B8
/* 01C5B8 8001B9B8 84F80014 */  lh    $t8, 0x14($a3)
/* 01C5BC 8001B9BC 2405FFFF */  li    $a1, -1
/* 01C5C0 8001B9C0 50B80009 */  beql  $a1, $t8, .L8001B9E8
/* 01C5C4 8001B9C4 84EC0000 */   lh    $t4, ($a3)
/* 01C5C8 8001B9C8 10000006 */  b     .L8001B9E4
/* 01C5CC 8001B9CC 01001025 */   move  $v0, $t0
glabel L8001B9D0
/* 01C5D0 8001B9D0 84EB0012 */  lh    $t3, 0x12($a3)
/* 01C5D4 8001B9D4 2405FFFF */  li    $a1, -1
/* 01C5D8 8001B9D8 50AB0003 */  beql  $a1, $t3, .L8001B9E8
/* 01C5DC 8001B9DC 84EC0000 */   lh    $t4, ($a3)
/* 01C5E0 8001B9E0 01001025 */  move  $v0, $t0
.L8001B9E4:
/* 01C5E4 8001B9E4 84EC0000 */  lh    $t4, ($a3)
.L8001B9E8:
/* 01C5E8 8001B9E8 2405000E */  li    $a1, 14
/* 01C5EC 8001B9EC 54AC000B */  bnel  $a1, $t4, .L8001BA1C
/* 01C5F0 8001B9F0 24790001 */   addiu $t9, $v1, 1
/* 01C5F4 8001B9F4 84ED0006 */  lh    $t5, 6($a3)
/* 01C5F8 8001B9F8 24060006 */  li    $a2, 6
/* 01C5FC 8001B9FC 54CD0007 */  bnel  $a2, $t5, .L8001BA1C
/* 01C600 8001BA00 24790001 */   addiu $t9, $v1, 1
/* 01C604 8001BA04 54480005 */  bnel  $v0, $t0, .L8001BA1C
/* 01C608 8001BA08 24790001 */   addiu $t9, $v1, 1
/* 01C60C 8001BA0C 0C0AC192 */  jal   func_802B0648
/* 01C610 8001BA10 00E02025 */   move  $a0, $a3
/* 01C614 8001BA14 86030008 */  lh    $v1, 8($s0)
/* 01C618 8001BA18 24790001 */  addiu $t9, $v1, 1
.L8001BA1C:
/* 01C61C 8001BA1C A6190008 */  sh    $t9, 8($s0)
/* 01C620 8001BA20 10000104 */  b     .L8001BE34
/* 01C624 8001BA24 86020004 */   lh    $v0, 4($s0)
.L8001BA28:
/* 01C628 8001BA28 A6000004 */  sh    $zero, 4($s0)
/* 01C62C 8001BA2C A6000000 */  sh    $zero, ($s0)
/* 01C630 8001BA30 10000100 */  b     .L8001BE34
/* 01C634 8001BA34 86020004 */   lh    $v0, 4($s0)
glabel L8001BA38
/* 01C638 8001BA38 0C0ACB10 */  jal   use_fake_itembox_item
/* 01C63C 8001BA3C 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01C640 8001BA40 A6020002 */  sh    $v0, 2($s0)
/* 01C644 8001BA44 86030002 */  lh    $v1, 2($s0)
/* 01C648 8001BA48 04600010 */  bltz  $v1, .L8001BA8C
/* 01C64C 8001BA4C 28610064 */   slti  $at, $v1, 0x64
/* 01C650 8001BA50 1020000E */  beqz  $at, .L8001BA8C
/* 01C654 8001BA54 2405000E */   li    $a1, 14
/* 01C658 8001BA58 860F0006 */  lh    $t7, 6($s0)
/* 01C65C 8001BA5C A6050000 */  sh    $a1, ($s0)
/* 01C660 8001BA60 A6000004 */  sh    $zero, 4($s0)
/* 01C664 8001BA64 25EE0001 */  addiu $t6, $t7, 1
/* 01C668 8001BA68 A60E0006 */  sh    $t6, 6($s0)
/* 01C66C 8001BA6C 0C0ADF8D */  jal   random_int
/* 01C670 8001BA70 24040003 */   li    $a0, 3
/* 01C674 8001BA74 0002C080 */  sll   $t8, $v0, 2
/* 01C678 8001BA78 0302C021 */  addu  $t8, $t8, $v0
/* 01C67C 8001BA7C 0018C080 */  sll   $t8, $t8, 2
/* 01C680 8001BA80 270B000A */  addiu $t3, $t8, 0xa
/* 01C684 8001BA84 10000002 */  b     .L8001BA90
/* 01C688 8001BA88 A60B000E */   sh    $t3, 0xe($s0)
.L8001BA8C:
/* 01C68C 8001BA8C A6000000 */  sh    $zero, ($s0)
.L8001BA90:
/* 01C690 8001BA90 100000E8 */  b     .L8001BE34
/* 01C694 8001BA94 86020004 */   lh    $v0, 4($s0)
glabel L8001BA98
/* 01C698 8001BA98 860C0002 */  lh    $t4, 2($s0)
/* 01C69C 8001BA9C 3C198016 */  lui   $t9, %hi(gActorList) # $t9, 0x8016
/* 01C6A0 8001BAA0 2739F9B8 */  addiu $t9, %lo(gActorList) # addiu $t9, $t9, -0x648
/* 01C6A4 8001BAA4 000C68C0 */  sll   $t5, $t4, 3
/* 01C6A8 8001BAA8 01AC6823 */  subu  $t5, $t5, $t4
/* 01C6AC 8001BAAC 000D6900 */  sll   $t5, $t5, 4
/* 01C6B0 8001BAB0 01B93821 */  addu  $a3, $t5, $t9
/* 01C6B4 8001BAB4 84E30002 */  lh    $v1, 2($a3)
/* 01C6B8 8001BAB8 306F8000 */  andi  $t7, $v1, 0x8000
/* 01C6BC 8001BABC 51E00010 */  beql  $t7, $zero, .L8001BB00
/* 01C6C0 8001BAC0 A6000004 */   sh    $zero, 4($s0)
/* 01C6C4 8001BAC4 84EE0000 */  lh    $t6, ($a3)
/* 01C6C8 8001BAC8 2401000D */  li    $at, 13
/* 01C6CC 8001BACC 55C1000C */  bnel  $t6, $at, .L8001BB00
/* 01C6D0 8001BAD0 A6000004 */   sh    $zero, 4($s0)
/* 01C6D4 8001BAD4 84F80006 */  lh    $t8, 6($a3)
/* 01C6D8 8001BAD8 57000009 */  bnel  $t8, $zero, .L8001BB00
/* 01C6DC 8001BADC A6000004 */   sh    $zero, 4($s0)
/* 01C6E0 8001BAE0 C4E40024 */  lwc1  $f4, 0x24($a3)
/* 01C6E4 8001BAE4 8FAB0030 */  lw    $t3, 0x30($sp)
/* 01C6E8 8001BAE8 4600218D */  trunc.w.s $f6, $f4
/* 01C6EC 8001BAEC 440D3000 */  mfc1  $t5, $f6
/* 01C6F0 8001BAF0 00000000 */  nop
/* 01C6F4 8001BAF4 516D0006 */  beql  $t3, $t5, .L8001BB10
/* 01C6F8 8001BAF8 86020004 */   lh    $v0, 4($s0)
/* 01C6FC 8001BAFC A6000004 */  sh    $zero, 4($s0)
.L8001BB00:
/* 01C700 8001BB00 A6000000 */  sh    $zero, ($s0)
/* 01C704 8001BB04 100000CB */  b     .L8001BE34
/* 01C708 8001BB08 86020004 */   lh    $v0, 4($s0)
/* 01C70C 8001BB0C 86020004 */  lh    $v0, 4($s0)
.L8001BB10:
/* 01C710 8001BB10 8619000E */  lh    $t9, 0xe($s0)
/* 01C714 8001BB14 240F000F */  li    $t7, 15
/* 01C718 8001BB18 0322082A */  slt   $at, $t9, $v0
/* 01C71C 8001BB1C 502000C6 */  beql  $at, $zero, .L8001BE38
/* 01C720 8001BB20 28412710 */   slti  $at, $v0, 0x2710
/* 01C724 8001BB24 A60F0000 */  sh    $t7, ($s0)
/* 01C728 8001BB28 100000C2 */  b     .L8001BE34
/* 01C72C 8001BB2C 86020004 */   lh    $v0, 4($s0)
glabel L8001BB30
/* 01C730 8001BB30 860E0002 */  lh    $t6, 2($s0)
/* 01C734 8001BB34 3C0C8016 */  lui   $t4, %hi(gActorList) # $t4, 0x8016
/* 01C738 8001BB38 258CF9B8 */  addiu $t4, %lo(gActorList) # addiu $t4, $t4, -0x648
/* 01C73C 8001BB3C 000EC0C0 */  sll   $t8, $t6, 3
/* 01C740 8001BB40 030EC023 */  subu  $t8, $t8, $t6
/* 01C744 8001BB44 0018C100 */  sll   $t8, $t8, 4
/* 01C748 8001BB48 030C3821 */  addu  $a3, $t8, $t4
/* 01C74C 8001BB4C 84E30002 */  lh    $v1, 2($a3)
/* 01C750 8001BB50 8FAC0030 */  lw    $t4, 0x30($sp)
/* 01C754 8001BB54 306B8000 */  andi  $t3, $v1, 0x8000
/* 01C758 8001BB58 51600011 */  beql  $t3, $zero, .L8001BBA0
/* 01C75C 8001BB5C 84EB0010 */   lh    $t3, 0x10($a3)
/* 01C760 8001BB60 84ED0000 */  lh    $t5, ($a3)
/* 01C764 8001BB64 2401000D */  li    $at, 13
/* 01C768 8001BB68 55A1000D */  bnel  $t5, $at, .L8001BBA0
/* 01C76C 8001BB6C 84EB0010 */   lh    $t3, 0x10($a3)
/* 01C770 8001BB70 84F90006 */  lh    $t9, 6($a3)
/* 01C774 8001BB74 5720000A */  bnel  $t9, $zero, .L8001BBA0
/* 01C778 8001BB78 84EB0010 */   lh    $t3, 0x10($a3)
/* 01C77C 8001BB7C C4E80024 */  lwc1  $f8, 0x24($a3)
/* 01C780 8001BB80 8FAF0030 */  lw    $t7, 0x30($sp)
/* 01C784 8001BB84 00E02025 */  move  $a0, $a3
/* 01C788 8001BB88 4600428D */  trunc.w.s $f10, $f8
/* 01C78C 8001BB8C 44185000 */  mfc1  $t8, $f10
/* 01C790 8001BB90 00000000 */  nop
/* 01C794 8001BB94 11F80006 */  beq   $t7, $t8, .L8001BBB0
/* 01C798 8001BB98 00000000 */   nop
/* 01C79C 8001BB9C 84EB0010 */  lh    $t3, 0x10($a3)
.L8001BBA0:
/* 01C7A0 8001BBA0 518B001E */  beql  $t4, $t3, .L8001BC1C
/* 01C7A4 8001BBA4 A6000004 */   sh    $zero, 4($s0)
/* 01C7A8 8001BBA8 1000001C */  b     .L8001BC1C
/* 01C7AC 8001BBAC A6000004 */   sh    $zero, 4($s0)
.L8001BBB0:
/* 01C7B0 8001BBB0 0C0A8419 */  jal   func_802A1064
/* 01C7B4 8001BBB4 AFA70028 */   sw    $a3, 0x28($sp)
/* 01C7B8 8001BBB8 8FAA0030 */  lw    $t2, 0x30($sp)
/* 01C7BC 8001BBBC 3C198016 */  lui   $t9, %hi(D_801631E0) # 0x8016
/* 01C7C0 8001BBC0 24080001 */  li    $t0, 1
/* 01C7C4 8001BBC4 000A6840 */  sll   $t5, $t2, 1
/* 01C7C8 8001BBC8 032DC821 */  addu  $t9, $t9, $t5
/* 01C7CC 8001BBCC 973931E0 */  lhu   $t9, %lo(D_801631E0)($t9) # 0x31e0($t9)
/* 01C7D0 8001BBD0 8FA70028 */  lw    $a3, 0x28($sp)
/* 01C7D4 8001BBD4 3C01403E */  li    $at, 0x403E0000 # 2.968750
/* 01C7D8 8001BBD8 55190010 */  bnel  $t0, $t9, .L8001BC1C
/* 01C7DC 8001BBDC A6000004 */   sh    $zero, 4($s0)
/* 01C7E0 8001BBE0 C4F0001C */  lwc1  $f16, 0x1c($a3)
/* 01C7E4 8001BBE4 44812800 */  mtc1  $at, $f5
/* 01C7E8 8001BBE8 44802000 */  mtc1  $zero, $f4
/* 01C7EC 8001BBEC 460084A1 */  cvt.d.s $f18, $f16
/* 01C7F0 8001BBF0 C4EC0018 */  lwc1  $f12, 0x18($a3)
/* 01C7F4 8001BBF4 46249180 */  add.d $f6, $f18, $f4
/* 01C7F8 8001BBF8 8CE60020 */  lw    $a2, 0x20($a3)
/* 01C7FC 8001BBFC AFA70028 */  sw    $a3, 0x28($sp)
/* 01C800 8001BC00 0C0AB870 */  jal   spawn_actor_on_surface
/* 01C804 8001BC04 462033A0 */   cvt.s.d $f14, $f6
/* 01C808 8001BC08 8FA70028 */  lw    $a3, 0x28($sp)
/* 01C80C 8001BC0C C4E8000C */  lwc1  $f8, 0xc($a3)
/* 01C810 8001BC10 46080280 */  add.s $f10, $f0, $f8
/* 01C814 8001BC14 E4EA001C */  swc1  $f10, 0x1c($a3)
/* 01C818 8001BC18 A6000004 */  sh    $zero, 4($s0)
.L8001BC1C:
/* 01C81C 8001BC1C A6000000 */  sh    $zero, ($s0)
/* 01C820 8001BC20 10000084 */  b     .L8001BE34
/* 01C824 8001BC24 86020004 */   lh    $v0, 4($s0)
glabel L8001BC28
/* 01C828 8001BC28 0C0ACBAF */  jal   use_thunder_item
/* 01C82C 8001BC2C 8FA4002C */   lw    $a0, 0x2c($sp)
/* 01C830 8001BC30 8FAA0030 */  lw    $t2, 0x30($sp)
/* 01C834 8001BC34 0C032B18 */  jal   func_800CAC60
/* 01C838 8001BC38 314400FF */   andi  $a0, $t2, 0xff
/* 01C83C 8001BC3C 0C02796F */  jal   func_8009E5BC
/* 01C840 8001BC40 00000000 */   nop
/* 01C844 8001BC44 860F0006 */  lh    $t7, 6($s0)
/* 01C848 8001BC48 240E0017 */  li    $t6, 23
/* 01C84C 8001BC4C A6000004 */  sh    $zero, 4($s0)
/* 01C850 8001BC50 25F80001 */  addiu $t8, $t7, 1
/* 01C854 8001BC54 A60E0000 */  sh    $t6, ($s0)
/* 01C858 8001BC58 A6180006 */  sh    $t8, 6($s0)
/* 01C85C 8001BC5C 10000075 */  b     .L8001BE34
/* 01C860 8001BC60 86020004 */   lh    $v0, 4($s0)
glabel L8001BC64
/* 01C864 8001BC64 86020004 */  lh    $v0, 4($s0)
/* 01C868 8001BC68 284100F1 */  slti  $at, $v0, 0xf1
/* 01C86C 8001BC6C 54200072 */  bnel  $at, $zero, .L8001BE38
/* 01C870 8001BC70 28412710 */   slti  $at, $v0, 0x2710
/* 01C874 8001BC74 0C032B50 */  jal   func_800CAD40
/* 01C878 8001BC78 93A40033 */   lbu   $a0, 0x33($sp)
/* 01C87C 8001BC7C A6000004 */  sh    $zero, 4($s0)
/* 01C880 8001BC80 A6000000 */  sh    $zero, ($s0)
/* 01C884 8001BC84 1000006B */  b     .L8001BE34
/* 01C888 8001BC88 86020004 */   lh    $v0, 4($s0)
glabel L8001BC8C
/* 01C88C 8001BC8C 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01C890 8001BC90 2419001A */  li    $t9, 26
/* 01C894 8001BC94 8D8B000C */  lw    $t3, 0xc($t4)
/* 01C898 8001BC98 356D2000 */  ori   $t5, $t3, 0x2000
/* 01C89C 8001BC9C AD8D000C */  sw    $t5, 0xc($t4)
/* 01C8A0 8001BCA0 860E0006 */  lh    $t6, 6($s0)
/* 01C8A4 8001BCA4 A6000004 */  sh    $zero, 4($s0)
/* 01C8A8 8001BCA8 A6190000 */  sh    $t9, ($s0)
/* 01C8AC 8001BCAC 25CF0001 */  addiu $t7, $t6, 1
/* 01C8B0 8001BCB0 A60F0006 */  sh    $t7, 6($s0)
/* 01C8B4 8001BCB4 1000005F */  b     .L8001BE34
/* 01C8B8 8001BCB8 86020004 */   lh    $v0, 4($s0)
glabel L8001BCBC
/* 01C8BC 8001BCBC 8FB8002C */  lw    $t8, 0x2c($sp)
/* 01C8C0 8001BCC0 8F0B00BC */  lw    $t3, 0xbc($t8)
/* 01C8C4 8001BCC4 316D0200 */  andi  $t5, $t3, 0x200
/* 01C8C8 8001BCC8 55A00003 */  bnel  $t5, $zero, .L8001BCD8
/* 01C8CC 8001BCCC A6000004 */   sh    $zero, 4($s0)
/* 01C8D0 8001BCD0 A6000000 */  sh    $zero, ($s0)
/* 01C8D4 8001BCD4 A6000004 */  sh    $zero, 4($s0)
.L8001BCD8:
/* 01C8D8 8001BCD8 10000056 */  b     .L8001BE34
/* 01C8DC 8001BCDC 86020004 */   lh    $v0, 4($s0)
glabel L8001BCE0
/* 01C8E0 8001BCE0 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01C8E4 8001BCE4 240F001C */  li    $t7, 28
/* 01C8E8 8001BCE8 8D99000C */  lw    $t9, 0xc($t4)
/* 01C8EC 8001BCEC 372E0800 */  ori   $t6, $t9, 0x800
/* 01C8F0 8001BCF0 AD8E000C */  sw    $t6, 0xc($t4)
/* 01C8F4 8001BCF4 86180006 */  lh    $t8, 6($s0)
/* 01C8F8 8001BCF8 A6000004 */  sh    $zero, 4($s0)
/* 01C8FC 8001BCFC A60F0000 */  sh    $t7, ($s0)
/* 01C900 8001BD00 270B0001 */  addiu $t3, $t8, 1
/* 01C904 8001BD04 A60B0006 */  sh    $t3, 6($s0)
/* 01C908 8001BD08 1000004A */  b     .L8001BE34
/* 01C90C 8001BD0C 86020004 */   lh    $v0, 4($s0)
glabel L8001BD10
/* 01C910 8001BD10 8FAD002C */  lw    $t5, 0x2c($sp)
/* 01C914 8001BD14 8DB900BC */  lw    $t9, 0xbc($t5)
/* 01C918 8001BD18 00197000 */  sll   $t6, $t9, 0
/* 01C91C 8001BD1C 05C20003 */  bltzl $t6, .L8001BD2C
/* 01C920 8001BD20 A6000004 */   sh    $zero, 4($s0)
/* 01C924 8001BD24 A6000000 */  sh    $zero, ($s0)
/* 01C928 8001BD28 A6000004 */  sh    $zero, 4($s0)
.L8001BD2C:
/* 01C92C 8001BD2C 10000041 */  b     .L8001BE34
/* 01C930 8001BD30 86020004 */   lh    $v0, 4($s0)
glabel L8001BD34
/* 01C934 8001BD34 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01C938 8001BD38 8D8F000C */  lw    $t7, 0xc($t4)
/* 01C93C 8001BD3C 35F80200 */  ori   $t8, $t7, 0x200
/* 01C940 8001BD40 AD98000C */  sw    $t8, 0xc($t4)
/* 01C944 8001BD44 860B0006 */  lh    $t3, 6($s0)
/* 01C948 8001BD48 A6000004 */  sh    $zero, 4($s0)
/* 01C94C 8001BD4C A6000000 */  sh    $zero, ($s0)
/* 01C950 8001BD50 256D0001 */  addiu $t5, $t3, 1
/* 01C954 8001BD54 A60D0006 */  sh    $t5, 6($s0)
/* 01C958 8001BD58 10000036 */  b     .L8001BE34
/* 01C95C 8001BD5C 86020004 */   lh    $v0, 4($s0)
glabel L8001BD60
/* 01C960 8001BD60 86020004 */  lh    $v0, 4($s0)
/* 01C964 8001BD64 8FB9002C */  lw    $t9, 0x2c($sp)
/* 01C968 8001BD68 2841003D */  slti  $at, $v0, 0x3d
/* 01C96C 8001BD6C 54200032 */  bnel  $at, $zero, .L8001BE38
/* 01C970 8001BD70 28412710 */   slti  $at, $v0, 0x2710
/* 01C974 8001BD74 8F2E000C */  lw    $t6, 0xc($t9)
/* 01C978 8001BD78 2418001D */  li    $t8, 29
/* 01C97C 8001BD7C 35CF0200 */  ori   $t7, $t6, 0x200
/* 01C980 8001BD80 AF2F000C */  sw    $t7, 0xc($t9)
/* 01C984 8001BD84 A6000004 */  sh    $zero, 4($s0)
/* 01C988 8001BD88 A6180000 */  sh    $t8, ($s0)
/* 01C98C 8001BD8C 10000029 */  b     .L8001BE34
/* 01C990 8001BD90 86020004 */   lh    $v0, 4($s0)
glabel L8001BD94
/* 01C994 8001BD94 86020004 */  lh    $v0, 4($s0)
/* 01C998 8001BD98 8FAC002C */  lw    $t4, 0x2c($sp)
/* 01C99C 8001BD9C 2841003D */  slti  $at, $v0, 0x3d
/* 01C9A0 8001BDA0 54200025 */  bnel  $at, $zero, .L8001BE38
/* 01C9A4 8001BDA4 28412710 */   slti  $at, $v0, 0x2710
/* 01C9A8 8001BDA8 8D8B000C */  lw    $t3, 0xc($t4)
/* 01C9AC 8001BDAC 240E001E */  li    $t6, 30
/* 01C9B0 8001BDB0 356D0200 */  ori   $t5, $t3, 0x200
/* 01C9B4 8001BDB4 AD8D000C */  sw    $t5, 0xc($t4)
/* 01C9B8 8001BDB8 A6000004 */  sh    $zero, 4($s0)
/* 01C9BC 8001BDBC A60E0000 */  sh    $t6, ($s0)
/* 01C9C0 8001BDC0 1000001C */  b     .L8001BE34
/* 01C9C4 8001BDC4 86020004 */   lh    $v0, 4($s0)
glabel L8001BDC8
/* 01C9C8 8001BDC8 240F0021 */  li    $t7, 33
/* 01C9CC 8001BDCC A6000004 */  sh    $zero, 4($s0)
/* 01C9D0 8001BDD0 24190258 */  li    $t9, 600
/* 01C9D4 8001BDD4 A60F0000 */  sh    $t7, ($s0)
/* 01C9D8 8001BDD8 A619000E */  sh    $t9, 0xe($s0)
/* 01C9DC 8001BDDC 10000015 */  b     .L8001BE34
/* 01C9E0 8001BDE0 86020004 */   lh    $v0, 4($s0)
glabel L8001BDE4
/* 01C9E4 8001BDE4 86020004 */  lh    $v0, 4($s0)
/* 01C9E8 8001BDE8 2401003C */  li    $at, 60
/* 01C9EC 8001BDEC 8FAB002C */  lw    $t3, 0x2c($sp)
/* 01C9F0 8001BDF0 0041001A */  div   $zero, $v0, $at
/* 01C9F4 8001BDF4 0000C010 */  mfhi  $t8
/* 01C9F8 8001BDF8 5700000F */  bnel  $t8, $zero, .L8001BE38
/* 01C9FC 8001BDFC 28412710 */   slti  $at, $v0, 0x2710
/* 01CA00 8001BE00 8D6D000C */  lw    $t5, 0xc($t3)
/* 01CA04 8001BE04 35AC0200 */  ori   $t4, $t5, 0x200
/* 01CA08 8001BE08 AD6C000C */  sw    $t4, 0xc($t3)
/* 01CA0C 8001BE0C 860E000E */  lh    $t6, 0xe($s0)
/* 01CA10 8001BE10 86020004 */  lh    $v0, 4($s0)
/* 01CA14 8001BE14 01C2082A */  slt   $at, $t6, $v0
/* 01CA18 8001BE18 50200007 */  beql  $at, $zero, .L8001BE38
/* 01CA1C 8001BE1C 28412710 */   slti  $at, $v0, 0x2710
/* 01CA20 8001BE20 A6000004 */  sh    $zero, 4($s0)
/* 01CA24 8001BE24 86020004 */  lh    $v0, 4($s0)
/* 01CA28 8001BE28 10000002 */  b     .L8001BE34
/* 01CA2C 8001BE2C A6000000 */   sh    $zero, ($s0)
.L8001BE30:
glabel L8001BE30
/* 01CA30 8001BE30 86020004 */  lh    $v0, 4($s0)
.L8001BE34:
/* 01CA34 8001BE34 28412710 */  slti  $at, $v0, 0x2710
.L8001BE38:
/* 01CA38 8001BE38 10200002 */  beqz  $at, .L8001BE44
/* 01CA3C 8001BE3C 244F0001 */   addiu $t7, $v0, 1
/* 01CA40 8001BE40 A60F0004 */  sh    $t7, 4($s0)
.L8001BE44:
/* 01CA44 8001BE44 8FB9002C */  lw    $t9, 0x2c($sp)
/* 01CA48 8001BE48 3C018000 */  lui   $at, (0x80002200 >> 16) # lui $at, 0x8000
/* 01CA4C 8001BE4C 34212200 */  ori   $at, (0x80002200 & 0xFFFF) # ori $at, $at, 0x2200
/* 01CA50 8001BE50 8F3800BC */  lw    $t8, 0xbc($t9)
/* 01CA54 8001BE54 03016824 */  and   $t5, $t8, $at
/* 01CA58 8001BE58 51A00003 */  beql  $t5, $zero, .L8001BE68
/* 01CA5C 8001BE5C 8FBF0024 */   lw    $ra, 0x24($sp)
/* 01CA60 8001BE60 A6000004 */  sh    $zero, 4($s0)
.L8001BE64:
/* 01CA64 8001BE64 8FBF0024 */  lw    $ra, 0x24($sp)
.L8001BE68:
/* 01CA68 8001BE68 8FB00020 */  lw    $s0, 0x20($sp)
/* 01CA6C 8001BE6C 27BD0030 */  addiu $sp, $sp, 0x30
/* 01CA70 8001BE70 03E00008 */  jr    $ra
/* 01CA74 8001BE74 00000000 */   nop
