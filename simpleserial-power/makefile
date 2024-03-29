# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
#
# Makefile for ChipWhisperer SimpleSerial-AES Program
#
#----------------------------------------------------------------------------
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make coff = Convert ELF to AVR COFF.
#
# make extcoff = Convert ELF to AVR Extended COFF.
#
# make program = Download the hex file to the device, using avrdude.
#                Please customize the avrdude settings below first!
#
# make debug = Start either simulavr or avarice as specified for debugging,
#              with avr-gdb or avr-insight as the front end for debugging.
#
# make filename.s = Just compile filename.c into the assembler code only.
#
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.
#
# To rebuild project do "make clean" then "make all".
#----------------------------------------------------------------------------

# Target file name (without extension).
# This is the name of the compiled .hex file.
TARGET = simpleserial-power

# List C source files here.
# Header files (.h) are automatically pulled in.
SRC += simpleserial-power.c

# -----------------------------------------------------------------------------
CRYPTO_TARGET = NONE

ifeq ($(CRYPTO_OPTIONS),)
CRYPTO_OPTIONS = AES128C
endif

ifneq ($(USE_TABLE),)
CDEFS += "-DUSE_TABLE"
endif

ifneq ($(NOP),)
CDEFS += "-DNOP"
endif

ifneq ($(MUL),)
CDEFS += "-DMUL"
endif

ifneq ($(MUL2),)
CDEFS += "-DMUL2"
endif

ifneq ($(SET_FF),)
CDEFS += "-DSET_FF"
endif

ifneq ($(SET_ZERO),)
CDEFS += "-DSET_ZERO=$(SET_ZERO)"
endif

ifneq ($(LDI),)
CDEFS += "-DLDI=$(LDI)"
endif

ifneq ($(BEFORE_LDI),)
CDEFS += "-DBEFORE_LDI='$(BEFORE_LDI)'"
endif

ifneq ($(LDI_AFTER_JMP),)
CDEFS += "-DLDI_AFTER_JMP=$(LDI_AFTER_JMP)"
endif

ifneq ($(SUM_A),)
CDEFS += "-DSUM_A=$(SUM_A)"
endif

ifneq ($(SUM_B),)
CDEFS += "-DSUM_B=$(SUM_B)"
endif

ifneq ($(ADC_AFTER_JMP),)
CDEFS += "-DADC_AFTER_JMP=$(ADC_AFTER_JMP)"
endif

ifneq ($(RETRIGGER),)
CDEFS += "-DRETRIGGER=$(RETRIGGER)"
endif

ifneq ($(FINAL_MOV),)
CDEFS += "-DFINAL_MOV=$(FINAL_MOV)"
endif

#Add simpleserial project to build
include ../simpleserial/Makefile.simpleserial

FIRMWAREPATH = ../.
include $(FIRMWAREPATH)/Makefile.inc

