# Assembly Macros

.set K0BASE, 0x80000000
.set K1BASE, 0xA0000000
.set K2BASE, 0xC0000000

.macro glabel label
    .global \label
    .balign 4
    \label:
.endm

.macro export x
	.global	\x
\x:
.endm

.macro leaf x
	.globl	\x
	.ent	\x,0
\x:
	.frame	$sp,0,$ra
.endm

.macro end proc
	.end	\proc
.endm

.macro stay2 stmnt arg1
    .set noreorder
    \stmnt, \arg1
    .set reorder
.endm

.macro .word32 x
    .word \x
.endm

.set	C0_INX,		$0
.set	C0_RAND,		$1
.set	C0_ENTRYLO0,	$2
.set	C0_ENTRYLO1,	$3
.set	C0_CONTEXT,	$4
.set	C0_PAGEMASK,	$5		/* page mask */
.set	C0_WIRED,	$6		/* # wired entries in tlb */
.set	C0_BADVADDR,	$8
.set	C0_COUNT,	$9		/* free-running counter */
.set	C0_ENTRYHI,	$10
.set	C0_SR,		$12
.set	C0_CAUSE,	$13
.set	C0_EPC,		$14
.set	C0_PRID,		$15		/* revision identifier */
.set	C0_COMPARE,	$11		/* counter comparison reg. */
.set	C0_CONFIG,	$16		/* hardware configuration */
.set	C0_LLADDR,	$17		/* load linked address */
.set	C0_WATCHLO,	$18		/* watchpoint */
.set	C0_WATCHHI,	$19		/* watchpoint */
.set	C0_ECC,		$26		/* S-cache ECC and primary parity */
.set	C0_CACHE_ERR,	$27		/* cache error status */
.set	C0_TAGLO,	$28		/* cache operations */
.set	C0_TAGHI,	$29		/* cache operations */
.set	C0_ERROR_EPC,	$30		/* ECC error prg. counter */

.set THREAD_NEXT, 0
.set THREAD_PRI, 4
.set THREAD_QUEUE, 8
.set THREAD_TLNEXT, 12
.set THREAD_STATE, 16
.set THREAD_FLAGS, 18
.set THREAD_ID, 20
.set THREAD_FP, 24
.set THREAD_AT, 32
.set THREAD_V0, 40
.set THREAD_V1, 48
.set THREAD_A0, 56
.set THREAD_A1, 64
.set THREAD_A2, 72
.set THREAD_A3, 80
.set THREAD_T0, 88
.set THREAD_T1, 96
.set THREAD_T2, 104
.set THREAD_T3, 112
.set THREAD_T4, 120
.set THREAD_T5, 128
.set THREAD_T6, 136
.set THREAD_T7, 144
.set THREAD_S0, 152
.set THREAD_S1, 160
.set THREAD_S2, 168
.set THREAD_S3, 176
.set THREAD_S4, 184
.set THREAD_S5, 192
.set THREAD_S6, 200
.set THREAD_S7, 208
.set THREAD_T8, 216
.set THREAD_T9, 224
.set THREAD_GP, 232
.set THREAD_SP, 240
.set THREAD_S8, 248
.set THREAD_RA, 256
.set THREAD_LO, 264
.set THREAD_HI, 272
.set THREAD_SR, 280
.set THREAD_PC, 284
.set THREAD_CAUSE, 288
.set THREAD_BADVADDR, 292
.set THREAD_RCP, 296
.set THREAD_FPCSR, 300
.set THREAD_FP0, 304
.set THREAD_FP2, 312
.set THREAD_FP4, 320
.set THREAD_FP6, 328
.set THREAD_FP8, 336
.set THREAD_FP10, 344
.set THREAD_FP12, 352
.set THREAD_FP14, 360
.set THREAD_FP16, 368
.set THREAD_FP18, 376
.set THREAD_FP20, 384
.set THREAD_FP22, 392
.set THREAD_FP24, 400
.set THREAD_FP26, 408
.set THREAD_FP28, 416
.set THREAD_FP30, 424

;#define MESG(type) (type << 3)

.set MQ_MTQUEUE, 0
.set MQ_FULLQUEUE, 4
.set MQ_VALIDCOUNT, 8
.set MQ_FIRST, 12
.set MQ_MSGCOUNT, 16
.set MQ_MSG, 20

.set OS_EVENTSTATE_MESSAGE_QUEUE, 0
.set OS_EVENTSTATE_MESSAGE, 4

/*
 * Status register
 */
.set	SR_CUMASK,	0xf0000000	/* coproc usable bits */

.set	SR_CU3,		0x80000000	/* Coprocessor 3 usable */
.set	SR_CU2,		0x40000000	/* Coprocessor 2 usable */
.set	SR_CU1,		0x20000000	/* Coprocessor 1 usable */
.set	SR_CU0,		0x10000000	/* Coprocessor 0 usable */
.set	SR_RP,		0x08000000	/* Reduced power (quarter speed) */
.set	SR_FR,		0x04000000	/* MIPS III FP register mode */
.set	SR_RE,		0x02000000	/* Reverse endian */
.set	SR_ITS,		0x01000000	/* Instruction trace support */
.set	SR_BEV,		0x00400000	/* Use boot exception vectors */
.set	SR_TS,		0x00200000	/* TLB shutdown */
.set	SR_SR,		0x00100000	/* Soft reset occured */
.set	SR_CH,		0x00040000	/* Cache hit for last 'cache' op */
.set	SR_CE,		0x00020000	/* Create ECC */
.set	SR_DE,		0x00010000	/* ECC of parity does not cause error */

/*
 * Interrupt enable bits
 * (NOTE: bits set to 1 enable the corresponding level interrupt)
 */
.set SR_IMASK,	0x0000ff00	/* Interrupt mask */
.set SR_IMASK8,	0x00000000	/* mask level 8 */
.set SR_IMASK7,	0x00008000	/* mask level 7 */
.set SR_IMASK6,	0x0000c000	/* mask level 6 */
.set SR_IMASK5,	0x0000e000	/* mask level 5 */
.set SR_IMASK4,	0x0000f000	/* mask level 4 */
.set SR_IMASK3,	0x0000f800	/* mask level 3 */
.set SR_IMASK2,	0x0000fc00	/* mask level 2 */
.set SR_IMASK1,	0x0000fe00	/* mask level 1 */
.set SR_IMASK0,	0x0000ff00	/* mask level 0 */

.set SR_IBIT8,	0x00008000	/* bit level 8 */
.set SR_IBIT7,	0x00004000	/* bit level 7 */
.set SR_IBIT6,	0x00002000	/* bit level 6 */
.set SR_IBIT5,	0x00001000	/* bit level 5 */
.set SR_IBIT4,	0x00000800	/* bit level 4 */
.set SR_IBIT3,	0x00000400	/* bit level 3 */
.set SR_IBIT2,	0x00000200	/* bit level 2 */
.set SR_IBIT1,	0x00000100	/* bit level 1 */

.set SR_IMASKSHIFT,	8

.set SR_KX,		0x00000080	/* extended-addr TLB vec in kernel */
.set SR_SX,		0x00000040	/* xtended-addr TLB vec supervisor */
.set SR_UX,		0x00000020	/* xtended-addr TLB vec in user mode */
.set SR_KSU_MASK,	0x00000018	/* mode mask */
.set SR_KSU_USR,	0x00000010	/* user mode */
.set SR_KSU_SUP,	0x00000008	/* supervisor mode */
.set SR_KSU_KER,	0x00000000	/* kernel mode */
.set SR_ERL,		0x00000004	/* Error level, 1=>cache error */
.set SR_EXL,		0x00000002	/* Exception level, 1=>exception */
.set SR_IE,		0x00000001	/* interrupt enable, 1=>enable */

