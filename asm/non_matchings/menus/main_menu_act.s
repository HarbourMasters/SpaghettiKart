.section .late_rodata

glabel jpt_800F2DFC
.word L800B2A6C, L800B3468, L800B3468, L800B2AA4
.word L800B2D30, L800B2F70, L800B32B4
.word L800B32B4, L800B2F70

.section .text
glabel main_menu_act
/* 0B35D8 800B29D8 27BDFFD0 */  addiu $sp, $sp, -0x30
/* 0B35DC 800B29DC AFBF0014 */  sw    $ra, 0x14($sp)
/* 0B35E0 800B29E0 AFA50034 */  sw    $a1, 0x34($sp)
/* 0B35E4 800B29E4 9498000C */  lhu   $t8, 0xc($a0)
/* 0B35E8 800B29E8 948F0006 */  lhu   $t7, 6($a0)
/* 0B35EC 800B29EC 3C09800E */  lui   $t1, %hi(gEnableDebugMode) # $t1, 0x800e
/* 0B35F0 800B29F0 9529C520 */  lhu   $t1, %lo(gEnableDebugMode)($t1)
/* 0B35F4 800B29F4 01F81825 */  or    $v1, $t7, $t8
/* 0B35F8 800B29F8 30AEFFFF */  andi  $t6, $a1, 0xffff
/* 0B35FC 800B29FC 3079FFFF */  andi  $t9, $v1, 0xffff
/* 0B3600 800B2A00 01C02825 */  move  $a1, $t6
/* 0B3604 800B2A04 15200007 */  bnez  $t1, .L800B2A24
/* 0B3608 800B2A08 03201825 */   move  $v1, $t9
/* 0B360C 800B2A0C 332A1000 */  andi  $t2, $t9, 0x1000
/* 0B3610 800B2A10 51400005 */  beql  $t2, $zero, .L800B2A28
/* 0B3614 800B2A14 A7A3002E */   sh    $v1, 0x2e($sp)
/* 0B3618 800B2A18 37238000 */  ori   $v1, $t9, 0x8000
/* 0B361C 800B2A1C 306BFFFF */  andi  $t3, $v1, 0xffff
/* 0B3620 800B2A20 01601825 */  move  $v1, $t3
.L800B2A24:
/* 0B3624 800B2A24 A7A3002E */  sh    $v1, 0x2e($sp)
.L800B2A28:
/* 0B3628 800B2A28 0C02D148 */  jal   func_800B4520
/* 0B362C 800B2A2C A7A50036 */   sh    $a1, 0x36($sp)
/* 0B3630 800B2A30 97A3002E */  lhu   $v1, 0x2e($sp)
/* 0B3634 800B2A34 144002A8 */  bnez  $v0, .L800B34D8
/* 0B3638 800B2A38 97A50036 */   lhu   $a1, 0x36($sp)
/* 0B363C 800B2A3C 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B3640 800B2A40 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B3644 800B2A44 910C0000 */  lbu   $t4, ($t0)
/* 0B3648 800B2A48 3C188019 */  lui   $t8, %hi(gPlayerCount) # 0x8019
/* 0B364C 800B2A4C 2D810009 */  sltiu $at, $t4, 9
/* 0B3650 800B2A50 10200293 */  beqz  $at, .L800B34A0
/* 0B3654 800B2A54 000C6080 */   sll   $t4, $t4, 2
/* 0B3658 800B2A58 3C01800F */  lui   $at, %hi(jpt_800F2DFC) # 0x800f
/* 0B365C 800B2A5C 002C0821 */  addu  $at, $at, $t4
/* 0B3660 800B2A60 8C2C2DFC */  lw    $t4, %lo(jpt_800F2DFC)($at) # 0x2dfc($at)
/* 0B3664 800B2A64 01800008 */  jr    $t4
/* 0B3668 800B2A68 00000000 */   nop   
glabel L800B2A6C # case 0
/* 0B366C 800B2A6C 3C0D8019 */  lui   $t5, %hi(gPlayerCount) # $t5, 0x8019
/* 0B3670 800B2A70 81ADEDF3 */  lb    $t5, %lo(gPlayerCount)($t5)
/* 0B3674 800B2A74 3C0F800F */  lui   $t7, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3678 800B2A78 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B367C 800B2A7C 01ED7821 */  addu  $t7, $t7, $t5
/* 0B3680 800B2A80 81EF86AB */  lb    $t7, %lo((D_800E86AC - 1))($t7) # -0x7955($t7)
/* 0B3684 800B2A84 000D7080 */  sll   $t6, $t5, 2
/* 0B3688 800B2A88 01CD7023 */  subu  $t6, $t6, $t5
/* 0B368C 800B2A8C 000E7080 */  sll   $t6, $t6, 2
/* 0B3690 800B2A90 000FC080 */  sll   $t8, $t7, 2
/* 0B3694 800B2A94 01D8C821 */  addu  $t9, $t6, $t8
/* 0B3698 800B2A98 00791821 */  addu  $v1, $v1, $t9
/* 0B369C 800B2A9C 1000028C */  b     .L800B34D0
/* 0B36A0 800B2AA0 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
glabel L800B2AA4 # case 3
/* 0B36A4 800B2AA4 30690100 */  andi  $t1, $v1, 0x100
/* 0B36A8 800B2AA8 11200010 */  beqz  $t1, .L800B2AEC
/* 0B36AC 800B2AAC 00603025 */   move  $a2, $v1
/* 0B36B0 800B2AB0 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B36B4 800B2AB4 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B36B8 800B2AB8 28410004 */  slti  $at, $v0, 4
/* 0B36BC 800B2ABC 1020000B */  beqz  $at, .L800B2AEC
/* 0B36C0 800B2AC0 244A0001 */   addiu $t2, $v0, 1
/* 0B36C4 800B2AC4 3C018019 */  lui   $at, %hi(gPlayerCount) # $at, 0x8019
/* 0B36C8 800B2AC8 A02AEDF3 */  sb    $t2, %lo(gPlayerCount)($at)
/* 0B36CC 800B2ACC 0C02D12B */  jal   func_800B44AC
/* 0B36D0 800B2AD0 AFA3001C */   sw    $v1, 0x1c($sp)
/* 0B36D4 800B2AD4 3C044900 */  lui   $a0, (0x49008000 >> 16) # lui $a0, 0x4900
/* 0B36D8 800B2AD8 0C032384 */  jal   play_sound2
/* 0B36DC 800B2ADC 34848000 */   ori   $a0, (0x49008000 & 0xFFFF) # ori $a0, $a0, 0x8000
/* 0B36E0 800B2AE0 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B36E4 800B2AE4 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B36E8 800B2AE8 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B2AEC:
/* 0B36EC 800B2AEC 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B36F0 800B2AF0 30CB0200 */  andi  $t3, $a2, 0x200
/* 0B36F4 800B2AF4 11600010 */  beqz  $t3, .L800B2B38
/* 0B36F8 800B2AF8 8042EDF3 */   lb    $v0, %lo(gPlayerCount)($v0)
/* 0B36FC 800B2AFC 28410002 */  slti  $at, $v0, 2
/* 0B3700 800B2B00 1420000D */  bnez  $at, .L800B2B38
/* 0B3704 800B2B04 244CFFFF */   addiu $t4, $v0, -1
/* 0B3708 800B2B08 3C018019 */  lui   $at, %hi(gPlayerCount) # $at, 0x8019
/* 0B370C 800B2B0C A02CEDF3 */  sb    $t4, %lo(gPlayerCount)($at)
/* 0B3710 800B2B10 0C02D12B */  jal   func_800B44AC
/* 0B3714 800B2B14 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B3718 800B2B18 3C044900 */  lui   $a0, (0x49008000 >> 16) # lui $a0, 0x4900
/* 0B371C 800B2B1C 0C032384 */  jal   play_sound2
/* 0B3720 800B2B20 34848000 */   ori   $a0, (0x49008000 & 0xFFFF) # ori $a0, $a0, 0x8000
/* 0B3724 800B2B24 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3728 800B2B28 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B372C 800B2B2C 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B3730 800B2B30 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3734 800B2B34 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B2B38:
/* 0B3738 800B2B38 3C04800E */  lui   $a0, %hi(gPlayerCountSelection1) # $a0, 0x800e
/* 0B373C 800B2B3C 2484C538 */  addiu $a0, %lo(gPlayerCountSelection1) # addiu $a0, $a0, -0x3ac8
/* 0B3740 800B2B40 24070001 */  li    $a3, 1
/* 0B3744 800B2B44 AC820000 */  sw    $v0, ($a0)
/* 0B3748 800B2B48 1047000A */  beq   $v0, $a3, .L800B2B74
/* 0B374C 800B2B4C 00401825 */   move  $v1, $v0
/* 0B3750 800B2B50 24010002 */  li    $at, 2
/* 0B3754 800B2B54 1061000A */  beq   $v1, $at, .L800B2B80
/* 0B3758 800B2B58 24040003 */   li    $a0, 3
/* 0B375C 800B2B5C 1064000B */  beq   $v1, $a0, .L800B2B8C
/* 0B3760 800B2B60 24010004 */   li    $at, 4
/* 0B3764 800B2B64 10610009 */  beq   $v1, $at, .L800B2B8C
/* 0B3768 800B2B68 00000000 */   nop   
/* 0B376C 800B2B6C 1000000A */  b     .L800B2B98
/* 0B3770 800B2B70 30CD4000 */   andi  $t5, $a2, 0x4000
.L800B2B74:
/* 0B3774 800B2B74 3C01800E */  lui   $at, %hi(gScreenModeSelection) # $at, 0x800e
/* 0B3778 800B2B78 10000006 */  b     .L800B2B94
/* 0B377C 800B2B7C AC20C530 */   sw    $zero, %lo(gScreenModeSelection)($at)
.L800B2B80:
/* 0B3780 800B2B80 3C01800E */  lui   $at, %hi(gScreenModeSelection) # $at, 0x800e
/* 0B3784 800B2B84 10000003 */  b     .L800B2B94
/* 0B3788 800B2B88 AC27C530 */   sw    $a3, %lo(gScreenModeSelection)($at)
.L800B2B8C:
/* 0B378C 800B2B8C 3C01800E */  lui   $at, %hi(gScreenModeSelection) # $at, 0x800e
/* 0B3790 800B2B90 AC24C530 */  sw    $a0, %lo(gScreenModeSelection)($at)
.L800B2B94:
/* 0B3794 800B2B94 30CD4000 */  andi  $t5, $a2, 0x4000
.L800B2B98:
/* 0B3798 800B2B98 11A00019 */  beqz  $t5, .L800B2C00
/* 0B379C 800B2B9C 30CA8000 */   andi  $t2, $a2, 0x8000
/* 0B37A0 800B2BA0 0C02783C */  jal   func_8009E0F0
/* 0B37A4 800B2BA4 24040014 */   li    $a0, 20
/* 0B37A8 800B2BA8 0C0328CC */  jal   func_800CA330
/* 0B37AC 800B2BAC 24040019 */   li    $a0, 25
/* 0B37B0 800B2BB0 24070001 */  li    $a3, 1
/* 0B37B4 800B2BB4 3C018019 */  lui   $at, %hi(D_8018EDE0) # $at, 0x8019
/* 0B37B8 800B2BB8 3C044900 */  lui   $a0, (0x49008002 >> 16) # lui $a0, 0x4900
/* 0B37BC 800B2BBC AC27EDE0 */  sw    $a3, %lo(D_8018EDE0)($at)
/* 0B37C0 800B2BC0 0C032384 */  jal   play_sound2
/* 0B37C4 800B2BC4 34848002 */   ori   $a0, (0x49008002 & 0xFFFF) # ori $a0, $a0, 0x8002
/* 0B37C8 800B2BC8 3C0F8019 */  lui   $t7, %hi(gPlayerCount) # $t7, 0x8019
/* 0B37CC 800B2BCC 81EFEDF3 */  lb    $t7, %lo(gPlayerCount)($t7)
/* 0B37D0 800B2BD0 3C18800F */  lui   $t8, %hi((D_800E86AC - 1)) # 0x800f
/* 0B37D4 800B2BD4 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B37D8 800B2BD8 030FC021 */  addu  $t8, $t8, $t7
/* 0B37DC 800B2BDC 831886AB */  lb    $t8, %lo((D_800E86AC - 1))($t8) # -0x7955($t8)
/* 0B37E0 800B2BE0 000F7080 */  sll   $t6, $t7, 2
/* 0B37E4 800B2BE4 01CF7023 */  subu  $t6, $t6, $t7
/* 0B37E8 800B2BE8 000E7080 */  sll   $t6, $t6, 2
/* 0B37EC 800B2BEC 0018C880 */  sll   $t9, $t8, 2
/* 0B37F0 800B2BF0 01D94821 */  addu  $t1, $t6, $t9
/* 0B37F4 800B2BF4 00691821 */  addu  $v1, $v1, $t1
/* 0B37F8 800B2BF8 10000235 */  b     .L800B34D0
/* 0B37FC 800B2BFC 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B2C00:
/* 0B3800 800B2C00 11400015 */  beqz  $t2, .L800B2C58
/* 0B3804 800B2C04 30D90020 */   andi  $t9, $a2, 0x20
/* 0B3808 800B2C08 240B0004 */  li    $t3, 4
/* 0B380C 800B2C0C 0C02D12B */  jal   func_800B44AC
/* 0B3810 800B2C10 A10B0000 */   sb    $t3, ($t0)
/* 0B3814 800B2C14 3C044900 */  lui   $a0, (0x49008001 >> 16) # lui $a0, 0x4900
/* 0B3818 800B2C18 0C032384 */  jal   play_sound2
/* 0B381C 800B2C1C 34848001 */   ori   $a0, (0x49008001 & 0xFFFF) # ori $a0, $a0, 0x8001
/* 0B3820 800B2C20 3C0C8019 */  lui   $t4, %hi(gPlayerCount) # $t4, 0x8019
/* 0B3824 800B2C24 818CEDF3 */  lb    $t4, %lo(gPlayerCount)($t4)
/* 0B3828 800B2C28 3C0F800F */  lui   $t7, %hi((D_800E86AC - 1)) # 0x800f
/* 0B382C 800B2C2C 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3830 800B2C30 01EC7821 */  addu  $t7, $t7, $t4
/* 0B3834 800B2C34 81EF86AB */  lb    $t7, %lo((D_800E86AC - 1))($t7) # -0x7955($t7)
/* 0B3838 800B2C38 000C6880 */  sll   $t5, $t4, 2
/* 0B383C 800B2C3C 01AC6823 */  subu  $t5, $t5, $t4
/* 0B3840 800B2C40 000D6880 */  sll   $t5, $t5, 2
/* 0B3844 800B2C44 000FC080 */  sll   $t8, $t7, 2
/* 0B3848 800B2C48 01B87021 */  addu  $t6, $t5, $t8
/* 0B384C 800B2C4C 006E1821 */  addu  $v1, $v1, $t6
/* 0B3850 800B2C50 1000021F */  b     .L800B34D0
/* 0B3854 800B2C54 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B2C58:
/* 0B3858 800B2C58 13200014 */  beqz  $t9, .L800B2CAC
/* 0B385C 800B2C5C 30CD0010 */   andi  $t5, $a2, 0x10
/* 0B3860 800B2C60 0C0278A0 */  jal   func_8009E280
/* 0B3864 800B2C64 A1070000 */   sb    $a3, ($t0)
/* 0B3868 800B2C68 3C044900 */  lui   $a0, (0x49009010 >> 16) # lui $a0, 0x4900
/* 0B386C 800B2C6C 0C032384 */  jal   play_sound2
/* 0B3870 800B2C70 34849010 */   ori   $a0, (0x49009010 & 0xFFFF) # ori $a0, $a0, 0x9010
/* 0B3874 800B2C74 3C098019 */  lui   $t1, %hi(gPlayerCount) # $t1, 0x8019
/* 0B3878 800B2C78 8129EDF3 */  lb    $t1, %lo(gPlayerCount)($t1)
/* 0B387C 800B2C7C 3C0B800F */  lui   $t3, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3880 800B2C80 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3884 800B2C84 01695821 */  addu  $t3, $t3, $t1
/* 0B3888 800B2C88 816B86AB */  lb    $t3, %lo((D_800E86AC - 1))($t3) # -0x7955($t3)
/* 0B388C 800B2C8C 00095080 */  sll   $t2, $t1, 2
/* 0B3890 800B2C90 01495023 */  subu  $t2, $t2, $t1
/* 0B3894 800B2C94 000A5080 */  sll   $t2, $t2, 2
/* 0B3898 800B2C98 000B6080 */  sll   $t4, $t3, 2
/* 0B389C 800B2C9C 014C7821 */  addu  $t7, $t2, $t4
/* 0B38A0 800B2CA0 006F1821 */  addu  $v1, $v1, $t7
/* 0B38A4 800B2CA4 1000020A */  b     .L800B34D0
/* 0B38A8 800B2CA8 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B2CAC:
/* 0B38AC 800B2CAC 11A00015 */  beqz  $t5, .L800B2D04
/* 0B38B0 800B2CB0 00026080 */   sll   $t4, $v0, 2
/* 0B38B4 800B2CB4 24180002 */  li    $t8, 2
/* 0B38B8 800B2CB8 0C027896 */  jal   func_8009E258
/* 0B38BC 800B2CBC A1180000 */   sb    $t8, ($t0)
/* 0B38C0 800B2CC0 3C044900 */  lui   $a0, (0x49009011 >> 16) # lui $a0, 0x4900
/* 0B38C4 800B2CC4 0C032384 */  jal   play_sound2
/* 0B38C8 800B2CC8 34849011 */   ori   $a0, (0x49009011 & 0xFFFF) # ori $a0, $a0, 0x9011
/* 0B38CC 800B2CCC 3C0E8019 */  lui   $t6, %hi(gPlayerCount) # $t6, 0x8019
/* 0B38D0 800B2CD0 81CEEDF3 */  lb    $t6, %lo(gPlayerCount)($t6)
/* 0B38D4 800B2CD4 3C09800F */  lui   $t1, %hi((D_800E86AC - 1)) # 0x800f
/* 0B38D8 800B2CD8 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B38DC 800B2CDC 012E4821 */  addu  $t1, $t1, $t6
/* 0B38E0 800B2CE0 812986AB */  lb    $t1, %lo((D_800E86AC - 1))($t1) # -0x7955($t1)
/* 0B38E4 800B2CE4 000EC880 */  sll   $t9, $t6, 2
/* 0B38E8 800B2CE8 032EC823 */  subu  $t9, $t9, $t6
/* 0B38EC 800B2CEC 0019C880 */  sll   $t9, $t9, 2
/* 0B38F0 800B2CF0 00095880 */  sll   $t3, $t1, 2
/* 0B38F4 800B2CF4 032B5021 */  addu  $t2, $t9, $t3
/* 0B38F8 800B2CF8 006A1821 */  addu  $v1, $v1, $t2
/* 0B38FC 800B2CFC 100001F4 */  b     .L800B34D0
/* 0B3900 800B2D00 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B2D04:
/* 0B3904 800B2D04 3C0F800F */  lui   $t7, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3908 800B2D08 01E27821 */  addu  $t7, $t7, $v0
/* 0B390C 800B2D0C 81EF86AB */  lb    $t7, %lo((D_800E86AC - 1))($t7) # -0x7955($t7)
/* 0B3910 800B2D10 01826023 */  subu  $t4, $t4, $v0
/* 0B3914 800B2D14 000C6080 */  sll   $t4, $t4, 2
/* 0B3918 800B2D18 000F6880 */  sll   $t5, $t7, 2
/* 0B391C 800B2D1C 018DC021 */  addu  $t8, $t4, $t5
/* 0B3920 800B2D20 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3924 800B2D24 00781821 */  addu  $v1, $v1, $t8
/* 0B3928 800B2D28 100001E9 */  b     .L800B34D0
/* 0B392C 800B2D2C 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
glabel L800B2D30 # case 4
/* 0B3930 800B2D30 306E0400 */  andi  $t6, $v1, 0x400
/* 0B3934 800B2D34 11C00017 */  beqz  $t6, .L800B2D94
/* 0B3938 800B2D38 00603025 */   move  $a2, $v1
/* 0B393C 800B2D3C 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3940 800B2D40 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3944 800B2D44 3C09800F */  lui   $t1, %hi(D_800E86AC) # $t1, 0x800f
/* 0B3948 800B2D48 252986AC */  addiu $t1, %lo(D_800E86AC) # addiu $t1, $t1, -0x7954
/* 0B394C 800B2D4C 3C19800F */  lui   $t9, %hi((gPlayerModeSelection - 1)) # 0x800f
/* 0B3950 800B2D50 0322C821 */  addu  $t9, $t9, $v0
/* 0B3954 800B2D54 00491821 */  addu  $v1, $v0, $t1
/* 0B3958 800B2D58 8064FFFF */  lb    $a0, -1($v1)
/* 0B395C 800B2D5C 83392B5F */  lb    $t9, %lo((gPlayerModeSelection - 1))($t9) # 0x2b5f($t9)
/* 0B3960 800B2D60 248B0001 */  addiu $t3, $a0, 1
/* 0B3964 800B2D64 0099082A */  slt   $at, $a0, $t9
/* 0B3968 800B2D68 5020000B */  beql  $at, $zero, .L800B2D98
/* 0B396C 800B2D6C 30CA0800 */   andi  $t2, $a2, 0x800
/* 0B3970 800B2D70 A06BFFFF */  sb    $t3, -1($v1)
/* 0B3974 800B2D74 0C02D12B */  jal   func_800B44AC
/* 0B3978 800B2D78 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B397C 800B2D7C 3C044900 */  lui   $a0, (0x49008000 >> 16) # lui $a0, 0x4900
/* 0B3980 800B2D80 0C032384 */  jal   play_sound2
/* 0B3984 800B2D84 34848000 */   ori   $a0, (0x49008000 & 0xFFFF) # ori $a0, $a0, 0x8000
/* 0B3988 800B2D88 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B398C 800B2D8C 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B3990 800B2D90 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B2D94:
/* 0B3994 800B2D94 30CA0800 */  andi  $t2, $a2, 0x800
.L800B2D98:
/* 0B3998 800B2D98 11400011 */  beqz  $t2, .L800B2DE0
/* 0B399C 800B2D9C 3C0F8019 */   lui   $t7, %hi(gPlayerCount) # $t7, 0x8019
/* 0B39A0 800B2DA0 81EFEDF3 */  lb    $t7, %lo(gPlayerCount)($t7)
/* 0B39A4 800B2DA4 3C0C800F */  lui   $t4, %hi(D_800E86AC) # $t4, 0x800f
/* 0B39A8 800B2DA8 258C86AC */  addiu $t4, %lo(D_800E86AC) # addiu $t4, $t4, -0x7954
/* 0B39AC 800B2DAC 01EC1821 */  addu  $v1, $t7, $t4
/* 0B39B0 800B2DB0 8064FFFF */  lb    $a0, -1($v1)
/* 0B39B4 800B2DB4 1880000A */  blez  $a0, .L800B2DE0
/* 0B39B8 800B2DB8 248DFFFF */   addiu $t5, $a0, -1
/* 0B39BC 800B2DBC A06DFFFF */  sb    $t5, -1($v1)
/* 0B39C0 800B2DC0 0C02D12B */  jal   func_800B44AC
/* 0B39C4 800B2DC4 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B39C8 800B2DC8 3C044900 */  lui   $a0, (0x49008000 >> 16) # lui $a0, 0x4900
/* 0B39CC 800B2DCC 0C032384 */  jal   play_sound2
/* 0B39D0 800B2DD0 34848000 */   ori   $a0, (0x49008000 & 0xFFFF) # ori $a0, $a0, 0x8000
/* 0B39D4 800B2DD4 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B39D8 800B2DD8 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B39DC 800B2DDC 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B2DE0:
/* 0B39E0 800B2DE0 30D84000 */  andi  $t8, $a2, 0x4000
/* 0B39E4 800B2DE4 13000015 */  beqz  $t8, .L800B2E3C
/* 0B39E8 800B2DE8 3C028019 */   lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B39EC 800B2DEC 24040003 */  li    $a0, 3
/* 0B39F0 800B2DF0 0C02D12B */  jal   func_800B44AC
/* 0B39F4 800B2DF4 A1040000 */   sb    $a0, ($t0)
/* 0B39F8 800B2DF8 3C044900 */  lui   $a0, (0x49008002 >> 16) # lui $a0, 0x4900
/* 0B39FC 800B2DFC 0C032384 */  jal   play_sound2
/* 0B3A00 800B2E00 34848002 */   ori   $a0, (0x49008002 & 0xFFFF) # ori $a0, $a0, 0x8002
/* 0B3A04 800B2E04 3C0E8019 */  lui   $t6, %hi(gPlayerCount) # $t6, 0x8019
/* 0B3A08 800B2E08 81CEEDF3 */  lb    $t6, %lo(gPlayerCount)($t6)
/* 0B3A0C 800B2E0C 3C19800F */  lui   $t9, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3A10 800B2E10 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3A14 800B2E14 032EC821 */  addu  $t9, $t9, $t6
/* 0B3A18 800B2E18 833986AB */  lb    $t9, %lo((D_800E86AC - 1))($t9) # -0x7955($t9)
/* 0B3A1C 800B2E1C 000E4880 */  sll   $t1, $t6, 2
/* 0B3A20 800B2E20 012E4823 */  subu  $t1, $t1, $t6
/* 0B3A24 800B2E24 00094880 */  sll   $t1, $t1, 2
/* 0B3A28 800B2E28 00195880 */  sll   $t3, $t9, 2
/* 0B3A2C 800B2E2C 012B5021 */  addu  $t2, $t1, $t3
/* 0B3A30 800B2E30 006A1821 */  addu  $v1, $v1, $t2
/* 0B3A34 800B2E34 100001A6 */  b     .L800B34D0
/* 0B3A38 800B2E38 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B2E3C:
/* 0B3A3C 800B2E3C 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3A40 800B2E40 3C04800F */  lui   $a0, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3A44 800B2E44 30CF8000 */  andi  $t7, $a2, 0x8000
/* 0B3A48 800B2E48 00822021 */  addu  $a0, $a0, $v0
/* 0B3A4C 800B2E4C 11E0003F */  beqz  $t7, .L800B2F4C
/* 0B3A50 800B2E50 808486AB */   lb    $a0, %lo((D_800E86AC - 1))($a0) # -0x7955($a0)
/* 0B3A54 800B2E54 00026080 */  sll   $t4, $v0, 2
/* 0B3A58 800B2E58 01826023 */  subu  $t4, $t4, $v0
/* 0B3A5C 800B2E5C 000C6080 */  sll   $t4, $t4, 2
/* 0B3A60 800B2E60 00046880 */  sll   $t5, $a0, 2
/* 0B3A64 800B2E64 018DC021 */  addu  $t8, $t4, $t5
/* 0B3A68 800B2E68 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3A6C 800B2E6C 00781821 */  addu  $v1, $v1, $t8
/* 0B3A70 800B2E70 8C632B70 */  lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
/* 0B3A74 800B2E74 3C044900 */  lui   $a0, (0x4900900A >> 16) # lui $a0, 0x4900
/* 0B3A78 800B2E78 24190005 */  li    $t9, 5
/* 0B3A7C 800B2E7C 1060000D */  beqz  $v1, .L800B2EB4
/* 0B3A80 800B2E80 3484900A */   ori   $a0, (0x4900900A & 0xFFFF) # ori $a0, $a0, 0x900a
/* 0B3A84 800B2E84 24070001 */  li    $a3, 1
/* 0B3A88 800B2E88 10670014 */  beq   $v1, $a3, .L800B2EDC
/* 0B3A8C 800B2E8C 240B0005 */   li    $t3, 5
/* 0B3A90 800B2E90 24010002 */  li    $at, 2
/* 0B3A94 800B2E94 1061000B */  beq   $v1, $at, .L800B2EC4
/* 0B3A98 800B2E98 24090005 */   li    $t1, 5
/* 0B3A9C 800B2E9C 24040003 */  li    $a0, 3
/* 0B3AA0 800B2EA0 10640014 */  beq   $v1, $a0, .L800B2EF4
/* 0B3AA4 800B2EA4 240A0006 */   li    $t2, 6
/* 0B3AA8 800B2EA8 240E0006 */  li    $t6, 6
/* 0B3AAC 800B2EAC 10000015 */  b     .L800B2F04
/* 0B3AB0 800B2EB0 A10E0000 */   sb    $t6, ($t0)
.L800B2EB4: # case 0
/* 0B3AB4 800B2EB4 0C032384 */  jal   play_sound2
/* 0B3AB8 800B2EB8 A1190000 */   sb    $t9, ($t0)
/* 0B3ABC 800B2EBC 10000011 */  b     .L800B2F04
/* 0B3AC0 800B2EC0 00000000 */   nop   
.L800B2EC4:
/* 0B3AC4 800B2EC4 3C044900 */  lui   $a0, (0x4900900C >> 16) # lui $a0, 0x4900
/* 0B3AC8 800B2EC8 A1090000 */  sb    $t1, ($t0)
/* 0B3ACC 800B2ECC 0C032384 */  jal   play_sound2
/* 0B3AD0 800B2ED0 3484900C */   ori   $a0, (0x4900900C & 0xFFFF) # ori $a0, $a0, 0x900c
/* 0B3AD4 800B2ED4 1000000B */  b     .L800B2F04
/* 0B3AD8 800B2ED8 00000000 */   nop   
.L800B2EDC:
/* 0B3ADC 800B2EDC 3C044900 */  lui   $a0, (0x4900900B >> 16) # lui $a0, 0x4900
/* 0B3AE0 800B2EE0 A10B0000 */  sb    $t3, ($t0)
/* 0B3AE4 800B2EE4 0C032384 */  jal   play_sound2
/* 0B3AE8 800B2EE8 3484900B */   ori   $a0, (0x4900900B & 0xFFFF) # ori $a0, $a0, 0x900b
/* 0B3AEC 800B2EEC 10000005 */  b     .L800B2F04
/* 0B3AF0 800B2EF0 00000000 */   nop   
.L800B2EF4:
/* 0B3AF4 800B2EF4 3C044900 */  lui   $a0, (0x4900900D >> 16) # lui $a0, 0x4900
/* 0B3AF8 800B2EF8 A10A0000 */  sb    $t2, ($t0)
/* 0B3AFC 800B2EFC 0C032384 */  jal   play_sound2
/* 0B3B00 800B2F00 3484900D */   ori   $a0, (0x4900900D & 0xFFFF) # ori $a0, $a0, 0x900d
.L800B2F04:
/* 0B3B04 800B2F04 0C02D12B */  jal   func_800B44AC
/* 0B3B08 800B2F08 00000000 */   nop   
/* 0B3B0C 800B2F0C 3C0F8019 */  lui   $t7, %hi(gPlayerCount) # $t7, 0x8019
/* 0B3B10 800B2F10 81EFEDF3 */  lb    $t7, %lo(gPlayerCount)($t7)
/* 0B3B14 800B2F14 3C0D800F */  lui   $t5, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3B18 800B2F18 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3B1C 800B2F1C 01AF6821 */  addu  $t5, $t5, $t7
/* 0B3B20 800B2F20 81AD86AB */  lb    $t5, %lo((D_800E86AC - 1))($t5) # -0x7955($t5)
/* 0B3B24 800B2F24 000F6080 */  sll   $t4, $t7, 2
/* 0B3B28 800B2F28 018F6023 */  subu  $t4, $t4, $t7
/* 0B3B2C 800B2F2C 000C6080 */  sll   $t4, $t4, 2
/* 0B3B30 800B2F30 000DC080 */  sll   $t8, $t5, 2
/* 0B3B34 800B2F34 01987021 */  addu  $t6, $t4, $t8
/* 0B3B38 800B2F38 3C018019 */  lui   $at, %hi(gMenuTimingCounter) # $at, 0x8019
/* 0B3B3C 800B2F3C 006E1821 */  addu  $v1, $v1, $t6
/* 0B3B40 800B2F40 AC20EE00 */  sw    $zero, %lo(gMenuTimingCounter)($at)
/* 0B3B44 800B2F44 10000162 */  b     .L800B34D0
/* 0B3B48 800B2F48 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B2F4C:
/* 0B3B4C 800B2F4C 0002C880 */  sll   $t9, $v0, 2
/* 0B3B50 800B2F50 0322C823 */  subu  $t9, $t9, $v0
/* 0B3B54 800B2F54 0019C880 */  sll   $t9, $t9, 2
/* 0B3B58 800B2F58 00044880 */  sll   $t1, $a0, 2
/* 0B3B5C 800B2F5C 03295821 */  addu  $t3, $t9, $t1
/* 0B3B60 800B2F60 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3B64 800B2F64 006B1821 */  addu  $v1, $v1, $t3
/* 0B3B68 800B2F68 10000159 */  b     .L800B34D0
/* 0B3B6C 800B2F6C 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
glabel L800B2F70 # case 5, 8
/* 0B3B70 800B2F70 14A00023 */  bnez  $a1, .L800B3000
/* 0B3B74 800B2F74 00603025 */   move  $a2, $v1
/* 0B3B78 800B2F78 3C0A8019 */  lui   $t2, %hi(gMenuTimingCounter) # $t2, 0x8019
/* 0B3B7C 800B2F7C 8D4AEE00 */  lw    $t2, %lo(gMenuTimingCounter)($t2)
/* 0B3B80 800B2F80 3C018019 */  lui   $at, %hi(gMenuTimingCounter) # $at, 0x8019
/* 0B3B84 800B2F84 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3B88 800B2F88 254F0001 */  addiu $t7, $t2, 1
/* 0B3B8C 800B2F8C AC2FEE00 */  sw    $t7, %lo(gMenuTimingCounter)($at)
/* 0B3B90 800B2F90 24010064 */  li    $at, 100
/* 0B3B94 800B2F94 11E10005 */  beq   $t7, $at, .L800B2FAC
/* 0B3B98 800B2F98 2401012C */   li    $at, 300
/* 0B3B9C 800B2F9C 01E1001A */  div   $zero, $t7, $at
/* 0B3BA0 800B2FA0 00006810 */  mfhi  $t5
/* 0B3BA4 800B2FA4 15A00016 */  bnez  $t5, .L800B3000
/* 0B3BA8 800B2FA8 00000000 */   nop   
.L800B2FAC:
/* 0B3BAC 800B2FAC 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3BB0 800B2FB0 3C18800F */  lui   $t8, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3BB4 800B2FB4 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3BB8 800B2FB8 0302C021 */  addu  $t8, $t8, $v0
/* 0B3BBC 800B2FBC 831886AB */  lb    $t8, %lo((D_800E86AC - 1))($t8) # -0x7955($t8)
/* 0B3BC0 800B2FC0 00026080 */  sll   $t4, $v0, 2
/* 0B3BC4 800B2FC4 01826023 */  subu  $t4, $t4, $v0
/* 0B3BC8 800B2FC8 000C6080 */  sll   $t4, $t4, 2
/* 0B3BCC 800B2FCC 00187080 */  sll   $t6, $t8, 2
/* 0B3BD0 800B2FD0 018EC821 */  addu  $t9, $t4, $t6
/* 0B3BD4 800B2FD4 00791821 */  addu  $v1, $v1, $t9
/* 0B3BD8 800B2FD8 8C632B70 */  lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
/* 0B3BDC 800B2FDC 3C044900 */  lui   $a0, (0x4900900E >> 16) # lui $a0, 0x4900
/* 0B3BE0 800B2FE0 24010002 */  li    $at, 2
/* 0B3BE4 800B2FE4 10600003 */  beqz  $v1, .L800B2FF4
/* 0B3BE8 800B2FE8 3484900E */   ori   $a0, (0x4900900E & 0xFFFF) # ori $a0, $a0, 0x900e
/* 0B3BEC 800B2FEC 14610004 */  bne   $v1, $at, .L800B3000
/* 0B3BF0 800B2FF0 00000000 */   nop   
.L800B2FF4:
/* 0B3BF4 800B2FF4 0C032384 */  jal   play_sound2
/* 0B3BF8 800B2FF8 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B3BFC 800B2FFC 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B3000:
/* 0B3C00 800B3000 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3C04 800B3004 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3C08 800B3008 3C0B800F */  lui   $t3, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3C0C 800B300C 3C0F800F */  lui   $t7, %hi(D_800E86B0) # $t7, 0x800f
/* 0B3C10 800B3010 01625821 */  addu  $t3, $t3, $v0
/* 0B3C14 800B3014 816B86AB */  lb    $t3, %lo((D_800E86AC - 1))($t3) # -0x7955($t3)
/* 0B3C18 800B3018 00024880 */  sll   $t1, $v0, 2
/* 0B3C1C 800B301C 01224823 */  subu  $t1, $t1, $v0
/* 0B3C20 800B3020 25EF86B0 */  addiu $t7, %lo(D_800E86B0) # addiu $t7, $t7, -0x7950
/* 0B3C24 800B3024 012B5021 */  addu  $t2, $t1, $t3
/* 0B3C28 800B3028 014F1821 */  addu  $v1, $t2, $t7
/* 0B3C2C 800B302C 8064FFFD */  lb    $a0, -3($v1)
/* 0B3C30 800B3030 30CD0800 */  andi  $t5, $a2, 0x800
/* 0B3C34 800B3034 11A0000C */  beqz  $t5, .L800B3068
/* 0B3C38 800B3038 00802825 */   move  $a1, $a0
/* 0B3C3C 800B303C 1880000A */  blez  $a0, .L800B3068
/* 0B3C40 800B3040 2498FFFF */   addiu $t8, $a0, -1
/* 0B3C44 800B3044 A078FFFD */  sb    $t8, -3($v1)
/* 0B3C48 800B3048 AFA40028 */  sw    $a0, 0x28($sp)
/* 0B3C4C 800B304C 0C02D12B */  jal   func_800B44AC
/* 0B3C50 800B3050 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B3C54 800B3054 3C044900 */  lui   $a0, (0x49008000 >> 16) # lui $a0, 0x4900
/* 0B3C58 800B3058 0C032384 */  jal   play_sound2
/* 0B3C5C 800B305C 34848000 */   ori   $a0, (0x49008000 & 0xFFFF) # ori $a0, $a0, 0x8000
/* 0B3C60 800B3060 8FA50028 */  lw    $a1, 0x28($sp)
/* 0B3C64 800B3064 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B3068:
/* 0B3C68 800B3068 30CC0400 */  andi  $t4, $a2, 0x400
/* 0B3C6C 800B306C 11800038 */  beqz  $t4, .L800B3150
/* 0B3C70 800B3070 00000000 */   nop   
/* 0B3C74 800B3074 AFA00024 */  sw    $zero, 0x24($sp)
/* 0B3C78 800B3078 AFA50028 */  sw    $a1, 0x28($sp)
/* 0B3C7C 800B307C 0C02D557 */  jal   func_800B555C
/* 0B3C80 800B3080 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B3C84 800B3084 8FA30024 */  lw    $v1, 0x24($sp)
/* 0B3C88 800B3088 8FA50028 */  lw    $a1, 0x28($sp)
/* 0B3C8C 800B308C 10400011 */  beqz  $v0, .L800B30D4
/* 0B3C90 800B3090 8FA6001C */   lw    $a2, 0x1c($sp)
/* 0B3C94 800B3094 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3C98 800B3098 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3C9C 800B309C 3C04800F */  lui   $a0, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3CA0 800B30A0 3C09800F */  lui   $t1, %hi((gGameModePlayerColumnExtra - 3)) # 0x800f
/* 0B3CA4 800B30A4 00822021 */  addu  $a0, $a0, $v0
/* 0B3CA8 800B30A8 808486AB */  lb    $a0, %lo((D_800E86AC - 1))($a0) # -0x7955($a0)
/* 0B3CAC 800B30AC 00027080 */  sll   $t6, $v0, 2
/* 0B3CB0 800B30B0 01C27023 */  subu  $t6, $t6, $v0
/* 0B3CB4 800B30B4 01C4C821 */  addu  $t9, $t6, $a0
/* 0B3CB8 800B30B8 01394821 */  addu  $t1, $t1, $t9
/* 0B3CBC 800B30BC 81292B6D */  lb    $t1, %lo((gGameModePlayerColumnExtra - 3))($t1) # 0x2b6d($t1)
/* 0B3CC0 800B30C0 00A9082A */  slt   $at, $a1, $t1
/* 0B3CC4 800B30C4 10200012 */  beqz  $at, .L800B3110
/* 0B3CC8 800B30C8 00000000 */   nop   
/* 0B3CCC 800B30CC 10000010 */  b     .L800B3110
/* 0B3CD0 800B30D0 24030001 */   li    $v1, 1
.L800B30D4:
/* 0B3CD4 800B30D4 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3CD8 800B30D8 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3CDC 800B30DC 3C04800F */  lui   $a0, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3CE0 800B30E0 3C0F800F */  lui   $t7, %hi((gGameModePlayerColumnDefault - 3)) # 0x800f
/* 0B3CE4 800B30E4 00822021 */  addu  $a0, $a0, $v0
/* 0B3CE8 800B30E8 808486AB */  lb    $a0, %lo((D_800E86AC - 1))($a0) # -0x7955($a0)
/* 0B3CEC 800B30EC 00025880 */  sll   $t3, $v0, 2
/* 0B3CF0 800B30F0 01625823 */  subu  $t3, $t3, $v0
/* 0B3CF4 800B30F4 01645021 */  addu  $t2, $t3, $a0
/* 0B3CF8 800B30F8 01EA7821 */  addu  $t7, $t7, $t2
/* 0B3CFC 800B30FC 81EF2B61 */  lb    $t7, %lo((gGameModePlayerColumnDefault - 3))($t7) # 0x2b61($t7)
/* 0B3D00 800B3100 00AF082A */  slt   $at, $a1, $t7
/* 0B3D04 800B3104 10200002 */  beqz  $at, .L800B3110
/* 0B3D08 800B3108 00000000 */   nop   
/* 0B3D0C 800B310C 24030001 */  li    $v1, 1
.L800B3110:
/* 0B3D10 800B3110 1060000F */  beqz  $v1, .L800B3150
/* 0B3D14 800B3114 00026880 */   sll   $t5, $v0, 2
/* 0B3D18 800B3118 01A26823 */  subu  $t5, $t5, $v0
/* 0B3D1C 800B311C 3C0C800F */  lui   $t4, %hi(D_800E86B0) # $t4, 0x800f
/* 0B3D20 800B3120 258C86B0 */  addiu $t4, %lo(D_800E86B0) # addiu $t4, $t4, -0x7950
/* 0B3D24 800B3124 01A4C021 */  addu  $t8, $t5, $a0
/* 0B3D28 800B3128 030C1821 */  addu  $v1, $t8, $t4
/* 0B3D2C 800B312C 806EFFFD */  lb    $t6, -3($v1)
/* 0B3D30 800B3130 AFA6001C */  sw    $a2, 0x1c($sp)
/* 0B3D34 800B3134 25D90001 */  addiu $t9, $t6, 1
/* 0B3D38 800B3138 0C02D12B */  jal   func_800B44AC
/* 0B3D3C 800B313C A079FFFD */   sb    $t9, -3($v1)
/* 0B3D40 800B3140 3C044900 */  lui   $a0, (0x49008000 >> 16) # lui $a0, 0x4900
/* 0B3D44 800B3144 0C032384 */  jal   play_sound2
/* 0B3D48 800B3148 34848000 */   ori   $a0, (0x49008000 & 0xFFFF) # ori $a0, $a0, 0x8000
/* 0B3D4C 800B314C 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B3150:
/* 0B3D50 800B3150 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3D54 800B3154 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3D58 800B3158 3C04800F */  lui   $a0, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3D5C 800B315C 3C05800F */  lui   $a1, %hi((D_800E86B0 - 3)) # 0x800f
/* 0B3D60 800B3160 00822021 */  addu  $a0, $a0, $v0
/* 0B3D64 800B3164 808486AB */  lb    $a0, %lo((D_800E86AC - 1))($a0) # -0x7955($a0)
/* 0B3D68 800B3168 00024880 */  sll   $t1, $v0, 2
/* 0B3D6C 800B316C 01224823 */  subu  $t1, $t1, $v0
/* 0B3D70 800B3170 01245821 */  addu  $t3, $t1, $a0
/* 0B3D74 800B3174 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B3D78 800B3178 00AB2821 */  addu  $a1, $a1, $t3
/* 0B3D7C 800B317C 30CA4000 */  andi  $t2, $a2, 0x4000
/* 0B3D80 800B3180 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B3D84 800B3184 11400015 */  beqz  $t2, .L800B31DC
/* 0B3D88 800B3188 80A586AD */   lb    $a1, %lo((D_800E86B0 - 3))($a1) # -0x7953($a1)
/* 0B3D8C 800B318C 240F0004 */  li    $t7, 4
/* 0B3D90 800B3190 0C02D12B */  jal   func_800B44AC
/* 0B3D94 800B3194 A10F0000 */   sb    $t7, ($t0)
/* 0B3D98 800B3198 3C044900 */  lui   $a0, (0x49008002 >> 16) # lui $a0, 0x4900
/* 0B3D9C 800B319C 0C032384 */  jal   play_sound2
/* 0B3DA0 800B31A0 34848002 */   ori   $a0, (0x49008002 & 0xFFFF) # ori $a0, $a0, 0x8002
/* 0B3DA4 800B31A4 3C0D8019 */  lui   $t5, %hi(gPlayerCount) # $t5, 0x8019
/* 0B3DA8 800B31A8 81ADEDF3 */  lb    $t5, %lo(gPlayerCount)($t5)
/* 0B3DAC 800B31AC 3C0C800F */  lui   $t4, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3DB0 800B31B0 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3DB4 800B31B4 018D6021 */  addu  $t4, $t4, $t5
/* 0B3DB8 800B31B8 818C86AB */  lb    $t4, %lo((D_800E86AC - 1))($t4) # -0x7955($t4)
/* 0B3DBC 800B31BC 000DC080 */  sll   $t8, $t5, 2
/* 0B3DC0 800B31C0 030DC023 */  subu  $t8, $t8, $t5
/* 0B3DC4 800B31C4 0018C080 */  sll   $t8, $t8, 2
/* 0B3DC8 800B31C8 000C7080 */  sll   $t6, $t4, 2
/* 0B3DCC 800B31CC 030EC821 */  addu  $t9, $t8, $t6
/* 0B3DD0 800B31D0 00791821 */  addu  $v1, $v1, $t9
/* 0B3DD4 800B31D4 100000BE */  b     .L800B34D0
/* 0B3DD8 800B31D8 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B31DC:
/* 0B3DDC 800B31DC 30C98000 */  andi  $t1, $a2, 0x8000
/* 0B3DE0 800B31E0 1120002C */  beqz  $t1, .L800B3294
/* 0B3DE4 800B31E4 0002C880 */   sll   $t9, $v0, 2
/* 0B3DE8 800B31E8 0C02D12B */  jal   func_800B44AC
/* 0B3DEC 800B31EC AFA50028 */   sw    $a1, 0x28($sp)
/* 0B3DF0 800B31F0 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3DF4 800B31F4 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3DF8 800B31F8 24070001 */  li    $a3, 1
/* 0B3DFC 800B31FC 8FA50028 */  lw    $a1, 0x28($sp)
/* 0B3E00 800B3200 14E2000E */  bne   $a3, $v0, .L800B323C
/* 0B3E04 800B3204 3C0B800F */   lui   $t3, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3E08 800B3208 01625821 */  addu  $t3, $t3, $v0
/* 0B3E0C 800B320C 816B86AB */  lb    $t3, %lo((D_800E86AC - 1))($t3) # -0x7955($t3)
/* 0B3E10 800B3210 54EB000B */  bnel  $a3, $t3, .L800B3240
/* 0B3E14 800B3214 240A0006 */   li    $t2, 6
/* 0B3E18 800B3218 54A70009 */  bnel  $a1, $a3, .L800B3240
/* 0B3E1C 800B321C 240A0006 */   li    $t2, 6
/* 0B3E20 800B3220 0C027896 */  jal   func_8009E258
/* 0B3E24 800B3224 00000000 */   nop   
/* 0B3E28 800B3228 3C044900 */  lui   $a0, (0x49009011 >> 16) # lui $a0, 0x4900
/* 0B3E2C 800B322C 0C032384 */  jal   play_sound2
/* 0B3E30 800B3230 34849011 */   ori   $a0, (0x49009011 & 0xFFFF) # ori $a0, $a0, 0x9011
/* 0B3E34 800B3234 10000009 */  b     .L800B325C
/* 0B3E38 800B3238 00000000 */   nop   
.L800B323C:
/* 0B3E3C 800B323C 240A0006 */  li    $t2, 6
.L800B3240:
/* 0B3E40 800B3240 3C018019 */  lui   $at, %hi(gMainMenuSelectionDepth) # $at, 0x8019
/* 0B3E44 800B3244 3C044900 */  lui   $a0, (0x49008001 >> 16) # lui $a0, 0x4900
/* 0B3E48 800B3248 A02AEDED */  sb    $t2, %lo(gMainMenuSelectionDepth)($at)
/* 0B3E4C 800B324C 0C032384 */  jal   play_sound2
/* 0B3E50 800B3250 34848001 */   ori   $a0, (0x49008001 & 0xFFFF) # ori $a0, $a0, 0x8001
/* 0B3E54 800B3254 3C018019 */  lui   $at, %hi(gMenuTimingCounter) # $at, 0x8019
/* 0B3E58 800B3258 AC20EE00 */  sw    $zero, %lo(gMenuTimingCounter)($at)
.L800B325C:
/* 0B3E5C 800B325C 3C0F8019 */  lui   $t7, %hi(gPlayerCount) # $t7, 0x8019
/* 0B3E60 800B3260 81EFEDF3 */  lb    $t7, %lo(gPlayerCount)($t7)
/* 0B3E64 800B3264 3C0C800F */  lui   $t4, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3E68 800B3268 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3E6C 800B326C 018F6021 */  addu  $t4, $t4, $t7
/* 0B3E70 800B3270 818C86AB */  lb    $t4, %lo((D_800E86AC - 1))($t4) # -0x7955($t4)
/* 0B3E74 800B3274 000F6880 */  sll   $t5, $t7, 2
/* 0B3E78 800B3278 01AF6823 */  subu  $t5, $t5, $t7
/* 0B3E7C 800B327C 000D6880 */  sll   $t5, $t5, 2
/* 0B3E80 800B3280 000CC080 */  sll   $t8, $t4, 2
/* 0B3E84 800B3284 01B87021 */  addu  $t6, $t5, $t8
/* 0B3E88 800B3288 006E1821 */  addu  $v1, $v1, $t6
/* 0B3E8C 800B328C 10000090 */  b     .L800B34D0
/* 0B3E90 800B3290 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B3294:
/* 0B3E94 800B3294 0322C823 */  subu  $t9, $t9, $v0
/* 0B3E98 800B3298 0019C880 */  sll   $t9, $t9, 2
/* 0B3E9C 800B329C 00044880 */  sll   $t1, $a0, 2
/* 0B3EA0 800B32A0 03295821 */  addu  $t3, $t9, $t1
/* 0B3EA4 800B32A4 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3EA8 800B32A8 006B1821 */  addu  $v1, $v1, $t3
/* 0B3EAC 800B32AC 10000088 */  b     .L800B34D0
/* 0B3EB0 800B32B0 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
glabel L800B32B4 # case 6, 7
/* 0B3EB4 800B32B4 14A00014 */  bnez  $a1, .L800B3308
/* 0B3EB8 800B32B8 00603025 */   move  $a2, $v1
/* 0B3EBC 800B32BC 3C0A8019 */  lui   $t2, %hi(gMenuTimingCounter) # $t2, 0x8019
/* 0B3EC0 800B32C0 8D4AEE00 */  lw    $t2, %lo(gMenuTimingCounter)($t2)
/* 0B3EC4 800B32C4 3C018019 */  lui   $at, %hi(gMenuTimingCounter) # $at, 0x8019
/* 0B3EC8 800B32C8 3C044900 */  lui   $a0, (0x4900900F >> 16) # lui $a0, 0x4900
/* 0B3ECC 800B32CC 254F0001 */  addiu $t7, $t2, 1
/* 0B3ED0 800B32D0 AC2FEE00 */  sw    $t7, %lo(gMenuTimingCounter)($at)
/* 0B3ED4 800B32D4 2401003C */  li    $at, 60
/* 0B3ED8 800B32D8 11E10006 */  beq   $t7, $at, .L800B32F4
/* 0B3EDC 800B32DC 3484900F */   ori   $a0, (0x4900900F & 0xFFFF) # ori $a0, $a0, 0x900f
/* 0B3EE0 800B32E0 2401012C */  li    $at, 300
/* 0B3EE4 800B32E4 01E1001A */  div   $zero, $t7, $at
/* 0B3EE8 800B32E8 00006010 */  mfhi  $t4
/* 0B3EEC 800B32EC 55800007 */  bnel  $t4, $zero, .L800B330C
/* 0B3EF0 800B32F0 30CD4000 */   andi  $t5, $a2, 0x4000
.L800B32F4:
/* 0B3EF4 800B32F4 0C032384 */  jal   play_sound2
/* 0B3EF8 800B32F8 AFA6001C */   sw    $a2, 0x1c($sp)
/* 0B3EFC 800B32FC 3C088019 */  lui   $t0, %hi(gMainMenuSelectionDepth) # $t0, 0x8019
/* 0B3F00 800B3300 2508EDED */  addiu $t0, %lo(gMainMenuSelectionDepth) # addiu $t0, $t0, -0x1213
/* 0B3F04 800B3304 8FA6001C */  lw    $a2, 0x1c($sp)
.L800B3308:
/* 0B3F08 800B3308 30CD4000 */  andi  $t5, $a2, 0x4000
.L800B330C:
/* 0B3F0C 800B330C 11A00032 */  beqz  $t5, .L800B33D8
/* 0B3F10 800B3310 30D98000 */   andi  $t9, $a2, 0x8000
/* 0B3F14 800B3314 3C028019 */  lui   $v0, %hi(gPlayerCount) # $v0, 0x8019
/* 0B3F18 800B3318 8042EDF3 */  lb    $v0, %lo(gPlayerCount)($v0)
/* 0B3F1C 800B331C 3C0E800F */  lui   $t6, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3F20 800B3320 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3F24 800B3324 01C27021 */  addu  $t6, $t6, $v0
/* 0B3F28 800B3328 81CE86AB */  lb    $t6, %lo((D_800E86AC - 1))($t6) # -0x7955($t6)
/* 0B3F2C 800B332C 0002C080 */  sll   $t8, $v0, 2
/* 0B3F30 800B3330 0302C023 */  subu  $t8, $t8, $v0
/* 0B3F34 800B3334 0018C080 */  sll   $t8, $t8, 2
/* 0B3F38 800B3338 000EC880 */  sll   $t9, $t6, 2
/* 0B3F3C 800B333C 03194821 */  addu  $t1, $t8, $t9
/* 0B3F40 800B3340 00691821 */  addu  $v1, $v1, $t1
/* 0B3F44 800B3344 8C632B70 */  lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
/* 0B3F48 800B3348 24070001 */  li    $a3, 1
/* 0B3F4C 800B334C 240B0005 */  li    $t3, 5
/* 0B3F50 800B3350 10600009 */  beqz  $v1, .L800B3378
/* 0B3F54 800B3354 00000000 */   nop   
/* 0B3F58 800B3358 10670007 */  beq   $v1, $a3, .L800B3378
/* 0B3F5C 800B335C 24010002 */   li    $at, 2
/* 0B3F60 800B3360 10610005 */  beq   $v1, $at, .L800B3378
/* 0B3F64 800B3364 24040003 */   li    $a0, 3
/* 0B3F68 800B3368 10640005 */  beq   $v1, $a0, .L800B3380
/* 0B3F6C 800B336C 240A0004 */   li    $t2, 4
/* 0B3F70 800B3370 10000004 */  b     .L800B3384
/* 0B3F74 800B3374 A10A0000 */   sb    $t2, ($t0)
.L800B3378:
/* 0B3F78 800B3378 10000002 */  b     .L800B3384
/* 0B3F7C 800B337C A10B0000 */   sb    $t3, ($t0)
.L800B3380:
/* 0B3F80 800B3380 A10A0000 */  sb    $t2, ($t0)
.L800B3384:
/* 0B3F84 800B3384 0C02D12B */  jal   func_800B44AC
/* 0B3F88 800B3388 00000000 */   nop   
/* 0B3F8C 800B338C 3C044900 */  lui   $a0, (0x49008002 >> 16) # lui $a0, 0x4900
/* 0B3F90 800B3390 0C032384 */  jal   play_sound2
/* 0B3F94 800B3394 34848002 */   ori   $a0, (0x49008002 & 0xFFFF) # ori $a0, $a0, 0x8002
/* 0B3F98 800B3398 3C0F8019 */  lui   $t7, %hi(gPlayerCount) # $t7, 0x8019
/* 0B3F9C 800B339C 81EFEDF3 */  lb    $t7, %lo(gPlayerCount)($t7)
/* 0B3FA0 800B33A0 3C0D800F */  lui   $t5, %hi((D_800E86AC - 1)) # 0x800f
/* 0B3FA4 800B33A4 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B3FA8 800B33A8 01AF6821 */  addu  $t5, $t5, $t7
/* 0B3FAC 800B33AC 81AD86AB */  lb    $t5, %lo((D_800E86AC - 1))($t5) # -0x7955($t5)
/* 0B3FB0 800B33B0 000F6080 */  sll   $t4, $t7, 2
/* 0B3FB4 800B33B4 018F6023 */  subu  $t4, $t4, $t7
/* 0B3FB8 800B33B8 000C6080 */  sll   $t4, $t4, 2
/* 0B3FBC 800B33BC 000D7080 */  sll   $t6, $t5, 2
/* 0B3FC0 800B33C0 018EC021 */  addu  $t8, $t4, $t6
/* 0B3FC4 800B33C4 3C018019 */  lui   $at, %hi(gMenuTimingCounter) # $at, 0x8019
/* 0B3FC8 800B33C8 00781821 */  addu  $v1, $v1, $t8
/* 0B3FCC 800B33CC AC20EE00 */  sw    $zero, %lo(gMenuTimingCounter)($at)
/* 0B3FD0 800B33D0 1000003F */  b     .L800B34D0
/* 0B3FD4 800B33D4 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B33D8:
/* 0B3FD8 800B33D8 13200016 */  beqz  $t9, .L800B3434
/* 0B3FDC 800B33DC 3C0C8019 */   lui   $t4, %hi(gPlayerCount) # $t4, 0x8019
/* 0B3FE0 800B33E0 0C027870 */  jal   func_8009E1C0
/* 0B3FE4 800B33E4 00000000 */   nop   
/* 0B3FE8 800B33E8 3C044900 */  lui   $a0, (0x49008016 >> 16) # lui $a0, 0x4900
/* 0B3FEC 800B33EC 0C032384 */  jal   play_sound2
/* 0B3FF0 800B33F0 34848016 */   ori   $a0, (0x49008016 & 0xFFFF) # ori $a0, $a0, 0x8016
/* 0B3FF4 800B33F4 0C02CA32 */  jal   func_800B28C8
/* 0B3FF8 800B33F8 00000000 */   nop   
/* 0B3FFC 800B33FC 3C098019 */  lui   $t1, %hi(gPlayerCount) # $t1, 0x8019
/* 0B4000 800B3400 8129EDF3 */  lb    $t1, %lo(gPlayerCount)($t1)
/* 0B4004 800B3404 3C0A800F */  lui   $t2, %hi((D_800E86AC - 1)) # 0x800f
/* 0B4008 800B3408 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B400C 800B340C 01495021 */  addu  $t2, $t2, $t1
/* 0B4010 800B3410 814A86AB */  lb    $t2, %lo((D_800E86AC - 1))($t2) # -0x7955($t2)
/* 0B4014 800B3414 00095880 */  sll   $t3, $t1, 2
/* 0B4018 800B3418 01695823 */  subu  $t3, $t3, $t1
/* 0B401C 800B341C 000B5880 */  sll   $t3, $t3, 2
/* 0B4020 800B3420 000A7880 */  sll   $t7, $t2, 2
/* 0B4024 800B3424 016F6821 */  addu  $t5, $t3, $t7
/* 0B4028 800B3428 006D1821 */  addu  $v1, $v1, $t5
/* 0B402C 800B342C 10000028 */  b     .L800B34D0
/* 0B4030 800B3430 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B3434:
/* 0B4034 800B3434 818CEDF3 */  lb    $t4, %lo(gPlayerCount)($t4)
/* 0B4038 800B3438 3C18800F */  lui   $t8, %hi((D_800E86AC - 1)) # 0x800f
/* 0B403C 800B343C 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B4040 800B3440 030CC021 */  addu  $t8, $t8, $t4
/* 0B4044 800B3444 831886AB */  lb    $t8, %lo((D_800E86AC - 1))($t8) # -0x7955($t8)
/* 0B4048 800B3448 000C7080 */  sll   $t6, $t4, 2
/* 0B404C 800B344C 01CC7023 */  subu  $t6, $t6, $t4
/* 0B4050 800B3450 000E7080 */  sll   $t6, $t6, 2
/* 0B4054 800B3454 0018C880 */  sll   $t9, $t8, 2
/* 0B4058 800B3458 01D94821 */  addu  $t1, $t6, $t9
/* 0B405C 800B345C 00691821 */  addu  $v1, $v1, $t1
/* 0B4060 800B3460 1000001B */  b     .L800B34D0
/* 0B4064 800B3464 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
glabel L800B3468 # case 1, 2
/* 0B4068 800B3468 3C0A8019 */  lui   $t2, %hi(gPlayerCount) # $t2, 0x8019
/* 0B406C 800B346C 814AEDF3 */  lb    $t2, %lo(gPlayerCount)($t2)
/* 0B4070 800B3470 3C0F800F */  lui   $t7, %hi((D_800E86AC - 1)) # 0x800f
/* 0B4074 800B3474 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B4078 800B3478 01EA7821 */  addu  $t7, $t7, $t2
/* 0B407C 800B347C 81EF86AB */  lb    $t7, %lo((D_800E86AC - 1))($t7) # -0x7955($t7)
/* 0B4080 800B3480 000A5880 */  sll   $t3, $t2, 2
/* 0B4084 800B3484 016A5823 */  subu  $t3, $t3, $t2
/* 0B4088 800B3488 000B5880 */  sll   $t3, $t3, 2
/* 0B408C 800B348C 000F6880 */  sll   $t5, $t7, 2
/* 0B4090 800B3490 016D6021 */  addu  $t4, $t3, $t5
/* 0B4094 800B3494 006C1821 */  addu  $v1, $v1, $t4
/* 0B4098 800B3498 1000000D */  b     .L800B34D0
/* 0B409C 800B349C 8C632B70 */   lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B34A0: # default
/* 0B40A0 800B34A0 8318EDF3 */  lb    $t8, %lo(gPlayerCount)($t8) # -0x120d($t8)
/* 0B40A4 800B34A4 3C19800F */  lui   $t9, %hi((D_800E86AC - 1)) # 0x800f
/* 0B40A8 800B34A8 3C03800F */  lui   $v1, %hi((gGameModePlayerSelection - 0xC)) # 0x800f
/* 0B40AC 800B34AC 0338C821 */  addu  $t9, $t9, $t8
/* 0B40B0 800B34B0 833986AB */  lb    $t9, %lo((D_800E86AC - 1))($t9) # -0x7955($t9)
/* 0B40B4 800B34B4 00187080 */  sll   $t6, $t8, 2
/* 0B40B8 800B34B8 01D87023 */  subu  $t6, $t6, $t8
/* 0B40BC 800B34BC 000E7080 */  sll   $t6, $t6, 2
/* 0B40C0 800B34C0 00194880 */  sll   $t1, $t9, 2
/* 0B40C4 800B34C4 01C95021 */  addu  $t2, $t6, $t1
/* 0B40C8 800B34C8 006A1821 */  addu  $v1, $v1, $t2
/* 0B40CC 800B34CC 8C632B70 */  lw    $v1, %lo((gGameModePlayerSelection - 0xC))($v1) # 0x2b70($v1)
.L800B34D0:
/* 0B40D0 800B34D0 3C01800E */  lui   $at, %hi(gModeSelection) # $at, 0x800e
/* 0B40D4 800B34D4 AC23C53C */  sw    $v1, %lo(gModeSelection)($at)
.L800B34D8:
/* 0B40D8 800B34D8 8FBF0014 */  lw    $ra, 0x14($sp)
/* 0B40DC 800B34DC 27BD0030 */  addiu $sp, $sp, 0x30
/* 0B40E0 800B34E0 03E00008 */  jr    $ra
/* 0B40E4 800B34E4 00000000 */   nop   
