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
# This Makefile is specialized to be used by the simulation and remote lab server.
#
######################################################################

# Check for sanity to avoid later confusion
ifneq ($(words $(CURDIR)),1)
 $(error Unsupported: GNU Make cannot build in directories containing spaces, build elsewhere: '$(CURDIR)')
endif

LFLTK=-lfltk_images -lpng -lz -lfltk
FLTK=-LDFLAGS "$(LFLTK)"
VBOOST=-CFLAGS "-std=c++11" -LDFLAGS "-lboost_system -lpthread"

HDL_SIM=top.sv
WARN=-Wno-CASEINCOMPLETE -Wno-WIDTH -Wno-COMBDLY
TMP=/tmp${subst $(HOME),,${PWD}}
CUR=${PWD}
CABLE?=1
DIVIDE_BY?=0
include Make_pk

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

default: $(HDL_SIM)
	@sintetize $(TMP) $(CABLE)

sim: $(HDL_SIM)
	mkdir -p $(TMP)/obj
	$(VERILATOR) --Mdir $(TMP)/obj $(WARN) -cc --exe +1800-2012ext+sv $(HDL_SIM) /labarc/util/veri.cpp /labarc/util/sim_socket.o $(FLTK) $(VBOOST)
	$(MAKE) -j 2 -C $(TMP)/obj -f Vtop.mk
	$(TMP)/obj/Vtop $(DIVIDE_BY) 1>&2

# from binary to object dump
%.objdump : a.out
	riscv32-unknown-elf-objdump -s -j .text | egrep "^( [0-9a-f]{8}){2}" | cut -b11-45 > $@

isa: a.out
	echo -n "until pc 0 " >a.cmd
	riscv32-unknown-elf-objdump -j .text -t a.out | grep " main" | cut -d' ' -f1 >>a.cmd
	prlimit --cpu=5 spike -d -s --cmd=a.cmd $(PK) a.out $(PROG_ARGS); echo $$?

# from .101 to binary
binary : $(wildcard *.101)
	grep ^[01] $< | sed 's/;.*//g' | sed 's/ //g' | binmake 
	riscv32-unknown-elf-objcopy --update-section .text=a.bin --strip-symbol=_end /labarc/util/riscv.out a.out

# from assembly to binary
a.out : $(wildcard *.s) $(sort $(patsubst %.c,%.s,$(wildcard *.c)))
	riscv32-unknown-elf-gcc $(RVLDFL) $^
	echo \<pre\>; riscv32-unknown-elf-objdump -d -j .text | sed -n '/<main>:/,/<exit>:/p' | head -n -1 | sed 's/>/\&\#62/g' | sed 's/</\&\#60/g'; echo \</pre\>

# from C to assembly
%.s : %.c
	riscv32-unknown-elf-gcc -O1 -S $<
	echo \<pre\>; grep -v "^[[:space:]]\." $@ | sed 's/^\t/\&nbsp;\t/g'; echo \</pre\>

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.s *.o *.log *.dmp *.vpd core a.out *.objdump *.sof *.fit.summary *.objdir