/* kseg0 to kseg1 */
.set K0_TO_K1, 0xA0000000

/* kseg1 to kseg0 */
.set K1_TO_K0, 0x9FFFFFFF

/* kseg0 to physical */
.set K0_TO_PHYS, 0x1FFFFFFF

/* kseg1 to physical */
.set K1_TO_PHYS, 0x1FFFFFFF

/* direct mapped to physical */
.set KDM_TO_PHYS, 0x1FFFFFFF

/* physical to kseg0 */
.set PHYS_TO_K0, 0x80000000

/* physical to kseg1 */
.set PHYS_TO_K1, 0xA0000000

.set OS_STATE_STOPPED,    1
.set OS_STATE_RUNNABLE,    2
.set OS_STATE_RUNNING,    4
.set OS_STATE_WAITING,    8

/* Hardware exception codes */
.set	EXC_INT,		(0<<2)	/* interrupt */
.set	EXC_MOD,		(1<<2)	/* TLB mod */
.set	EXC_RMISS,	(2<<2)	/* Read TLB Miss */
.set	EXC_WMISS,	(3<<2)	/* Write TLB Miss */
.set	EXC_RADE,	(4<<2)	/* Read Address Error */
.set	EXC_WADE,	(5<<2)	/* Write Address Error */
.set	EXC_IBE,		(6<<2)	/* Instruction Bus Error */
.set	EXC_DBE,		(7<<2)	/* Data Bus Error */
.set	EXC_SYSCALL,	(8<<2)	/* SYSCALL */
.set	EXC_BREAK,	(9<<2)	/* BREAKpoint */
.set	EXC_II,		(10<<2)	/* Illegal Instruction */
.set	EXC_CPU,		(11<<2)	/* CoProcessor Unusable */
.set	EXC_OV,		(12<<2)	/* OVerflow */
.set	EXC_TRAP,	(13<<2)	/* Trap exception */
.set	EXC_VCEI,	(14<<2)	/* Virt. Coherency on Inst. fetch */
.set	EXC_FPE,		(15<<2)	/* Floating Point Exception */
.set	EXC_WATCH,	(23<<2)	/* Watchpoint reference */
.set	EXC_VCED,	(31<<2)	/* Virt. Coherency on data read */

/*
 * Cause Register
 */
.set CAUSE_BD,	0x80000000	/* Branch delay slot */
.set CAUSE_CEMASK,	0x30000000	/* coprocessor error */
.set CAUSE_CESHIFT,	28

/* Interrupt pending bits */
.set CAUSE_IP8,	0x00008000	/* External level 8 pending - COMPARE */
.set CAUSE_IP7,	0x00004000	/* External level 7 pending - INT4 */
.set CAUSE_IP6,	0x00002000	/* External level 6 pending - INT3 */
.set CAUSE_IP5,	0x00001000	/* External level 5 pending - INT2 */
.set CAUSE_IP4,	0x00000800	/* External level 4 pending - INT1 */
.set CAUSE_IP3,	0x00000400	/* External level 3 pending - INT0 */
.set CAUSE_SW2,	0x00000200	/* Software level 2 pending */
.set CAUSE_SW1,	0x00000100	/* Software level 1 pending */

.set CAUSE_IPMASK,	0x0000FF00	/* Pending interrupt mask */
.set CAUSE_IPSHIFT,	8

.set CAUSE_EXCMASK,	0x0000007C	/* Cause code bits */

.set CAUSE_EXCSHIFT,	2

.set OS_EVENT_SW1,              0     /* CPU SW1 interrupt */
.set OS_EVENT_SW2,              1     /* CPU SW2 interrupt */
.set OS_EVENT_CART,             2     /* Cartridge interrupt: used by rmon */
.set OS_EVENT_COUNTER,          3     /* Counter int: used by VI/Timer Mgr */
.set OS_EVENT_SP,               4     /* SP task done interrupt */
.set OS_EVENT_SI,               5     /* SI (controller) interrupt */
.set OS_EVENT_AI,               6     /* AI interrupt */
.set OS_EVENT_VI,               7     /* VI interrupt: used by VI/Timer Mgr */
.set OS_EVENT_PI,               8     /* PI interrupt: used by PI Manager */
.set OS_EVENT_DP,               9     /* DP full sync interrupt */
.set OS_EVENT_CPU_BREAK,        10    /* CPU breakpoint: used by rmon */
.set OS_EVENT_SP_BREAK,         11    /* SP breakpoint:  used by rmon */
.set OS_EVENT_FAULT,            12    /* CPU fault event: used by rmon */
.set OS_EVENT_THREADSTATUS,     13    /* CPU thread status: used by rmon */
.set OS_EVENT_PRENMI,           14    /* Pre NMI interrupt */
#ifndef _FINALROM
.set OS_EVENT_RDB_READ_DONE,    15    /* RDB read ok event: used by rmon */
.set OS_EVENT_RDB_LOG_DONE,     16    /* read of log data complete */
.set OS_EVENT_RDB_DATA_DONE,    17    /* read of hostio data complete */
.set OS_EVENT_RDB_REQ_RAMROM,   18    /* host needs ramrom access */
.set OS_EVENT_RDB_FREE_RAMROM,  19    /* host is done with ramrom access */
.set OS_EVENT_RDB_DBG_DONE,     20
.set OS_EVENT_RDB_FLUSH_PROF,   21
.set OS_EVENT_RDB_ACK_PROF,     22

/* Flags for debugging purpose */

.set OS_FLAG_CPU_BREAK,	1	/* Break exception has occurred */
.set OS_FLAG_FAULT,		2	/* CPU fault has occurred */

/* Interrupt masks */

.set OS_IM_NONE,	0x00000001
.set OS_IM_SW1,	0x00000501
.set OS_IM_SW2,	0x00000601
.set OS_IM_CART,	0x00000c01
.set OS_IM_PRENMI,	0x00001401
.set OS_IM_RDBWRITE,	0x00002401
.set OS_IM_RDBREAD,	0x00004401
.set OS_IM_COUNTER,	0x00008401
.set OS_IM_CPU,	0x0000ff01
.set OS_IM_SP,	0x00010401
.set OS_IM_SI,	0x00020401
.set OS_IM_AI,	0x00040401
.set OS_IM_VI,	0x00080401
.set OS_IM_PI,	0x00100401
.set OS_IM_DP,	0x00200401
.set OS_IM_ALL,	0x003fff01
.set RCP_IMASK,	0x003f0000
.set RCP_IMASKSHIFT,	16

/* Recommended thread priorities for the system threads */

.set OS_PRIORITY_MAX,		255
.set OS_PRIORITY_VIMGR,	254
.set OS_PRIORITY_RMON,	250
.set OS_PRIORITY_RMONSPIN,	200
.set OS_PRIORITY_PIMGR,	150
.set OS_PRIORITY_SIMGR,	140
.set OS_PRIORITY_APPMAX,	127
.set OS_PRIORITY_IDLE,	  0	/* Must be 0 */


/* Flags to turn blocking on/off when sending/receiving message */

.set OS_MESG_NOBLOCK,		0
.set OS_MESG_BLOCK,		1

/* Flags to indicate direction of data transfer */

.set OS_READ,			0		/* device -> RDRAM */
.set OS_WRITE,		1		/* device <- RDRAM */
.set OS_OTHERS,		2		/* for Leo disk only */

/*
 * I/O message types
 */
