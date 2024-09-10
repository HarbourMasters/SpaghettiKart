.section .late_rodata

glabel D_800F2EA0
.float 20.0304203

glabel D_800F2EA4
.float 16.546

glabel D_800F2EA8
.float 16.713

.section .text

glabel audio_init
/* 0BCCDC 800BC0DC 27BDFF70 */  addiu $sp, $sp, -0x90
/* 0BCCE0 800BC0E0 3C0E800F */  lui   $t6, %hi(gAudioLoadLock) # $t6, 0x800f
/* 0BCCE4 800BC0E4 AFBF002C */  sw    $ra, 0x2c($sp)
/* 0BCCE8 800BC0E8 AFB50028 */  sw    $s5, 0x28($sp)
/* 0BCCEC 800BC0EC AFB40024 */  sw    $s4, 0x24($sp)
/* 0BCCF0 800BC0F0 AFB30020 */  sw    $s3, 0x20($sp)
/* 0BCCF4 800BC0F4 AFB2001C */  sw    $s2, 0x1c($sp)
/* 0BCCF8 800BC0F8 AFB10018 */  sw    $s1, 0x18($sp)
/* 0BCCFC 800BC0FC AFB00014 */  sw    $s0, 0x14($sp)
/* 0BCD00 800BC100 25CEA5DC */  addiu $t6, %lo(gAudioLoadLock) # addiu $t6, $t6, -0x5a24
/* 0BCD04 800BC104 ADC00000 */  sw    $zero, ($t6)
/* 0BCD08 800BC108 3C04800F */  lui   $a0, %hi(gAudioHeapSize) # $a0, 0x800f
/* 0BCD0C 800BC10C 8C84A5D0 */  lw    $a0, %lo(gAudioHeapSize)($a0)
/* 0BCD10 800BC110 00001825 */  move  $v1, $zero
/* 0BCD14 800BC114 3C0A803B */  lui   $t2, %hi(gAudioGlobalsEndMarker) # $t2, 0x803b
/* 0BCD18 800BC118 04810003 */  bgez  $a0, .L800BC128
/* 0BCD1C 800BC11C 000478C3 */   sra   $t7, $a0, 3
/* 0BCD20 800BC120 24810007 */  addiu $at, $a0, 7
/* 0BCD24 800BC124 000178C3 */  sra   $t7, $at, 3
.L800BC128:
/* 0BCD28 800BC128 19E0000C */  blez  $t7, .L800BC15C
/* 0BCD2C 800BC12C 3C04803B */   lui   $a0, %hi(gGfxSPTaskOutputBufferSize) # 0x803b
/* 0BCD30 800BC130 3C18803B */  lui   $t8, %hi(gAudioHeap) # 0x803b
/* 0BCD34 800BC134 270271B0 */  addiu $v0, $t8, %lo(gAudioHeap) # 0x71b0
/* 0BCD38 800BC138 000FC8C0 */  sll   $t9, $t7, 3
/* 0BCD3C 800BC13C 03221821 */  addu  $v1, $t9, $v0
.L800BC140:
/* 0BCD40 800BC140 24420008 */  addiu $v0, $v0, 8
/* 0BCD44 800BC144 0043082B */  sltu  $at, $v0, $v1
/* 0BCD48 800BC148 24080000 */  li    $t0, 0
/* 0BCD4C 800BC14C 24090000 */  li    $t1, 0
/* 0BCD50 800BC150 AC49FFFC */  sw    $t1, -4($v0)
/* 0BCD54 800BC154 1420FFFA */  bnez  $at, .L800BC140
/* 0BCD58 800BC158 AC48FFF8 */   sw    $t0, -8($v0)
.L800BC15C:
/* 0BCD5C 800BC15C 2482F780 */  addiu $v0, $a0, %lo(gGfxSPTaskOutputBufferSize) # -0x880
/* 0BCD60 800BC160 254A71A0 */  addiu $t2, %lo(gAudioGlobalsEndMarker) # addiu $t2, $t2, 0x71a0
/* 0BCD64 800BC164 01421823 */  subu  $v1, $t2, $v0
/* 0BCD68 800BC168 000380C2 */  srl   $s0, $v1, 3
/* 0BCD6C 800BC16C 06000007 */  bltz  $s0, .L800BC18C
.L800BC170:
/* 0BCD70 800BC170 2610FFFF */   addiu $s0, $s0, -1
/* 0BCD74 800BC174 240C0000 */  li    $t4, 0
/* 0BCD78 800BC178 240D0000 */  li    $t5, 0
/* 0BCD7C 800BC17C AC4D0004 */  sw    $t5, 4($v0)
/* 0BCD80 800BC180 AC4C0000 */  sw    $t4, ($v0)
/* 0BCD84 800BC184 0601FFFA */  bgez  $s0, .L800BC170
/* 0BCD88 800BC188 24420008 */   addiu $v0, $v0, 8
.L800BC18C:
/* 0BCD8C 800BC18C 3C028000 */  lui   $v0, %hi(osTvType) # $v0, 0x8000
/* 0BCD90 800BC190 8C420300 */  lw    $v0, %lo(osTvType)($v0)
/* 0BCD94 800BC194 10400007 */  beqz  $v0, .L800BC1B4
/* 0BCD98 800BC198 24010001 */   li    $at, 1
/* 0BCD9C 800BC19C 10410015 */  beq   $v0, $at, .L800BC1F4
/* 0BCDA0 800BC1A0 24010002 */   li    $at, 2
/* 0BCDA4 800BC1A4 1041000B */  beq   $v0, $at, .L800BC1D4
/* 0BCDA8 800BC1A8 00000000 */   nop   
/* 0BCDAC 800BC1AC 10000011 */  b     .L800BC1F4
/* 0BCDB0 800BC1B0 00000000 */   nop   
.L800BC1B4:
/* 0BCDB4 800BC1B4 3C01800F */  lui   $at, %hi(D_800F2EA0) # $at, 0x800f
/* 0BCDB8 800BC1B8 C4242EA0 */  lwc1  $f4, %lo(D_800F2EA0)($at)
/* 0BCDBC 800BC1BC 3C01803B */  lui   $at, %hi(D_803B7178) # $at, 0x803b
/* 0BCDC0 800BC1C0 240E0032 */  li    $t6, 50
/* 0BCDC4 800BC1C4 E4247178 */  swc1  $f4, %lo(D_803B7178)($at)
/* 0BCDC8 800BC1C8 3C01803B */  lui   $at, %hi(gRefreshRate) # $at, 0x803b
/* 0BCDCC 800BC1CC 10000010 */  b     .L800BC210
/* 0BCDD0 800BC1D0 AC2E717C */   sw    $t6, %lo(gRefreshRate)($at)
.L800BC1D4:
/* 0BCDD4 800BC1D4 3C01800F */  lui   $at, %hi(D_800F2EA4) # $at, 0x800f
/* 0BCDD8 800BC1D8 C4262EA4 */  lwc1  $f6, %lo(D_800F2EA4)($at)
/* 0BCDDC 800BC1DC 3C01803B */  lui   $at, %hi(D_803B7178) # $at, 0x803b
/* 0BCDE0 800BC1E0 240F003C */  li    $t7, 60
/* 0BCDE4 800BC1E4 E4267178 */  swc1  $f6, %lo(D_803B7178)($at)
/* 0BCDE8 800BC1E8 3C01803B */  lui   $at, %hi(gRefreshRate) # $at, 0x803b
/* 0BCDEC 800BC1EC 10000008 */  b     .L800BC210
/* 0BCDF0 800BC1F0 AC2F717C */   sw    $t7, %lo(gRefreshRate)($at)
.L800BC1F4:
/* 0BCDF4 800BC1F4 3C01800F */  lui   $at, %hi(D_800F2EA8) # $at, 0x800f
/* 0BCDF8 800BC1F8 C4282EA8 */  lwc1  $f8, %lo(D_800F2EA8)($at)
/* 0BCDFC 800BC1FC 3C01803B */  lui   $at, %hi(D_803B7178) # $at, 0x803b
/* 0BCE00 800BC200 2419003C */  li    $t9, 60
/* 0BCE04 800BC204 E4287178 */  swc1  $f8, %lo(D_803B7178)($at)
/* 0BCE08 800BC208 3C01803B */  lui   $at, %hi(gRefreshRate) # $at, 0x803b
/* 0BCE0C 800BC20C AC39717C */  sw    $t9, %lo(gRefreshRate)($at)
.L800BC210:
/* 0BCE10 800BC210 0C032FD2 */  jal   port_eu_init
/* 0BCE14 800BC214 00000000 */   nop   
/* 0BCE18 800BC218 3C02803B */  lui   $v0, %hi(gAiBufferLengths) # $v0, 0x803b
/* 0BCE1C 800BC21C 3C04803B */  lui   $a0, %hi(D_803B7192) # $a0, 0x803b
/* 0BCE20 800BC220 24847192 */  addiu $a0, %lo(D_803B7192) # addiu $a0, $a0, 0x7192
/* 0BCE24 800BC224 2442718C */  addiu $v0, %lo(gAiBufferLengths) # addiu $v0, $v0, 0x718c
/* 0BCE28 800BC228 240300A0 */  li    $v1, 160
.L800BC22C:
/* 0BCE2C 800BC22C 24420002 */  addiu $v0, $v0, 2
/* 0BCE30 800BC230 0044082B */  sltu  $at, $v0, $a0
/* 0BCE34 800BC234 1420FFFD */  bnez  $at, .L800BC22C
/* 0BCE38 800BC238 A443FFFE */   sh    $v1, -2($v0)
/* 0BCE3C 800BC23C 3C18803B */  lui   $t8, %hi(gAudioFrameCount) # $t8, 0x803b
/* 0BCE40 800BC240 271870B8 */  addiu $t8, %lo(gAudioFrameCount) # addiu $t8, $t8, 0x70b8
/* 0BCE44 800BC244 AF000000 */  sw    $zero, ($t8)
/* 0BCE48 800BC248 3C01803B */  lui   $at, %hi(gAudioTaskIndex) # $at, 0x803b
/* 0BCE4C 800BC24C AC2070C0 */  sw    $zero, %lo(gAudioTaskIndex)($at)
/* 0BCE50 800BC250 3C01803B */  lui   $at, %hi(gCurrAiBufferIndex) # $at, 0x803b
/* 0BCE54 800BC254 AC2070C4 */  sw    $zero, %lo(gCurrAiBufferIndex)($at)
/* 0BCE58 800BC258 3C01803B */  lui   $at, %hi(gAudioLibSoundMode) # $at, 0x803b
/* 0BCE5C 800BC25C A02070B6 */  sb    $zero, %lo(gAudioLibSoundMode)($at)
/* 0BCE60 800BC260 3C02803B */  lui   $v0, %hi(gAudioTasks) # $v0, 0x803b
/* 0BCE64 800BC264 3C01803B */  lui   $at, %hi(gAudioTask) # $at, 0x803b
/* 0BCE68 800BC268 244270D8 */  addiu $v0, %lo(gAudioTasks) # addiu $v0, $v0, 0x70d8
/* 0BCE6C 800BC26C AC2070D4 */  sw    $zero, %lo(gAudioTask)($at)
/* 0BCE70 800BC270 3C04803B */  lui   $a0, %hi(D_803B6720) # $a0, 0x803b
/* 0BCE74 800BC274 3C05803B */  lui   $a1, %hi(D_803B6738) # $a1, 0x803b
/* 0BCE78 800BC278 AC400034 */  sw    $zero, 0x34($v0)
/* 0BCE7C 800BC27C AC400084 */  sw    $zero, 0x84($v0)
/* 0BCE80 800BC280 24A56738 */  addiu $a1, %lo(D_803B6738) # addiu $a1, $a1, 0x6738
/* 0BCE84 800BC284 24846720 */  addiu $a0, %lo(D_803B6720) # addiu $a0, $a0, 0x6720
/* 0BCE88 800BC288 0C033358 */  jal   osCreateMesgQueue
/* 0BCE8C 800BC28C 24060001 */   li    $a2, 1
/* 0BCE90 800BC290 3C04803B */  lui   $a0, %hi(gCurrAudioFrameDmaQueue) # $a0, 0x803b
/* 0BCE94 800BC294 3C05803B */  lui   $a1, %hi(gCurrAudioFrameDmaMesgBufs) # $a1, 0x803b
/* 0BCE98 800BC298 24A56020 */  addiu $a1, %lo(gCurrAudioFrameDmaMesgBufs) # addiu $a1, $a1, 0x6020
/* 0BCE9C 800BC29C 24846008 */  addiu $a0, %lo(gCurrAudioFrameDmaQueue) # addiu $a0, $a0, 0x6008
/* 0BCEA0 800BC2A0 0C033358 */  jal   osCreateMesgQueue
/* 0BCEA4 800BC2A4 24060040 */   li    $a2, 64
/* 0BCEA8 800BC2A8 3C01803B */  lui   $at, %hi(gCurrAudioFrameDmaCount) # $at, 0x803b
/* 0BCEAC 800BC2AC AC2070BC */  sw    $zero, %lo(gCurrAudioFrameDmaCount)($at)
/* 0BCEB0 800BC2B0 3C01803B */  lui   $at, %hi(gSampleDmaNumListItems) # $at, 0x803b
/* 0BCEB4 800BC2B4 3C04800F */  lui   $a0, %hi(gAudioInitPoolSize) # $a0, 0x800f
/* 0BCEB8 800BC2B8 AC206E58 */  sw    $zero, %lo(gSampleDmaNumListItems)($at)
/* 0BCEBC 800BC2BC 0C02E43C */  jal   sound_init_main_pools
/* 0BCEC0 800BC2C0 8C84A5D4 */   lw    $a0, %lo(gAudioInitPoolSize)($a0)
/* 0BCEC4 800BC2C4 3C06803B */  lui   $a2, %hi(gAiBuffers) # $a2, 0x803b
/* 0BCEC8 800BC2C8 3C14803B */  lui   $s4, %hi(gAudioInitPool) # $s4, 0x803b
/* 0BCECC 800BC2CC 3C11803B */  lui   $s1, %hi(gAiBufferLengths) # $s1, 0x803b
/* 0BCED0 800BC2D0 2631718C */  addiu $s1, %lo(gAiBufferLengths) # addiu $s1, $s1, 0x718c
/* 0BCED4 800BC2D4 2694FBD8 */  addiu $s4, %lo(gAudioInitPool) # addiu $s4, $s4, -0x428
/* 0BCED8 800BC2D8 24C67180 */  addiu $a2, %lo(gAiBuffers) # addiu $a2, $a2, 0x7180
/* 0BCEDC 800BC2DC 24100A00 */  li    $s0, 2560
/* 0BCEE0 800BC2E0 02802025 */  move  $a0, $s4
.L800BC2E4:
/* 0BCEE4 800BC2E4 24050A00 */  li    $a1, 2560
/* 0BCEE8 800BC2E8 0C02E3FF */  jal   soundAlloc
/* 0BCEEC 800BC2EC AFA6003C */   sw    $a2, 0x3c($sp)
/* 0BCEF0 800BC2F0 8FA6003C */  lw    $a2, 0x3c($sp)
/* 0BCEF4 800BC2F4 00001825 */  move  $v1, $zero
/* 0BCEF8 800BC2F8 ACC20000 */  sw    $v0, ($a2)
.L800BC2FC:
/* 0BCEFC 800BC2FC 8CC80000 */  lw    $t0, ($a2)
/* 0BCF00 800BC300 01034821 */  addu  $t1, $t0, $v1
/* 0BCF04 800BC304 A5200000 */  sh    $zero, ($t1)
/* 0BCF08 800BC308 8CCA0000 */  lw    $t2, ($a2)
/* 0BCF0C 800BC30C 01435821 */  addu  $t3, $t2, $v1
/* 0BCF10 800BC310 A5600002 */  sh    $zero, 2($t3)
/* 0BCF14 800BC314 8CCC0000 */  lw    $t4, ($a2)
/* 0BCF18 800BC318 01836821 */  addu  $t5, $t4, $v1
/* 0BCF1C 800BC31C A5A00004 */  sh    $zero, 4($t5)
/* 0BCF20 800BC320 8CCE0000 */  lw    $t6, ($a2)
/* 0BCF24 800BC324 01C37821 */  addu  $t7, $t6, $v1
/* 0BCF28 800BC328 24630008 */  addiu $v1, $v1, 8
/* 0BCF2C 800BC32C 1470FFF3 */  bne   $v1, $s0, .L800BC2FC
/* 0BCF30 800BC330 A5E00006 */   sh    $zero, 6($t7)
/* 0BCF34 800BC334 24C60004 */  addiu $a2, $a2, 4
/* 0BCF38 800BC338 00D1082B */  sltu  $at, $a2, $s1
/* 0BCF3C 800BC33C 5420FFE9 */  bnel  $at, $zero, .L800BC2E4
/* 0BCF40 800BC340 02802025 */   move  $a0, $s4
/* 0BCF44 800BC344 3C01803B */  lui   $at, %hi(gAudioResetPresetIdToLoad) # $at, 0x803b
/* 0BCF48 800BC348 3C18803B */  lui   $t8, %hi(gAudioResetStatus) # $t8, 0x803b
/* 0BCF4C 800BC34C A0200501 */  sb    $zero, %lo(gAudioResetPresetIdToLoad)($at)
/* 0BCF50 800BC350 27180500 */  addiu $t8, %lo(gAudioResetStatus) # addiu $t8, $t8, 0x500
/* 0BCF54 800BC354 24190001 */  li    $t9, 1
/* 0BCF58 800BC358 0C02E803 */  jal   audio_shut_down_and_reset_step
/* 0BCF5C 800BC35C A3190000 */   sb    $t9, ($t8)
/* 0BCF60 800BC360 3C11803B */  lui   $s1, %hi(gSeqFileHeader) # $s1, 0x803b
/* 0BCF64 800BC364 3C1200BC */  lui   $s2, %hi(_sequencesSegmentRomStart) # $s2, 0xbc
/* 0BCF68 800BC368 26525F60 */  addiu $s2, %lo(_sequencesSegmentRomStart) # addiu $s2, $s2, 0x5f60
/* 0BCF6C 800BC36C 2631706C */  addiu $s1, %lo(gSeqFileHeader) # addiu $s1, $s1, 0x706c
/* 0BCF70 800BC370 27B50060 */  addiu $s5, $sp, 0x60
/* 0BCF74 800BC374 AE350000 */  sw    $s5, ($s1)
/* 0BCF78 800BC378 02A02825 */  move  $a1, $s5
/* 0BCF7C 800BC37C 02402025 */  move  $a0, $s2
/* 0BCF80 800BC380 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BCF84 800BC384 24060010 */   li    $a2, 16
/* 0BCF88 800BC388 8E280000 */  lw    $t0, ($s1)
/* 0BCF8C 800BC38C 3C02803B */  lui   $v0, %hi(gSequenceCount) # $v0, 0x803b
/* 0BCF90 800BC390 2401FFF0 */  li    $at, -16
/* 0BCF94 800BC394 85090002 */  lh    $t1, 2($t0)
/* 0BCF98 800BC398 2442707C */  addiu $v0, %lo(gSequenceCount) # addiu $v0, $v0, 0x707c
/* 0BCF9C 800BC39C 02802025 */  move  $a0, $s4
/* 0BCFA0 800BC3A0 3130FFFF */  andi  $s0, $t1, 0xffff
/* 0BCFA4 800BC3A4 001050C0 */  sll   $t2, $s0, 3
/* 0BCFA8 800BC3A8 01408025 */  move  $s0, $t2
/* 0BCFAC 800BC3AC 26100013 */  addiu $s0, $s0, 0x13
/* 0BCFB0 800BC3B0 02012824 */  and   $a1, $s0, $at
/* 0BCFB4 800BC3B4 00A08025 */  move  $s0, $a1
/* 0BCFB8 800BC3B8 0C02E3FF */  jal   soundAlloc
/* 0BCFBC 800BC3BC A4490000 */   sh    $t1, ($v0)
/* 0BCFC0 800BC3C0 AE220000 */  sw    $v0, ($s1)
/* 0BCFC4 800BC3C4 02402025 */  move  $a0, $s2
/* 0BCFC8 800BC3C8 00402825 */  move  $a1, $v0
/* 0BCFCC 800BC3CC 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BCFD0 800BC3D0 02003025 */   move  $a2, $s0
/* 0BCFD4 800BC3D4 8E240000 */  lw    $a0, ($s1)
/* 0BCFD8 800BC3D8 0C02ED0F */  jal   func_800BB43C
/* 0BCFDC 800BC3DC 02402825 */   move  $a1, $s2
/* 0BCFE0 800BC3E0 3C110096 */  lui   $s1, %hi(_audio_banksSegmentRomStart) # $s1, 0x96
/* 0BCFE4 800BC3E4 3C12803B */  lui   $s2, %hi(gAlCtlHeader) # $s2, 0x803b
/* 0BCFE8 800BC3E8 26527070 */  addiu $s2, %lo(gAlCtlHeader) # addiu $s2, $s2, 0x7070
/* 0BCFEC 800BC3EC 26316260 */  addiu $s1, %lo(_audio_banksSegmentRomStart) # addiu $s1, $s1, 0x6260
/* 0BCFF0 800BC3F0 AE550000 */  sw    $s5, ($s2)
/* 0BCFF4 800BC3F4 02202025 */  move  $a0, $s1
/* 0BCFF8 800BC3F8 02A02825 */  move  $a1, $s5
/* 0BCFFC 800BC3FC 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BD000 800BC400 24060010 */   li    $a2, 16
/* 0BD004 800BC404 8E4C0000 */  lw    $t4, ($s2)
/* 0BD008 800BC408 2401FFF0 */  li    $at, -16
/* 0BD00C 800BC40C 02802025 */  move  $a0, $s4
/* 0BD010 800BC410 85930002 */  lh    $s3, 2($t4)
/* 0BD014 800BC414 001380C0 */  sll   $s0, $s3, 3
/* 0BD018 800BC418 26100013 */  addiu $s0, $s0, 0x13
/* 0BD01C 800BC41C 02012824 */  and   $a1, $s0, $at
/* 0BD020 800BC420 0C02E3FF */  jal   soundAlloc
/* 0BD024 800BC424 00A08025 */   move  $s0, $a1
/* 0BD028 800BC428 AE420000 */  sw    $v0, ($s2)
/* 0BD02C 800BC42C 02202025 */  move  $a0, $s1
/* 0BD030 800BC430 00402825 */  move  $a1, $v0
/* 0BD034 800BC434 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BD038 800BC438 02003025 */   move  $a2, $s0
/* 0BD03C 800BC43C 8E440000 */  lw    $a0, ($s2)
/* 0BD040 800BC440 0C02ED0F */  jal   func_800BB43C
/* 0BD044 800BC444 02202825 */   move  $a1, $s1
/* 0BD048 800BC448 00132880 */  sll   $a1, $s3, 2
/* 0BD04C 800BC44C 00B32823 */  subu  $a1, $a1, $s3
/* 0BD050 800BC450 00052880 */  sll   $a1, $a1, 2
/* 0BD054 800BC454 0C02E3FF */  jal   soundAlloc
/* 0BD058 800BC458 02802025 */   move  $a0, $s4
/* 0BD05C 800BC45C 3C07803B */  lui   $a3, %hi(gCtlEntries) # $a3, 0x803b
/* 0BD060 800BC460 24E77080 */  addiu $a3, %lo(gCtlEntries) # addiu $a3, $a3, 0x7080
/* 0BD064 800BC464 ACE20000 */  sw    $v0, ($a3)
/* 0BD068 800BC468 1A600019 */  blez  $s3, .L800BC4D0
/* 0BD06C 800BC46C 00001825 */   move  $v1, $zero
/* 0BD070 800BC470 00008825 */  move  $s1, $zero
/* 0BD074 800BC474 00008025 */  move  $s0, $zero
.L800BC478:
/* 0BD078 800BC478 8E4E0000 */  lw    $t6, ($s2)
/* 0BD07C 800BC47C 02A02825 */  move  $a1, $s5
/* 0BD080 800BC480 24060010 */  li    $a2, 16
/* 0BD084 800BC484 01D17821 */  addu  $t7, $t6, $s1
/* 0BD088 800BC488 8DE40004 */  lw    $a0, 4($t7)
/* 0BD08C 800BC48C 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BD090 800BC490 AFA3008C */   sw    $v1, 0x8c($sp)
/* 0BD094 800BC494 3C07803B */  lui   $a3, %hi(gCtlEntries) # $a3, 0x803b
/* 0BD098 800BC498 24E77080 */  addiu $a3, %lo(gCtlEntries) # addiu $a3, $a3, 0x7080
/* 0BD09C 800BC49C 8CF80000 */  lw    $t8, ($a3)
/* 0BD0A0 800BC4A0 8FB90060 */  lw    $t9, 0x60($sp)
/* 0BD0A4 800BC4A4 8FA3008C */  lw    $v1, 0x8c($sp)
/* 0BD0A8 800BC4A8 03104021 */  addu  $t0, $t8, $s0
/* 0BD0AC 800BC4AC A1190001 */  sb    $t9, 1($t0)
/* 0BD0B0 800BC4B0 8CEA0000 */  lw    $t2, ($a3)
/* 0BD0B4 800BC4B4 8FA90064 */  lw    $t1, 0x64($sp)
/* 0BD0B8 800BC4B8 24630001 */  addiu $v1, $v1, 1
/* 0BD0BC 800BC4BC 01505821 */  addu  $t3, $t2, $s0
/* 0BD0C0 800BC4C0 2610000C */  addiu $s0, $s0, 0xc
/* 0BD0C4 800BC4C4 26310008 */  addiu $s1, $s1, 8
/* 0BD0C8 800BC4C8 1473FFEB */  bne   $v1, $s3, .L800BC478
/* 0BD0CC 800BC4CC A1690002 */   sb    $t1, 2($t3)
.L800BC4D0:
/* 0BD0D0 800BC4D0 3C11803B */  lui   $s1, %hi(gAlTbl) # $s1, 0x803b
/* 0BD0D4 800BC4D4 3C120098 */  lui   $s2, %hi(_audio_tablesSegmentRomStart) # $s2, 0x98
/* 0BD0D8 800BC4D8 26529AA0 */  addiu $s2, %lo(_audio_tablesSegmentRomStart) # addiu $s2, $s2, -0x6560
/* 0BD0DC 800BC4DC 26317074 */  addiu $s1, %lo(gAlTbl) # addiu $s1, $s1, 0x7074
/* 0BD0E0 800BC4E0 AE350000 */  sw    $s5, ($s1)
/* 0BD0E4 800BC4E4 02402025 */  move  $a0, $s2
/* 0BD0E8 800BC4E8 02A02825 */  move  $a1, $s5
/* 0BD0EC 800BC4EC 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BD0F0 800BC4F0 24060010 */   li    $a2, 16
/* 0BD0F4 800BC4F4 8E2C0000 */  lw    $t4, ($s1)
/* 0BD0F8 800BC4F8 2401FFF0 */  li    $at, -16
/* 0BD0FC 800BC4FC 02802025 */  move  $a0, $s4
/* 0BD100 800BC500 85900002 */  lh    $s0, 2($t4)
/* 0BD104 800BC504 001068C0 */  sll   $t5, $s0, 3
/* 0BD108 800BC508 01A08025 */  move  $s0, $t5
/* 0BD10C 800BC50C 26100013 */  addiu $s0, $s0, 0x13
/* 0BD110 800BC510 02012824 */  and   $a1, $s0, $at
/* 0BD114 800BC514 0C02E3FF */  jal   soundAlloc
/* 0BD118 800BC518 00A08025 */   move  $s0, $a1
/* 0BD11C 800BC51C AE220000 */  sw    $v0, ($s1)
/* 0BD120 800BC520 02402025 */  move  $a0, $s2
/* 0BD124 800BC524 00402825 */  move  $a1, $v0
/* 0BD128 800BC528 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BD12C 800BC52C 02003025 */   move  $a2, $s0
/* 0BD130 800BC530 8E240000 */  lw    $a0, ($s1)
/* 0BD134 800BC534 0C02ED0F */  jal   func_800BB43C
/* 0BD138 800BC538 02402825 */   move  $a1, $s2
/* 0BD13C 800BC53C 02802025 */  move  $a0, $s4
/* 0BD140 800BC540 0C02E3FF */  jal   soundAlloc
/* 0BD144 800BC544 24050100 */   li    $a1, 256
/* 0BD148 800BC548 3C03803B */  lui   $v1, %hi(gAlBankSets) # $v1, 0x803b
/* 0BD14C 800BC54C 24637078 */  addiu $v1, %lo(gAlBankSets) # addiu $v1, $v1, 0x7078
/* 0BD150 800BC550 3C0400BF */  lui   $a0, %hi(_instrument_setsSegmentRomStart) # $a0, 0xbf
/* 0BD154 800BC554 AC620000 */  sw    $v0, ($v1)
/* 0BD158 800BC558 248490E0 */  addiu $a0, %lo(_instrument_setsSegmentRomStart) # addiu $a0, $a0, -0x6f20
/* 0BD15C 800BC55C 00402825 */  move  $a1, $v0
/* 0BD160 800BC560 0C02EAA0 */  jal   audio_dma_copy_immediate
/* 0BD164 800BC564 24060100 */   li    $a2, 256
/* 0BD168 800BC568 3C10800F */  lui   $s0, %hi(D_800EA5D8) # $s0, 0x800f
/* 0BD16C 800BC56C 2610A5D8 */  addiu $s0, %lo(D_800EA5D8) # addiu $s0, $s0, -0x5a28
/* 0BD170 800BC570 8E050000 */  lw    $a1, ($s0)
/* 0BD174 800BC574 0C02E3FF */  jal   soundAlloc
/* 0BD178 800BC578 02802025 */   move  $a0, $s4
/* 0BD17C 800BC57C 3C04803B */  lui   $a0, %hi(gUnkPool1) # $a0, 0x803b
/* 0BD180 800BC580 248401B8 */  addiu $a0, %lo(gUnkPool1) # addiu $a0, $a0, 0x1b8
/* 0BD184 800BC584 00402825 */  move  $a1, $v0
/* 0BD188 800BC588 0C02E41F */  jal   sound_alloc_pool_init
/* 0BD18C 800BC58C 8E060000 */   lw    $a2, ($s0)
/* 0BD190 800BC590 0C030497 */  jal   init_sequence_players
/* 0BD194 800BC594 00000000 */   nop   
/* 0BD198 800BC598 3C0F7655 */  lui   $t7, (0x76557364 >> 16) # lui $t7, 0x7655
/* 0BD19C 800BC59C 3C18800F */  lui   $t8, %hi(gAudioLoadLock) # $t8, 0x800f
/* 0BD1A0 800BC5A0 2718A5DC */  addiu $t8, %lo(gAudioLoadLock) # addiu $t8, $t8, -0x5a24
/* 0BD1A4 800BC5A4 35EF7364 */  ori   $t7, (0x76557364 & 0xFFFF) # ori $t7, $t7, 0x7364
/* 0BD1A8 800BC5A8 AF0F0000 */  sw    $t7, ($t8)
/* 0BD1AC 800BC5AC 8FBF002C */  lw    $ra, 0x2c($sp)
/* 0BD1B0 800BC5B0 8FB50028 */  lw    $s5, 0x28($sp)
/* 0BD1B4 800BC5B4 8FB40024 */  lw    $s4, 0x24($sp)
/* 0BD1B8 800BC5B8 8FB30020 */  lw    $s3, 0x20($sp)
/* 0BD1BC 800BC5BC 8FB2001C */  lw    $s2, 0x1c($sp)
/* 0BD1C0 800BC5C0 8FB10018 */  lw    $s1, 0x18($sp)
/* 0BD1C4 800BC5C4 8FB00014 */  lw    $s0, 0x14($sp)
/* 0BD1C8 800BC5C8 03E00008 */  jr    $ra
/* 0BD1CC 800BC5CC 27BD0090 */   addiu $sp, $sp, 0x90
