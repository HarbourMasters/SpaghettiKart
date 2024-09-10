# Makefile to rebuild MK64 split image

include util.mk

include safe_gcc.mk

# Default target
default: all

# Preprocessor definitions
DEFINES :=

#==============================================================================#
# Build Options                                                                #
#==============================================================================#

# Configure the build options with 'make SETTING=value' (ex. make VERSION=eu).
# Run 'make clean' prior to changing versions.

# Build for the N64 (turn this off for ports)
TARGET_N64 ?= 1

# COMPILER - selects the C compiler to use
#   ido - uses the SGI IRIS Development Option compiler, which is used to build
#         an original matching N64 ROM
#   gcc - uses the GNU C Compiler
COMPILER ?= ido
$(eval $(call validate-option,COMPILER,ido gcc))

# Add debug tools with 'make DEBUG=1' and modify the macros in include/debug.h
# Adds crash screen enhancement and activates debug mode
# Run make clean first
DEBUG ?= 0

# Compile with GCC
GCC ?= 0

# VERSION - selects the version of the game to build
#   us - builds the 1997 North American version
#   eu - builds the 1997 1.1 PAL version
VERSION ?= us
$(eval $(call validate-option,VERSION,us eu-1.0 eu-final))

ifeq      ($(VERSION),us)
  DEFINES += VERSION_US=1
  GRUCODE   ?= f3dex_old
else ifeq ($(VERSION),eu-1.0)
  DEFINES += VERSION_EU=1 VERSION_EU_1_0=1
  GRUCODE   ?= f3dex_old
else ifeq ($(VERSION),eu-final)
  DEFINES += VERSION_EU=1 VERSION_EU_1_1=1
  GRUCODE   ?= f3dex_old
endif

ifeq ($(DEBUG),1)
  DEFINES += DEBUG=1
  COMPARE ?= 0
endif

TARGET := mk64.$(VERSION)

BASEROM := baserom.$(VERSION).z64



# GRUCODE - selects which RSP microcode to use.
#   f3dex_old - default, version 0.95. An early version of F3DEX.
#   f3dex     - latest version of F3DEX, used on iQue and Lodgenet.
#   f3dex2    - F3DEX2, currently unsupported.
# Note that 3/4 player mode uses F3DLX
$(eval $(call validate-option,GRUCODE,f3dex_old f3dex f3dex2))

ifeq ($(GRUCODE),f3dex_old) # Fast3DEX 0.95
  DEFINES += F3DEX_GBI=1 F3D_OLD=1
else ifeq ($(GRUCODE),f3dex) # Fast3DEX 1.23
  DEFINES += F3DEX_GBI=1 F3DEX_GBI_SHARED=1
else ifeq ($(GRUCODE),f3dex2) # Fast3DEX2
  DEFINES += F3DEX_GBI_2=1 F3DEX_GBI_SHARED=1
endif

ifeq      ($(COMPILER),ido)
  MIPSISET := -mips2
else ifeq ($(COMPILER),gcc)
  DEFINES += AVOID_UB=1 NON_MATCHING=1
  NON_MATCHING := 1
  VERSION_ASFLAGS := --defsym AVOID_UB=1
  MIPSISET     := -mips3
endif



# NON_MATCHING - whether to build a matching, identical copy of the ROM
#   1 - enable some alternate, more portable code that does not produce a matching ROM
#   0 - build a matching ROM
NON_MATCHING ?= 0
$(eval $(call validate-option,NON_MATCHING,0 1))

ifeq ($(TARGET_N64),0)
  NON_MATCHING := 1
endif

ifeq ($(NON_MATCHING),1)
  DEFINES += NON_MATCHING=1 AVOID_UB=1
  VERSION_ASFLAGS := --defsym AVOID_UB=1
  COMPARE := 0
endif

# GCC define is to link gcc's std lib.
ifeq ($(GCC), 1)
    DEFINES += AVOID_UB=1 GCC=1
endif

# COMPARE - whether to verify the SHA-1 hash of the ROM after building
#   1 - verifies the SHA-1 hash of the selected version of the game
#   0 - does not verify the hash
COMPARE ?= 1
$(eval $(call validate-option,COMPARE,0 1))



# Whether to hide commands or not
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
  V := @
endif



# Whether to colorize build messages
COLOR ?= 1

ifeq ($(OS),Windows_NT)
    DETECTED_OS=windows
    # Set Windows temporary directory to its environment variable
    export TMPDIR=$(TEMP)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        DETECTED_OS=linux
    else ifeq ($(UNAME_S),Darwin)
        DETECTED_OS=macos
    endif
endif