.set OS_MESG_TYPE_BASE,	(10)
.set OS_MESG_TYPE_LOOPBACK,	(OS_MESG_TYPE_BASE+0)
.set OS_MESG_TYPE_DMAREAD,	(OS_MESG_TYPE_BASE+1)
.set OS_MESG_TYPE_DMAWRITE,	(OS_MESG_TYPE_BASE+2)
.set OS_MESG_TYPE_VRETRACE,	(OS_MESG_TYPE_BASE+3)
.set OS_MESG_TYPE_COUNTER,	(OS_MESG_TYPE_BASE+4)
.set OS_MESG_TYPE_EDMAREAD,	(OS_MESG_TYPE_BASE+5)
.set OS_MESG_TYPE_EDMAWRITE,	(OS_MESG_TYPE_BASE+6)

/*
 * I/O message priority
 */
.set OS_MESG_PRI_NORMAL,	0
.set OS_MESG_PRI_HIGH,	1

/*
 * Page size argument for TLB routines
 */
.set OS_PM_4K,	0x0000000
.set OS_PM_16K,	0x0006000
.set OS_PM_64K,	0x001e000
.set OS_PM_256K,	0x007e000
.set OS_PM_1M,	0x01fe000
.set OS_PM_4M,	0x07fe000
.set OS_PM_16M,	0x1ffe000

/*
 * Stack size for I/O device managers: PIM (PI Manager), VIM (VI Manager),
 *	SIM (SI Manager)
 *
 */
.set OS_PIM_STACKSIZE,	4096
.set OS_VIM_STACKSIZE,	4096
.set OS_SIM_STACKSIZE,	4096

.set OS_MIN_STACKSIZE,	72

/*
 * Values for osTvType 
 */
.set OS_TV_PAL,		0
.set OS_TV_NTSC,		1
.set OS_TV_MPAL,		2

/*
 * Video Interface (VI) mode type
 */
.set OS_VI_NTSC_LPN1,		0	/* NTSC */
.set OS_VI_NTSC_LPF1,		1
.set OS_VI_NTSC_LAN1,		2
.set OS_VI_NTSC_LAF1,		3
.set OS_VI_NTSC_LPN2,		4
.set OS_VI_NTSC_LPF2,		5
.set OS_VI_NTSC_LAN2,		6
.set OS_VI_NTSC_LAF2,		7
.set OS_VI_NTSC_HPN1,		8
.set OS_VI_NTSC_HPF1,		9
.set OS_VI_NTSC_HAN1,		10
.set OS_VI_NTSC_HAF1,		11
.set OS_VI_NTSC_HPN2,		12
.set OS_VI_NTSC_HPF2,		13

.set OS_VI_PAL_LPN1,		14	/* PAL */
.set OS_VI_PAL_LPF1,		15
.set OS_VI_PAL_LAN1,		16
.set OS_VI_PAL_LAF1,		17
.set OS_VI_PAL_LPN2,		18
.set OS_VI_PAL_LPF2,		19
.set OS_VI_PAL_LAN2,		20
.set OS_VI_PAL_LAF2,		21
.set OS_VI_PAL_HPN1,		22
.set OS_VI_PAL_HPF1,		23
.set OS_VI_PAL_HAN1,		24
.set OS_VI_PAL_HAF1,		25
.set OS_VI_PAL_HPN2,		26
.set OS_VI_PAL_HPF2,		27

.set OS_VI_MPAL_LPN1,		28	/* MPAL - mainly Brazil */
.set OS_VI_MPAL_LPF1,		29
.set OS_VI_MPAL_LAN1,		30
.set OS_VI_MPAL_LAF1,		31
.set OS_VI_MPAL_LPN2,		32
.set OS_VI_MPAL_LPF2,		33
.set OS_VI_MPAL_LAN2,		34
.set OS_VI_MPAL_LAF2,		35
.set OS_VI_MPAL_HPN1,		36
.set OS_VI_MPAL_HPF1,		37
.set OS_VI_MPAL_HAN1,		38
.set OS_VI_MPAL_HAF1,		39
.set OS_VI_MPAL_HPN2,		40
.set OS_VI_MPAL_HPF2,		41

/*
 * Video Interface (VI) special features
 */
.set OS_VI_GAMMA_ON,			0x0001
.set OS_VI_GAMMA_OFF,			0x0002
.set OS_VI_GAMMA_DITHER_ON,		0x0004
.set OS_VI_GAMMA_DITHER_OFF,		0x0008
.set OS_VI_DIVOT_ON,			0x0010
.set OS_VI_DIVOT_OFF,			0x0020
.set OS_VI_DITHER_FILTER_ON,		0x0040
.set OS_VI_DITHER_FILTER_OFF,		0x0080

/*
 * Video Interface (VI) mode attribute bit
 */
.set OS_VI_BIT_NONINTERLACE,		0x0001          /* lo-res */
.set OS_VI_BIT_INTERLACE,		0x0002          /* lo-res */
.set OS_VI_BIT_NORMALINTERLACE,	0x0004          /* hi-res */
.set OS_VI_BIT_DEFLICKINTERLACE,	0x0008          /* hi-res */
.set OS_VI_BIT_ANTIALIAS,		0x0010
.set OS_VI_BIT_POINTSAMPLE,		0x0020
.set OS_VI_BIT_16PIXEL,		0x0040
.set OS_VI_BIT_32PIXEL,		0x0080
.set OS_VI_BIT_LORES,			0x0100
.set OS_VI_BIT_HIRES,			0x0200
.set OS_VI_BIT_NTSC,			0x0400
.set OS_VI_BIT_PAL,			0x0800

/*************************************************************************
 * SP Memory
 */
.set SP_DMEM_START,		0x04000000	/* read/write */
.set SP_DMEM_END,		0x04000FFF
.set SP_IMEM_START,		0x04001000	/* read/write */
.set SP_IMEM_END,		0x04001FFF

/*************************************************************************
 * SP CP0 Registers
 */

.set SP_BASE_REG,		0x04040000

/* SP memory address (R/W): [11:0] DMEM/IMEM address; [12] 0=DMEM,1=IMEM */
.set SP_MEM_ADDR_REG,		(SP_BASE_REG+0x00)	/* Master */
						
/* SP DRAM DMA address (R/W): [23:0] RDRAM address */
.set SP_DRAM_ADDR_REG,	(SP_BASE_REG+0x04)	/* Slave */

/* SP read DMA length (R/W): [11:0] length, [19:12] count, [31:20] skip */
/* direction: I/DMEM <- RDRAM */
.set SP_RD_LEN_REG,		(SP_BASE_REG+0x08)	/* R/W: read len */

/* SP write DMA length (R/W): [11:0] length, [19:12] count, [31:20] skip */
/* direction: I/DMEM -> RDRAM */
.set SP_WR_LEN_REG,		(SP_BASE_REG+0x0C)	/* R/W: write len */

/* SP status (R/W): [14:0] valid bits; see below for write/read mode */
.set SP_STATUS_REG,		(SP_BASE_REG+0x10)

/* SP DMA full (R): [0] valid bit; dma full */
.set SP_DMA_FULL_REG,		(SP_BASE_REG+0x14)

/* SP DMA busy (R): [0] valid bit; dma busy */
.set SP_DMA_BUSY_REG,		(SP_BASE_REG+0x18)

/* SP semaphore (R/W): Read:  [0] semaphore flag (set on read) */
/*                     Write: [] clear semaphore flag */
.set SP_SEMAPHORE_REG,	(SP_BASE_REG+0x1C)

/* SP PC (R/W): [11:0] program counter */
.set SP_PC_REG,		0x04080000

