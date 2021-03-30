######################################################################
#
# DESCRIPTION: Verilator with GUI
#
# This calls the object directory makefile.  That allows the objects to
# be placed in the "current directory" which simplifies the Makefile.
#
# Copyright 2003-2018 by Wilson Snyder. This program is free software; you can
# redistribute it and/or modify it under the terms of either the GNU
# Lesser General Public License Version 3 or the Perl Artistic License
# Version 2.0.
#
# Modifications by Icaro Dantas de Araujo Lima and Elmar Melcher at UFCG, 2018
#
######################################################################

# Check for sanity to avoid later confusion
ifneq ($(words $(CURDIR)),1)
 $(error Unsupported: GNU Make cannot build in directories containing spaces, build elsewhere: '$(CURDIR)')
endif

ifeq ($(findstring MINGW,$(shell uname)),MINGW)
  CFLTK=-I/usr/local/include
  LFLTK=-L/usr/local/lib -mwindows -lfltk_images -lfltk_png -lfltk_z -lfltk -lole32 -luuid -lcomctl32 -lws2_32
  FLTK=-CFLAGS "$(CFLTK)" -LDFLAGS "$(LFLTK)"
else
  LFLTK=-lfltk_images -lpng -lz -lfltk
  FLTK=-LDFLAGS "$(LFLTK)"
endif

UTIL=/labarc/util
HDL_SIM=top.sv # $(UTIL)/inst_sim.sv $(UTIL)/memo_sim.sv
WARN=-Wno-CASEINCOMPLETE -Wno-WIDTH -Wno-COMBDLY
TMP=/tmp${subst $(HOME),,${PWD}}
CUR=${PWD}
CABLE?=1

######################################################################

.SUFFIXES: .objdump .s .c

# If $VERILATOR_ROOT isn't in the environment, we assume it is part of a
# package install, and verilator is in your path. Otherwise find the
# binary relative to $VERILATOR_ROOT (such as when inside the git sources).
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

default:
	@sintetize $(TMP) $(CABLE)

sim: # inst.objdump
	mkdir -p $(TMP)/obj
	$(VERILATOR) --Mdir $(TMP)/obj $(WARN) -cc --exe +1800-2012ext+sv -y $(UTIL)/sim $(HDL_SIM) $(UTIL)/veri.cpp $(UTIL)/sim_socket.o
	$(MAKE) -j 2 -C $(TMP)/obj -f Vtop.mk
	$(TMP)/obj/Vtop

# from assembly to object dump
.s.objdump :
	riscv32-unknown-elf-gcc -nostdlib -nostartfiles -T$(UTIL)/link.ld $< && \
	riscv32-unknown-elf-objdump -s -j .text | egrep " [0-9a-f]{4} [0-9a-f]{8}" | cut -b7-41 > $@

# from C to assembly
.c.s :
	riscv32-unknown-elf-gcc -O1 -S $<

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.s *.o *.log *.dmp *.vpd core a.out *.objdump *.sof *.fit.summary