# display selected options unless 'make clean' or 'make distclean' is run
ifeq ($(filter clean distclean,$(MAKECMDGOALS)),)
  $(info ==== Build Options ====)
  $(info Version:        $(VERSION))
  $(info Microcode:      $(GRUCODE))
  $(info Target:         $(TARGET))
  ifeq ($(COMPARE),1)
    $(info Compare ROM:    yes)
  else
    $(info Compare ROM:    no)
  endif
  ifeq ($(NON_MATCHING),1)
    $(info Build Matching: no)
  else
    $(info Build Matching: yes)
  endif
  $(info =======================)
endif

#==============================================================================#
# Universal Dependencies                                                       #
#==============================================================================#

TOOLS_DIR := tools

# (This is a bit hacky, but a lot of rules implicitly depend
# on tools and assets, and we use directory globs further down
# in the makefile that we want should cover assets.)
ifeq ($(DETECTED_OS),windows)
# because python3 is a command to trigger windows store, and python on windows it's just called python
  ifneq ($(PYTHON),)
  else ifneq ($(call find-command,python),)
    PYTHON := python
  else ifneq ($(call find-command,python3),)
    PYTHON := python3
  endif
else
  PYTHON ?= python3
endif

DUMMY != $(PYTHON) --version || echo FAIL
ifeq ($(DUMMY),FAIL)
  $(error Unable to find python)
endif

ifeq ($(filter clean distclean print-%,$(MAKECMDGOALS)),)
   # Make tools if out of date
  DUMMY != make -C $(TOOLS_DIR)
  ifeq ($(DUMMY),FAIL)
    $(error Failed to build tools)
  endif
  $(info Building ROM...)

  # Make sure assets exist
  NOEXTRACT ?= 0
  ifeq ($(NOEXTRACT),0)
    DUMMY != $(PYTHON) extract_assets.py $(VERSION) >&2 || echo FAIL
    ifeq ($(DUMMY),FAIL)
      $(error Failed to extract assets)
    endif
  endif
endif



#==============================================================================#
# Target Executable and Sources                                                #
#==============================================================================#

BUILD_DIR_BASE := build
# BUILD_DIR is location where all build artifacts are placed
BUILD_DIR      := $(BUILD_DIR_BASE)/$(VERSION)
ROM            := $(BUILD_DIR)/$(TARGET).z64
ELF            := $(BUILD_DIR)/$(TARGET).elf
LD_SCRIPT      := mk64.ld
ASSET_DIR      := assets
BIN_DIR        := bin
DATA_DIR       := data
INCLUDE_DIRS   := include

# Directories containing source files
SRC_ASSETS_DIR := assets/code/ceremony_data assets/code/startup_logo assets/code/data_800E45C0 assets/code/data_segment2 assets/code/data_800E8700 assets/code/common_data
SRC_DIRS       := src src/data src/buffers src/racing src/ending src/audio src/debug src/os src/os/math courses assets/code/ceremony_data assets/code/startup_logo $(SRC_ASSETS_DIR)
ASM_DIRS       := asm asm/os asm/unused $(DATA_DIR) $(DATA_DIR)/sound_data $(DATA_DIR)/karts


# Directories containing course source and data files
COURSE_DIRS := $(shell find courses -mindepth 1 -type d)
TEXTURES_DIR = textures
TEXTURE_DIRS := textures/common

ALL_DIRS = $(BUILD_DIR) $(addprefix $(BUILD_DIR)/,$(SRC_DIRS) $(COURSE_DIRS) include $(ASM_DIRS) $(TEXTURES_DIR)/raw \
	$(TEXTURES_DIR)/standalone $(TEXTURES_DIR)/startup_logo $(TEXTURES_DIR)/crash_screen $(TEXTURES_DIR)/trophy $(TEXTURES_DIR)/courses        \
	$(TEXTURE_DIRS) $(TEXTURE_DIRS)/tlut $(BIN_DIR)) assets/course_metadata

# file dependencies generated by splitter
MAKEFILE_SPLIT = Makefile.split
include $(MAKEFILE_SPLIT)