/* SP MEM address: bit 12 specifies if address is IMEM or DMEM */
.set SP_DMA_DMEM,		0x0000		/* Bit 12: 0=DMEM, 1=IMEM */
.set SP_DMA_IMEM,		0x1000		/* Bit 12: 0=DMEM, 1=IMEM */

/*
 * Values to clear/set bit in status reg (SP_STATUS_REG - write)
 */
.set SP_CLR_HALT,		0x00001	    /* Bit  0: clear halt */
.set SP_SET_HALT,		0x00002	    /* Bit  1: set halt */
.set SP_CLR_BROKE,		0x00004	    /* Bit  2: clear broke */
.set SP_CLR_INTR,		0x00008	    /* Bit  3: clear intr */
.set SP_SET_INTR,		0x00010	    /* Bit  4: set intr */
.set SP_CLR_SSTEP,		0x00020	    /* Bit  5: clear sstep */
.set SP_SET_SSTEP,		0x00040	    /* Bit  6: set sstep */
.set SP_CLR_INTR_BREAK,	0x00080	    /* Bit  7: clear intr on break */
.set SP_SET_INTR_BREAK,	0x00100	    /* Bit  8: set intr on break */
.set SP_CLR_SIG0,		0x00200	    /* Bit  9: clear signal 0 */
.set SP_SET_SIG0,		0x00400	    /* Bit 10: set signal 0 */
.set SP_CLR_SIG1,		0x00800	    /* Bit 11: clear signal 1 */
.set SP_SET_SIG1,		0x01000	    /* Bit 12: set signal 1 */
.set SP_CLR_SIG2,		0x02000	    /* Bit 13: clear signal 2 */
.set SP_SET_SIG2,		0x04000	    /* Bit 14: set signal 2 */
.set SP_CLR_SIG3,		0x08000	    /* Bit 15: clear signal 3 */
.set SP_SET_SIG3,		0x10000	    /* Bit 16: set signal 3 */
.set SP_CLR_SIG4,		0x20000	    /* Bit 17: clear signal 4 */
.set SP_SET_SIG4,		0x40000	    /* Bit 18: set signal 4 */
.set SP_CLR_SIG5,		0x80000	    /* Bit 19: clear signal 5 */
.set SP_SET_SIG5,	       0x100000	    /* Bit 20: set signal 5 */
.set SP_CLR_SIG6,	       0x200000	    /* Bit 21: clear signal 6 */
.set SP_SET_SIG6,	       0x400000	    /* Bit 22: set signal 6 */
.set SP_CLR_SIG7,	       0x800000	    /* Bit 23: clear signal 7 */
.set SP_SET_SIG7,	      0x1000000	    /* Bit 24: set signal 7 */

/*
 * Patterns to interpret status reg (SP_STATUS_REG - read)
 */
.set SP_STATUS_HALT,		0x001		/* Bit  0: halt */
.set SP_STATUS_BROKE,		0x002		/* Bit  1: broke */
.set SP_STATUS_DMA_BUSY,	0x004		/* Bit  2: dma busy */
.set SP_STATUS_DMA_FULL,	0x008		/* Bit  3: dma full */
.set SP_STATUS_IO_FULL,	0x010		/* Bit  4: io full */
.set SP_STATUS_SSTEP,		0x020		/* Bit  5: single step */
.set SP_STATUS_INTR_BREAK,	0x040		/* Bit  6: interrupt on break */
.set SP_STATUS_SIG0,		0x080		/* Bit  7: signal 0 set */
.set SP_STATUS_SIG1,		0x100		/* Bit  8: signal 1 set */
.set SP_STATUS_SIG2,		0x200		/* Bit  9: signal 2 set */
.set SP_STATUS_SIG3,		0x400		/* Bit 10: signal 3 set */
.set SP_STATUS_SIG4,		0x800		/* Bit 11: signal 4 set */
.set SP_STATUS_SIG5,	       0x1000		/* Bit 12: signal 5 set */
.set SP_STATUS_SIG6,	       0x2000		/* Bit 13: signal 6 set */
.set SP_STATUS_SIG7,	       0x4000		/* Bit 14: signal 7 set */

/* 
 * Use of SIG bits
 */
.set SP_CLR_YIELD,		SP_CLR_SIG0
.set SP_SET_YIELD,		SP_SET_SIG0
.set SP_STATUS_YIELD,		SP_STATUS_SIG0
.set SP_CLR_YIELDED,		SP_CLR_SIG1
.set SP_SET_YIELDED,		SP_SET_SIG1
.set SP_STATUS_YIELDED,	SP_STATUS_SIG1
.set SP_CLR_TASKDONE,		SP_CLR_SIG2
.set SP_SET_TASKDONE,		SP_SET_SIG2
.set SP_STATUS_TASKDONE,	SP_STATUS_SIG2
.set SP_CLR_RSPSIGNAL,	SP_CLR_SIG3
.set SP_SET_RSPSIGNAL,	SP_SET_SIG3
.set SP_STATUS_RSPSIGNAL,	SP_STATUS_SIG3
.set SP_CLR_CPUSIGNAL,	SP_CLR_SIG4
.set SP_SET_CPUSIGNAL,	SP_SET_SIG4
.set SP_STATUS_CPUSIGNAL,	SP_STATUS_SIG4

/* SP IMEM BIST REG (R/W): [6:0] BIST status bits; see below for detail */
.set SP_IBIST_REG,	0x04080004

/*
 * Patterns to interpret status reg (SP_BIST_REG - write)
 */
.set SP_IBIST_CHECK,		0x01		/* Bit 0: BIST check */
.set SP_IBIST_GO,		0x02		/* Bit 1: BIST go */
.set SP_IBIST_CLEAR,		0x04		/* Bit 2: BIST clear */

/*
 * Patterns to interpret status reg (SP_BIST_REG - read)
 */
/* First 2 bits are same as in write mode:  
 * Bit 0: BIST check; Bit 1: BIST go 
 */
.set SP_IBIST_DONE,		0x04		/* Bit 2: BIST done */
.set SP_IBIST_FAILED,		0x78		/* Bit [6:3]: BIST fail */


/*************************************************************************
 * DP Command Registers 
 */
.set DPC_BASE_REG,		0x04100000

/* DP CMD DMA start (R/W): [23:0] DMEM/RDRAM start address */
.set DPC_START_REG,		(DPC_BASE_REG+0x00)

/* DP CMD DMA end (R/W): [23:0] DMEM/RDRAM end address */
.set DPC_END_REG,		(DPC_BASE_REG+0x04)

/* DP CMD DMA end (R): [23:0] DMEM/RDRAM current address */
.set DPC_CURRENT_REG,		(DPC_BASE_REG+0x08)	

/* DP CMD status (R/W): [9:0] valid bits - see below for definitions */
.set DPC_STATUS_REG,		(DPC_BASE_REG+0x0C)

/* DP clock counter (R): [23:0] clock counter */
.set DPC_CLOCK_REG,		(DPC_BASE_REG+0x10)	

/* DP buffer busy counter (R): [23:0] clock counter */
.set DPC_BUFBUSY_REG,		(DPC_BASE_REG+0x14)

/* DP pipe busy counter (R): [23:0] clock counter */
.set DPC_PIPEBUSY_REG,	(DPC_BASE_REG+0x18)

/* DP TMEM load counter (R): [23:0] clock counter */
.set DPC_TMEM_REG,		(DPC_BASE_REG+0x1C)

/*
 * Values to clear/set bit in status reg (DPC_STATUS_REG - write)
 */
