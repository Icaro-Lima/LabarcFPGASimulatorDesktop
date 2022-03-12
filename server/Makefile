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
	@echo "<span style=color:blue>compile and simulate Systemverilog</span>" 1>&2
	mkdir -p $(TMP)/obj
	$(VERILATOR) --Mdir $(TMP)/obj $(WARN) -cc --exe +1800-2012ext+sv -I/labarc/util/sim $(HDL_SIM) /labarc/util/veri.cpp /labarc/util/sim_socket.o $(FLTK) $(VBOOST)
	$(MAKE) -j 2 -C $(TMP)/obj -f Vtop.mk
	$(TMP)/obj/Vtop $(DIVIDE_BY)

# from binary to object dump
%.objdump : a.out
	@echo "<span style="color:blue">from the executable file called a.out, create a file called $@ which contains only RISC-V instruction as hexadecimal numbers</span>"
	riscv32-unknown-elf-objdump -s -j .text | egrep "^( [0-9a-f]{8}){2}" | cut -b11-45 > $@ 

isa: a.out
	@echo "<span style="color:blue">create a spike command file called a.cmd which will advance the pc until <i>main</i> at the start of the simulation</span>"
	echo -n "until pc 0 " >a.cmd
	riscv32-unknown-elf-objdump -j .text -t a.out | grep " main" | cut -d' ' -f1 >>a.cmd
	@echo "<span style="color:blue">call RISC-V ISA simulator <i>spike</i>. Limit server CPU usage to 5 seconds.</span>"
	prlimit --cpu=5 spike -d -s --cmd=a.cmd $(PK) a.out $(PROG_ARGS); echo $$?

# from .101 to elf
binary : $(wildcard *.101)
	@echo "<span style="color:blue">remove comment and space from $^ and transform the strings of 0's and 1's to binary values</span>"
	grep ^\ *[01] $< | sed 's/;.*//g' | sed 's/ //g' | binmake
	@echo "<span style="color:blue">The binary values are now in a  <i>bare metal</i> file called a.bin .</span>"
	@echo "<span style="color:blue">create an executable file called a.out, copying from another file (riscv.out) while inserting a.bin after <i>main</i></span>"
	riscv32-unknown-elf-objcopy --update-section .text=a.bin --strip-symbol=_end /labarc/util/riscv.out a.out
	@echo "<span style="color:blue">listing of the executable file a.out, in hexadecimal and disassembly:</span>"
	@echo \<pre\>; riscv32-unknown-elf-objdump -d -j .text | sed -n '/<main>:/,/<exit>:/p' | head -n -1 | sed 's/>/\&\#62/g' | sed 's/</\&\#60/g'; echo \</pre\>

# from assembly to elf
a.out : $(wildcard *.s) $(sort $(patsubst %.c,%.s,$(wildcard *.c)))
	@echo "<span style="color:blue">call assembler to transform assembly file(s) $^ into one executable file called a.out</span>"
	riscv32-unknown-elf-gcc $(RVLDFL) $^ 2>&1
	@if ! [ -z "$(RVLDFL)" ]; then echo "<span style="color:blue">The option -nostartfiles excludes code that is only necessary to run the executable file on Linux. The option -T\$(RISCV)/link.ld puts <i>main</i> at address 0x80000000, as spike demands.</span>"; fi
	@echo "<span style="color:blue">listing of the executable file a.out, in hexadecimal and disassembly:</span>"
	@echo \<pre\>; riscv32-unknown-elf-objdump -d -j .text | sed -n '/<main>:/,/<exit>:/p' | sed '/<exit>:/d' | sed 's/>/\&\#62/g' | sed 's/</\&\#60/g'; echo \</pre\>

# from C to assembly
%.s : %.c
	@echo "<span style="color:blue">compile the file called $< to an assembly file called $@</span>"
	riscv32-unknown-elf-gcc -O1 -S $< 2>&1
	@echo "<span style="color:blue">listing of the assembly file $@:</span>"
	@echo \<pre\>; grep -v "^[[:space:]]\." $@ | sed 's/^\t/\&nbsp;\t/g'; echo \</pre\>

# from C++ to assembly
%.s : %.cpp
	@echo "<span style="color:blue">compile the file called $< to an assembly file called $@</span>"
	riscv32-unknown-elf-g++ -O1 -S $< 2>&1
	@echo "<span style="color:blue">listing of the assembly file $@:</span>"
	@echo \<pre\>; grep -v "^[[:space:]]\." $@ | sed 's/^\t/\&nbsp;\t/g'; echo \</pre\>

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.s *.o *.log *.dmp *.vpd core a.out *.objdump *.sof *.fit.summary *.objdir