# These are files that need to be encoded into EUC-JP in order for the ROM to match
# We filter them out from the regular C_FILES since we don't need nor want the
# UTF-8 versions getting compiled
EUC_JP_FILES := src/ending/credits.c src/code_80005FD0.c src/code_80091750.c
C_FILES := $(filter-out %.inc.c,$(filter-out $(EUC_JP_FILES),$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))))
S_FILES := $(foreach dir,$(ASM_DIRS),$(wildcard $(dir)/*.s))
# Include source files in courses/course_name/files.c but exclude .inc.c files.
COURSE_FILES := $(foreach dir,$(COURSE_DIRS),$(filter-out %.inc.c,$(wildcard $(dir)/*.c)))


# Object files
O_FILES := \
  $(foreach file,$(C_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
  $(foreach file,$(COURSE_FILES),$(BUILD_DIR)/$(file:.c=.o)) \
  $(foreach file,$(S_FILES),$(BUILD_DIR)/$(file:.s=.o)) \
  $(EUC_JP_FILES:%.c=$(BUILD_DIR)/%.jp.o)

# Automatic dependency files
DEP_FILES := $(O_FILES:.o=.d) $(BUILD_DIR)/$(LD_SCRIPT).d

# Files with GLOBAL_ASM blocks
GLOBAL_ASM_C_FILES != grep -rl 'GLOBAL_ASM(' $(wildcard src/*.c)
GLOBAL_ASM_OS_FILES != grep -rl 'GLOBAL_ASM(' $(wildcard src/os/*.c)
GLOBAL_ASM_AUDIO_C_FILES != grep -rl 'GLOBAL_ASM(' $(wildcard src/audio/*.c)
GLOBAL_ASM_RACING_C_FILES != grep -rl 'GLOBAL_ASM(' $(wildcard src/racing/*.c)
GLOBAL_ASM_O_FILES = $(foreach file,$(GLOBAL_ASM_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
GLOBAL_ASM_OS_O_FILES = $(foreach file,$(GLOBAL_ASM_OS_FILES),$(BUILD_DIR)/$(file:.c=.o))
GLOBAL_ASM_AUDIO_O_FILES = $(foreach file,$(GLOBAL_ASM_AUDIO_C_FILES),$(BUILD_DIR)/$(file:.c=.o))
GLOBAL_ASM_RACING_O_FILES = $(foreach file,$(GLOBAL_ASM_RACING_C_FILES),$(BUILD_DIR)/$(file:.c=.o))



#==============================================================================#
# Compiler Options                                                             #
#==============================================================================#

# detect prefix for MIPS toolchain
ifneq ($(CROSS),)
else ifneq      ($(call find-command,mips-linux-gnu-ld),)
	CROSS := mips-linux-gnu-
else ifneq ($(call find-command,mips64-linux-gnu-ld),)
	CROSS := mips64-linux-gnu-
else ifneq ($(call find-command,mips64-elf-ld),)
	CROSS := mips64-elf-
else
	$(error Unable to detect a suitable MIPS toolchain installed)
endif

AS      := $(CROSS)as
ifeq ($(COMPILER),gcc)
  CC      := $(CROSS)gcc
else
  IDO_ROOT := $(TOOLS_DIR)/ido-recomp/$(DETECTED_OS)
  CC      := $(IDO_ROOT)/cc
endif
LD      := $(CROSS)ld
AR      := $(CROSS)ar
OBJDUMP := $(CROSS)objdump
OBJCOPY := $(CROSS)objcopy

OPT_FLAGS := -O2

ifeq ($(TARGET_N64),1)
  TARGET_CFLAGS := -nostdinc -DTARGET_N64 -D_LANGUAGE_C
  CC_CFLAGS := -fno-builtin
endif

INCLUDE_DIRS := include $(BUILD_DIR) $(BUILD_DIR)/include src src/racing src/ending .
ifeq ($(TARGET_N64),1)
  INCLUDE_DIRS += include/libc
endif

C_DEFINES := $(foreach d,$(DEFINES),-D$(d))
DEF_INC_CFLAGS := $(foreach i,$(INCLUDE_DIRS),-I$(i)) $(C_DEFINES)

# Prefer clang as C preprocessor if installed on the system
ifneq (,$(call find-command,clang))
  CPP      := clang
  CPPFLAGS := -E -P -x c -Wno-trigraphs $(DEF_INC_CFLAGS)
else ifneq (,$(call find-command,cpp))
  CPP      := cpp
  CPPFLAGS := -P -Wno-trigraphs $(DEF_INC_CFLAGS)
else
  $(error Unable to find cpp or clang)
endif

# Check code syntax with host compiler
CC_CHECK ?= gcc
CC_CHECK_CFLAGS := -fsyntax-only -fsigned-char $(CC_CFLAGS) $(TARGET_CFLAGS) -std=gnu90 -Wall -Wempty-body -Wextra -Wno-format-security -Wno-main -DNON_MATCHING -DAVOID_UB $(DEF_INC_CFLAGS)

# C compiler options
HIDE_WARNINGS := -woff 838,649
CFLAGS = -G 0 $(OPT_FLAGS) $(TARGET_CFLAGS) $(MIPSISET) $(DEF_INC_CFLAGS)
ifeq ($(COMPILER),gcc)
  CFLAGS += -mno-shared -march=vr4300 -mfix4300 -mabi=32 -mhard-float -mdivide-breaks -fno-stack-protector -fno-common -fno-zero-initialized-in-bss -fno-PIC -mno-abicalls -fno-strict-aliasing -fno-inline-functions -ffreestanding -fwrapv -Wall -Wextra
else
  CFLAGS += $(HIDE_WARNINGS) -non_shared -Wab,-r4300_mul -Xcpluscomm -fullwarn -signed -32
endif

ASFLAGS = -march=vr4300 -mabi=32 -I include -I $(BUILD_DIR) $(VERSION_ASFLAGS) $(foreach d,$(DEFINES),--defsym $(d))

# Fills end of rom
OBJCOPYFLAGS = --pad-to=0xC00000 --gap-fill=0xFF

LDFLAGS = -T undefined_syms.txt -T $(BUILD_DIR)/$(LD_SCRIPT) -Map $(BUILD_DIR)/$(TARGET).map --no-check-sections

# Ensure that gcc treats the code as 32-bit
CC_CHECK += -m32

# Prevent a crash with -sopt
export LANG := C



#==============================================================================#
# Miscellaneous Tools                                                          #
#==============================================================================#

MIO0TOOL              := $(TOOLS_DIR)/mio0
N64CKSUM              := $(TOOLS_DIR)/n64cksum
N64GRAPHICS           := $(TOOLS_DIR)/n64graphics
DLPACKER              := $(TOOLS_DIR)/displaylist_packer
BIN2C                 := $(PYTHON) $(TOOLS_DIR)/bin2c.py
EXTRACT_DATA_FOR_MIO  := $(TOOLS_DIR)/extract_data_for_mio
ASSET_EXTRACT         := $(PYTHON) $(TOOLS_DIR)/new_extract_assets.py
LINKONLY_GENERATOR    := $(PYTHON) $(TOOLS_DIR)/linkonly_generator.py
TORCH                 := $(TOOLS_DIR)/torch/cmake-build-release/torch
EMULATOR               = mupen64plus
EMU_FLAGS              = --noosd
LOADER                 = loader64
LOADER_FLAGS           = -vwf
SHA1SUM               ?= sha1sum
FALSE                 ?= false
PRINT                 ?= printf
TOUCH                 ?= touch

ifeq ($(COLOR),1)
NO_COL  := \033[0m
RED     := \033[0;31m
GREEN   := \033[0;32m
BLUE    := \033[0;34m
YELLOW  := \033[0;33m
BLINK   := \033[33;5m
endif

# Use Objcopy instead of extract_data_for_mio
ifeq ($(COMPILER),gcc)
  EXTRACT_DATA_FOR_MIO := $(OBJCOPY) -O binary --only-section=.data
endif

# Common build print status function
define print
  @$(PRINT) "$(GREEN)$(1) $(YELLOW)$(2)$(GREEN) -> $(BLUE)$(3)$(NO_COL)\n"
endef

# Override commmands for GCC Safe Files
ifeq ($(GCC),1)
  $(BUILD_DIR)/src/main.o:                          OPT_FLAGS := -g
  $(BUILD_DIR)/src/racing/skybox_and_splitscreen.o: OPT_FLAGS := -g
  $(BUILD_DIR)/src/racing/render_courses.o:         OPT_FLAGS := -g
  $(SAFE_C_FILES): OPT_FLAGS := -O3
  $(SAFE_C_FILES): CC        := $(CROSS)gcc
  $(SAFE_C_FILES): MIPSISET  := -mips3
  $(SAFE_C_FILES): CFLAGS    := -G 0 $(OPT_FLAGS) $(TARGET_CFLAGS) $(MIPSISET) $(DEF_INC_CFLAGS) -mno-shared -march=vr4300 -mfix4300 -mabi=32 -mhard-float \
   -mdivide-breaks -fno-stack-protector -fno-common -fno-zero-initialized-in-bss -fno-PIC -mno-abicalls -fno-strict-aliasing -fno-inline-functions          \
   -ffreestanding -fwrapv -Wall -Wextra -ffast-math -fno-unsafe-math-optimizations
  $(SAFE_C_FILES): CC_CHECK := gcc -m32
endif



#==============================================================================#
# Main Targets                                                                 #
#==============================================================================#

all: $(ROM)
ifeq ($(COMPARE),1)
	@$(PRINT) "$(GREEN)Checking if ROM matches.. $(NO_COL)\n"
	@$(SHA1SUM) -c $(TARGET).sha1 > $(NULL_OUT) && $(PRINT) "$(TARGET): $(GREEN)OK$(NO_COL)\n" || ($(PRINT) "$(YELLOW)Building the ROM file has succeeded, but does not match the original ROM.\nThis is expected, and not an error, if you are making modifications.\nTo silence this message, use 'make COMPARE=0.' $(NO_COL)\n" && $(FALSE))
endif

assets:
	@echo "Extracting torch assets..."
	$(V)$(TORCH) code $(BASEROM)
	$(V)$(TORCH) header $(BASEROM)
	$(V)$(TORCH) modding export $(BASEROM)

doc:
	$(V)$(PYTHON) $(TOOLS_DIR)/doxygen_symbol_gen.py
	doxygen
	@$(PRINT) "$(GREEN)Documentation generated in docs/html$(NO_COL)\n"
	@$(PRINT) "$(GREEN)Results can be viewed by opening docs/html/index.html in a web browser$(NO_COL)\n"

format:
	@$(PYTHON) $(TOOLS_DIR)/format.py -j $(N_THREADS)

clean:
	$(RM) -r $(BUILD_DIR)

distclean: distclean_assets
	$(RM) -r $(BUILD_DIR_BASE)
	$(PYTHON) extract_assets.py --clean
	make -C $(TOOLS_DIR) clean

distclean_assets: ;

test: $(ROM)
	$(EMULATOR) $(EMU_FLAGS) $<

load: $(ROM)
	$(LOADER) $(LOADER_FLAGS) $<

# Make sure build directory exists before compiling anything
DUMMY != mkdir -p $(ALL_DIRS)

#==============================================================================#
# Texture Generation                                                           #
#==============================================================================#

# RGBA32, RGBA16, IA16, IA8, IA4, IA1, I8, I4
$(BUILD_DIR)/%: %.png
	@$(PRINT) "$(GREEN)Converting:  $(BLUE) $< -> $@$(NO_COL)\n"
	$(V)$(N64GRAPHICS) -i $@ -g $< -f $(lastword $(subst ., ,$@))

$(BUILD_DIR)/textures/%.mio0: $(BUILD_DIR)/textures/%
	$(V)$(MIO0TOOL) -c $< $@

ASSET_INCLUDES := $(shell find $(ASSET_DIR)/include -type f -name "*.mk")

$(foreach inc,$(ASSET_INCLUDES),$(eval include $(inc)))



#==============================================================================#
# Compressed Segment Generation                                                #
#==============================================================================#

$(BUILD_DIR)/%.mio0: %.bin
	@$(PRINT) "$(GREEN)Compressing binary files:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(MIO0TOOL) -c $< $@

$(BUILD_DIR)/%.mio0.o: $(BUILD_DIR)/%.mio0.s
	@$(PRINT) "$(GREEN)Compiling mio0:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(BUILD_DIR)/%.mio0.s: $(BUILD_DIR)/%.mio0
	$(call print,Generating mio0 asm:,$<,$@)
	$(V)$(PRINT) ".section .data\n\n.balign 4\n\n.incbin \"$<\"\n" > $@

$(BUILD_DIR)/src/crash_screen.o: src/crash_screen.c
	@$(PRINT) "$(GREEN)Compiling Crash Screen:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(N64GRAPHICS) -i $(BUILD_DIR)/textures/crash_screen/crash_screen_font.ia1.inc.c -g textures/crash_screen/crash_screen_font.ia1.png -f ia1 -s u8
	@$(CC_CHECK) $(CC_CHECK_CFLAGS) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(V)$(CC) -c $(CFLAGS) -o $@ $<
	$(V)$(PYTHON) $(TOOLS_DIR)/set_o32abi_bit.py $@

#==============================================================================#
# Common Textures Segment Generation                                           #
#==============================================================================#

TEXTURE_FILES := $(foreach dir,$(TEXTURE_DIRS),$(subst .png, , $(wildcard $(dir)/*)))

TEXTURE_FILES_TLUT := $(foreach dir,$(TEXTURE_DIRS)/tlut,$(subst .png, , $(wildcard $(dir)/*)))


$(TEXTURE_FILES):
	@$(PRINT) "$(GREEN)Converting:  $(BLUE) $< -> $@$(NO_COL)\n"
	$(V)$(N64GRAPHICS) -i $(BUILD_DIR)/$@.inc.c -g $@.png -f $(lastword $(subst ., ,$@)) -s u8

# TLUT
$(TEXTURE_FILES_TLUT):
	@$(PRINT) "$(GREEN)Converting:  $(BLUE) $< -> $@$(NO_COL)\n"
	$(V)$(N64GRAPHICS) -i $(BUILD_DIR)/$@.inc.c -g $@.png -f $(lastword $(subst ., ,$@)) -s u8 -c $(lastword $(subst ., ,$(subst .$(lastword $(subst ., ,$(TEXTURE_FILES_TLUT))), ,$(TEXTURE_FILES_TLUT)))) -p $(BUILD_DIR)/$@.tlut.inc.c

# common textures
$(BUILD_DIR)/assets/code/common_data/common_data.o: assets/code/common_data/common_data.c $(TEXTURE_FILES) $(TEXTURE_FILES_TLUT)
	@$(PRINT) "$(GREEN)Compiling Common Textures:  $(BLUE)$@ $(NO_COL)\n"
	@$(CC_CHECK) $(CC_CHECK_CFLAGS) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(V)$(CC) -c $(CFLAGS) -o $@ $<
	$(V)$(PYTHON) $(TOOLS_DIR)/set_o32abi_bit.py $@



#==============================================================================#
# Course Packed Displaylists Generation                                        #
#==============================================================================#


%/course_textures.linkonly.c %/course_textures.linkonly.h: %/course_offsets.c
	$(V)$(LINKONLY_GENERATOR) $(lastword $(subst /, ,$*))

# Its unclear why this is necessary. Everything I undesrtand about `make` says that just
# `$(BUILD_DIR)/%/course_displaylists.inc.o: %/course_textures.linkonly.h`
# Should work identical to this. But in practice it doesn't :(
COURSE_DISPLAYLIST_OFILES := $(foreach dir,$(COURSE_DIRS),$(BUILD_DIR)/$(dir)/course_displaylists.inc.o)
$(COURSE_DISPLAYLIST_OFILES): $(BUILD_DIR)/%/course_displaylists.inc.o: %/course_textures.linkonly.h

%/course_textures.linkonly.elf: %/course_textures.linkonly.o
	$(V)$(LD) -t -e 0 -Ttext=05000000 -Map $@.map -o $@ $< --no-check-sections

%/course_displaylists.inc.elf: %/course_displaylists.inc.o %/course_textures.linkonly.elf
	$(V)$(LD) -t -e 0 -Ttext=07000000 -Map $@.map -R $*/course_textures.linkonly.elf -o $@ $< --no-check-sections

%/course_displaylists.inc.bin: %/course_displaylists.inc.elf
	$(V)$(EXTRACT_DATA_FOR_MIO) $< $@

# Displaylists are packed using a custom format
%/course_displaylists_packed.inc.bin: %/course_displaylists.inc.bin
	@$(PRINT) "$(GREEN)Compressing Course Displaylists:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(DLPACKER) $< $@



#==============================================================================#
# Course Geography Generation                                                  #
#==============================================================================#

COURSE_GEOGRAPHY_TARGETS := $(foreach dir,$(COURSE_DIRS),$(BUILD_DIR)/$(dir)/course_geography.mio0.o)

%/course_vertices.inc.elf: %/course_vertices.inc.o
	$(V)$(LD) -t -e 0 -Ttext=0F000000 -Map $@.map -o $@ $< --no-check-sections

%/course_vertices.inc.bin: %/course_vertices.inc.elf
	$(V)$(EXTRACT_DATA_FOR_MIO) $< $@

%/course_vertices.inc.mio0: %/course_vertices.inc.bin
	@$(PRINT) "$(GREEN)Compressing Course Geography:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(MIO0TOOL) -c $< $@

# Course vertices and displaylists are included together due to no alignment between the two files.
%/course_geography.mio0.s: %/course_vertices.inc.mio0 %/course_displaylists_packed.inc.bin
	$(V)$(PRINT) ".include \"macros.inc\"\n\n.section .data\n\n.balign 4\n\nglabel d_course_$(lastword $(subst /, ,$*))_vertex\n\n.incbin \"$(@D)/course_vertices.inc.mio0\"\n\n.balign 4\n\nglabel d_course_$(lastword $(subst /, ,$*))_packed\n\n.incbin \"$(@D)/course_displaylists_packed.inc.bin\"\n\n.balign 0x10\n" > $@



#==============================================================================#
# Course Data Generation                                                       #
#==============================================================================#

COURSE_DATA_ELFS := $(foreach dir,$(COURSE_DIRS),$(BUILD_DIR)/$(dir)/course_data.elf)
LDFLAGS += $(foreach elf,$(COURSE_DATA_ELFS),-R $(elf))

COURSE_DATA_TARGETS := $(foreach dir,$(COURSE_DIRS),$(BUILD_DIR)/$(dir)/course_data.mio0.o)

%/course_data.elf: %/course_data.o %/course_displaylists.inc.elf
	$(V)$(LD) -t -e 0 -Ttext=06000000 -Map $@.map -R $*/course_displaylists.inc.elf -o $@ $< --no-check-sections

%/course_data.bin: %/course_data.elf
	$(V)$(EXTRACT_DATA_FOR_MIO) $< $@

%/course_data.mio0: %/course_data.bin
	@$(PRINT) "$(GREEN)Compressing Course Data:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(MIO0TOOL) -c $< $@

%/course_data.mio0.s: %/course_data.mio0
	$(V)$(PRINT) ".include \"macros.inc\"\n\n.section .data\n\n.balign 4\n\n.incbin \"$<\"\n\n" > $@


#==============================================================================#
# Source Code Generation                                                       #
#==============================================================================#
$(BUILD_DIR)/%.jp.c: %.c
	$(call print,Encoding:,$<,$@)
	$(V)iconv -t EUC-JP -f UTF-8 $< > $@

$(BUILD_DIR)/%.o: %.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC_CHECK) $(CC_CHECK_CFLAGS) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(V)$(CC) -c $(CFLAGS) -o $@ $<
	$(V)$(PYTHON) $(TOOLS_DIR)/set_o32abi_bit.py $@

$(BUILD_DIR)/%.o: $(BUILD_DIR)/%.c
	$(call print,Compiling:,$<,$@)
	$(V)$(CC_CHECK) $(CC_CHECK_CFLAGS) -MMD -MP -MT $@ -MF $(BUILD_DIR)/$*.d $<
	$(V)$(CC) -c $(CFLAGS) -o $@ $<

$(BUILD_DIR)/%.o: %.s $(MIO0_FILES) $(RAW_TEXTURE_FILES)
	$(V)$(AS) $(ASFLAGS) -o $@ $<

$(EUC_JP_FILES:%.c=$(BUILD_DIR)/%.jp.o): CC := $(PYTHON) $(TOOLS_DIR)/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --

$(GLOBAL_ASM_O_FILES): CC := $(PYTHON) $(TOOLS_DIR)/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --

$(GLOBAL_ASM_OS_O_FILES): CC := $(PYTHON) $(TOOLS_DIR)/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --

$(GLOBAL_ASM_AUDIO_O_FILES): CC := $(PYTHON) $(TOOLS_DIR)/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --

$(GLOBAL_ASM_RACING_O_FILES): CC := $(PYTHON) $(TOOLS_DIR)/asm_processor/build.py $(CC) -- $(AS) $(ASFLAGS) --

#==============================================================================#
# Libultra Definitions                                                         #
#==============================================================================#

$(BUILD_DIR)/src/os/%.o:          OPT_FLAGS :=
$(BUILD_DIR)/src/os/math/%.o:     OPT_FLAGS := -O2
$(BUILD_DIR)/src/os/math/ll%.o:   OPT_FLAGS :=
$(BUILD_DIR)/src/os/math/ll%.o:   MIPSISET := -mips3
$(BUILD_DIR)/src/os/ldiv.o:       OPT_FLAGS := -O2
$(BUILD_DIR)/src/os/string.o:     OPT_FLAGS := -O2
$(BUILD_DIR)/src/os/gu%.o:        OPT_FLAGS := -O3
$(BUILD_DIR)/src/os/al%.o:        OPT_FLAGS := -O3
$(BUILD_DIR)/src/os/__osLeoInterrupt.o:        OPT_FLAGS := -O1
$(BUILD_DIR)/src/os/_Printf.o:        OPT_FLAGS := -O3
$(BUILD_DIR)/src/os/_Litob.o:        OPT_FLAGS := -O3
$(BUILD_DIR)/src/os/_Ldtob.o:        OPT_FLAGS := -O3
$(BUILD_DIR)/src/os/osSyncPrintf.o:        OPT_FLAGS := -O3

# Alternate compiler flags needed for matching
ifeq ($(COMPILER),ido)
    $(BUILD_DIR)/src/audio/%.o:        OPT_FLAGS := -O2 -use_readwrite_const
    $(BUILD_DIR)/src/audio/port_eu.o:  OPT_FLAGS := -O2 -framepointer
    $(BUILD_DIR)/src/audio/external.o:  OPT_FLAGS := -O2 -framepointer
endif



#==============================================================================#
# Compile Trophy and Podium Models                                             #
#==============================================================================#

LDFLAGS += -R $(BUILD_DIR)/assets/code/ceremony_data/ceremony_data.elf

%/ceremony_data.elf: %/ceremony_data.o
	$(V)$(LD) -t -e 0 -Ttext=0B000000 -Map $@.map -o $@ $< --no-check-sections

%/ceremony_data.bin: %/ceremony_data.elf
	$(V)$(EXTRACT_DATA_FOR_MIO) $< $@

%/ceremony_data.mio0: %/ceremony_data.bin
	@$(PRINT) "$(GREEN)Compressing Trophy Model:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(MIO0TOOL) -c $< $@

%/ceremony_data.mio0.s: %/ceremony_data.mio0
	$(V)$(PRINT) ".include \"macros.inc\"\n\n.data\n\n.balign 4\n\nglabel ceremony_data\n\n.incbin \"$<\"\n\n.balign 16\nglabel ceremonyData_end\n" > $@


#==============================================================================#
# Compile Startup Logo                                                         #
#==============================================================================#

LDFLAGS += -R $(BUILD_DIR)/assets/code/startup_logo/startup_logo.elf

%/startup_logo.elf: %/startup_logo.o
	$(V)$(LD) -t -e 0 -Ttext=06000000 -Map $@.map -o $@ $< --no-check-sections

%/startup_logo.bin: %/startup_logo.elf
	$(V)$(EXTRACT_DATA_FOR_MIO) $< $@

%/startup_logo.mio0: %/startup_logo.bin
	@$(PRINT) "$(GREEN)Compressing Startup Logo Model:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(MIO0TOOL) -c $< $@

%/startup_logo.mio0.s: %/startup_logo.mio0
	$(V)$(PRINT) ".include \"macros.inc\"\n\n.data\n\n.balign 4\n\nglabel startup_logo\n\n.incbin \"$<\"\n\n.balign 16\n\nglabel startupLogo_end\n" > $@

#==============================================================================#
# Compile Common Textures                                                      #
#==============================================================================#

LDFLAGS += -R $(BUILD_DIR)/assets/code/common_data/common_data.elf

%/common_data.elf: %/common_data.o
	$(V)$(LD) -t -e 0 -Ttext=0D000000 -Map $@.map -o $@ $< --no-check-sections

%/common_data.bin: %/common_data.elf
	$(V)$(EXTRACT_DATA_FOR_MIO) $< $@

%/common_data.mio0: %/common_data.bin
	@$(PRINT) "$(GREEN)Compressing Common Textures:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(MIO0TOOL) -c $< $@

%/common_data.mio0.s: %/common_data.mio0
	$(V)$(PRINT) ".include \"macros.inc\"\n\n.section .data\n\n.balign 4\n\n.incbin \"$<\"\n\n" > $@



#==============================================================================#
# Finalize and Link                                                            #
#==============================================================================#

# Run linker script through the C preprocessor
$(BUILD_DIR)/$(LD_SCRIPT): $(LD_SCRIPT)
	$(call print,Preprocessing linker script:,$<,$@)
	$(V)$(CPP) $(CPPFLAGS) -DBUILD_DIR=$(BUILD_DIR) -MMD -MP -MT $@ -MF $@.d -o $@ $<

# Link MK64 ELF file
$(ELF): $(O_FILES) $(COURSE_DATA_TARGETS) $(BUILD_DIR)/$(LD_SCRIPT) $(BUILD_DIR)/assets/code/startup_logo/startup_logo.mio0.o $(BUILD_DIR)/assets/code/ceremony_data/ceremony_data.mio0.o $(BUILD_DIR)/assets/code/common_data/common_data.mio0.o $(COURSE_GEOGRAPHY_TARGETS) undefined_syms.txt
	@$(PRINT) "$(GREEN)Linking ELF file:  $(BLUE)$@ $(NO_COL)\n"
	$(V)$(LD) $(LDFLAGS) -o $@

# Build ROM
$(ROM): $(ELF)
	$(call print,Building ROM:,$<,$@)
	$(V)$(OBJCOPY) $(OBJCOPYFLAGS) $< $(@:.z64=.bin) -O binary
	$(V)$(N64CKSUM) $(@:.z64=.bin) $@
	$(V)$(PYTHON) $(TOOLS_DIR)/doxygen_symbol_gen.py

$(BUILD_DIR)/$(TARGET).hex: $(TARGET).z64
	$(V)xxd $< > $@

$(BUILD_DIR)/$(TARGET).objdump: $(ELF)
	$(V)$(OBJDUMP) -D $< > $@


.PHONY: all clean distclean distclean_assets default diff test load assets
# with no prerequisites, .SECONDARY causes no intermediate target to be removed
.SECONDARY:

# Remove built-in rules, to improve performance
MAKEFLAGS += --no-builtin-rules

-include $(DEP_FILES)


print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
