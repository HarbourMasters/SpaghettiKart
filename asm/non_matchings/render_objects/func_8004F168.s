glabel func_8004F168
/* 04FD68 8004F168 000570C0 */  sll   $t6, $a1, 3
/* 04FD6C 8004F16C 01C57023 */  subu  $t6, $t6, $a1
/* 04FD70 8004F170 000E7100 */  sll   $t6, $t6, 4
/* 04FD74 8004F174 01C57023 */  subu  $t6, $t6, $a1
/* 04FD78 8004F178 3C0F800E */  lui   $t7, %hi(gPlayerOne) # $t7, 0x800e
/* 04FD7C 8004F17C 8DEFC4DC */  lw    $t7, %lo(gPlayerOne)($t7)
/* 04FD80 8004F180 27BDFFD0 */  addiu $sp, $sp, -0x30
/* 04FD84 8004F184 000E7080 */  sll   $t6, $t6, 2
/* 04FD88 8004F188 01C57023 */  subu  $t6, $t6, $a1
/* 04FD8C 8004F18C 000E70C0 */  sll   $t6, $t6, 3
/* 04FD90 8004F190 AFBF002C */  sw    $ra, 0x2c($sp)
/* 04FD94 8004F194 AFA40030 */  sw    $a0, 0x30($sp)
/* 04FD98 8004F198 AFA60038 */  sw    $a2, 0x38($sp)
/* 04FD9C 8004F19C 01CF1821 */  addu  $v1, $t6, $t7
/* 04FDA0 8004F1A0 94780000 */  lhu   $t8, ($v1)
/* 04FDA4 8004F1A4 00A03825 */  move  $a3, $a1
/* 04FDA8 8004F1A8 3C018019 */  lui   $at, %hi(gMinimapPlayerScale) # $at, 0x8019
/* 04FDAC 8004F1AC 33198000 */  andi  $t9, $t8, 0x8000
/* 04FDB0 8004F1B0 13200088 */  beqz  $t9, .L8004F3D4
/* 04FDB4 8004F1B4 00044040 */   sll   $t0, $a0, 1
/* 04FDB8 8004F1B8 C420D2A0 */  lwc1  $f0, %lo(gMinimapPlayerScale)($at)
/* 04FDBC 8004F1BC C4640014 */  lwc1  $f4, 0x14($v1)
/* 04FDC0 8004F1C0 3C098019 */  lui   $t1, %hi(gMinimapX) # 0x8019
/* 04FDC4 8004F1C4 C466001C */  lwc1  $f6, 0x1c($v1)
/* 04FDC8 8004F1C8 46002082 */  mul.s $f2, $f4, $f0
/* 04FDCC 8004F1CC 01284821 */  addu  $t1, $t1, $t0
/* 04FDD0 8004F1D0 3C0A8019 */  lui   $t2, %hi(D_8018D2F0) # $t2, 0x8019
/* 04FDD4 8004F1D4 3C0C8019 */  lui   $t4, %hi(gMinimapWidth) # $t4, 0x8019
/* 04FDD8 8004F1D8 858CD2B0 */  lh    $t4, %lo(gMinimapWidth)($t4)
/* 04FDDC 8004F1DC 854AD2F0 */  lh    $t2, %lo(D_8018D2F0)($t2)
/* 04FDE0 8004F1E0 8529D2C0 */  lh    $t1, %lo(gMinimapX)($t1) # -0x2d40($t1)
/* 04FDE4 8004F1E4 4600120D */  trunc.w.s $f8, $f2
/* 04FDE8 8004F1E8 46003302 */  mul.s $f12, $f6, $f0
/* 04FDEC 8004F1EC 3C0F8019 */  lui   $t7, %hi(gMinimapPlayerX) # $t7, 0x8019
/* 04FDF0 8004F1F0 85EFD2E0 */  lh    $t7, %lo(gMinimapPlayerX)($t7)
/* 04FDF4 8004F1F4 01001025 */  move  $v0, $t0
/* 04FDF8 8004F1F8 012A5821 */  addu  $t3, $t1, $t2
/* 04FDFC 8004F1FC 05810003 */  bgez  $t4, .L8004F20C
/* 04FE00 8004F200 000C6843 */   sra   $t5, $t4, 1
/* 04FE04 8004F204 25810001 */  addiu $at, $t4, 1
/* 04FE08 8004F208 00016843 */  sra   $t5, $at, 1
.L8004F20C:
/* 04FE0C 8004F20C 44084000 */  mfc1  $t0, $f8
/* 04FE10 8004F210 016D7023 */  subu  $t6, $t3, $t5
/* 04FE14 8004F214 3C0D8019 */  lui   $t5, %hi(gMinimapY) # 0x8019
/* 04FE18 8004F218 01CFC021 */  addu  $t8, $t6, $t7
/* 04FE1C 8004F21C 3C0E8019 */  lui   $t6, %hi(D_8018D2F8) # $t6, 0x8019
/* 04FE20 8004F220 01A26821 */  addu  $t5, $t5, $v0
/* 04FE24 8004F224 00084C00 */  sll   $t1, $t0, 0x10
/* 04FE28 8004F228 85ADD2D8 */  lh    $t5, %lo(gMinimapY)($t5) # -0x2d28($t5)
/* 04FE2C 8004F22C 85CED2F8 */  lh    $t6, %lo(D_8018D2F8)($t6)
/* 04FE30 8004F230 3C198019 */  lui   $t9, %hi(gMinimapHeight) # $t9, 0x8019
/* 04FE34 8004F234 4600628D */  trunc.w.s $f10, $f12
/* 04FE38 8004F238 8739D2B8 */  lh    $t9, %lo(gMinimapHeight)($t9)
/* 04FE3C 8004F23C 00095403 */  sra   $t2, $t1, 0x10
/* 04FE40 8004F240 030A2021 */  addu  $a0, $t8, $t2
/* 04FE44 8004F244 3C188019 */  lui   $t8, %hi(gMinimapPlayerY) # $t8, 0x8019
/* 04FE48 8004F248 01AE7821 */  addu  $t7, $t5, $t6
/* 04FE4C 8004F24C 8718D2E8 */  lh    $t8, %lo(gMinimapPlayerY)($t8)
/* 04FE50 8004F250 440E5000 */  mfc1  $t6, $f10
/* 04FE54 8004F254 00046400 */  sll   $t4, $a0, 0x10
/* 04FE58 8004F258 000C5C03 */  sra   $t3, $t4, 0x10
/* 04FE5C 8004F25C 01602025 */  move  $a0, $t3
/* 04FE60 8004F260 07210003 */  bgez  $t9, .L8004F270
/* 04FE64 8004F264 00194043 */   sra   $t0, $t9, 1
/* 04FE68 8004F268 27210001 */  addiu $at, $t9, 1
/* 04FE6C 8004F26C 00014043 */  sra   $t0, $at, 1
.L8004F270:
/* 04FE70 8004F270 01E84823 */  subu  $t1, $t7, $t0
/* 04FE74 8004F274 8FA80038 */  lw    $t0, 0x38($sp)
/* 04FE78 8004F278 01385021 */  addu  $t2, $t1, $t8
/* 04FE7C 8004F27C 014E2821 */  addu  $a1, $t2, $t6
/* 04FE80 8004F280 0005CC00 */  sll   $t9, $a1, 0x10
/* 04FE84 8004F284 24010008 */  li    $at, 8
/* 04FE88 8004F288 1101003F */  beq   $t0, $at, .L8004F388
/* 04FE8C 8004F28C 00192C03 */   sra   $a1, $t9, 0x10
/* 04FE90 8004F290 00074880 */  sll   $t1, $a3, 2
/* 04FE94 8004F294 3C188016 */  lui   $t8, %hi(gGPCurrentRaceRankByPlayerId)
/* 04FE98 8004F298 0309C021 */  addu  $t8, $t8, $t1
/* 04FE9C 8004F29C 8F1843B8 */  lw    $t8, %lo(gGPCurrentRaceRankByPlayerId)($t8)
/* 04FEA0 8004F2A0 3C02800E */  lui   $v0, %hi(gModeSelection) # $v0, 0x800e
/* 04FEA4 8004F2A4 3C073F80 */  lui   $a3, 0x3f80
/* 04FEA8 8004F2A8 1700001F */  bnez  $t8, .L8004F328
/* 04FEAC 8004F2AC 8FAD0038 */   lw    $t5, 0x38($sp)
/* 04FEB0 8004F2B0 8C42C53C */  lw    $v0, %lo(gModeSelection)($v0)
/* 04FEB4 8004F2B4 24010003 */  li    $at, 3
/* 04FEB8 8004F2B8 1041001B */  beq   $v0, $at, .L8004F328
/* 04FEBC 8004F2BC 24010001 */   li    $at, 1
/* 04FEC0 8004F2C0 10410019 */  beq   $v0, $at, .L8004F328
/* 04FEC4 8004F2C4 3C0A0D03 */   lui   $t2, %hi(common_texture_minimap_kart_character) # $t2, 0xd03
/* 04FEC8 8004F2C8 8466002E */  lh    $a2, 0x2e($v1)
/* 04FECC 8004F2CC 34018000 */  li    $at, 32768
/* 04FED0 8004F2D0 000869C0 */  sll   $t5, $t0, 7
/* 04FED4 8004F2D4 00C13021 */  addu  $a2, $a2, $at
/* 04FED8 8004F2D8 30CCFFFF */  andi  $t4, $a2, 0xffff
/* 04FEDC 8004F2DC 01803025 */  move  $a2, $t4
/* 04FEE0 8004F2E0 254ACCD8 */  addiu $t2, %lo(common_texture_minimap_kart_character) # addiu $t2, $t2, -0x3328
/* 04FEE4 8004F2E4 3C190D00 */  lui   $t9, %hi(common_vtx_player_minimap_icon) # $t9, 0xd00
/* 04FEE8 8004F2E8 27395470 */  addiu $t9, %lo(common_vtx_player_minimap_icon) # addiu $t9, $t9, 0x5470
/* 04FEEC 8004F2EC 01AA7021 */  addu  $t6, $t5, $t2
/* 04FEF0 8004F2F0 240C0008 */  li    $t4, 8
/* 04FEF4 8004F2F4 240F0008 */  li    $t7, 8
/* 04FEF8 8004F2F8 24090008 */  li    $t1, 8
/* 04FEFC 8004F2FC 24180008 */  li    $t8, 8
/* 04FF00 8004F300 AFB80020 */  sw    $t8, 0x20($sp)
/* 04FF04 8004F304 AFA9001C */  sw    $t1, 0x1c($sp)
/* 04FF08 8004F308 AFAF0018 */  sw    $t7, 0x18($sp)
/* 04FF0C 8004F30C AFAC0024 */  sw    $t4, 0x24($sp)
/* 04FF10 8004F310 AFAE0010 */  sw    $t6, 0x10($sp)
/* 04FF14 8004F314 AFB90014 */  sw    $t9, 0x14($sp)
/* 04FF18 8004F318 0C011909 */  jal   func_80046424
/* 04FF1C 8004F31C 3C073F80 */   lui   $a3, 0x3f80
/* 04FF20 8004F320 1000002D */  b     .L8004F3D8
/* 04FF24 8004F324 8FBF002C */   lw    $ra, 0x2c($sp)
.L8004F328:
/* 04FF28 8004F328 8466002E */  lh    $a2, 0x2e($v1)
/* 04FF2C 8004F32C 34018000 */  li    $at, 32768
/* 04FF30 8004F330 3C0E0D03 */  lui   $t6, %hi(common_texture_minimap_kart_character) # $t6, 0xd03
/* 04FF34 8004F334 00C13021 */  addu  $a2, $a2, $at
/* 04FF38 8004F338 30C8FFFF */  andi  $t0, $a2, 0xffff
/* 04FF3C 8004F33C 01003025 */  move  $a2, $t0
/* 04FF40 8004F340 25CECCD8 */  addiu $t6, %lo(common_texture_minimap_kart_character) # addiu $t6, $t6, -0x3328
/* 04FF44 8004F344 000D51C0 */  sll   $t2, $t5, 7
/* 04FF48 8004F348 3C0F0D00 */  lui   $t7, %hi(common_vtx_player_minimap_icon) # $t7, 0xd00
/* 04FF4C 8004F34C 25EF5470 */  addiu $t7, %lo(common_vtx_player_minimap_icon) # addiu $t7, $t7, 0x5470
/* 04FF50 8004F350 014EC821 */  addu  $t9, $t2, $t6
/* 04FF54 8004F354 24080008 */  li    $t0, 8
/* 04FF58 8004F358 24090008 */  li    $t1, 8
/* 04FF5C 8004F35C 24180008 */  li    $t8, 8
/* 04FF60 8004F360 240C0008 */  li    $t4, 8
/* 04FF64 8004F364 AFAC0020 */  sw    $t4, 0x20($sp)
/* 04FF68 8004F368 AFB8001C */  sw    $t8, 0x1c($sp)
/* 04FF6C 8004F36C AFA90018 */  sw    $t1, 0x18($sp)
/* 04FF70 8004F370 AFA80024 */  sw    $t0, 0x24($sp)
/* 04FF74 8004F374 AFB90010 */  sw    $t9, 0x10($sp)
/* 04FF78 8004F378 0C0118EC */  jal   func_800463B0
/* 04FF7C 8004F37C AFAF0014 */   sw    $t7, 0x14($sp)
/* 04FF80 8004F380 10000015 */  b     .L8004F3D8
/* 04FF84 8004F384 8FBF002C */   lw    $ra, 0x2c($sp)
.L8004F388:
/* 04FF88 8004F388 00075880 */  sll   $t3, $a3, 2
/* 04FF8C 8004F38C 3C0D8016 */  lui   $t5, %hi(gGPCurrentRaceRankByPlayerId)
/* 04FF90 8004F390 01AB6821 */  addu  $t5, $t5, $t3
/* 04FF94 8004F394 8DAD43B8 */  lw    $t5, %lo(gGPCurrentRaceRankByPlayerId)($t5)
/* 04FF98 8004F398 24070008 */  li    $a3, 8
/* 04FF9C 8004F39C 24060008 */  li    $a2, 8
/* 04FFA0 8004F3A0 15A00009 */  bnez  $t5, .L8004F3C8
/* 04FFA4 8004F3A4 3C0E0D03 */   lui   $t6, %hi(common_texture_minimap_progress_dot) # $t6, 0xd03
/* 04FFA8 8004F3A8 3C0A0D03 */  lui   $t2, %hi(common_texture_minimap_progress_dot) # $t2, 0xd03
/* 04FFAC 8004F3AC 254AD0D8 */  addiu $t2, %lo(common_texture_minimap_progress_dot) # addiu $t2, $t2, -0x2f28
/* 04FFB0 8004F3B0 AFAA0010 */  sw    $t2, 0x10($sp)
/* 04FFB4 8004F3B4 24060008 */  li    $a2, 8
/* 04FFB8 8004F3B8 0C013114 */  jal   func_8004C450
/* 04FFBC 8004F3BC 24070008 */   li    $a3, 8
/* 04FFC0 8004F3C0 10000005 */  b     .L8004F3D8
/* 04FFC4 8004F3C4 8FBF002C */   lw    $ra, 0x2c($sp)
.L8004F3C8:
/* 04FFC8 8004F3C8 25CED0D8 */  addiu $t6, %lo(common_texture_minimap_progress_dot) # addiu $t6, $t6, -0x2f28
/* 04FFCC 8004F3CC 0C0130D9 */  jal   draw_hud_2d_texture
/* 04FFD0 8004F3D0 AFAE0010 */   sw    $t6, 0x10($sp)
.L8004F3D4:
/* 04FFD4 8004F3D4 8FBF002C */  lw    $ra, 0x2c($sp)
.L8004F3D8:
/* 04FFD8 8004F3D8 27BD0030 */  addiu $sp, $sp, 0x30
/* 04FFDC 8004F3DC 03E00008 */  jr    $ra
/* 04FFE0 8004F3E0 00000000 */   nop
