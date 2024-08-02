.section .late_rodata
glabel D_800F2FE0
.float 20.0304203

.section .text
glabel audio_init
/* BCCDC 800BC0DC 27BDFF70 */  addiu      $sp, $sp, -0x90
/* BCCE0 800BC0E0 3C0E800F */  lui        $t6, %hi(gAudioLoadLock)
/* BCCE4 800BC0E4 AFBF002C */  sw         $ra, 0x2C($sp)
/* BCCE8 800BC0E8 AFB50028 */  sw         $s5, 0x28($sp)
/* BCCEC 800BC0EC AFB40024 */  sw         $s4, 0x24($sp)
/* BCCF0 800BC0F0 AFB30020 */  sw         $s3, 0x20($sp)
/* BCCF4 800BC0F4 AFB2001C */  sw         $s2, 0x1C($sp)
/* BCCF8 800BC0F8 AFB10018 */  sw         $s1, 0x18($sp)
/* BCCFC 800BC0FC AFB00014 */  sw         $s0, 0x14($sp)
/* BCD00 800BC100 25CEA71C */  addiu      $t6, $t6, %lo(gAudioLoadLock)
/* BCD04 800BC104 ADC00000 */  sw         $zero, 0x0($t6)
/* BCD08 800BC108 3C04800F */  lui        $a0, %hi(gAudioHeapSize)
/* BCD0C 800BC10C 8C84A710 */  lw         $a0, %lo(gAudioHeapSize)($a0)
/* BCD10 800BC110 00001825 */  or         $v1, $zero, $zero
/* BCD14 800BC114 3C0A803B */  lui        $t2, %hi(D_803B71A0)
/* BCD18 800BC118 04810003 */  bgez       $a0, .L800BC128
/* BCD1C 800BC11C 000478C3 */   sra       $t7, $a0, 3
/* BCD20 800BC120 24810007 */  addiu      $at, $a0, 0x7
/* BCD24 800BC124 000178C3 */  sra        $t7, $at, 3
.L800BC128:
/* BCD28 800BC128 19E0000C */  blez       $t7, .L800BC15C
/* BCD2C 800BC12C 3C04803B */   lui       $a0, %hi(gGfxSPTaskOutputBufferSize)
/* BCD30 800BC130 3C18803B */  lui        $t8, %hi(gAudioHeap)
/* BCD34 800BC134 270271B0 */  addiu      $v0, $t8, %lo(gAudioHeap)
/* BCD38 800BC138 000FC8C0 */  sll        $t9, $t7, 3
/* BCD3C 800BC13C 03221821 */  addu       $v1, $t9, $v0
.L800BC140:
/* BCD40 800BC140 24420008 */  addiu      $v0, $v0, 0x8
/* BCD44 800BC144 0043082B */  sltu       $at, $v0, $v1
/* BCD48 800BC148 24080000 */  addiu      $t0, $zero, 0x0
/* BCD4C 800BC14C 24090000 */  addiu      $t1, $zero, 0x0
/* BCD50 800BC150 AC49FFFC */  sw         $t1, -0x4($v0)
/* BCD54 800BC154 1420FFFA */  bnez       $at, .L800BC140
/* BCD58 800BC158 AC48FFF8 */   sw        $t0, -0x8($v0)
.L800BC15C:
/* BCD5C 800BC15C 2482F780 */  addiu      $v0, $a0, %lo(gGfxSPTaskOutputBufferSize)
/* BCD60 800BC160 254A71A0 */  addiu      $t2, $t2, %lo(D_803B71A0)
/* BCD64 800BC164 01421823 */  subu       $v1, $t2, $v0
/* BCD68 800BC168 000380C2 */  srl        $s0, $v1, 3
/* BCD6C 800BC16C 06000008 */  bltz       $s0, .L800BC190
/* BCD70 800BC170 3C01800F */   lui       $at, %hi(D_800F2FE0)
.L800BC174:
/* BCD74 800BC174 2610FFFF */  addiu      $s0, $s0, -0x1
/* BCD78 800BC178 240C0000 */  addiu      $t4, $zero, 0x0
/* BCD7C 800BC17C 240D0000 */  addiu      $t5, $zero, 0x0
/* BCD80 800BC180 AC4D0004 */  sw         $t5, 0x4($v0)
/* BCD84 800BC184 AC4C0000 */  sw         $t4, 0x0($v0)
/* BCD88 800BC188 0601FFFA */  bgez       $s0, .L800BC174
/* BCD8C 800BC18C 24420008 */   addiu     $v0, $v0, 0x8
.L800BC190:
/* BCD90 800BC190 C4242FE0 */  lwc1       $ft0, %lo(D_800F2FE0)($at)
/* BCD94 800BC194 3C01803B */  lui        $at, %hi(D_803B7178)
/* BCD98 800BC198 240E0032 */  addiu      $t6, $zero, 0x32
/* BCD9C 800BC19C E4247178 */  swc1       $ft0, %lo(D_803B7178)($at)
/* BCDA0 800BC1A0 3C01803B */  lui        $at, %hi(gRefreshRate)
/* BCDA4 800BC1A4 0C032FF6 */  jal        port_eu_init
/* BCDA8 800BC1A8 AC2E717C */   sw        $t6, %lo(gRefreshRate)($at)
/* BCDAC 800BC1AC 3C02803B */  lui        $v0, %hi(gAiBufferLengths)
/* BCDB0 800BC1B0 3C04803B */  lui        $a0, %hi(D_803B7192)
/* BCDB4 800BC1B4 24847192 */  addiu      $a0, $a0, %lo(D_803B7192)
/* BCDB8 800BC1B8 2442718C */  addiu      $v0, $v0, %lo(gAiBufferLengths)
/* BCDBC 800BC1BC 240300A0 */  addiu      $v1, $zero, 0xA0
.L800BC1C0:
/* BCDC0 800BC1C0 24420002 */  addiu      $v0, $v0, 0x2
/* BCDC4 800BC1C4 0044082B */  sltu       $at, $v0, $a0
/* BCDC8 800BC1C8 1420FFFD */  bnez       $at, .L800BC1C0
/* BCDCC 800BC1CC A443FFFE */   sh        $v1, -0x2($v0)
/* BCDD0 800BC1D0 3C0F803B */  lui        $t7, %hi(gAudioFrameCount)
/* BCDD4 800BC1D4 25EF70B8 */  addiu      $t7, $t7, %lo(gAudioFrameCount)
/* BCDD8 800BC1D8 ADE00000 */  sw         $zero, 0x0($t7)
/* BCDDC 800BC1DC 3C01803B */  lui        $at, %hi(gAudioTaskIndex)
/* BCDE0 800BC1E0 AC2070C0 */  sw         $zero, %lo(gAudioTaskIndex)($at)
/* BCDE4 800BC1E4 3C01803B */  lui        $at, %hi(gCurrAiBufferIndex)
/* BCDE8 800BC1E8 AC2070C4 */  sw         $zero, %lo(gCurrAiBufferIndex)($at)
/* BCDEC 800BC1EC 3C01803B */  lui        $at, %hi(gAudioLibSoundMode)
/* BCDF0 800BC1F0 A02070B6 */  sb         $zero, %lo(gAudioLibSoundMode)($at)
/* BCDF4 800BC1F4 3C02803B */  lui        $v0, %hi(gAudioTasks)
/* BCDF8 800BC1F8 3C01803B */  lui        $at, %hi(gAudioTask)
/* BCDFC 800BC1FC 244270D8 */  addiu      $v0, $v0, %lo(gAudioTasks)
/* BCE00 800BC200 AC2070D4 */  sw         $zero, %lo(gAudioTask)($at)
/* BCE04 800BC204 3C04803B */  lui        $a0, %hi(D_803B6720)
/* BCE08 800BC208 3C05803B */  lui        $a1, %hi(D_803B6738)
/* BCE0C 800BC20C AC400034 */  sw         $zero, 0x34($v0)
/* BCE10 800BC210 AC400084 */  sw         $zero, 0x84($v0)
/* BCE14 800BC214 24A56738 */  addiu      $a1, $a1, %lo(D_803B6738)
/* BCE18 800BC218 24846720 */  addiu      $a0, $a0, %lo(D_803B6720)
/* BCE1C 800BC21C 0C03337C */  jal        osCreateMesgQueue
/* BCE20 800BC220 24060001 */   addiu     $a2, $zero, 0x1
/* BCE24 800BC224 3C04803B */  lui        $a0, %hi(gCurrAudioFrameDmaQueue)
/* BCE28 800BC228 3C05803B */  lui        $a1, %hi(gCurrAudioFrameDmaMesgBufs)
/* BCE2C 800BC22C 24A56020 */  addiu      $a1, $a1, %lo(gCurrAudioFrameDmaMesgBufs)
/* BCE30 800BC230 24846008 */  addiu      $a0, $a0, %lo(gCurrAudioFrameDmaQueue)
/* BCE34 800BC234 0C03337C */  jal        osCreateMesgQueue
/* BCE38 800BC238 24060040 */   addiu     $a2, $zero, 0x40
/* BCE3C 800BC23C 3C01803B */  lui        $at, %hi(gCurrAudioFrameDmaCount)
/* BCE40 800BC240 AC2070BC */  sw         $zero, %lo(gCurrAudioFrameDmaCount)($at)
/* BCE44 800BC244 3C01803B */  lui        $at, %hi(gSampleDmaNumListItems)
/* BCE48 800BC248 3C04800F */  lui        $a0, %hi(gAudioInitPoolSize)
/* BCE4C 800BC24C AC206E58 */  sw         $zero, %lo(gSampleDmaNumListItems)($at)
/* BCE50 800BC250 0C02E43C */  jal        sound_init_main_pools
/* BCE54 800BC254 8C84A714 */   lw        $a0, %lo(gAudioInitPoolSize)($a0)
/* BCE58 800BC258 3C06803B */  lui        $a2, %hi(gAiBuffers)
/* BCE5C 800BC25C 3C14803B */  lui        $s4, %hi(gAudioInitPool)
/* BCE60 800BC260 3C11803B */  lui        $s1, %hi(gAiBufferLengths)
/* BCE64 800BC264 2631718C */  addiu      $s1, $s1, %lo(gAiBufferLengths)
/* BCE68 800BC268 2694FBD8 */  addiu      $s4, $s4, %lo(gAudioInitPool)
/* BCE6C 800BC26C 24C67180 */  addiu      $a2, $a2, %lo(gAiBuffers)
/* BCE70 800BC270 24100AA0 */  addiu      $s0, $zero, 0xAA0
/* BCE74 800BC274 02802025 */  or         $a0, $s4, $zero
.L800BC278:
/* BCE78 800BC278 24050AA0 */  addiu      $a1, $zero, 0xAA0
/* BCE7C 800BC27C 0C02E3FF */  jal        soundAlloc
/* BCE80 800BC280 AFA6003C */   sw        $a2, 0x3C($sp)
/* BCE84 800BC284 8FA6003C */  lw         $a2, 0x3C($sp)
/* BCE88 800BC288 00001825 */  or         $v1, $zero, $zero
/* BCE8C 800BC28C ACC20000 */  sw         $v0, 0x0($a2)
.L800BC290:
/* BCE90 800BC290 8CD90000 */  lw         $t9, 0x0($a2)
/* BCE94 800BC294 0323C021 */  addu       $t8, $t9, $v1
/* BCE98 800BC298 A7000000 */  sh         $zero, 0x0($t8)
/* BCE9C 800BC29C 8CC80000 */  lw         $t0, 0x0($a2)
/* BCEA0 800BC2A0 01034821 */  addu       $t1, $t0, $v1
/* BCEA4 800BC2A4 A5200002 */  sh         $zero, 0x2($t1)
/* BCEA8 800BC2A8 8CCA0000 */  lw         $t2, 0x0($a2)
/* BCEAC 800BC2AC 01435821 */  addu       $t3, $t2, $v1
/* BCEB0 800BC2B0 A5600004 */  sh         $zero, 0x4($t3)
/* BCEB4 800BC2B4 8CCC0000 */  lw         $t4, 0x0($a2)
/* BCEB8 800BC2B8 01836821 */  addu       $t5, $t4, $v1
/* BCEBC 800BC2BC 24630008 */  addiu      $v1, $v1, 0x8
/* BCEC0 800BC2C0 1470FFF3 */  bne        $v1, $s0, .L800BC290
/* BCEC4 800BC2C4 A5A00006 */   sh        $zero, 0x6($t5)
/* BCEC8 800BC2C8 24C60004 */  addiu      $a2, $a2, 0x4
/* BCECC 800BC2CC 00D1082B */  sltu       $at, $a2, $s1
/* BCED0 800BC2D0 5420FFE9 */  bnel       $at, $zero, .L800BC278
/* BCED4 800BC2D4 02802025 */   or        $a0, $s4, $zero
/* BCED8 800BC2D8 3C01803B */  lui        $at, %hi(gAudioResetPresetIdToLoad)
/* BCEDC 800BC2DC 3C0F803B */  lui        $t7, %hi(gAudioResetStatus)
/* BCEE0 800BC2E0 A0200501 */  sb         $zero, %lo(gAudioResetPresetIdToLoad)($at)
/* BCEE4 800BC2E4 25EF0500 */  addiu      $t7, $t7, %lo(gAudioResetStatus)
/* BCEE8 800BC2E8 240E0001 */  addiu      $t6, $zero, 0x1
/* BCEEC 800BC2EC 0C02E803 */  jal        audio_shut_down_and_reset_step
/* BCEF0 800BC2F0 A1EE0000 */   sb        $t6, 0x0($t7)
/* BCEF4 800BC2F4 3C11803B */  lui        $s1, %hi(gSeqFileHeader)
/* BCEF8 800BC2F8 3C1200BC */  lui        $s2, %hi(_sequencesSegmentRomStart)
/* BCEFC 800BC2FC 26526160 */  addiu      $s2, $s2, %lo(_sequencesSegmentRomStart)
/* BCF00 800BC300 2631706C */  addiu      $s1, $s1, %lo(gSeqFileHeader)
/* BCF04 800BC304 27B50060 */  addiu      $s5, $sp, 0x60
/* BCF08 800BC308 AE350000 */  sw         $s5, 0x0($s1)
/* BCF0C 800BC30C 02A02825 */  or         $a1, $s5, $zero
/* BCF10 800BC310 02402025 */  or         $a0, $s2, $zero
/* BCF14 800BC314 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BCF18 800BC318 24060010 */   addiu     $a2, $zero, 0x10
/* BCF1C 800BC31C 8E390000 */  lw         $t9, 0x0($s1)
/* BCF20 800BC320 3C02803B */  lui        $v0, %hi(gSequenceCount)
/* BCF24 800BC324 2401FFF0 */  addiu      $at, $zero, -0x10
/* BCF28 800BC328 87380002 */  lh         $t8, 0x2($t9)
/* BCF2C 800BC32C 2442707C */  addiu      $v0, $v0, %lo(gSequenceCount)
/* BCF30 800BC330 02802025 */  or         $a0, $s4, $zero
/* BCF34 800BC334 3310FFFF */  andi       $s0, $t8, 0xFFFF
/* BCF38 800BC338 001040C0 */  sll        $t0, $s0, 3
/* BCF3C 800BC33C 01008025 */  or         $s0, $t0, $zero
/* BCF40 800BC340 26100013 */  addiu      $s0, $s0, 0x13
/* BCF44 800BC344 02012824 */  and        $a1, $s0, $at
/* BCF48 800BC348 00A08025 */  or         $s0, $a1, $zero
/* BCF4C 800BC34C 0C02E3FF */  jal        soundAlloc
/* BCF50 800BC350 A4580000 */   sh        $t8, 0x0($v0)
/* BCF54 800BC354 AE220000 */  sw         $v0, 0x0($s1)
/* BCF58 800BC358 02402025 */  or         $a0, $s2, $zero
/* BCF5C 800BC35C 00402825 */  or         $a1, $v0, $zero
/* BCF60 800BC360 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BCF64 800BC364 02003025 */   or        $a2, $s0, $zero
/* BCF68 800BC368 8E240000 */  lw         $a0, 0x0($s1)
/* BCF6C 800BC36C 0C02ED0F */  jal        func_800BB43C
/* BCF70 800BC370 02402825 */   or        $a1, $s2, $zero
/* BCF74 800BC374 3C110096 */  lui        $s1, %hi(_audio_banksSegmentRomStart)
/* BCF78 800BC378 3C12803B */  lui        $s2, %hi(gAlCtlHeader)
/* BCF7C 800BC37C 26527070 */  addiu      $s2, $s2, %lo(gAlCtlHeader)
/* BCF80 800BC380 26316460 */  addiu      $s1, $s1, %lo(_audio_banksSegmentRomStart)
/* BCF84 800BC384 AE550000 */  sw         $s5, 0x0($s2)
/* BCF88 800BC388 02202025 */  or         $a0, $s1, $zero
/* BCF8C 800BC38C 02A02825 */  or         $a1, $s5, $zero
/* BCF90 800BC390 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BCF94 800BC394 24060010 */   addiu     $a2, $zero, 0x10
/* BCF98 800BC398 8E4A0000 */  lw         $t2, 0x0($s2)
/* BCF9C 800BC39C 2401FFF0 */  addiu      $at, $zero, -0x10
/* BCFA0 800BC3A0 02802025 */  or         $a0, $s4, $zero
/* BCFA4 800BC3A4 85530002 */  lh         $s3, 0x2($t2)
/* BCFA8 800BC3A8 001380C0 */  sll        $s0, $s3, 3
/* BCFAC 800BC3AC 26100013 */  addiu      $s0, $s0, 0x13
/* BCFB0 800BC3B0 02012824 */  and        $a1, $s0, $at
/* BCFB4 800BC3B4 0C02E3FF */  jal        soundAlloc
/* BCFB8 800BC3B8 00A08025 */   or        $s0, $a1, $zero
/* BCFBC 800BC3BC AE420000 */  sw         $v0, 0x0($s2)
/* BCFC0 800BC3C0 02202025 */  or         $a0, $s1, $zero
/* BCFC4 800BC3C4 00402825 */  or         $a1, $v0, $zero
/* BCFC8 800BC3C8 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BCFCC 800BC3CC 02003025 */   or        $a2, $s0, $zero
/* BCFD0 800BC3D0 8E440000 */  lw         $a0, 0x0($s2)
/* BCFD4 800BC3D4 0C02ED0F */  jal        func_800BB43C
/* BCFD8 800BC3D8 02202825 */   or        $a1, $s1, $zero
/* BCFDC 800BC3DC 00132880 */  sll        $a1, $s3, 2
/* BCFE0 800BC3E0 00B32823 */  subu       $a1, $a1, $s3
/* BCFE4 800BC3E4 00052880 */  sll        $a1, $a1, 2
/* BCFE8 800BC3E8 0C02E3FF */  jal        soundAlloc
/* BCFEC 800BC3EC 02802025 */   or        $a0, $s4, $zero
/* BCFF0 800BC3F0 3C07803B */  lui        $a3, %hi(gCtlEntries)
/* BCFF4 800BC3F4 24E77080 */  addiu      $a3, $a3, %lo(gCtlEntries)
/* BCFF8 800BC3F8 ACE20000 */  sw         $v0, 0x0($a3)
/* BCFFC 800BC3FC 1A600019 */  blez       $s3, .L800BC464
/* BD000 800BC400 00001825 */   or        $v1, $zero, $zero
/* BD004 800BC404 00008825 */  or         $s1, $zero, $zero
/* BD008 800BC408 00008025 */  or         $s0, $zero, $zero
.L800BC40C:
/* BD00C 800BC40C 8E4C0000 */  lw         $t4, 0x0($s2)
/* BD010 800BC410 02A02825 */  or         $a1, $s5, $zero
/* BD014 800BC414 24060010 */  addiu      $a2, $zero, 0x10
/* BD018 800BC418 01916821 */  addu       $t5, $t4, $s1
/* BD01C 800BC41C 8DA40004 */  lw         $a0, 0x4($t5)
/* BD020 800BC420 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BD024 800BC424 AFA3008C */   sw        $v1, 0x8C($sp)
/* BD028 800BC428 3C07803B */  lui        $a3, %hi(gCtlEntries)
/* BD02C 800BC42C 24E77080 */  addiu      $a3, $a3, %lo(gCtlEntries)
/* BD030 800BC430 8CEF0000 */  lw         $t7, 0x0($a3)
/* BD034 800BC434 8FAE0060 */  lw         $t6, 0x60($sp)
/* BD038 800BC438 8FA3008C */  lw         $v1, 0x8C($sp)
/* BD03C 800BC43C 01F0C821 */  addu       $t9, $t7, $s0
/* BD040 800BC440 A32E0001 */  sb         $t6, 0x1($t9)
/* BD044 800BC444 8CE80000 */  lw         $t0, 0x0($a3)
/* BD048 800BC448 8FB80064 */  lw         $t8, 0x64($sp)
/* BD04C 800BC44C 24630001 */  addiu      $v1, $v1, 0x1
/* BD050 800BC450 01104821 */  addu       $t1, $t0, $s0
/* BD054 800BC454 2610000C */  addiu      $s0, $s0, 0xC
/* BD058 800BC458 26310008 */  addiu      $s1, $s1, 0x8
/* BD05C 800BC45C 1473FFEB */  bne        $v1, $s3, .L800BC40C
/* BD060 800BC460 A1380002 */   sb        $t8, 0x2($t1)
.L800BC464:
/* BD064 800BC464 3C11803B */  lui        $s1, %hi(gAlTbl)
/* BD068 800BC468 3C120098 */  lui        $s2, %hi(_audio_tablesSegmentRomStart)
/* BD06C 800BC46C 26529CA0 */  addiu      $s2, $s2, %lo(_audio_tablesSegmentRomStart)
/* BD070 800BC470 26317074 */  addiu      $s1, $s1, %lo(gAlTbl)
/* BD074 800BC474 AE350000 */  sw         $s5, 0x0($s1)
/* BD078 800BC478 02402025 */  or         $a0, $s2, $zero
/* BD07C 800BC47C 02A02825 */  or         $a1, $s5, $zero
/* BD080 800BC480 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BD084 800BC484 24060010 */   addiu     $a2, $zero, 0x10
/* BD088 800BC488 8E2A0000 */  lw         $t2, 0x0($s1)
/* BD08C 800BC48C 2401FFF0 */  addiu      $at, $zero, -0x10
/* BD090 800BC490 02802025 */  or         $a0, $s4, $zero
/* BD094 800BC494 85500002 */  lh         $s0, 0x2($t2)
/* BD098 800BC498 001058C0 */  sll        $t3, $s0, 3
/* BD09C 800BC49C 01608025 */  or         $s0, $t3, $zero
/* BD0A0 800BC4A0 26100013 */  addiu      $s0, $s0, 0x13
/* BD0A4 800BC4A4 02012824 */  and        $a1, $s0, $at
/* BD0A8 800BC4A8 0C02E3FF */  jal        soundAlloc
/* BD0AC 800BC4AC 00A08025 */   or        $s0, $a1, $zero
/* BD0B0 800BC4B0 AE220000 */  sw         $v0, 0x0($s1)
/* BD0B4 800BC4B4 02402025 */  or         $a0, $s2, $zero
/* BD0B8 800BC4B8 00402825 */  or         $a1, $v0, $zero
/* BD0BC 800BC4BC 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BD0C0 800BC4C0 02003025 */   or        $a2, $s0, $zero
/* BD0C4 800BC4C4 8E240000 */  lw         $a0, 0x0($s1)
/* BD0C8 800BC4C8 0C02ED0F */  jal        func_800BB43C
/* BD0CC 800BC4CC 02402825 */   or        $a1, $s2, $zero
/* BD0D0 800BC4D0 02802025 */  or         $a0, $s4, $zero
/* BD0D4 800BC4D4 0C02E3FF */  jal        soundAlloc
/* BD0D8 800BC4D8 24050100 */   addiu     $a1, $zero, 0x100
/* BD0DC 800BC4DC 3C03803B */  lui        $v1, %hi(gAlBankSets)
/* BD0E0 800BC4E0 24637078 */  addiu      $v1, $v1, %lo(gAlBankSets)
/* BD0E4 800BC4E4 3C0400BF */  lui        $a0, %hi(_instrument_setsSegmentRomStart+0x20)
/* BD0E8 800BC4E8 AC620000 */  sw         $v0, 0x0($v1)
/* BD0EC 800BC4EC 248493E0 */  addiu      $a0, $a0, %lo(_instrument_setsSegmentRomStart+0x20)
/* BD0F0 800BC4F0 00402825 */  or         $a1, $v0, $zero
/* BD0F4 800BC4F4 0C02EAA0 */  jal        audio_dma_copy_immediate
/* BD0F8 800BC4F8 24060100 */   addiu     $a2, $zero, 0x100
/* BD0FC 800BC4FC 3C10800F */  lui        $s0, %hi(D_800EA5D8)
/* BD100 800BC500 2610A718 */  addiu      $s0, $s0, %lo(D_800EA5D8)
/* BD104 800BC504 8E050000 */  lw         $a1, 0x0($s0)
/* BD108 800BC508 0C02E3FF */  jal        soundAlloc
/* BD10C 800BC50C 02802025 */   or        $a0, $s4, $zero
/* BD110 800BC510 3C04803B */  lui        $a0, %hi(gUnkPool1)
/* BD114 800BC514 248401B8 */  addiu      $a0, $a0, %lo(gUnkPool1)
/* BD118 800BC518 00402825 */  or         $a1, $v0, $zero
/* BD11C 800BC51C 0C02E41F */  jal        sound_alloc_pool_init
/* BD120 800BC520 8E060000 */   lw        $a2, 0x0($s0)
/* BD124 800BC524 0C030499 */  jal        init_sequence_players
/* BD128 800BC528 00000000 */   nop
/* BD12C 800BC52C 3C0D7655 */  lui        $t5, (0x76557364 >> 16)
/* BD130 800BC530 3C0F800F */  lui        $t7, %hi(gAudioLoadLock)
/* BD134 800BC534 25EFA71C */  addiu      $t7, $t7, %lo(gAudioLoadLock)
/* BD138 800BC538 35AD7364 */  ori        $t5, $t5, (0x76557364 & 0xFFFF)
/* BD13C 800BC53C ADED0000 */  sw         $t5, 0x0($t7)
/* BD140 800BC540 8FBF002C */  lw         $ra, 0x2C($sp)
/* BD144 800BC544 8FB50028 */  lw         $s5, 0x28($sp)
/* BD148 800BC548 8FB40024 */  lw         $s4, 0x24($sp)
/* BD14C 800BC54C 8FB30020 */  lw         $s3, 0x20($sp)
/* BD150 800BC550 8FB2001C */  lw         $s2, 0x1C($sp)
/* BD154 800BC554 8FB10018 */  lw         $s1, 0x18($sp)
/* BD158 800BC558 8FB00014 */  lw         $s0, 0x14($sp)
/* BD15C 800BC55C 03E00008 */  jr         $ra
/* BD160 800BC560 27BD0090 */   addiu     $sp, $sp, 0x90
/* BD164 800BC564 00000000 */  nop
/* BD168 800BC568 00000000 */  nop
/* BD16C 800BC56C 00000000 */  nop
