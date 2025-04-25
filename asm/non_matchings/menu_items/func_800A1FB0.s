.section .late_rodata

glabel jpt_800F1B5C
.word L800A2054, L800A2054, L800A2054, L800A2054
.word L800A2C7C, L800A2C7C, L800A2C7C, L800A2C7C
.word L800A2C7C, L800A2118, L800A2118, L800A21E4
.word L800A2C7C, L800A2C7C, L800A2C7C, L800A2C7C
.word L800A2C7C, L800A2C7C, L800A2C7C, L800A23DC
.word L800A23DC, L800A223C, L800A223C, L800A223C
.word L800A223C, L800A2C7C, L800A2C7C, L800A2C7C
.word L800A2C7C, L800A23DC, L800A23DC, L800A22AC
.word L800A22AC, L800A22AC, L800A22AC, L800A26A8
.word L800A26A8, L800A29D0, L800A29D0, L800A29D0
.word L800A2C7C, L800A2C7C, L800A2C7C, L800A2C7C
.word L800A2314, L800A2314, L800A2C7C, L800A2C7C
.word L800A2C7C, L800A2384, L800A2384

glabel D_800F1C28
.float 0.9

glabel D_800F1C2C
.float 0.9

glabel D_800F1C30
.float 0.8

glabel D_800F1C34
.float 0.9

glabel D_800F1C38
.float 0.6

glabel D_800F1C3C
.float 0.8

glabel D_800F1C40
.float 0.6

glabel D_800F1C44
.float 0.6

glabel jpt_800F1C48
.word L800A2CA0, L800A2CA0, L800A2CBC, L800A2CBC
.word L800A2CBC, L800A2CBC, L800A2CBC, L800A2CBC
.word L800A2CBC, L800A2CBC, L800A2CA0, L800A2CA0
.word L800A2CBC, L800A2CBC, L800A2CBC, L800A2CBC
.word L800A2CA0, L800A2CA0

.section .text