.set DPC_CLR_XBUS_DMEM_DMA,	0x0001		/* Bit 0: clear xbus_dmem_dma */
.set DPC_SET_XBUS_DMEM_DMA,	0x0002		/* Bit 1: set xbus_dmem_dma */
.set DPC_CLR_FREEZE,		0x0004		/* Bit 2: clear freeze */
.set DPC_SET_FREEZE,		0x0008		/* Bit 3: set freeze */
.set DPC_CLR_FLUSH,		0x0010		/* Bit 4: clear flush */
.set DPC_SET_FLUSH,		0x0020		/* Bit 5: set flush */
.set DPC_CLR_TMEM_CTR,	0x0040		/* Bit 6: clear tmem ctr */
.set DPC_CLR_PIPE_CTR,	0x0080		/* Bit 7: clear pipe ctr */
.set DPC_CLR_CMD_CTR,		0x0100		/* Bit 8: clear cmd ctr */
.set DPC_CLR_CLOCK_CTR,	0x0200		/* Bit 9: clear clock ctr */

/*
 * Patterns to interpret status reg (DPC_STATUS_REG - read)
 */
.set DPC_STATUS_XBUS_DMEM_DMA,	0x001	/* Bit  0: xbus_dmem_dma */
.set DPC_STATUS_FREEZE,		0x002	/* Bit  1: freeze */
.set DPC_STATUS_FLUSH,		0x004	/* Bit  2: flush */
/*#define DPC_STATUS_FROZEN		0x008*/	/* Bit  3: frozen */
.set DPC_STATUS_START_GCLK,		0x008	/* Bit  3: start gclk */
.set DPC_STATUS_TMEM_BUSY,		0x010	/* Bit  4: tmem busy */
.set DPC_STATUS_PIPE_BUSY,		0x020	/* Bit  5: pipe busy */
.set DPC_STATUS_CMD_BUSY,		0x040	/* Bit  6: cmd busy */
.set DPC_STATUS_CBUF_READY,		0x080	/* Bit  7: cbuf ready */
.set DPC_STATUS_DMA_BUSY,		0x100	/* Bit  8: dma busy */
.set DPC_STATUS_END_VALID,		0x200	/* Bit  9: end valid */
.set DPC_STATUS_START_VALID,		0x400	/* Bit 10: start valid */


/*************************************************************************
 * DP Span Registers 
 */
.set DPS_BASE_REG,		0x04200000

/* DP tmem bist (R/W): [10:0] BIST status bits; see below for detail */
.set DPS_TBIST_REG,		(DPS_BASE_REG+0x00)

/* DP span test mode (R/W): [0] Span buffer test access enable */
.set DPS_TEST_MODE_REG,	(DPS_BASE_REG+0x04)

/* DP span buffer test address (R/W): [6:0] bits; see below for detail */
.set DPS_BUFTEST_ADDR_REG,	(DPS_BASE_REG+0x08)

/* DP span buffer test data (R/W): [31:0] span buffer data */
.set DPS_BUFTEST_DATA_REG,	(DPS_BASE_REG+0x0C)

/*
 * Patterns to interpret status reg (DPS_TMEM_BIST_REG - write)
 */
.set DPS_TBIST_CHECK,		0x01		/* Bit 0: BIST check */
.set DPS_TBIST_GO,		0x02		/* Bit 1: BIST go */
.set DPS_TBIST_CLEAR,		0x04		/* Bit 2: BIST clear */

/*
 * Patterns to interpret status reg (DPS_TMEM_BIST_REG - read)
 */
/* First 2 bits are same as in write mode:  
 * Bit 0: BIST check; Bit 1: BIST go 
 */
.set DPS_TBIST_DONE,		0x004		/* Bit 2: BIST done */
.set DPS_TBIST_FAILED,	0x7F8		/* Bit [10:3]: BIST fail */


/*************************************************************************
 * MIPS Interface (MI) Registers 
 */
.set MI_BASE_REG,		0x04300000

/* 
 * MI init mode (W): [6:0] init length, [7] clear init mode, [8] set init mode
 *                   [9/10] clear/set ebus test mode, [11] clear DP interrupt
 *              (R): [6:0] init length, [7] init mode, [8] ebus test mode
 */
.set MI_INIT_MODE_REG,	(MI_BASE_REG+0x00)
.set MI_MODE_REG,		MI_INIT_MODE_REG

/*
 * Values to clear/set bit in mode reg (MI_MODE_REG - write)
 */
.set MI_CLR_INIT,		0x0080		/* Bit  7: clear init mode */
.set MI_SET_INIT,		0x0100		/* Bit  8: set init mode */
.set MI_CLR_EBUS,		0x0200		/* Bit  9: clear ebus test */
.set MI_SET_EBUS,		0x0400		/* Bit 10: set ebus test mode */
.set MI_CLR_DP_INTR,		0x0800		/* Bit 11: clear dp interrupt */
.set MI_CLR_RDRAM,		0x1000		/* Bit 12: clear RDRAM reg */
.set MI_SET_RDRAM,		0x2000		/* Bit 13: set RDRAM reg mode */

/*
 * Patterns to interpret mode reg (MI_MODE_REG - read)
 */
.set MI_MODE_INIT,		0x0080		/* Bit  7: init mode */
.set MI_MODE_EBUS,		0x0100		/* Bit  8: ebus test mode */
.set MI_MODE_RDRAM,		0x0200		/* Bit  9: RDRAM reg mode */

/* MI version (R): [7:0] io, [15:8] rac, [23:16] rdp, [31:24] rsp */
.set MI_VERSION_REG,		(MI_BASE_REG+0x04)
.set MI_NOOP_REG,		MI_VERSION_REG

/* MI interrupt (R): [5:0] valid bits - see below for bit patterns */
.set MI_INTR_REG,		(MI_BASE_REG+0x08)

/* 
 * MI interrupt mask (W): [11:0] valid bits - see below for bit patterns 
 *                   (R): [5:0] valid bits - see below for bit patterns 
 */
.set MI_INTR_MASK_REG,	(MI_BASE_REG+0x0C)

/*
 * The following are values to check for interrupt setting (MI_INTR_REG)
 */
.set MI_INTR_SP,		0x01		/* Bit 0: SP intr */
.set MI_INTR_SI,		0x02		/* Bit 1: SI intr */
.set MI_INTR_AI,		0x04		/* Bit 2: AI intr */
.set MI_INTR_VI,		0x08		/* Bit 3: VI intr */
.set MI_INTR_PI,		0x10		/* Bit 4: PI intr */
.set MI_INTR_DP,		0x20		/* Bit 5: DP intr */

/*
 * The following are values to clear/set various interrupt bit mask
 * They can be ORed together to manipulate multiple bits 
 * (MI_INTR_MASK_REG - write)
 */
.set MI_INTR_MASK_CLR_SP,	0x0001		/* Bit  0: clear SP mask */
.set MI_INTR_MASK_SET_SP,	0x0002		/* Bit  1: set SP mask */
.set MI_INTR_MASK_CLR_SI,	0x0004		/* Bit  2: clear SI mask */
.set MI_INTR_MASK_SET_SI,	0x0008		/* Bit  3: set SI mask */
.set MI_INTR_MASK_CLR_AI,	0x0010		/* Bit  4: clear AI mask */
.set MI_INTR_MASK_SET_AI,	0x0020		/* Bit  5: set AI mask */
.set MI_INTR_MASK_CLR_VI,	0x0040		/* Bit  6: clear VI mask */
.set MI_INTR_MASK_SET_VI,	0x0080		/* Bit  7: set VI mask */
.set MI_INTR_MASK_CLR_PI,	0x0100		/* Bit  8: clear PI mask */
.set MI_INTR_MASK_SET_PI,	0x0200		/* Bit  9: set PI mask */
.set MI_INTR_MASK_CLR_DP,	0x0400		/* Bit 10: clear DP mask */
.set MI_INTR_MASK_SET_DP,	0x0800		/* Bit 11: set DP mask */

