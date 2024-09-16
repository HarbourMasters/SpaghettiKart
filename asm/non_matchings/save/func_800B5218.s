glabel func_800B5218
/* 0B5E18 800B5218 3C0E8019 */  lui   $t6, %hi(gCupSelection) # $t6, 0x8019
/* 0B5E1C 800B521C 81CEEE09 */  lb    $t6, %lo(gCupSelection)($t6)
/* 0B5E20 800B5220 3C188019 */  lui   $t8, %hi(gCourseIndexInCup) # $t8, 0x8019
/* 0B5E24 800B5224 8318EE0B */  lb    $t8, %lo(gCourseIndexInCup)($t8)
/* 0B5E28 800B5228 3C09800F */  lui   $t1, %hi(gCharacterSelections) # $t1, 0x800f
/* 0B5E2C 800B522C 812986A8 */  lb    $t1, %lo(gCharacterSelections)($t1)
/* 0B5E30 800B5230 27BDFFB8 */  addiu $sp, $sp, -0x48
/* 0B5E34 800B5234 000E7880 */  sll   $t7, $t6, 2
/* 0B5E38 800B5238 3C058019 */  lui   $a1, %hi(playerHUD+0x4) # $a1, 0x8019
/* 0B5E3C 800B523C 3C068019 */  lui   $a2, %hi(playerHUD) # $a2, 0x8019
/* 0B5E40 800B5240 01F8C821 */  addu  $t9, $t7, $t8
/* 0B5E44 800B5244 AFBF0014 */  sw    $ra, 0x14($sp)
/* 0B5E48 800B5248 AFB90038 */  sw    $t9, 0x38($sp)
/* 0B5E4C 800B524C 24070001 */  li    $a3, 1
/* 0B5E50 800B5250 24C6CA70 */  addiu $a2, %lo(playerHUD) # addiu $a2, $a2, -0x3590
/* 0B5E54 800B5254 24A5CA74 */  addiu $a1, %lo(playerHUD+0x4) # addiu $a1, $a1, -0x358c
/* 0B5E58 800B5258 24020001 */  li    $v0, 1
/* 0B5E5C 800B525C 24080003 */  li    $t0, 3
/* 0B5E60 800B5260 AFA90028 */  sw    $t1, 0x28($sp)
.L800B5264:
/* 0B5E64 800B5264 8CA30020 */  lw    $v1, 0x20($a1)
/* 0B5E68 800B5268 8CC40020 */  lw    $a0, 0x20($a2)
/* 0B5E6C 800B526C 240A0001 */  li    $t2, 1
/* 0B5E70 800B5270 00025880 */  sll   $t3, $v0, 2
/* 0B5E74 800B5274 0064082A */  slt   $at, $v1, $a0
/* 0B5E78 800B5278 10200005 */  beqz  $at, .L800B5290
/* 0B5E7C 800B527C 3C0C8019 */   lui   $t4, %hi(playerHUD) # $t4, 0x8019
/* 0B5E80 800B5280 258CCA70 */  addiu $t4, %lo(playerHUD) # addiu $t4, $t4, -0x3590
/* 0B5E84 800B5284 004A3804 */  sllv  $a3, $t2, $v0
/* 0B5E88 800B5288 10000005 */  b     .L800B52A0
/* 0B5E8C 800B528C 016C3021 */   addu  $a2, $t3, $t4
.L800B5290:
/* 0B5E90 800B5290 14830003 */  bne   $a0, $v1, .L800B52A0
/* 0B5E94 800B5294 240D0001 */   li    $t5, 1
/* 0B5E98 800B5298 004D7004 */  sllv  $t6, $t5, $v0
/* 0B5E9C 800B529C 00EE3825 */  or    $a3, $a3, $t6
.L800B52A0:
/* 0B5EA0 800B52A0 24420001 */  addiu $v0, $v0, 1
/* 0B5EA4 800B52A4 1448FFEF */  bne   $v0, $t0, .L800B5264
/* 0B5EA8 800B52A8 24A50004 */   addiu $a1, $a1, 4
/* 0B5EAC 800B52AC AFA60018 */  sw    $a2, 0x18($sp)
/* 0B5EB0 800B52B0 0C02D3CB */  jal   func_800B4F2C
/* 0B5EB4 800B52B4 AFA70024 */   sw    $a3, 0x24($sp)
/* 0B5EB8 800B52B8 8FA60018 */  lw    $a2, 0x18($sp)
/* 0B5EBC 800B52BC 3C01000F */  lui   $at, (0x000FFFFF >> 16) # lui $at, 0xf
/* 0B5EC0 800B52C0 3421FFFF */  ori   $at, (0x000FFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 0B5EC4 800B52C4 8CC50020 */  lw    $a1, 0x20($a2)
/* 0B5EC8 800B52C8 00417824 */  and   $t7, $v0, $at
/* 0B5ECC 800B52CC 8FA70024 */  lw    $a3, 0x24($sp)
/* 0B5ED0 800B52D0 00AF082B */  sltu  $at, $a1, $t7
/* 0B5ED4 800B52D4 10200021 */  beqz  $at, .L800B535C
/* 0B5ED8 800B52D8 00001025 */   move  $v0, $zero
/* 0B5EDC 800B52DC 8FA30038 */  lw    $v1, 0x38($sp)
/* 0B5EE0 800B52E0 3C0C8019 */  lui   $t4, %hi(gSaveData) # $t4, 0x8019
/* 0B5EE4 800B52E4 258CEB90 */  addiu $t4, %lo(gSaveData) # addiu $t4, $t4, -0x1470
/* 0B5EE8 800B52E8 04610003 */  bgez  $v1, .L800B52F8
/* 0B5EEC 800B52EC 0003C083 */   sra   $t8, $v1, 2
/* 0B5EF0 800B52F0 24610003 */  addiu $at, $v1, 3
/* 0B5EF4 800B52F4 0001C083 */  sra   $t8, $at, 2
.L800B52F8:
/* 0B5EF8 800B52F8 0018C880 */  sll   $t9, $t8, 2
/* 0B5EFC 800B52FC 0338C823 */  subu  $t9, $t9, $t8
/* 0B5F00 800B5300 0019C940 */  sll   $t9, $t9, 5
/* 0B5F04 800B5304 04610004 */  bgez  $v1, .L800B5318
/* 0B5F08 800B5308 30690003 */   andi  $t1, $v1, 3
/* 0B5F0C 800B530C 11200002 */  beqz  $t1, .L800B5318
/* 0B5F10 800B5310 00000000 */   nop
/* 0B5F14 800B5314 2529FFFC */  addiu $t1, -4
.L800B5318:
/* 0B5F18 800B5318 00095080 */  sll   $t2, $t1, 2
/* 0B5F1C 800B531C 01495023 */  subu  $t2, $t2, $t1
/* 0B5F20 800B5320 000A50C0 */  sll   $t2, $t2, 3
/* 0B5F24 800B5324 032A5821 */  addu  $t3, $t9, $t2
/* 0B5F28 800B5328 016C1021 */  addu  $v0, $t3, $t4
/* 0B5F2C 800B532C 2444000F */  addiu $a0, $v0, 0xf
/* 0B5F30 800B5330 AFA20020 */  sw    $v0, 0x20($sp)
/* 0B5F34 800B5334 8FA60028 */  lw    $a2, 0x28($sp)
/* 0B5F38 800B5338 0C02D373 */  jal   populate_time_trial_record
/* 0B5F3C 800B533C AFA70024 */   sw    $a3, 0x24($sp)
/* 0B5F40 800B5340 8FA20020 */  lw    $v0, 0x20($sp)
/* 0B5F44 800B5344 240D0001 */  li    $t5, 1
/* 0B5F48 800B5348 A04D0012 */  sb    $t5, 0x12($v0)
/* 0B5F4C 800B534C 0C02D178 */  jal   func_800B45E0
/* 0B5F50 800B5350 8FA40038 */   lw    $a0, 0x38($sp)
/* 0B5F54 800B5354 10000001 */  b     .L800B535C
/* 0B5F58 800B5358 8FA20024 */   lw    $v0, 0x24($sp)
.L800B535C:
/* 0B5F5C 800B535C 8FBF0014 */  lw    $ra, 0x14($sp)
/* 0B5F60 800B5360 27BD0048 */  addiu $sp, $sp, 0x48
/* 0B5F64 800B5364 03E00008 */  jr    $ra
/* 0B5F68 800B5368 00000000 */   nop