glabel func_800A1FB0
/* 0A2BB0 800A1FB0 27BDFF18 */  addiu $sp, $sp, -0xe8
/* 0A2BB4 800A1FB4 AFB00048 */  sw    $s0, 0x48($sp)
/* 0A2BB8 800A1FB8 3C108015 */  lui   $s0, %hi(gDisplayListHead) # $s0, 0x8015
/* 0A2BBC 800A1FBC 26100298 */  addiu $s0, %lo(gDisplayListHead) # addiu $s0, $s0, 0x298
/* 0A2BC0 800A1FC0 AFBF006C */  sw    $ra, 0x6c($sp)
/* 0A2BC4 800A1FC4 AFA400E8 */  sw    $a0, 0xe8($sp)
/* 0A2BC8 800A1FC8 240E00F0 */  li    $t6, 240
/* 0A2BCC 800A1FCC 240F0064 */  li    $t7, 100
/* 0A2BD0 800A1FD0 AFBE0068 */  sw    $fp, 0x68($sp)
/* 0A2BD4 800A1FD4 AFB70064 */  sw    $s7, 0x64($sp)
/* 0A2BD8 800A1FD8 AFB60060 */  sw    $s6, 0x60($sp)
/* 0A2BDC 800A1FDC AFB5005C */  sw    $s5, 0x5c($sp)
/* 0A2BE0 800A1FE0 AFB40058 */  sw    $s4, 0x58($sp)
/* 0A2BE4 800A1FE4 AFB30054 */  sw    $s3, 0x54($sp)
/* 0A2BE8 800A1FE8 AFB20050 */  sw    $s2, 0x50($sp)
/* 0A2BEC 800A1FEC AFB1004C */  sw    $s1, 0x4c($sp)
/* 0A2BF0 800A1FF0 F7B80040 */  sdc1  $f24, 0x40($sp)
/* 0A2BF4 800A1FF4 F7B60038 */  sdc1  $f22, 0x38($sp)
/* 0A2BF8 800A1FF8 F7B40030 */  sdc1  $f20, 0x30($sp)
/* 0A2BFC 800A1FFC AFAF0020 */  sw    $t7, 0x20($sp)
/* 0A2C00 800A2000 AFAE0010 */  sw    $t6, 0x10($sp)
/* 0A2C04 800A2004 8E040000 */  lw    $a0, ($s0)
/* 0A2C08 800A2008 AFA00014 */  sw    $zero, 0x14($sp)
/* 0A2C0C 800A200C AFA00018 */  sw    $zero, 0x18($sp)
/* 0A2C10 800A2010 AFA0001C */  sw    $zero, 0x1c($sp)
/* 0A2C14 800A2014 00002825 */  move  $a1, $zero
/* 0A2C18 800A2018 00003025 */  move  $a2, $zero
/* 0A2C1C 800A201C 0C02637E */  jal   draw_box
/* 0A2C20 800A2020 24070140 */   li    $a3, 320
/* 0A2C24 800A2024 AE020000 */  sw    $v0, ($s0)
/* 0A2C28 800A2028 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2C2C 800A202C 8063EDEC */  lb    $v1, %lo(gSubMenuSelection)($v1)
/* 0A2C30 800A2030 2478FFEB */  addiu $t8, $v1, -0x15
/* 0A2C34 800A2034 2F010033 */  sltiu $at, $t8, 0x33
/* 0A2C38 800A2038 10200310 */  beqz  $at, .L800A2C7C
/* 0A2C3C 800A203C 0018C080 */   sll   $t8, $t8, 2
/* 0A2C40 800A2040 3C01800F */  lui   $at, %hi(jpt_800F1B5C)
/* 0A2C44 800A2044 00380821 */  addu  $at, $at, $t8
/* 0A2C48 800A2048 8C381B5C */  lw    $t8, %lo(jpt_800F1B5C)($at)
/* 0A2C4C 800A204C 03000008 */  jr    $t8
/* 0A2C50 800A2050 00000000 */   nop   
glabel L800A2054
/* 0A2C54 800A2054 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 0A2C58 800A2058 4481B000 */  mtc1  $at, $f22
/* 0A2C5C 800A205C 3C01800F */  lui   $at, %hi(D_800F1C28) # $at, 0x800f
/* 0A2C60 800A2060 3C11800E */  lui   $s1, %hi(gTextOptionMenu) # $s1, 0x800e
/* 0A2C64 800A2064 26317868 */  addiu $s1, %lo(gTextOptionMenu) # addiu $s1, $s1, 0x7868
/* 0A2C68 800A2068 C4341C28 */  lwc1  $f20, %lo(D_800F1C28)($at)
/* 0A2C6C 800A206C 00009025 */  move  $s2, $zero
/* 0A2C70 800A2070 2464FFEB */  addiu $a0, $v1, -0x15
/* 0A2C74 800A2074 24100055 */  li    $s0, 85
/* 0A2C78 800A2078 24130004 */  li    $s3, 4
.L800A207C:
/* 0A2C7C 800A207C 02402825 */  move  $a1, $s2
/* 0A2C80 800A2080 0C024889 */  jal   set_text_color_rainbow_if_selected
/* 0A2C84 800A2084 24060003 */   li    $a2, 3
/* 0A2C88 800A2088 24040032 */  li    $a0, 50
/* 0A2C8C 800A208C 02002825 */  move  $a1, $s0
/* 0A2C90 800A2090 8E260000 */  lw    $a2, ($s1)
/* 0A2C94 800A2094 00003825 */  move  $a3, $zero
/* 0A2C98 800A2098 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A2C9C 800A209C 0C024CC9 */  jal   print_text_mode_1
/* 0A2CA0 800A20A0 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A2CA4 800A20A4 3C048019 */  lui   $a0, %hi(gSubMenuSelection) # $a0, 0x8019
/* 0A2CA8 800A20A8 8084EDEC */  lb    $a0, %lo(gSubMenuSelection)($a0)
/* 0A2CAC 800A20AC 26310004 */  addiu $s1, $s1, 4
/* 0A2CB0 800A20B0 24190032 */  li    $t9, 50
/* 0A2CB4 800A20B4 2484FFEB */  addiu $a0, $a0, -0x15
/* 0A2CB8 800A20B8 56440004 */  bnel  $s2, $a0, .L800A20CC
/* 0A2CBC 800A20BC 26520001 */   addiu $s2, $s2, 1
/* 0A2CC0 800A20C0 A7B900E0 */  sh    $t9, 0xe0($sp)
/* 0A2CC4 800A20C4 A7B000E2 */  sh    $s0, 0xe2($sp)
/* 0A2CC8 800A20C8 26520001 */  addiu $s2, $s2, 1
.L800A20CC:
/* 0A2CCC 800A20CC 1653FFEB */  bne   $s2, $s3, .L800A207C
/* 0A2CD0 800A20D0 26100023 */   addiu $s0, $s0, 0x23
/* 0A2CD4 800A20D4 0C024C36 */  jal   set_text_color
/* 0A2CD8 800A20D8 24040001 */   li    $a0, 1
/* 0A2CDC 800A20DC 3C088019 */  lui   $t0, %hi(gSoundMode) # $t0, 0x8019
/* 0A2CE0 800A20E0 9108EDF2 */  lbu   $t0, %lo(gSoundMode)($t0)
/* 0A2CE4 800A20E4 3C06800E */  lui   $a2, %hi(gSoundModeNames)
/* 0A2CE8 800A20E8 240400E6 */  li    $a0, 230
/* 0A2CEC 800A20EC 00084880 */  sll   $t1, $t0, 2
/* 0A2CF0 800A20F0 00C93021 */  addu  $a2, $a2, $t1
/* 0A2CF4 800A20F4 8CC67710 */  lw    $a2, %lo(gSoundModeNames)($a2)
/* 0A2CF8 800A20F8 24050078 */  li    $a1, 120
/* 0A2CFC 800A20FC 00003825 */  move  $a3, $zero
/* 0A2D00 800A2100 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A2D04 800A2104 0C024DBB */  jal   print_text1_center_mode_1
/* 0A2D08 800A2108 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A2D0C 800A210C 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2D10 800A2110 100002DA */  b     .L800A2C7C
/* 0A2D14 800A2114 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A2118
/* 0A2D18 800A2118 0C024C36 */  jal   set_text_color
/* 0A2D1C 800A211C 24040003 */   li    $a0, 3
/* 0A2D20 800A2120 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 0A2D24 800A2124 3C11800E */  lui   $s1, %hi(D_800E7878) # $s1, 0x800e
/* 0A2D28 800A2128 3C12800E */  lui   $s2, %hi(D_800E7884) # $s2, 0x800e
/* 0A2D2C 800A212C 4481B000 */  mtc1  $at, $f22
/* 0A2D30 800A2130 26527884 */  addiu $s2, %lo(D_800E7884) # addiu $s2, $s2, 0x7884
/* 0A2D34 800A2134 26317878 */  addiu $s1, %lo(D_800E7878) # addiu $s1, $s1, 0x7878
/* 0A2D38 800A2138 24100055 */  li    $s0, 85
.L800A213C:
/* 0A2D3C 800A213C 24040028 */  li    $a0, 40
/* 0A2D40 800A2140 02002825 */  move  $a1, $s0
/* 0A2D44 800A2144 8E260000 */  lw    $a2, ($s1)
/* 0A2D48 800A2148 00003825 */  move  $a3, $zero
/* 0A2D4C 800A214C E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A2D50 800A2150 0C024CC9 */  jal   print_text_mode_1
/* 0A2D54 800A2154 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A2D58 800A2158 26310004 */  addiu $s1, $s1, 4
/* 0A2D5C 800A215C 0232082B */  sltu  $at, $s1, $s2
/* 0A2D60 800A2160 1420FFF6 */  bnez  $at, .L800A213C
/* 0A2D64 800A2164 26100014 */   addiu $s0, $s0, 0x14
/* 0A2D68 800A2168 3C048019 */  lui   $a0, %hi(gSubMenuSelection) # $a0, 0x8019
/* 0A2D6C 800A216C 8084EDEC */  lb    $a0, %lo(gSubMenuSelection)($a0)
/* 0A2D70 800A2170 3C11800E */  lui   $s1, %hi(D_800E7840) # $s1, 0x800e
/* 0A2D74 800A2174 26317840 */  addiu $s1, %lo(D_800E7840) # addiu $s1, $s1, 0x7840
/* 0A2D78 800A2178 00009025 */  move  $s2, $zero
/* 0A2D7C 800A217C 24100096 */  li    $s0, 150
/* 0A2D80 800A2180 2484FFE2 */  addiu $a0, $a0, -0x1e
.L800A2184:
/* 0A2D84 800A2184 02402825 */  move  $a1, $s2
/* 0A2D88 800A2188 0C024889 */  jal   set_text_color_rainbow_if_selected
/* 0A2D8C 800A218C 24060001 */   li    $a2, 1
/* 0A2D90 800A2190 24040084 */  li    $a0, 132
/* 0A2D94 800A2194 02002825 */  move  $a1, $s0
/* 0A2D98 800A2198 8E260000 */  lw    $a2, ($s1)
/* 0A2D9C 800A219C 00003825 */  move  $a3, $zero
/* 0A2DA0 800A21A0 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A2DA4 800A21A4 0C024CC9 */  jal   print_text_mode_1
/* 0A2DA8 800A21A8 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A2DAC 800A21AC 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2DB0 800A21B0 8063EDEC */  lb    $v1, %lo(gSubMenuSelection)($v1)
/* 0A2DB4 800A21B4 26310004 */  addiu $s1, $s1, 4
/* 0A2DB8 800A21B8 24010002 */  li    $at, 2
/* 0A2DBC 800A21BC 2464FFE2 */  addiu $a0, $v1, -0x1e
/* 0A2DC0 800A21C0 16440003 */  bne   $s2, $a0, .L800A21D0
/* 0A2DC4 800A21C4 240A0084 */   li    $t2, 132
/* 0A2DC8 800A21C8 A7AA00E0 */  sh    $t2, 0xe0($sp)
/* 0A2DCC 800A21CC A7B000E2 */  sh    $s0, 0xe2($sp)
.L800A21D0:
/* 0A2DD0 800A21D0 26520001 */  addiu $s2, $s2, 1
/* 0A2DD4 800A21D4 1641FFEB */  bne   $s2, $at, .L800A2184
/* 0A2DD8 800A21D8 26100019 */   addiu $s0, $s0, 0x19
/* 0A2DDC 800A21DC 100002A8 */  b     .L800A2C80
/* 0A2DE0 800A21E0 246FFFD8 */   addiu $t7, $v1, -0x28
glabel L800A21E4
/* 0A2DE4 800A21E4 0C024C36 */  jal   set_text_color
/* 0A2DE8 800A21E8 24040003 */   li    $a0, 3
/* 0A2DEC 800A21EC 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 0A2DF0 800A21F0 3C11800E */  lui   $s1, %hi(D_800E7884) # $s1, 0x800e
/* 0A2DF4 800A21F4 3C12800E */  lui   $s2, %hi(D_800E7890) # $s2, 0x800e
/* 0A2DF8 800A21F8 4481B000 */  mtc1  $at, $f22
/* 0A2DFC 800A21FC 26527890 */  addiu $s2, %lo(D_800E7890) # addiu $s2, $s2, 0x7890
/* 0A2E00 800A2200 26317884 */  addiu $s1, %lo(D_800E7884) # addiu $s1, $s1, 0x7884
/* 0A2E04 800A2204 24100055 */  li    $s0, 85
.L800A2208:
/* 0A2E08 800A2208 24040032 */  li    $a0, 50
/* 0A2E0C 800A220C 02002825 */  move  $a1, $s0
/* 0A2E10 800A2210 8E260000 */  lw    $a2, ($s1)
/* 0A2E14 800A2214 00003825 */  move  $a3, $zero
/* 0A2E18 800A2218 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A2E1C 800A221C 0C024CC9 */  jal   print_text_mode_1
/* 0A2E20 800A2220 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A2E24 800A2224 26310004 */  addiu $s1, $s1, 4
/* 0A2E28 800A2228 1632FFF7 */  bne   $s1, $s2, .L800A2208
/* 0A2E2C 800A222C 26100014 */   addiu $s0, $s0, 0x14
/* 0A2E30 800A2230 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2E34 800A2234 10000291 */  b     .L800A2C7C
/* 0A2E38 800A2238 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A223C
/* 0A2E3C 800A223C 0C024C36 */  jal   set_text_color
/* 0A2E40 800A2240 24040002 */   li    $a0, 2
/* 0A2E44 800A2244 3C028019 */  lui   $v0, %hi(gSubMenuSelection) # $v0, 0x8019
/* 0A2E48 800A2248 8042EDEC */  lb    $v0, %lo(gSubMenuSelection)($v0)
/* 0A2E4C 800A224C 3C0D800E */  lui   $t5, %hi(D_800E78D0) # $t5, 0x800e
/* 0A2E50 800A2250 25AD78D0 */  addiu $t5, %lo(D_800E78D0) # addiu $t5, $t5, 0x78d0
/* 0A2E54 800A2254 2442FFD6 */  addiu $v0, $v0, -0x2a
/* 0A2E58 800A2258 00025880 */  sll   $t3, $v0, 2
/* 0A2E5C 800A225C 01625823 */  subu  $t3, $t3, $v0
/* 0A2E60 800A2260 000B6080 */  sll   $t4, $t3, 2
/* 0A2E64 800A2264 3C01800F */  lui   $at, %hi(D_800F1C2C) # $at, 0x800f
/* 0A2E68 800A2268 C4341C2C */  lwc1  $f20, %lo(D_800F1C2C)($at)
/* 0A2E6C 800A226C 018D8821 */  addu  $s1, $t4, $t5
/* 0A2E70 800A2270 24100055 */  li    $s0, 85
/* 0A2E74 800A2274 24120091 */  li    $s2, 145
.L800A2278:
/* 0A2E78 800A2278 24040032 */  li    $a0, 50
/* 0A2E7C 800A227C 02002825 */  move  $a1, $s0
/* 0A2E80 800A2280 8E260000 */  lw    $a2, ($s1)
/* 0A2E84 800A2284 00003825 */  move  $a3, $zero
/* 0A2E88 800A2288 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A2E8C 800A228C 0C024CC9 */  jal   print_text_mode_1
/* 0A2E90 800A2290 E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A2E94 800A2294 26100014 */  addiu $s0, $s0, 0x14
/* 0A2E98 800A2298 1612FFF7 */  bne   $s0, $s2, .L800A2278
/* 0A2E9C 800A229C 26310004 */   addiu $s1, $s1, 4
/* 0A2EA0 800A22A0 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2EA4 800A22A4 10000275 */  b     .L800A2C7C
/* 0A2EA8 800A22A8 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A22AC
/* 0A2EAC 800A22AC 0C024C36 */  jal   set_text_color
/* 0A2EB0 800A22B0 24040002 */   li    $a0, 2
/* 0A2EB4 800A22B4 3C028019 */  lui   $v0, %hi(gSubMenuSelection) # $v0, 0x8019
/* 0A2EB8 800A22B8 8042EDEC */  lb    $v0, %lo(gSubMenuSelection)($v0)
/* 0A2EBC 800A22BC 3C18800E */  lui   $t8, %hi(D_800E7890) # $t8, 0x800e
/* 0A2EC0 800A22C0 27187890 */  addiu $t8, %lo(D_800E7890) # addiu $t8, $t8, 0x7890
/* 0A2EC4 800A22C4 244EFFCC */  addiu $t6, $v0, -0x34
/* 0A2EC8 800A22C8 000E7900 */  sll   $t7, $t6, 4
/* 0A2ECC 800A22CC 3C01800F */  lui   $at, %hi(D_800F1C30) # $at, 0x800f
/* 0A2ED0 800A22D0 C4341C30 */  lwc1  $f20, %lo(D_800F1C30)($at)
/* 0A2ED4 800A22D4 01F88821 */  addu  $s1, $t7, $t8
/* 0A2ED8 800A22D8 24100055 */  li    $s0, 85
/* 0A2EDC 800A22DC 241200A5 */  li    $s2, 165
.L800A22E0:
/* 0A2EE0 800A22E0 24040023 */  li    $a0, 35
/* 0A2EE4 800A22E4 02002825 */  move  $a1, $s0
/* 0A2EE8 800A22E8 8E260000 */  lw    $a2, ($s1)
/* 0A2EEC 800A22EC 00003825 */  move  $a3, $zero
/* 0A2EF0 800A22F0 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A2EF4 800A22F4 0C024CC9 */  jal   print_text_mode_1
/* 0A2EF8 800A22F8 E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A2EFC 800A22FC 26100014 */  addiu $s0, $s0, 0x14
/* 0A2F00 800A2300 1612FFF7 */  bne   $s0, $s2, .L800A22E0
/* 0A2F04 800A2304 26310004 */   addiu $s1, $s1, 4
/* 0A2F08 800A2308 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2F0C 800A230C 1000025B */  b     .L800A2C7C
/* 0A2F10 800A2310 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A2314
/* 0A2F14 800A2314 0C024C36 */  jal   set_text_color
/* 0A2F18 800A2318 24040002 */   li    $a0, 2
/* 0A2F1C 800A231C 3C028019 */  lui   $v0, %hi(gSubMenuSelection) # $v0, 0x8019
/* 0A2F20 800A2320 8042EDEC */  lb    $v0, %lo(gSubMenuSelection)($v0)
/* 0A2F24 800A2324 3C09800E */  lui   $t1, %hi(D_800E7900) # $t1, 0x800e
/* 0A2F28 800A2328 25297900 */  addiu $t1, %lo(D_800E7900) # addiu $t1, $t1, 0x7900
/* 0A2F2C 800A232C 2442FFBF */  addiu $v0, $v0, -0x41
/* 0A2F30 800A2330 0002C880 */  sll   $t9, $v0, 2
/* 0A2F34 800A2334 0322C823 */  subu  $t9, $t9, $v0
/* 0A2F38 800A2338 00194080 */  sll   $t0, $t9, 2
/* 0A2F3C 800A233C 3C01800F */  lui   $at, %hi(D_800F1C34) # $at, 0x800f
/* 0A2F40 800A2340 C4341C34 */  lwc1  $f20, %lo(D_800F1C34)($at)
/* 0A2F44 800A2344 01098821 */  addu  $s1, $t0, $t1
/* 0A2F48 800A2348 24100055 */  li    $s0, 85
/* 0A2F4C 800A234C 24120091 */  li    $s2, 145
.L800A2350:
/* 0A2F50 800A2350 24040041 */  li    $a0, 65
/* 0A2F54 800A2354 02002825 */  move  $a1, $s0
/* 0A2F58 800A2358 8E260000 */  lw    $a2, ($s1)
/* 0A2F5C 800A235C 00003825 */  move  $a3, $zero
/* 0A2F60 800A2360 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A2F64 800A2364 0C024CC9 */  jal   print_text_mode_1
/* 0A2F68 800A2368 E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A2F6C 800A236C 26100014 */  addiu $s0, $s0, 0x14
/* 0A2F70 800A2370 1612FFF7 */  bne   $s0, $s2, .L800A2350
/* 0A2F74 800A2374 26310004 */   addiu $s1, $s1, 4
/* 0A2F78 800A2378 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2F7C 800A237C 1000023F */  b     .L800A2C7C
/* 0A2F80 800A2380 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A2384
/* 0A2F84 800A2384 0C024C36 */  jal   set_text_color
/* 0A2F88 800A2388 24040003 */   li    $a0, 3
/* 0A2F8C 800A238C 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 0A2F90 800A2390 3C11800E */  lui   $s1, %hi(D_800E7A48) # $s1, 0x800e
/* 0A2F94 800A2394 3C12800E */  lui   $s2, %hi(D_800E7A54) # $s2, 0x800e
/* 0A2F98 800A2398 4481B000 */  mtc1  $at, $f22
/* 0A2F9C 800A239C 26527A54 */  addiu $s2, %lo(D_800E7A54) # addiu $s2, $s2, 0x7a54
/* 0A2FA0 800A23A0 26317A48 */  addiu $s1, %lo(D_800E7A48) # addiu $s1, $s1, 0x7a48
/* 0A2FA4 800A23A4 24100055 */  li    $s0, 85
.L800A23A8:
/* 0A2FA8 800A23A8 24040050 */  li    $a0, 80
/* 0A2FAC 800A23AC 02002825 */  move  $a1, $s0
/* 0A2FB0 800A23B0 8E260000 */  lw    $a2, ($s1)
/* 0A2FB4 800A23B4 00003825 */  move  $a3, $zero
/* 0A2FB8 800A23B8 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A2FBC 800A23BC 0C024CC9 */  jal   print_text_mode_1
/* 0A2FC0 800A23C0 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A2FC4 800A23C4 26310004 */  addiu $s1, $s1, 4
/* 0A2FC8 800A23C8 1632FFF7 */  bne   $s1, $s2, .L800A23A8
/* 0A2FCC 800A23CC 26100014 */   addiu $s0, $s0, 0x14
/* 0A2FD0 800A23D0 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A2FD4 800A23D4 10000229 */  b     .L800A2C7C
/* 0A2FD8 800A23D8 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A23DC
/* 0A2FDC 800A23DC 24010028 */  li    $at, 40
/* 0A2FE0 800A23E0 1061000B */  beq   $v1, $at, .L800A2410
/* 0A2FE4 800A23E4 24150028 */   li    $s5, 40
/* 0A2FE8 800A23E8 24010029 */  li    $at, 41
/* 0A2FEC 800A23EC 10610008 */  beq   $v1, $at, .L800A2410
/* 0A2FF0 800A23F0 24010032 */   li    $at, 50
/* 0A2FF4 800A23F4 10610008 */  beq   $v1, $at, .L800A2418
/* 0A2FF8 800A23F8 24150032 */   li    $s5, 50
/* 0A2FFC 800A23FC 24010033 */  li    $at, 51
/* 0A3000 800A2400 10610005 */  beq   $v1, $at, .L800A2418
/* 0A3004 800A2404 8FB400C0 */   lw    $s4, 0xc0($sp)
/* 0A3008 800A2408 10000004 */  b     .L800A241C
/* 0A300C 800A240C 8FB500C4 */   lw    $s5, 0xc4($sp)
.L800A2410:
/* 0A3010 800A2410 10000002 */  b     .L800A241C
/* 0A3014 800A2414 0000A025 */   move  $s4, $zero
.L800A2418:
/* 0A3018 800A2418 24140001 */  li    $s4, 1
.L800A241C:
/* 0A301C 800A241C 0C024C36 */  jal   set_text_color
/* 0A3020 800A2420 26840001 */   addiu $a0, $s4, 1
/* 0A3024 800A2424 3C01800F */  lui   $at, %hi(D_800F1C38) # $at, 0x800f
/* 0A3028 800A2428 C4361C38 */  lwc1  $f22, %lo(D_800F1C38)($at)
/* 0A302C 800A242C 00145080 */  sll   $t2, $s4, 2
/* 0A3030 800A2430 3C06800E */  lui   $a2, %hi(D_800E7920)
/* 0A3034 800A2434 00CA3021 */  addu  $a2, $a2, $t2
/* 0A3038 800A2438 8CC67920 */  lw    $a2, %lo(D_800E7920)($a2)
/* 0A303C 800A243C 240400A0 */  li    $a0, 160
/* 0A3040 800A2440 24050055 */  li    $a1, 85
/* 0A3044 800A2444 00003825 */  move  $a3, $zero
/* 0A3048 800A2448 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A304C 800A244C 0C024DBB */  jal   print_text1_center_mode_1
/* 0A3050 800A2450 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A3054 800A2454 00146900 */  sll   $t5, $s4, 4
/* 0A3058 800A2458 01B46821 */  addu  $t5, $t5, $s4
/* 0A305C 800A245C 3C013F40 */  li    $at, 0x3F400000 # 0.750000
/* 0A3060 800A2460 4481C000 */  mtc1  $at, $f24
/* 0A3064 800A2464 000D68C0 */  sll   $t5, $t5, 3
/* 0A3068 800A2468 01B46821 */  addu  $t5, $t5, $s4
/* 0A306C 800A246C 3C013F00 */  li    $at, 0x3F000000 # 0.500000
/* 0A3070 800A2470 3C0C800E */  lui   $t4, %hi(D_800E7918) # $t4, 0x800e
/* 0A3074 800A2474 258C7918 */  addiu $t4, %lo(D_800E7918) # addiu $t4, $t4, 0x7918
/* 0A3078 800A2478 25AE002A */  addiu $t6, $t5, 0x2a
/* 0A307C 800A247C 240B005C */  li    $t3, 92
/* 0A3080 800A2480 3C17800E */  lui   $s7, %hi(gGlobalTimer) # $s7, 0x800e
/* 0A3084 800A2484 4481A000 */  mtc1  $at, $f20
/* 0A3088 800A2488 26F7C54C */  addiu $s7, %lo(gGlobalTimer) # addiu $s7, $s7, -0x3ab4
/* 0A308C 800A248C AFAB0088 */  sw    $t3, 0x88($sp)
/* 0A3090 800A2490 AFAE0078 */  sw    $t6, 0x78($sp)
/* 0A3094 800A2494 AFAC0084 */  sw    $t4, 0x84($sp)
/* 0A3098 800A2498 24160020 */  li    $s6, 32
/* 0A309C 800A249C 2413002A */  li    $s3, 42
/* 0A30A0 800A24A0 241E0003 */  li    $fp, 3
.L800A24A4:
/* 0A30A4 800A24A4 0C024C36 */  jal   set_text_color
/* 0A30A8 800A24A8 03C02025 */   move  $a0, $fp
/* 0A30AC 800A24AC 8FAF0084 */  lw    $t7, 0x84($sp)
/* 0A30B0 800A24B0 8FA40088 */  lw    $a0, 0x88($sp)
/* 0A30B4 800A24B4 2405007D */  li    $a1, 125
/* 0A30B8 800A24B8 8DE60000 */  lw    $a2, ($t7)
/* 0A30BC 800A24BC E7B80014 */  swc1  $f24, 0x14($sp)
/* 0A30C0 800A24C0 E7B80010 */  swc1  $f24, 0x10($sp)
/* 0A30C4 800A24C4 0C024DBB */  jal   print_text1_center_mode_1
/* 0A30C8 800A24C8 00003825 */   move  $a3, $zero
/* 0A30CC 800A24CC 00008025 */  move  $s0, $zero
/* 0A30D0 800A24D0 24120096 */  li    $s2, 150
.L800A24D4:
/* 0A30D4 800A24D4 8FB80078 */  lw    $t8, 0x78($sp)
/* 0A30D8 800A24D8 1278000F */  beq   $s3, $t8, .L800A2518
/* 0A30DC 800A24DC 00000000 */   nop   
/* 0A30E0 800A24E0 3C198019 */  lui   $t9, %hi(gSubMenuSelection) # $t9, 0x8019
/* 0A30E4 800A24E4 8339EDEC */  lb    $t9, %lo(gSubMenuSelection)($t9)
/* 0A30E8 800A24E8 02002825 */  move  $a1, $s0
/* 0A30EC 800A24EC 24060001 */  li    $a2, 1
/* 0A30F0 800A24F0 0C024870 */  jal   text_rainbow_effect
/* 0A30F4 800A24F4 03352023 */   subu  $a0, $t9, $s5
/* 0A30F8 800A24F8 3C088019 */  lui   $t0, %hi(gSubMenuSelection) # $t0, 0x8019
/* 0A30FC 800A24FC 8108EDEC */  lb    $t0, %lo(gSubMenuSelection)($t0)
/* 0A3100 800A2500 01154823 */  subu  $t1, $t0, $s5
/* 0A3104 800A2504 5609001C */  bnel  $s0, $t1, .L800A2578
/* 0A3108 800A2508 26110001 */   addiu $s1, $s0, 1
/* 0A310C 800A250C A7B600E0 */  sh    $s6, 0xe0($sp)
/* 0A3110 800A2510 10000018 */  b     .L800A2574
/* 0A3114 800A2514 A7B200E2 */   sh    $s2, 0xe2($sp)
.L800A2518:
/* 0A3118 800A2518 12800014 */  beqz  $s4, .L800A256C
/* 0A311C 800A251C 8FAA00E8 */   lw    $t2, 0xe8($sp)
/* 0A3120 800A2520 8D4B0020 */  lw    $t3, 0x20($t2)
/* 0A3124 800A2524 160B0011 */  bne   $s0, $t3, .L800A256C
/* 0A3128 800A2528 00000000 */   nop   
/* 0A312C 800A252C 8EEC0000 */  lw    $t4, ($s7)
/* 0A3130 800A2530 019E001A */  div   $zero, $t4, $fp
/* 0A3134 800A2534 00002010 */  mfhi  $a0
/* 0A3138 800A2538 17C00002 */  bnez  $fp, .L800A2544
/* 0A313C 800A253C 00000000 */   nop   
/* 0A3140 800A2540 0007000D */  break 7
.L800A2544:
/* 0A3144 800A2544 2401FFFF */  li    $at, -1
/* 0A3148 800A2548 17C10004 */  bne   $fp, $at, .L800A255C
/* 0A314C 800A254C 3C018000 */   lui   $at, 0x8000
/* 0A3150 800A2550 15810002 */  bne   $t4, $at, .L800A255C
/* 0A3154 800A2554 00000000 */   nop   
/* 0A3158 800A2558 0006000D */  break 6
.L800A255C:
/* 0A315C 800A255C 0C024C36 */  jal   set_text_color
/* 0A3160 800A2560 00000000 */   nop   
/* 0A3164 800A2564 10000004 */  b     .L800A2578
/* 0A3168 800A2568 26110001 */   addiu $s1, $s0, 1
.L800A256C:
/* 0A316C 800A256C 0C024C36 */  jal   set_text_color
/* 0A3170 800A2570 24040001 */   li    $a0, 1
.L800A2574:
/* 0A3174 800A2574 26110001 */  addiu $s1, $s0, 1
.L800A2578:
/* 0A3178 800A2578 02202025 */  move  $a0, $s1
/* 0A317C 800A257C 0C029E25 */  jal   convert_number_to_ascii
/* 0A3180 800A2580 27A500B8 */   addiu $a1, $sp, 0xb8
/* 0A3184 800A2584 02C02025 */  move  $a0, $s6
/* 0A3188 800A2588 02402825 */  move  $a1, $s2
/* 0A318C 800A258C 27A600B9 */  addiu $a2, $sp, 0xb9
/* 0A3190 800A2590 00003825 */  move  $a3, $zero
/* 0A3194 800A2594 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A3198 800A2598 0C024CC9 */  jal   print_text_mode_1
/* 0A319C 800A259C E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A31A0 800A25A0 2401002A */  li    $at, 42
/* 0A31A4 800A25A4 16610006 */  bne   $s3, $at, .L800A25C0
/* 0A31A8 800A25A8 3C0F8019 */   lui   $t7, %hi(gSomeDLBuffer) # $t7, 0x8019
/* 0A31AC 800A25AC 3C0E8019 */  lui   $t6, %hi(D_8018EE10) # $t6, 0x8019
/* 0A31B0 800A25B0 25CEEE10 */  addiu $t6, %lo(D_8018EE10) # addiu $t6, $t6, -0x11f0
/* 0A31B4 800A25B4 001069C0 */  sll   $t5, $s0, 7
/* 0A31B8 800A25B8 10000004 */  b     .L800A25CC
/* 0A31BC 800A25BC 01AE1821 */   addu  $v1, $t5, $t6
.L800A25C0:
/* 0A31C0 800A25C0 8DEFD9C0 */  lw    $t7, %lo(gSomeDLBuffer)($t7)
/* 0A31C4 800A25C4 0010C1C0 */  sll   $t8, $s0, 7
/* 0A31C8 800A25C8 01F81821 */  addu  $v1, $t7, $t8
.L800A25CC:
/* 0A31CC 800A25CC 90790004 */  lbu   $t9, 4($v1)
/* 0A31D0 800A25D0 02602025 */  move  $a0, $s3
/* 0A31D4 800A25D4 02402825 */  move  $a1, $s2
/* 0A31D8 800A25D8 1720000B */  bnez  $t9, .L800A2608
/* 0A31DC 800A25DC 3C0D800F */   lui   $t5, %hi(gCupCourseOrder)
/* 0A31E0 800A25E0 3C06800E */  lui   $a2, %hi(D_800E7A44) # $a2, 0x800e
/* 0A31E4 800A25E4 8CC67A44 */  lw    $a2, %lo(D_800E7A44)($a2)
/* 0A31E8 800A25E8 02602025 */  move  $a0, $s3
/* 0A31EC 800A25EC 02402825 */  move  $a1, $s2
/* 0A31F0 800A25F0 00003825 */  move  $a3, $zero
/* 0A31F4 800A25F4 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A31F8 800A25F8 0C024CC9 */  jal   print_text_mode_1
/* 0A31FC 800A25FC E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A3200 800A2600 10000019 */  b     .L800A2668
/* 0A3204 800A2604 24010002 */   li    $at, 2
.L800A2608:
/* 0A3208 800A2608 80620005 */  lb    $v0, 5($v1)
/* 0A320C 800A260C 3C06800E */  lui   $a2, %hi(gCourseNamesDup2)
/* 0A3210 800A2610 E7B40014 */  swc1  $f20, 0x14($sp)
/* 0A3214 800A2614 04410003 */  bgez  $v0, .L800A2624
/* 0A3218 800A2618 00024083 */   sra   $t0, $v0, 2
/* 0A321C 800A261C 24410003 */  addiu $at, $v0, 3
/* 0A3220 800A2620 00014083 */  sra   $t0, $at, 2
.L800A2624:
/* 0A3224 800A2624 000848C0 */  sll   $t1, $t0, 3
/* 0A3228 800A2628 04410004 */  bgez  $v0, .L800A263C
/* 0A322C 800A262C 304A0003 */   andi  $t2, $v0, 3
/* 0A3230 800A2630 11400002 */  beqz  $t2, .L800A263C
/* 0A3234 800A2634 00000000 */   nop   
/* 0A3238 800A2638 254AFFFC */  addiu $t2, $t2, -4
.L800A263C:
/* 0A323C 800A263C 000A5840 */  sll   $t3, $t2, 1
/* 0A3240 800A2640 012B6021 */  addu  $t4, $t1, $t3
/* 0A3244 800A2644 01AC6821 */  addu  $t5, $t5, $t4
/* 0A3248 800A2648 85AD2BB4 */  lh    $t5, %lo(gCupCourseOrder)($t5)
/* 0A324C 800A264C E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A3250 800A2650 00003825 */  move  $a3, $zero
/* 0A3254 800A2654 000D7080 */  sll   $t6, $t5, 2
/* 0A3258 800A2658 00CE3021 */  addu  $a2, $a2, $t6
/* 0A325C 800A265C 0C024CC9 */  jal   print_text_mode_1
/* 0A3260 800A2660 8CC675C4 */   lw    $a2, %lo(gCourseNamesDup2)($a2)
/* 0A3264 800A2664 24010002 */  li    $at, 2
.L800A2668:
/* 0A3268 800A2668 02208025 */  move  $s0, $s1
/* 0A326C 800A266C 1621FF99 */  bne   $s1, $at, .L800A24D4
/* 0A3270 800A2670 2652001E */   addiu $s2, $s2, 0x1e
/* 0A3274 800A2674 8FAF0088 */  lw    $t7, 0x88($sp)
/* 0A3278 800A2678 8FB90084 */  lw    $t9, 0x84($sp)
/* 0A327C 800A267C 26730089 */  addiu $s3, $s3, 0x89
/* 0A3280 800A2680 2401013C */  li    $at, 316
/* 0A3284 800A2684 25F80082 */  addiu $t8, $t7, 0x82
/* 0A3288 800A2688 27280004 */  addiu $t0, $t9, 4
/* 0A328C 800A268C AFA80084 */  sw    $t0, 0x84($sp)
/* 0A3290 800A2690 AFB80088 */  sw    $t8, 0x88($sp)
/* 0A3294 800A2694 1661FF83 */  bne   $s3, $at, .L800A24A4
/* 0A3298 800A2698 26D60089 */   addiu $s6, $s6, 0x89
/* 0A329C 800A269C 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A32A0 800A26A0 10000176 */  b     .L800A2C7C
/* 0A32A4 800A26A4 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A26A8
/* 0A32A8 800A26A8 0C024C36 */  jal   set_text_color
/* 0A32AC 800A26AC 24040002 */   li    $a0, 2
/* 0A32B0 800A26B0 3C10800E */  lui   $s0, %hi(D_800E7928) # $s0, 0x800e
/* 0A32B4 800A26B4 3C01800F */  lui   $at, %hi(D_800F1C3C) # $at, 0x800f
/* 0A32B8 800A26B8 3C12800E */  lui   $s2, %hi(D_800E7930) # $s2, 0x800e
/* 0A32BC 800A26BC 26527930 */  addiu $s2, %lo(D_800E7930) # addiu $s2, $s2, 0x7930
/* 0A32C0 800A26C0 C4341C3C */  lwc1  $f20, %lo(D_800F1C3C)($at)
/* 0A32C4 800A26C4 26107928 */  addiu $s0, %lo(D_800E7928) # addiu $s0, $s0, 0x7928
/* 0A32C8 800A26C8 2411004D */  li    $s1, 77
.L800A26CC:
/* 0A32CC 800A26CC 240400A0 */  li    $a0, 160
/* 0A32D0 800A26D0 02202825 */  move  $a1, $s1
/* 0A32D4 800A26D4 8E060000 */  lw    $a2, ($s0)
/* 0A32D8 800A26D8 00003825 */  move  $a3, $zero
/* 0A32DC 800A26DC E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A32E0 800A26E0 0C024DBB */  jal   print_text1_center_mode_1
/* 0A32E4 800A26E4 E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A32E8 800A26E8 26100004 */  addiu $s0, $s0, 4
/* 0A32EC 800A26EC 0212082B */  sltu  $at, $s0, $s2
/* 0A32F0 800A26F0 1420FFF6 */  bnez  $at, .L800A26CC
/* 0A32F4 800A26F4 26310014 */   addiu $s1, $s1, 0x14
/* 0A32F8 800A26F8 3C013F40 */  li    $at, 0x3F400000 # 0.750000
/* 0A32FC 800A26FC 4481C000 */  mtc1  $at, $f24
/* 0A3300 800A2700 3C01800F */  lui   $at, %hi(D_800F1C40) # $at, 0x800f
/* 0A3304 800A2704 C4361C40 */  lwc1  $f22, %lo(D_800F1C40)($at)
/* 0A3308 800A2708 3C013F00 */  li    $at, 0x3F000000 # 0.500000
/* 0A330C 800A270C 3C09800E */  lui   $t1, %hi(D_800E7918) # $t1, 0x800e
/* 0A3310 800A2710 25297918 */  addiu $t1, %lo(D_800E7918) # addiu $t1, $t1, 0x7918
/* 0A3314 800A2714 240A005C */  li    $t2, 92
/* 0A3318 800A2718 3C17800E */  lui   $s7, %hi(gGlobalTimer) # $s7, 0x800e
/* 0A331C 800A271C 4481A000 */  mtc1  $at, $f20
/* 0A3320 800A2720 26F7C54C */  addiu $s7, %lo(gGlobalTimer) # addiu $s7, $s7, -0x3ab4
/* 0A3324 800A2724 AFAA0088 */  sw    $t2, 0x88($sp)
/* 0A3328 800A2728 AFA90084 */  sw    $t1, 0x84($sp)
/* 0A332C 800A272C 24160020 */  li    $s6, 32
/* 0A3330 800A2730 2413002A */  li    $s3, 42
/* 0A3334 800A2734 241E0003 */  li    $fp, 3
/* 0A3338 800A2738 27B500A8 */  addiu $s5, $sp, 0xa8
/* 0A333C 800A273C 2414002A */  li    $s4, 42
.L800A2740:
/* 0A3340 800A2740 0C024C36 */  jal   set_text_color
/* 0A3344 800A2744 03C02025 */   move  $a0, $fp
/* 0A3348 800A2748 8FAB0084 */  lw    $t3, 0x84($sp)
/* 0A334C 800A274C 8FA40088 */  lw    $a0, 0x88($sp)
/* 0A3350 800A2750 2405007D */  li    $a1, 125
/* 0A3354 800A2754 8D660000 */  lw    $a2, ($t3)
/* 0A3358 800A2758 E7B80014 */  swc1  $f24, 0x14($sp)
/* 0A335C 800A275C E7B80010 */  swc1  $f24, 0x10($sp)
/* 0A3360 800A2760 0C024DBB */  jal   print_text1_center_mode_1
/* 0A3364 800A2764 00003825 */   move  $a3, $zero
/* 0A3368 800A2768 00008025 */  move  $s0, $zero
/* 0A336C 800A276C 24120096 */  li    $s2, 150
.L800A2770:
/* 0A3370 800A2770 5674001A */  bnel  $s3, $s4, .L800A27DC
/* 0A3374 800A2774 8FAF00E8 */   lw    $t7, 0xe8($sp)
/* 0A3378 800A2778 8FAC00E8 */  lw    $t4, 0xe8($sp)
/* 0A337C 800A277C 8D8D001C */  lw    $t5, 0x1c($t4)
/* 0A3380 800A2780 160D0011 */  bne   $s0, $t5, .L800A27C8
/* 0A3384 800A2784 00000000 */   nop   
/* 0A3388 800A2788 8EEE0000 */  lw    $t6, ($s7)
/* 0A338C 800A278C 01DE001A */  div   $zero, $t6, $fp
/* 0A3390 800A2790 00002010 */  mfhi  $a0
/* 0A3394 800A2794 17C00002 */  bnez  $fp, .L800A27A0
/* 0A3398 800A2798 00000000 */   nop   
/* 0A339C 800A279C 0007000D */  break 7
.L800A27A0:
/* 0A33A0 800A27A0 2401FFFF */  li    $at, -1
/* 0A33A4 800A27A4 17C10004 */  bne   $fp, $at, .L800A27B8
/* 0A33A8 800A27A8 3C018000 */   lui   $at, 0x8000
/* 0A33AC 800A27AC 15C10002 */  bne   $t6, $at, .L800A27B8
/* 0A33B0 800A27B0 00000000 */   nop   
/* 0A33B4 800A27B4 0006000D */  break 6
.L800A27B8:
/* 0A33B8 800A27B8 0C024C36 */  jal   set_text_color
/* 0A33BC 800A27BC 00000000 */   nop   
/* 0A33C0 800A27C0 1000001C */  b     .L800A2834
/* 0A33C4 800A27C4 26110001 */   addiu $s1, $s0, 1
.L800A27C8:
/* 0A33C8 800A27C8 0C024C36 */  jal   set_text_color
/* 0A33CC 800A27CC 24040001 */   li    $a0, 1
/* 0A33D0 800A27D0 10000018 */  b     .L800A2834
/* 0A33D4 800A27D4 26110001 */   addiu $s1, $s0, 1
/* 0A33D8 800A27D8 8FAF00E8 */  lw    $t7, 0xe8($sp)
.L800A27DC:
/* 0A33DC 800A27DC 8DF80020 */  lw    $t8, 0x20($t7)
/* 0A33E0 800A27E0 16180011 */  bne   $s0, $t8, .L800A2828
/* 0A33E4 800A27E4 00000000 */   nop   
/* 0A33E8 800A27E8 8EF90000 */  lw    $t9, ($s7)
/* 0A33EC 800A27EC 033E001A */  div   $zero, $t9, $fp
/* 0A33F0 800A27F0 00002010 */  mfhi  $a0
/* 0A33F4 800A27F4 17C00002 */  bnez  $fp, .L800A2800
/* 0A33F8 800A27F8 00000000 */   nop   
/* 0A33FC 800A27FC 0007000D */  break 7
.L800A2800:
/* 0A3400 800A2800 2401FFFF */  li    $at, -1
/* 0A3404 800A2804 17C10004 */  bne   $fp, $at, .L800A2818
/* 0A3408 800A2808 3C018000 */   lui   $at, 0x8000
/* 0A340C 800A280C 17210002 */  bne   $t9, $at, .L800A2818
/* 0A3410 800A2810 00000000 */   nop   
/* 0A3414 800A2814 0006000D */  break 6
.L800A2818:
/* 0A3418 800A2818 0C024C36 */  jal   set_text_color
/* 0A341C 800A281C 00000000 */   nop   
/* 0A3420 800A2820 10000004 */  b     .L800A2834
/* 0A3424 800A2824 26110001 */   addiu $s1, $s0, 1
.L800A2828:
/* 0A3428 800A2828 0C024C36 */  jal   set_text_color
/* 0A342C 800A282C 24040001 */   li    $a0, 1
/* 0A3430 800A2830 26110001 */  addiu $s1, $s0, 1
.L800A2834:
/* 0A3434 800A2834 02202025 */  move  $a0, $s1
/* 0A3438 800A2838 0C029E25 */  jal   convert_number_to_ascii
/* 0A343C 800A283C 02A02825 */   move  $a1, $s5
/* 0A3440 800A2840 02C02025 */  move  $a0, $s6
/* 0A3444 800A2844 02402825 */  move  $a1, $s2
/* 0A3448 800A2848 27A600A9 */  addiu $a2, $sp, 0xa9
/* 0A344C 800A284C 00003825 */  move  $a3, $zero
/* 0A3450 800A2850 E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A3454 800A2854 0C024CC9 */  jal   print_text_mode_1
/* 0A3458 800A2858 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A345C 800A285C 2401002A */  li    $at, 42
/* 0A3460 800A2860 16610006 */  bne   $s3, $at, .L800A287C
/* 0A3464 800A2864 3C098019 */   lui   $t1, %hi(gSomeDLBuffer) # $t1, 0x8019
/* 0A3468 800A2868 3C0A8019 */  lui   $t2, %hi(D_8018EE10) # $t2, 0x8019
/* 0A346C 800A286C 254AEE10 */  addiu $t2, %lo(D_8018EE10) # addiu $t2, $t2, -0x11f0
/* 0A3470 800A2870 001041C0 */  sll   $t0, $s0, 7
/* 0A3474 800A2874 10000004 */  b     .L800A2888
/* 0A3478 800A2878 010A1821 */   addu  $v1, $t0, $t2
.L800A287C:
/* 0A347C 800A287C 8D29D9C0 */  lw    $t1, %lo(gSomeDLBuffer)($t1)
/* 0A3480 800A2880 001059C0 */  sll   $t3, $s0, 7
/* 0A3484 800A2884 012B1821 */  addu  $v1, $t1, $t3
.L800A2888:
/* 0A3488 800A2888 906C0004 */  lbu   $t4, 4($v1)
/* 0A348C 800A288C 02602025 */  move  $a0, $s3
/* 0A3490 800A2890 02402825 */  move  $a1, $s2
/* 0A3494 800A2894 1580000B */  bnez  $t4, .L800A28C4
/* 0A3498 800A2898 3C08800F */   lui   $t0, %hi(gCupCourseOrder)
/* 0A349C 800A289C 3C06800E */  lui   $a2, %hi(D_800E7A44) # $a2, 0x800e
/* 0A34A0 800A28A0 8CC67A44 */  lw    $a2, %lo(D_800E7A44)($a2)
/* 0A34A4 800A28A4 02602025 */  move  $a0, $s3
/* 0A34A8 800A28A8 02402825 */  move  $a1, $s2
/* 0A34AC 800A28AC 00003825 */  move  $a3, $zero
/* 0A34B0 800A28B0 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A34B4 800A28B4 0C024CC9 */  jal   print_text_mode_1
/* 0A34B8 800A28B8 E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A34BC 800A28BC 10000019 */  b     .L800A2924
/* 0A34C0 800A28C0 24010002 */   li    $at, 2
.L800A28C4:
/* 0A34C4 800A28C4 80620005 */  lb    $v0, 5($v1)
/* 0A34C8 800A28C8 3C06800E */  lui   $a2, %hi(gCourseNamesDup2)
/* 0A34CC 800A28CC E7B40014 */  swc1  $f20, 0x14($sp)
/* 0A34D0 800A28D0 04410003 */  bgez  $v0, .L800A28E0
/* 0A34D4 800A28D4 00026883 */   sra   $t5, $v0, 2
/* 0A34D8 800A28D8 24410003 */  addiu $at, $v0, 3
/* 0A34DC 800A28DC 00016883 */  sra   $t5, $at, 2
.L800A28E0:
/* 0A34E0 800A28E0 000D70C0 */  sll   $t6, $t5, 3
/* 0A34E4 800A28E4 04410004 */  bgez  $v0, .L800A28F8
/* 0A34E8 800A28E8 304F0003 */   andi  $t7, $v0, 3
/* 0A34EC 800A28EC 11E00002 */  beqz  $t7, .L800A28F8
/* 0A34F0 800A28F0 00000000 */   nop   
/* 0A34F4 800A28F4 25EFFFFC */  addiu $t7, $t7, -4
.L800A28F8:
/* 0A34F8 800A28F8 000FC040 */  sll   $t8, $t7, 1
/* 0A34FC 800A28FC 01D8C821 */  addu  $t9, $t6, $t8
/* 0A3500 800A2900 01194021 */  addu  $t0, $t0, $t9
/* 0A3504 800A2904 85082BB4 */  lh    $t0, %lo(gCupCourseOrder)($t0)
/* 0A3508 800A2908 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A350C 800A290C 00003825 */  move  $a3, $zero
/* 0A3510 800A2910 00085080 */  sll   $t2, $t0, 2
/* 0A3514 800A2914 00CA3021 */  addu  $a2, $a2, $t2
/* 0A3518 800A2918 0C024CC9 */  jal   print_text_mode_1
/* 0A351C 800A291C 8CC675C4 */   lw    $a2, %lo(gCourseNamesDup2)($a2)
/* 0A3520 800A2920 24010002 */  li    $at, 2
.L800A2924:
/* 0A3524 800A2924 02208025 */  move  $s0, $s1
/* 0A3528 800A2928 1621FF91 */  bne   $s1, $at, .L800A2770
/* 0A352C 800A292C 2652001E */   addiu $s2, $s2, 0x1e
/* 0A3530 800A2930 8FA90088 */  lw    $t1, 0x88($sp)
/* 0A3534 800A2934 8FAC0084 */  lw    $t4, 0x84($sp)
/* 0A3538 800A2938 26730089 */  addiu $s3, $s3, 0x89
/* 0A353C 800A293C 2A61013C */  slti  $at, $s3, 0x13c
/* 0A3540 800A2940 252B0082 */  addiu $t3, $t1, 0x82
/* 0A3544 800A2944 258D0004 */  addiu $t5, $t4, 4
/* 0A3548 800A2948 AFAD0084 */  sw    $t5, 0x84($sp)
/* 0A354C 800A294C AFAB0088 */  sw    $t3, 0x88($sp)
/* 0A3550 800A2950 1420FF7B */  bnez  $at, .L800A2740
/* 0A3554 800A2954 26D60089 */   addiu $s6, $s6, 0x89
/* 0A3558 800A2958 3C13800E */  lui   $s3, %hi(D_800E7930) # $s3, 0x800e
/* 0A355C 800A295C 26737930 */  addiu $s3, %lo(D_800E7930) # addiu $s3, $s3, 0x7930
/* 0A3560 800A2960 00009025 */  move  $s2, $zero
/* 0A3564 800A2964 2411006E */  li    $s1, 110
.L800A2968:
/* 0A3568 800A2968 3C048019 */  lui   $a0, %hi(gSubMenuSelection) # $a0, 0x8019
/* 0A356C 800A296C 8084EDEC */  lb    $a0, %lo(gSubMenuSelection)($a0)
/* 0A3570 800A2970 240F00D2 */  li    $t7, 210
/* 0A3574 800A2974 02002825 */  move  $a1, $s0
/* 0A3578 800A2978 2484FFC8 */  addiu $a0, $a0, -0x38
/* 0A357C 800A297C 16440003 */  bne   $s2, $a0, .L800A298C
/* 0A3580 800A2980 00000000 */   nop   
/* 0A3584 800A2984 A7B100E0 */  sh    $s1, 0xe0($sp)
/* 0A3588 800A2988 A7AF00E2 */  sh    $t7, 0xe2($sp)
.L800A298C:
/* 0A358C 800A298C 0C024870 */  jal   text_rainbow_effect
/* 0A3590 800A2990 24060003 */   li    $a2, 3
/* 0A3594 800A2994 02202025 */  move  $a0, $s1
/* 0A3598 800A2998 240500D2 */  li    $a1, 210
/* 0A359C 800A299C 8E660000 */  lw    $a2, ($s3)
/* 0A35A0 800A29A0 00003825 */  move  $a3, $zero
/* 0A35A4 800A29A4 E7B80010 */  swc1  $f24, 0x10($sp)
/* 0A35A8 800A29A8 0C024CC9 */  jal   print_text_mode_1
/* 0A35AC 800A29AC E7B80014 */   swc1  $f24, 0x14($sp)
/* 0A35B0 800A29B0 26520001 */  addiu $s2, $s2, 1
/* 0A35B4 800A29B4 24010002 */  li    $at, 2
/* 0A35B8 800A29B8 26310032 */  addiu $s1, $s1, 0x32
/* 0A35BC 800A29BC 1641FFEA */  bne   $s2, $at, .L800A2968
/* 0A35C0 800A29C0 26730004 */   addiu $s3, $s3, 4
/* 0A35C4 800A29C4 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A35C8 800A29C8 100000AC */  b     .L800A2C7C
/* 0A35CC 800A29CC 8063EDEC */   lb    $v1, %lo(gSubMenuSelection)($v1)
glabel L800A29D0
/* 0A35D0 800A29D0 2475FFC6 */  addiu $s5, $v1, -0x3a
/* 0A35D4 800A29D4 06A10003 */  bgez  $s5, .L800A29E4
/* 0A35D8 800A29D8 00157043 */   sra   $t6, $s5, 1
/* 0A35DC 800A29DC 26A10001 */  addiu $at, $s5, 1
/* 0A35E0 800A29E0 00017043 */  sra   $t6, $at, 1
.L800A29E4:
/* 0A35E4 800A29E4 01C0A825 */  move  $s5, $t6
/* 0A35E8 800A29E8 0C024C36 */  jal   set_text_color
/* 0A35EC 800A29EC 24040002 */   li    $a0, 2
/* 0A35F0 800A29F0 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 0A35F4 800A29F4 4481B000 */  mtc1  $at, $f22
/* 0A35F8 800A29F8 3C19800E */  lui   $t9, %hi(D_800E7938) # $t9, 0x800e
/* 0A35FC 800A29FC 27397938 */  addiu $t9, %lo(D_800E7938) # addiu $t9, $t9, 0x7938
/* 0A3600 800A2A00 0015C080 */  sll   $t8, $s5, 2
/* 0A3604 800A2A04 03191021 */  addu  $v0, $t8, $t9
/* 0A3608 800A2A08 8C460000 */  lw    $a2, ($v0)
/* 0A360C 800A2A0C AFA20078 */  sw    $v0, 0x78($sp)
/* 0A3610 800A2A10 240400A0 */  li    $a0, 160
/* 0A3614 800A2A14 24050055 */  li    $a1, 85
/* 0A3618 800A2A18 00003825 */  move  $a3, $zero
/* 0A361C 800A2A1C E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A3620 800A2A20 0C024DBB */  jal   print_text1_center_mode_1
/* 0A3624 800A2A24 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A3628 800A2A28 3C013F40 */  li    $at, 0x3F400000 # 0.750000
/* 0A362C 800A2A2C 4481C000 */  mtc1  $at, $f24
/* 0A3630 800A2A30 3C01800F */  lui   $at, %hi(D_800F1C44) # $at, 0x800f
/* 0A3634 800A2A34 C4361C44 */  lwc1  $f22, %lo(D_800F1C44)($at)
/* 0A3638 800A2A38 3C013F00 */  li    $at, 0x3F000000 # 0.500000
/* 0A363C 800A2A3C 3C0A800E */  lui   $t2, %hi(D_800E7918) # $t2, 0x800e
/* 0A3640 800A2A40 254A7918 */  addiu $t2, %lo(D_800E7918) # addiu $t2, $t2, 0x7918
/* 0A3644 800A2A44 2408005C */  li    $t0, 92
/* 0A3648 800A2A48 3C17800E */  lui   $s7, %hi(gGlobalTimer) # $s7, 0x800e
/* 0A364C 800A2A4C 3C15800E */  lui   $s5, %hi(D_800E7938) # $s5, 0x800e
/* 0A3650 800A2A50 4481A000 */  mtc1  $at, $f20
/* 0A3654 800A2A54 26B57938 */  addiu $s5, %lo(D_800E7938) # addiu $s5, $s5, 0x7938
/* 0A3658 800A2A58 26F7C54C */  addiu $s7, %lo(gGlobalTimer) # addiu $s7, $s7, -0x3ab4
/* 0A365C 800A2A5C AFA80088 */  sw    $t0, 0x88($sp)
/* 0A3660 800A2A60 AFAA0084 */  sw    $t2, 0x84($sp)
/* 0A3664 800A2A64 24160020 */  li    $s6, 32
/* 0A3668 800A2A68 2413002A */  li    $s3, 42
/* 0A366C 800A2A6C 241E0003 */  li    $fp, 3
/* 0A3670 800A2A70 2414002A */  li    $s4, 42
.L800A2A74:
/* 0A3674 800A2A74 0C024C36 */  jal   set_text_color
/* 0A3678 800A2A78 03C02025 */   move  $a0, $fp
/* 0A367C 800A2A7C 8FA90084 */  lw    $t1, 0x84($sp)
/* 0A3680 800A2A80 8FA40088 */  lw    $a0, 0x88($sp)
/* 0A3684 800A2A84 2405007D */  li    $a1, 125
/* 0A3688 800A2A88 8D260000 */  lw    $a2, ($t1)
/* 0A368C 800A2A8C E7B80014 */  swc1  $f24, 0x14($sp)
/* 0A3690 800A2A90 E7B80010 */  swc1  $f24, 0x10($sp)
/* 0A3694 800A2A94 0C024DBB */  jal   print_text1_center_mode_1
/* 0A3698 800A2A98 00003825 */   move  $a3, $zero
/* 0A369C 800A2A9C 00008025 */  move  $s0, $zero
/* 0A36A0 800A2AA0 24120096 */  li    $s2, 150
.L800A2AA4:
/* 0A36A4 800A2AA4 1674001F */  bne   $s3, $s4, .L800A2B24
/* 0A36A8 800A2AA8 8FAB00E8 */   lw    $t3, 0xe8($sp)
/* 0A36AC 800A2AAC 8D6C001C */  lw    $t4, 0x1c($t3)
/* 0A36B0 800A2AB0 8FAD0078 */  lw    $t5, 0x78($sp)
/* 0A36B4 800A2AB4 160C0017 */  bne   $s0, $t4, .L800A2B14
/* 0A36B8 800A2AB8 00000000 */   nop   
/* 0A36BC 800A2ABC 55B50006 */  bnel  $t5, $s5, .L800A2AD8
/* 0A36C0 800A2AC0 8EEF0000 */   lw    $t7, ($s7)
/* 0A36C4 800A2AC4 0C024C36 */  jal   set_text_color
/* 0A36C8 800A2AC8 24040002 */   li    $a0, 2
/* 0A36CC 800A2ACC 10000020 */  b     .L800A2B50
/* 0A36D0 800A2AD0 26110001 */   addiu $s1, $s0, 1
/* 0A36D4 800A2AD4 8EEF0000 */  lw    $t7, ($s7)
.L800A2AD8:
/* 0A36D8 800A2AD8 01FE001A */  div   $zero, $t7, $fp
/* 0A36DC 800A2ADC 00002010 */  mfhi  $a0
/* 0A36E0 800A2AE0 17C00002 */  bnez  $fp, .L800A2AEC
/* 0A36E4 800A2AE4 00000000 */   nop   
/* 0A36E8 800A2AE8 0007000D */  break 7
.L800A2AEC:
/* 0A36EC 800A2AEC 2401FFFF */  li    $at, -1
/* 0A36F0 800A2AF0 17C10004 */  bne   $fp, $at, .L800A2B04
/* 0A36F4 800A2AF4 3C018000 */   lui   $at, 0x8000
/* 0A36F8 800A2AF8 15E10002 */  bne   $t7, $at, .L800A2B04
/* 0A36FC 800A2AFC 00000000 */   nop   
/* 0A3700 800A2B00 0006000D */  break 6
.L800A2B04:
/* 0A3704 800A2B04 0C024C36 */  jal   set_text_color
/* 0A3708 800A2B08 00000000 */   nop   
/* 0A370C 800A2B0C 10000010 */  b     .L800A2B50
/* 0A3710 800A2B10 26110001 */   addiu $s1, $s0, 1
.L800A2B14:
/* 0A3714 800A2B14 0C024C36 */  jal   set_text_color
/* 0A3718 800A2B18 24040001 */   li    $a0, 1
/* 0A371C 800A2B1C 1000000C */  b     .L800A2B50
/* 0A3720 800A2B20 26110001 */   addiu $s1, $s0, 1
.L800A2B24:
/* 0A3724 800A2B24 8FAE00E8 */  lw    $t6, 0xe8($sp)
/* 0A3728 800A2B28 8DD80020 */  lw    $t8, 0x20($t6)
/* 0A372C 800A2B2C 16180005 */  bne   $s0, $t8, .L800A2B44
/* 0A3730 800A2B30 00000000 */   nop   
/* 0A3734 800A2B34 0C024C36 */  jal   set_text_color
/* 0A3738 800A2B38 24040002 */   li    $a0, 2
/* 0A373C 800A2B3C 10000004 */  b     .L800A2B50
/* 0A3740 800A2B40 26110001 */   addiu $s1, $s0, 1
.L800A2B44:
/* 0A3744 800A2B44 0C024C36 */  jal   set_text_color
/* 0A3748 800A2B48 24040001 */   li    $a0, 1
/* 0A374C 800A2B4C 26110001 */  addiu $s1, $s0, 1
.L800A2B50:
/* 0A3750 800A2B50 02202025 */  move  $a0, $s1
/* 0A3754 800A2B54 0C029E25 */  jal   convert_number_to_ascii
/* 0A3758 800A2B58 27A50098 */   addiu $a1, $sp, 0x98
/* 0A375C 800A2B5C 02C02025 */  move  $a0, $s6
/* 0A3760 800A2B60 02402825 */  move  $a1, $s2
/* 0A3764 800A2B64 27A60099 */  addiu $a2, $sp, 0x99
/* 0A3768 800A2B68 00003825 */  move  $a3, $zero
/* 0A376C 800A2B6C E7B60010 */  swc1  $f22, 0x10($sp)
/* 0A3770 800A2B70 0C024CC9 */  jal   print_text_mode_1
/* 0A3774 800A2B74 E7B60014 */   swc1  $f22, 0x14($sp)
/* 0A3778 800A2B78 2401002A */  li    $at, 42
/* 0A377C 800A2B7C 16610006 */  bne   $s3, $at, .L800A2B98
/* 0A3780 800A2B80 3C0A8019 */   lui   $t2, %hi(gSomeDLBuffer) # $t2, 0x8019
/* 0A3784 800A2B84 3C088019 */  lui   $t0, %hi(D_8018EE10) # $t0, 0x8019
/* 0A3788 800A2B88 2508EE10 */  addiu $t0, %lo(D_8018EE10) # addiu $t0, $t0, -0x11f0
/* 0A378C 800A2B8C 0010C9C0 */  sll   $t9, $s0, 7
/* 0A3790 800A2B90 10000004 */  b     .L800A2BA4
/* 0A3794 800A2B94 03281821 */   addu  $v1, $t9, $t0
.L800A2B98:
/* 0A3798 800A2B98 8D4AD9C0 */  lw    $t2, %lo(gSomeDLBuffer)($t2)
/* 0A379C 800A2B9C 001049C0 */  sll   $t1, $s0, 7
/* 0A37A0 800A2BA0 01491821 */  addu  $v1, $t2, $t1
.L800A2BA4:
/* 0A37A4 800A2BA4 906B0004 */  lbu   $t3, 4($v1)
/* 0A37A8 800A2BA8 02602025 */  move  $a0, $s3
/* 0A37AC 800A2BAC 02402825 */  move  $a1, $s2
/* 0A37B0 800A2BB0 1560000B */  bnez  $t3, .L800A2BE0
/* 0A37B4 800A2BB4 3C19800F */   lui   $t9, %hi(gCupCourseOrder)
/* 0A37B8 800A2BB8 3C06800E */  lui   $a2, %hi(D_800E7A44) # $a2, 0x800e
/* 0A37BC 800A2BBC 8CC67A44 */  lw    $a2, %lo(D_800E7A44)($a2)
/* 0A37C0 800A2BC0 02602025 */  move  $a0, $s3
/* 0A37C4 800A2BC4 02402825 */  move  $a1, $s2
/* 0A37C8 800A2BC8 00003825 */  move  $a3, $zero
/* 0A37CC 800A2BCC E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A37D0 800A2BD0 0C024CC9 */  jal   print_text_mode_1
/* 0A37D4 800A2BD4 E7B40014 */   swc1  $f20, 0x14($sp)
/* 0A37D8 800A2BD8 10000019 */  b     .L800A2C40
/* 0A37DC 800A2BDC 24010002 */   li    $at, 2
.L800A2BE0:
/* 0A37E0 800A2BE0 80620005 */  lb    $v0, 5($v1)
/* 0A37E4 800A2BE4 3C06800E */  lui   $a2, %hi(gCourseNamesDup2)
/* 0A37E8 800A2BE8 E7B40014 */  swc1  $f20, 0x14($sp)
/* 0A37EC 800A2BEC 04410003 */  bgez  $v0, .L800A2BFC
/* 0A37F0 800A2BF0 00026083 */   sra   $t4, $v0, 2
/* 0A37F4 800A2BF4 24410003 */  addiu $at, $v0, 3
/* 0A37F8 800A2BF8 00016083 */  sra   $t4, $at, 2
.L800A2BFC:
/* 0A37FC 800A2BFC 000C68C0 */  sll   $t5, $t4, 3
/* 0A3800 800A2C00 04410004 */  bgez  $v0, .L800A2C14
/* 0A3804 800A2C04 304F0003 */   andi  $t7, $v0, 3
/* 0A3808 800A2C08 11E00002 */  beqz  $t7, .L800A2C14
/* 0A380C 800A2C0C 00000000 */   nop   
/* 0A3810 800A2C10 25EFFFFC */  addiu $t7, $t7, -4
.L800A2C14:
/* 0A3814 800A2C14 000F7040 */  sll   $t6, $t7, 1
/* 0A3818 800A2C18 01AEC021 */  addu  $t8, $t5, $t6
/* 0A381C 800A2C1C 0338C821 */  addu  $t9, $t9, $t8
/* 0A3820 800A2C20 87392BB4 */  lh    $t9, %lo(gCupCourseOrder)($t9)
/* 0A3824 800A2C24 E7B40010 */  swc1  $f20, 0x10($sp)
/* 0A3828 800A2C28 00003825 */  move  $a3, $zero
/* 0A382C 800A2C2C 00194080 */  sll   $t0, $t9, 2
/* 0A3830 800A2C30 00C83021 */  addu  $a2, $a2, $t0
/* 0A3834 800A2C34 0C024CC9 */  jal   print_text_mode_1
/* 0A3838 800A2C38 8CC675C4 */   lw    $a2, %lo(gCourseNamesDup2)($a2)
/* 0A383C 800A2C3C 24010002 */  li    $at, 2
.L800A2C40:
/* 0A3840 800A2C40 02208025 */  move  $s0, $s1
/* 0A3844 800A2C44 1621FF97 */  bne   $s1, $at, .L800A2AA4
/* 0A3848 800A2C48 2652001E */   addiu $s2, $s2, 0x1e
/* 0A384C 800A2C4C 8FAA0088 */  lw    $t2, 0x88($sp)
/* 0A3850 800A2C50 8FAB0084 */  lw    $t3, 0x84($sp)
/* 0A3854 800A2C54 26730089 */  addiu $s3, $s3, 0x89
/* 0A3858 800A2C58 2401013C */  li    $at, 316
/* 0A385C 800A2C5C 25490082 */  addiu $t1, $t2, 0x82
/* 0A3860 800A2C60 256C0004 */  addiu $t4, $t3, 4
/* 0A3864 800A2C64 AFAC0084 */  sw    $t4, 0x84($sp)
/* 0A3868 800A2C68 AFA90088 */  sw    $t1, 0x88($sp)
/* 0A386C 800A2C6C 1661FF81 */  bne   $s3, $at, .L800A2A74
/* 0A3870 800A2C70 26D60089 */   addiu $s6, $s6, 0x89
/* 0A3874 800A2C74 3C038019 */  lui   $v1, %hi(gSubMenuSelection) # $v1, 0x8019
/* 0A3878 800A2C78 8063EDEC */  lb    $v1, %lo(gSubMenuSelection)($v1)
.L800A2C7C:
glabel L800A2C7C
/* 0A387C 800A2C7C 246FFFD8 */  addiu $t7, $v1, -0x28
.L800A2C80:
/* 0A3880 800A2C80 2DE10012 */  sltiu $at, $t7, 0x12
/* 0A3884 800A2C84 1020000D */  beqz  $at, .L800A2CBC
/* 0A3888 800A2C88 000F7880 */   sll   $t7, $t7, 2
/* 0A388C 800A2C8C 3C01800F */  lui   $at, %hi(jpt_800F1C48)
/* 0A3890 800A2C90 002F0821 */  addu  $at, $at, $t7
/* 0A3894 800A2C94 8C2F1C48 */  lw    $t7, %lo(jpt_800F1C48)($at)
/* 0A3898 800A2C98 01E00008 */  jr    $t7
/* 0A389C 800A2C9C 00000000 */   nop   
glabel L800A2CA0
/* 0A38A0 800A2CA0 87AD00E0 */  lh    $t5, 0xe0($sp)
/* 0A38A4 800A2CA4 87B800E2 */  lh    $t8, 0xe2($sp)
/* 0A38A8 800A2CA8 25AEFFFB */  addiu $t6, $t5, -5
/* 0A38AC 800A2CAC 2719FFFA */  addiu $t9, $t8, -6
/* 0A38B0 800A2CB0 A7AE00E0 */  sh    $t6, 0xe0($sp)
/* 0A38B4 800A2CB4 10000007 */  b     .L800A2CD4
/* 0A38B8 800A2CB8 A7B900E2 */   sh    $t9, 0xe2($sp)
.L800A2CBC:
glabel L800A2CBC
/* 0A38BC 800A2CBC 87A800E0 */  lh    $t0, 0xe0($sp)
/* 0A38C0 800A2CC0 87A900E2 */  lh    $t1, 0xe2($sp)
/* 0A38C4 800A2CC4 250AFFF6 */  addiu $t2, $t0, -0xa
/* 0A38C8 800A2CC8 252BFFF8 */  addiu $t3, $t1, -8
/* 0A38CC 800A2CCC A7AA00E0 */  sh    $t2, 0xe0($sp)
/* 0A38D0 800A2CD0 A7AB00E2 */  sh    $t3, 0xe2($sp)
.L800A2CD4:
/* 0A38D4 800A2CD4 8FA400E8 */  lw    $a0, 0xe8($sp)
/* 0A38D8 800A2CD8 0C0299AA */  jal   pause_menu_item_box_cursor
/* 0A38DC 800A2CDC 27A500E0 */   addiu $a1, $sp, 0xe0
/* 0A38E0 800A2CE0 8FBF006C */  lw    $ra, 0x6c($sp)
/* 0A38E4 800A2CE4 D7B40030 */  ldc1  $f20, 0x30($sp)
/* 0A38E8 800A2CE8 D7B60038 */  ldc1  $f22, 0x38($sp)
/* 0A38EC 800A2CEC D7B80040 */  ldc1  $f24, 0x40($sp)
/* 0A38F0 800A2CF0 8FB00048 */  lw    $s0, 0x48($sp)
/* 0A38F4 800A2CF4 8FB1004C */  lw    $s1, 0x4c($sp)
/* 0A38F8 800A2CF8 8FB20050 */  lw    $s2, 0x50($sp)
/* 0A38FC 800A2CFC 8FB30054 */  lw    $s3, 0x54($sp)
/* 0A3900 800A2D00 8FB40058 */  lw    $s4, 0x58($sp)
/* 0A3904 800A2D04 8FB5005C */  lw    $s5, 0x5c($sp)
/* 0A3908 800A2D08 8FB60060 */  lw    $s6, 0x60($sp)
/* 0A390C 800A2D0C 8FB70064 */  lw    $s7, 0x64($sp)
/* 0A3910 800A2D10 8FBE0068 */  lw    $fp, 0x68($sp)
/* 0A3914 800A2D14 03E00008 */  jr    $ra
/* 0A3918 800A2D18 27BD00E8 */   addiu $sp, $sp, 0xe8