/*
 * The following are values to check for interrupt mask setting 
 * (MI_INTR_MASK_REG - read)
 */
.set MI_INTR_MASK_SP,		0x01		/* Bit 0: SP intr mask */
.set MI_INTR_MASK_SI,		0x02		/* Bit 1: SI intr mask */
.set MI_INTR_MASK_AI,		0x04		/* Bit 2: AI intr mask */
.set MI_INTR_MASK_VI,		0x08		/* Bit 3: VI intr mask */
.set MI_INTR_MASK_PI,		0x10		/* Bit 4: PI intr mask */
.set MI_INTR_MASK_DP,		0x20		/* Bit 5: DP intr mask */


/*************************************************************************
 * Video Interface (VI) Registers 
 */
.set VI_BASE_REG,		0x04400000

/* VI status/control (R/W): [15-0] valid bits: 
 *	[1:0]   = type[1:0] (pixel size) 
 *			0: blank (no data, no sync)
 *                      1: reserved
 *                      2: 5/5/5/3 ("16" bit)
 *                      3: 8/8/8/8 (32 bit)
 *	[2]     = gamma_dither_enable (normally on, unless "special effect")
 *	[3]     = gamma_enable (normally on, unless MPEG/JPEG)
 *	[4]     = divot_enable (normally on if antialiased, unless decal lines)
 *	[5]     = reserved - always off
 *	[6]     = serrate (always on if interlaced, off if not)
 *	[7]     = reserved - diagnostics only
 *	[9:8]   = anti-alias (aa) mode[1:0] 
 *			0: aa & resamp (always fetch extra lines)
 *                      1: aa & resamp (fetch extra lines if needed)
 *                      2: resamp only (treat as all fully covered)
 *                      3: neither (replicate pixels, no interpolate)
 *	[11]    = reserved - diagnostics only
 *	[15:12] = reserved
 *
 */
.set VI_STATUS_REG,		(VI_BASE_REG+0x00)
.set VI_CONTROL_REG,		VI_STATUS_REG

/* VI origin (R/W): [23:0] frame buffer origin in bytes */
.set VI_ORIGIN_REG,		(VI_BASE_REG+0x04)
.set VI_DRAM_ADDR_REG,	VI_ORIGIN_REG

/* VI width (R/W): [11:0] frame buffer line width in pixels */
.set VI_WIDTH_REG,		(VI_BASE_REG+0x08)	
.set VI_H_WIDTH_REG,		VI_WIDTH_REG

/* VI vertical intr (R/W): [9:0] interrupt when current half-line = V_INTR */
.set VI_INTR_REG,		(VI_BASE_REG+0x0C)	
.set VI_V_INTR_REG,		VI_INTR_REG

/* 
 * VI current vertical line (R/W): [9:0] current half line, sampled once per
 *	line (the lsb of V_CURRENT is constant within a field, and in
 *	interlaced modes gives the field number - which is constant for non-
 *	interlaced modes)
 * 	- Any write to this register will clear interrupt line
 */
.set VI_CURRENT_REG,		(VI_BASE_REG+0x10)	
.set VI_V_CURRENT_LINE_REG,	VI_CURRENT_REG

/* 
 * VI video timing (R/W): [ 7: 0] horizontal sync width in pixels, 
 *			  [15: 8] color burst width in pixels,
 *                        [19:16] vertical sync width in half lines,   
 *			  [29:20] start of color burst in pixels from h-sync
 */
.set VI_BURST_REG,		(VI_BASE_REG+0x14)	
.set VI_TIMING_REG,		VI_BURST_REG

/* VI vertical sync (R/W): [9:0] number of half-lines per field */
.set VI_V_SYNC_REG,		(VI_BASE_REG+0x18)	

/* VI horizontal sync (R/W): [11: 0] total duration of a line in 1/4 pixel 
 *			     [20:16] a 5-bit leap pattern used for PAL only 
 *				     (h_sync_period)
 */
.set VI_H_SYNC_REG,		(VI_BASE_REG+0x1C)	

/* 
 * VI horizontal sync leap (R/W): [11: 0] identical to h_sync_period
 *                                [27:16] identical to h_sync_period
 */
.set VI_LEAP_REG,		(VI_BASE_REG+0x20)	
.set VI_H_SYNC_LEAP_REG,	VI_LEAP_REG

/* 
 * VI horizontal video (R/W): [ 9: 0] end of active video in screen pixels
 *                          : [25:16] start of active video in screen pixels
 */
.set VI_H_START_REG,		(VI_BASE_REG+0x24)
.set VI_H_VIDEO_REG,		VI_H_START_REG

/* 
 * VI vertical video (R/W): [ 9: 0] end of active video in screen half-lines
 *                        : [25:16] start of active video in screen half-lines
 */
.set VI_V_START_REG,		(VI_BASE_REG+0x28)
.set VI_V_VIDEO_REG,		VI_V_START_REG

/* 
 * VI vertical burst (R/W): [ 9: 0] end of color burst enable in half-lines
 *                        : [25:16] start of color burst enable in half-lines
 */
.set VI_V_BURST_REG,		(VI_BASE_REG+0x2C)	

/* VI x-scale (R/W): [11: 0] 1/horizontal scale up factor (2.10 format)
 *		     [27:16] horizontal subpixel offset (2.10 format)
 */
.set VI_X_SCALE_REG,		(VI_BASE_REG+0x30)	

/* VI y-scale (R/W): [11: 0] 1/vertical scale up factor (2.10 format)
 *		     [27:16] vertical subpixel offset (2.10 format)
 */
.set VI_Y_SCALE_REG,		(VI_BASE_REG+0x34)	

/*
 * Patterns to interpret VI_CONTROL_REG
 */
.set VI_CTRL_TYPE_16,		 0x00002    /* Bit [1:0] pixel size: 16 bit */
.set VI_CTRL_TYPE_32,		 0x00003    /* Bit [1:0] pixel size: 32 bit */
.set VI_CTRL_GAMMA_DITHER_ON,	 0x00004    /* Bit 2: default = on */
.set VI_CTRL_GAMMA_ON,	 0x00008    /* Bit 3: default = on */
.set VI_CTRL_DIVOT_ON,	 0x00010    /* Bit 4: default = on */
.set VI_CTRL_SERRATE_ON,	 0x00040    /* Bit 6: on if interlaced */
.set VI_CTRL_ANTIALIAS_MASK,	 0x00300    /* Bit [9:8] anti-alias mode */
.set VI_CTRL_DITHER_FILTER_ON, 0x10000    /* Bit 16: dither-filter mode */

/*
 * Possible video clocks (NTSC or PAL)
 */
.set VI_NTSC_CLOCK,		48681812        /* Hz = 48.681812 MHz */
.set VI_PAL_CLOCK,		49656530        /* Hz = 49.656530 MHz */
.set VI_MPAL_CLOCK,		48628316        /* Hz = 48.628316 MHz */


/*************************************************************************
 * Audio Interface (AI) Registers 
 *
 * The address and length registers are double buffered; that is, they
 * can be written twice before becoming full.
 * The address must be written before the length.
 */
.set AI_BASE_REG,		0x04500000

