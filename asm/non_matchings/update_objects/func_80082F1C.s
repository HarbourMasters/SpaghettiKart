.section .late_rodata

glabel D_800EF1BC
.float 0.027

.section .text

glabel func_80082F1C
/* 083B1C 80082F1C 000470C0 */  sll   $t6, $a0, 3
/* 083B20 80082F20 01C47023 */  subu  $t6, $t6, $a0
/* 083B24 80082F24 3C0F8016 */  lui   $t7, %hi(gObjectList) # $t7, 0x8016
/* 083B28 80082F28 25EF5C18 */  addiu $t7, %lo(gObjectList) # addiu $t7, $t7, 0x5c18
/* 083B2C 80082F2C 000E7140 */  sll   $t6, $t6, 5
/* 083B30 80082F30 01CF1021 */  addu  $v0, $t6, $t7
/* 083B34 80082F34 3C180601 */  lui   $t8, %hi(d_course_yoshi_valley_unk5) # $t8, 0x601
/* 083B38 80082F38 3C190601 */  lui   $t9, %hi(d_course_yoshi_valley_unk4) # $t9, 0x601
/* 083B3C 80082F3C 27184798 */  addiu $t8, %lo(d_course_yoshi_valley_unk5) # addiu $t8, $t8, 0x4798
/* 083B40 80082F40 27394794 */  addiu $t9, %lo(d_course_yoshi_valley_unk4) # addiu $t9, $t9, 0x4794
/* 083B44 80082F44 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 083B48 80082F48 AC580070 */  sw    $t8, 0x70($v0)
/* 083B4C 80082F4C AC590074 */  sw    $t9, 0x74($v0)
/* 083B50 80082F50 3C01800F */  lui   $at, %hi(D_800EF1BC) # $at, 0x800f
/* 083B54 80082F54 C424F1BC */  lwc1  $f4, %lo(D_800EF1BC)($at)
/* 083B58 80082F58 AFBF001C */  sw    $ra, 0x1c($sp)
/* 083B5C 80082F5C AFB00018 */  sw    $s0, 0x18($sp)
/* 083B60 80082F60 00808025 */  move  $s0, $a0
/* 083B64 80082F64 AFA5002C */  sw    $a1, 0x2c($sp)
/* 083B68 80082F68 0C01C922 */  jal   object_next_state
/* 083B6C 80082F6C E4440000 */   swc1  $f4, ($v0)
/* 083B70 80082F70 8FA9002C */  lw    $t1, 0x2c($sp)
/* 083B74 80082F74 3C0B800E */  lui   $t3, %hi(D_800E5DF4) # $t3, 0x800e
/* 083B78 80082F78 256B5DF4 */  addiu $t3, %lo(D_800E5DF4) # addiu $t3, $t3, 0x5df4
/* 083B7C 80082F7C 000950C0 */  sll   $t2, $t1, 3
/* 083B80 80082F80 014B1021 */  addu  $v0, $t2, $t3
/* 083B84 80082F84 844C0000 */  lh    $t4, ($v0)
/* 083B88 80082F88 844D0002 */  lh    $t5, 2($v0)
/* 083B8C 80082F8C 844E0004 */  lh    $t6, 4($v0)
/* 083B90 80082F90 448C3000 */  mtc1  $t4, $f6
/* 083B94 80082F94 3C018019 */  lui   $at, %hi(xOrientation) # $at, 0x8019
/* 083B98 80082F98 C42AD01C */  lwc1  $f10, %lo(xOrientation)($at)
/* 083B9C 80082F9C 46803220 */  cvt.s.w $f8, $f6
/* 083BA0 80082FA0 448D9000 */  mtc1  $t5, $f18
/* 083BA4 80082FA4 448E2000 */  mtc1  $t6, $f4
/* 083BA8 80082FA8 AFA20024 */  sw    $v0, 0x24($sp)
/* 083BAC 80082FAC 02002025 */  move  $a0, $s0
/* 083BB0 80082FB0 468094A0 */  cvt.s.w $f18, $f18
/* 083BB4 80082FB4 460A4402 */  mul.s $f16, $f8, $f10
/* 083BB8 80082FB8 44069000 */  mfc1  $a2, $f18
/* 083BBC 80082FBC 46802120 */  cvt.s.w $f4, $f4
/* 083BC0 80082FC0 44058000 */  mfc1  $a1, $f16
/* 083BC4 80082FC4 44072000 */  mfc1  $a3, $f4
/* 083BC8 80082FC8 0C022DF5 */  jal   set_obj_origin_pos
/* 083BCC 80082FCC 00000000 */   nop   
/* 083BD0 80082FD0 44800000 */  mtc1  $zero, $f0
/* 083BD4 80082FD4 02002025 */  move  $a0, $s0
/* 083BD8 80082FD8 44050000 */  mfc1  $a1, $f0
/* 083BDC 80082FDC 44060000 */  mfc1  $a2, $f0
/* 083BE0 80082FE0 44070000 */  mfc1  $a3, $f0
/* 083BE4 80082FE4 0C022E03 */  jal   set_obj_origin_offset
/* 083BE8 80082FE8 00000000 */   nop   
/* 083BEC 80082FEC 8FA20024 */  lw    $v0, 0x24($sp)
/* 083BF0 80082FF0 02002025 */  move  $a0, $s0
/* 083BF4 80082FF4 00002825 */  move  $a1, $zero
/* 083BF8 80082FF8 00003825 */  move  $a3, $zero
/* 083BFC 80082FFC 0C022E22 */  jal   set_obj_direction_angle
/* 083C00 80083000 94460006 */   lhu   $a2, 6($v0)
/* 083C04 80083004 8FBF001C */  lw    $ra, 0x1c($sp)
/* 083C08 80083008 8FB00018 */  lw    $s0, 0x18($sp)
/* 083C0C 8008300C 27BD0028 */  addiu $sp, $sp, 0x28
/* 083C10 80083010 03E00008 */  jr    $ra
/* 083C14 80083014 00000000 */   nop   