/* AI DRAM address (W): [23:0] starting RDRAM address (8B-aligned) */
.set AI_DRAM_ADDR_REG,	(AI_BASE_REG+0x00)	/* R0: DRAM address */

/* AI length (R/W): [14:0] transfer length (v1.0) - Bottom 3 bits are ignored */
/*                  [17:0] transfer length (v2.0) - Bottom 3 bits are ignored */
.set AI_LEN_REG,		(AI_BASE_REG+0x04)	/* R1: Length */

/* AI control (W): [0] DMA enable - if LSB == 1, DMA is enabled */
.set AI_CONTROL_REG,		(AI_BASE_REG+0x08)	/* R2: DMA Control */

/* 
 * AI status (R): [31]/[0] ai_full (addr & len buffer full), [30] ai_busy 
 *		  Note that a 1->0 transition in ai_full will set interrupt
 *           (W): clear audio interrupt 
 */
.set AI_STATUS_REG,		(AI_BASE_REG+0x0C)	/* R3: Status */

/* 
 * AI DAC sample period register (W): [13:0] dac rate 
 *   - vid_clock/(dperiod + 1) is the DAC sample rate
 *   - (dperiod + 1) >= 66 * (aclockhp + 1) must be true
 */
.set AI_DACRATE_REG,		(AI_BASE_REG+0x10)	/* R4: DAC rate 14-lsb*/

/* 
 * AI bit rate (W): [3:0] bit rate (abus clock half period register - aclockhp)
 *   - vid_clock/(2 * (aclockhp + 1)) is the DAC clock rate
 *   - The abus clock stops if aclockhp is zero
 */
.set AI_BITRATE_REG,		(AI_BASE_REG+0x14)	/* R5: Bit rate 4-lsb */

/* Value for control register */
.set AI_CONTROL_DMA_ON,	0x01			/* LSB = 1: DMA enable*/
.set AI_CONTROL_DMA_OFF,	0x00			/* LSB = 1: DMA enable*/

/* Value for status register */
.set AI_STATUS_FIFO_FULL,	0x80000000		/* Bit 31: full */
.set AI_STATUS_DMA_BUSY,	0x40000000		/* Bit 30: busy */

/* DAC rate = video clock / audio frequency
 *   - DAC rate >= (66 * Bit rate) must be true
 */
.set AI_MAX_DAC_RATE,         16384           /* 14-bit+1 */
.set AI_MIN_DAC_RATE,         132

/* Bit rate <= (DAC rate / 66) */
.set AI_MAX_BIT_RATE,         16              /* 4-bit+1 */
.set AI_MIN_BIT_RATE,         2

/*
 * Maximum and minimum values for audio frequency based on video clocks
 *   max frequency = (video clock / min dac rate)
 *   min frequency = (video clock / max dac rate)
 */
.set AI_NTSC_MAX_FREQ,        368000          /* 368 KHz */
.set AI_NTSC_MIN_FREQ,        3000            /*   3 KHz ~ 2971 Hz */

.set AI_PAL_MAX_FREQ,         376000          /* 376 KHz */
.set AI_PAL_MIN_FREQ,         3050            /*   3 KHz ~ 3031 Hz */

.set AI_MPAL_MAX_FREQ,        368000          /* 368 KHz */
.set AI_MPAL_MIN_FREQ,        3000            /*   3 KHz ~ 2968 Hz */


/*************************************************************************
 * Peripheral Interface (PI) Registers 
 */
.set PI_BASE_REG,		0x04600000

/* PI DRAM address (R/W): [23:0] starting RDRAM address */
.set PI_DRAM_ADDR_REG,	(PI_BASE_REG+0x00)	/* DRAM address */

/* PI pbus (cartridge) address (R/W): [31:0] starting AD16 address */
.set PI_CART_ADDR_REG,	(PI_BASE_REG+0x04)

/* PI read length (R/W): [23:0] read data length */
.set PI_RD_LEN_REG,		(PI_BASE_REG+0x08)

/* PI write length (R/W): [23:0] write data length */
.set PI_WR_LEN_REG,		(PI_BASE_REG+0x0C)

/* 
 * PI status (R): [0] DMA busy, [1] IO busy, [2], error
 *           (W): [0] reset controller (and abort current op), [1] clear intr
 */
.set PI_STATUS_REG,		(PI_BASE_REG+0x10)

/* PI dom1 latency (R/W): [7:0] domain 1 device latency */
.set PI_BSD_DOM1_LAT_REG,	(PI_BASE_REG+0x14)

/* PI dom1 pulse width (R/W): [7:0] domain 1 device R/W strobe pulse width */
.set PI_BSD_DOM1_PWD_REG,	(PI_BASE_REG+0x18)

/* PI dom1 page size (R/W): [3:0] domain 1 device page size */
.set PI_BSD_DOM1_PGS_REG,	(PI_BASE_REG+0x1C)    /*   page size */

/* PI dom1 release (R/W): [1:0] domain 1 device R/W release duration */
.set PI_BSD_DOM1_RLS_REG,	(PI_BASE_REG+0x20)

/* PI dom2 latency (R/W): [7:0] domain 2 device latency */
.set PI_BSD_DOM2_LAT_REG,	(PI_BASE_REG+0x24)    /* Domain 2 latency */

/* PI dom2 pulse width (R/W): [7:0] domain 2 device R/W strobe pulse width */
.set PI_BSD_DOM2_PWD_REG,	(PI_BASE_REG+0x28)    /*   pulse width */

/* PI dom2 page size (R/W): [3:0] domain 2 device page size */
.set PI_BSD_DOM2_PGS_REG,	(PI_BASE_REG+0x2C)    /*   page size */

/* PI dom2 release (R/W): [1:0] domain 2 device R/W release duration */
.set PI_BSD_DOM2_RLS_REG,	(PI_BASE_REG+0x30)    /*   release duration */

.set PI_DOMAIN1_REG,		PI_BSD_DOM1_LAT_REG
.set PI_DOMAIN2_REG,		PI_BSD_DOM2_LAT_REG

.set PI_DOM_LAT_OFS,		0x00
.set PI_DOM_PWD_OFS,		0x04
.set PI_DOM_PGS_OFS,		0x08
.set PI_DOM_RLS_OFS,		0x0C

/*
 * PI status register has 3 bits active when read from (PI_STATUS_REG - read)
 *	Bit 0: DMA busy - set when DMA is in progress
 *	Bit 1: IO busy  - set when IO is in progress
 *	Bit 2: Error    - set when CPU issues IO request while DMA is busy
 */
.set PI_STATUS_ERROR,		0x04
.set PI_STATUS_IO_BUSY,	0x02
.set PI_STATUS_DMA_BUSY,	0x01

/* PI status register has 2 bits active when written to:
 *	Bit 0: When set, reset PIC
 *	Bit 1: When set, clear interrupt flag
 * The values of the two bits can be ORed together to both reset PIC and 
 * clear interrupt at the same time.
 *
 * Note: 
 *	- The PIC does generate an interrupt at the end of each DMA. CPU 
 *	needs to clear the interrupt flag explicitly (from an interrupt 
 *	handler) by writing into the STATUS register with bit 1 set.
 *
 *	- When a DMA completes, the interrupt flag is set.  CPU can issue
 *	another request even while the interrupt flag is set (as long as
 *	PIC is idle).  However, it is the CPU's responsibility for
 *	maintaining accurate correspondence between DMA completions and
 *	interrupts.
 *
 *	- When PIC is reset, if PIC happens to be busy, an interrupt will
 *	be generated as PIC returns to idle.  Otherwise, no interrupt will
 *	be generated and PIC remains idle.
 */
/*
 * Values to clear interrupt/reset PIC (PI_STATUS_REG - write)
 */
.set PI_STATUS_RESET,		0x01
.set PI_SET_RESET,		PI_STATUS_RESET

.set PI_STATUS_CLR_INTR,	0x02
.set PI_CLR_INTR,		PI_STATUS_CLR_INTR

.set PI_DMA_BUFFER_SIZE,	128

.set PI_DOM1_ADDR1,		0x06000000	/* to 0x07FFFFFF */
.set PI_DOM1_ADDR2,		0x10000000	/* to 0x1FBFFFFF */
.set PI_DOM1_ADDR3,		0x1FD00000	/* to 0x7FFFFFFF */
.set PI_DOM2_ADDR1,		0x05000000	/* to 0x05FFFFFF */
.set PI_DOM2_ADDR2,		0x08000000	/* to 0x0FFFFFFF */


/*************************************************************************
 * RDRAM Interface (RI) Registers 
 */
.set RI_BASE_REG,		0x04700000

/* RI mode (R/W): [1:0] operating mode, [2] stop T active, [3] stop R active */
.set RI_MODE_REG,		(RI_BASE_REG+0x00)	

/* RI config (R/W): [5:0] current control input, [6] current control enable */
.set RI_CONFIG_REG,		(RI_BASE_REG+0x04)

/* RI current load (W): [] any write updates current control register */
.set RI_CURRENT_LOAD_REG,	(RI_BASE_REG+0x08)

/* RI select (R/W): [2:0] receive select, [2:0] transmit select */
.set RI_SELECT_REG,		(RI_BASE_REG+0x0C)

/* RI refresh (R/W): [7:0] clean refresh delay, [15:8] dirty refresh delay,
 *		     [16] refresh bank, [17] refresh enable 
 *		     [18] refresh optimize 
 */
.set RI_REFRESH_REG,		(RI_BASE_REG+0x10)
.set RI_COUNT_REG,		RI_REFRESH_REG

/* RI latency (R/W): [3:0] DMA latency/overlap */
.set RI_LATENCY_REG,		(RI_BASE_REG+0x14)

/* RI error (R): [0] nack error, [1] ack error */
.set RI_RERROR_REG,		(RI_BASE_REG+0x18)

/* RI error (W): [] any write clears all error bits */
.set RI_WERROR_REG,		(RI_BASE_REG+0x1C)


/*************************************************************************
 * Serial Interface (SI) Registers 
 */
.set SI_BASE_REG,		0x04800000

/* SI DRAM address (R/W): [23:0] starting RDRAM address */
.set SI_DRAM_ADDR_REG,	(SI_BASE_REG+0x00)	/* R0: DRAM address */

/* SI address read 64B (W): [] any write causes a 64B DMA write */
.set SI_PIF_ADDR_RD64B_REG,	(SI_BASE_REG+0x04)	/* R1: 64B PIF->DRAM */

/* Address SI_BASE_REG + (0x08, 0x0c, 0x14) are reserved */

/* SI address write 64B (W): [] any write causes a 64B DMA read */
.set SI_PIF_ADDR_WR64B_REG,	(SI_BASE_REG+0x10)	/* R4: 64B DRAM->PIF */

/* 
 * SI status (W): [] any write clears interrupt
 *           (R): [0] DMA busy, [1] IO read busy, [2] reserved
 *                [3] DMA error, [12] interrupt
 */
.set SI_STATUS_REG,		(SI_BASE_REG+0x18)	/* R6: Status */

/* SI status register has the following bits active:
 *	0:   DMA busy		- set when DMA is in progress
 *	1:   IO busy		- set when IO access is in progress
 *	3:   DMA error		- set when there are overlapping DMA requests
 *     12:   Interrupt		- Interrupt set
 */
.set SI_STATUS_DMA_BUSY,	0x0001
.set SI_STATUS_RD_BUSY,	0x0002
.set SI_STATUS_DMA_ERROR,	0x0008
.set SI_STATUS_INTERRUPT,	0x1000

/*
 * 'cache' instruction definitions
 */

/* Target cache */
.set CACH_PI,		0x0	/* specifies primary inst. cache */
.set CACH_PD,		0x1	/* primary data cache */
.set CACH_SI,		0x2	/* secondary instruction cache */
.set CACH_SD,		0x3	/* secondary data cache */

/* Cache operations */
.set C_IINV,		0x0	/* index invalidate (inst, 2nd inst) */
.set C_IWBINV,	0x0	/* index writeback inval (d, sd) */
.set C_ILT,		0x4	/* index load tag (all) */
.set C_IST,		0x8	/* index store tag (all) */
.set C_CDX,		0xc	/* create dirty exclusive (d, sd) */
.set C_HINV,		0x10	/* hit invalidate (all) */
.set C_HWBINV,	0x14	/* hit writeback inv. (d, sd) */
.set C_FILL,		0x14	/* fill (i) */
.set C_HWB,		0x18	/* hit writeback (i, d, sd) */
.set C_HSV,		0x1c	/* hit set virt. (si, sd) */

/*
 * Cache size definitions
 */
.set ICACHE_SIZE,		0x4000			/* 16K */
.set ICACHE_LINESIZE,		32			/* 8 words */
.set ICACHE_LINEMASK,		(ICACHE_LINESIZE-1)

.set DCACHE_SIZE,		0x2000			/* 8K */
.set DCACHE_LINESIZE,		16			/* 4 words */
.set DCACHE_LINEMASK,		(DCACHE_LINESIZE-1)

/*
 * Segment base addresses and sizes
 */
.set KUBASE,		0
.set KUSIZE,		0x80000000
.set K0BASE,		0x80000000
.set K0SIZE,		0x20000000
.set K1BASE,		0xA0000000
.set K1SIZE,		0x20000000
.set K2BASE,		0xC0000000
.set K2SIZE,		0x20000000

/*
 * TLB size constants
 */

.set NTLBENTRIES,	31	/* entry 31 is reserved by rdb */

.set TLBHI_VPN2MASK,		0xffffe000
.set TLBHI_VPN2SHIFT,		13
.set TLBHI_PIDMASK,		0xff
.set TLBHI_PIDSHIFT,		0
.set TLBHI_NPID,		255		/* 255 to fit in 8 bits */

.set TLBLO_PFNMASK,		0x3fffffc0
.set TLBLO_PFNSHIFT,		6
.set TLBLO_CACHMASK,		0x38		/* cache coherency algorithm */
.set TLBLO_CACHSHIFT,		3
.set TLBLO_UNCACHED,		0x10		/* not cached */
.set TLBLO_NONCOHRNT,		0x18		/* Cacheable non-coherent */
.set TLBLO_EXLWR,		0x28		/* Exclusive write */
.set TLBLO_D,			0x4		/* writeable */
.set TLBLO_V,			0x2		/* valid bit */
.set TLBLO_G,			0x1		/* global access bit */

.set TLBINX_PROBE,		0x80000000
.set TLBINX_INXMASK,		0x3f
.set TLBINX_INXSHIFT,		0

.set TLBRAND_RANDMASK,	0x3f
.set TLBRAND_RANDSHIFT,	0

.set TLBWIRED_WIREDMASK,	0x3f

.set TLBCTXT_BASEMASK,	0xff800000
.set TLBCTXT_BASESHIFT,	23
.set TLBCTXT_BASEBITS,	9

.set TLBCTXT_VPNMASK,		0x7ffff0
.set TLBCTXT_VPNSHIFT,	4

.set TLBPGMASK_4K,		0x0
.set TLBPGMASK_16K,		0x6000
.set TLBPGMASK_64K,		0x1e000
