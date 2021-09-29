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
  CFLTK=-I/mingw64/include
  LFLTK=-L/mingw64/lib -mwindows -lfltk_images -lfltk -lole32 -luuid -lcomctl32 -lws2_32
  FLTK=-CFLAGS "$(CFLTK)" -LDFLAGS "$(LFLTK)"
  CTHREAD=-DMINGW
  VBOOST=-LDFLAGS "-L/mingw64/lib -lboost_system-mt -lws2_32 -lwsock32"
else
  LFLTK=-lfltk_images -lpng -lz -lfltk
  FLTK=-LDFLAGS "$(LFLTK)"
  BOOST=-std=c++11 -lboost_system -lpthread
  VBOOST=-CFLAGS "-std=c++11" -LDFLAGS "-lboost_system -lpthread"
endif

HDL_SIM=$(wildcard *.sv)
WARN=-Wno-CASEINCOMPLETE -Wno-WIDTH -Wno-COMBDLY

######################################################################


# If $VERILATOR_ROOT isn't in the environment, we assume it is part of a
# package install, and verilator is in your path. Otherwise find the
# binary relative to $VERILATOR_ROOT (such as when inside the git sources).
ifeq ($(VERILATOR_ROOT),)
VERILATOR = verilator
else
export VERILATOR_ROOT
VERILATOR = $(VERILATOR_ROOT)/bin/verilator
endif

TMP=/tmp${subst $(HOME),,${PWD}}
CUR=${PWD}
CABLE?=1
CABLE_1=$(shell expr $(CABLE) - 1)
DIVIDE_BY=$(shell grep parameter top.sv | grep divide_by | grep -oP '(?<!\d)\d*(?!\d)' )

# if there is a .101 file, use that one to create a.out instead of C or assembly
ifeq ($(wildcard *.101),)
aout_exe = aout_s
else
aout_exe = aout_101
endif

# if there are program ARGS, we need the proxy kernel
ifeq ($(ARGS),)
RVLDFL=-nostartfiles -T/usr/local/riscv/link.ld
PK =
else
RVLDFL =
PK = pk
endif

default: $(HDL_SIM) sim_socket.o remote.bin
	$(VERILATOR) $(WARN) -cc --exe +1800-2012ext+sv top.sv veri.cpp ../sim_socket.o $(VBOOST)
	$(MAKE) -j 2 -C obj_dir -f Vtop.mk
	obj_dir/Vtop $(DIVIDE_BY)

# this only works on a LABARC workstation with Quartus properly configured 
syn: $(HDL_SIM) inst.objdump
	@sintetize $(TMP) $(CABLE)
	quartus_stp -t /labarc/util/inst.tcl inst.objdump $(CABLE_1) | egrep --line-buffered "(Error|Running|successful)"
	quartus_stp -t /labarc/util/qr.tcl $(CABLE_1) | egrep --line-buffered "(Error|Running|successful)"

# from elf to object dump
%.objdump : a.out
	@echo "****** disassembly listing of executable file?"
	riscv32-unknown-elf-objdump -d -j .text | sed -n '/<main>:/,/<exit>:/p' | sed '/<exit>:/d' | sed "s/  *\t/\t/g"
	@echo "****** from the executable file called a.out, create a file called $@ which contains only RISC-V instruction as hexadecimal numbers"
	riscv32-unknown-elf-objdump -s -j .text | egrep "^( [0-9a-f]{8}){2}" | cut -b11-45 > $@

isa: a.out spike-gui.bin
	@echo "****** create a spike command file called a.cmd which will advance the pc until main at the start of the simulation"
	echo -n "until pc 0 " >a.cmd
	riscv32-unknown-elf-objdump -j .text -t a.out | grep " main" | cut -d' ' -f1 >>a.cmd
	@echo "****** write disassembly listing of executable file to a file called disa.txt, which will be read by spike GUI"
	riscv32-unknown-elf-objdump -d -j .text | sed -n '/<main>:/,/<exit>:/p' | sed '/<exit>:/d' | sed "s/  *\t/\t/g" >disa.txt
	@echo "****** call RISC-V ISA simulator spike with GUI"
	rm -f q.log
	( while [ $$(cat q.log 2>/dev/null | wc -l) -eq 0 ]; do sleep 0.2; done; ./spike-gui.bin $$(cut -d' ' -f8 q.log) ) &
	(spike -d -s --debug-cmd=a.cmd $(PK) a.out $(ARGS); echo $$? ) | tee q.log

a.out:  $(wildcard *.[sc1]*)
	@make $(aout_exe)

# from assembly to elf
aout_s : $(wildcard *.s) $(sort $(patsubst %.c,%.s,$(wildcard *.c)))
	@echo "****** call assembler to transform assembly file(s) $^ into one executable file called a.out"
	riscv32-unknown-elf-gcc $(RVLDFL) $^
	@if ! [ -z "$(RVLDFL)" ]; then echo "****** The option -nostartfiles excludes code that is only necessary to run the executable file on Linux. The option -T\$(RISCV)/link.ld puts main at address 0x80000000, as spike demands."; fi

# from .101 to elf
aout_101 : $(wildcard *.101) binmake
	@echo "****** remove comment and space from $^ and transform the strings of 0's and 1's to binary values"
	grep ^\ *[01] $< | sed 's/;.*//g' | sed 's/ //g' | ./binmake
	@echo "****** The binary values are now in a bare metal file called a.bin ."
	@echo "****** create an executable file called a.out, copying from another file (riscv.out) while inserting a.bin after main"
	riscv32-unknown-elf-objcopy --update-section .text=a.bin --strip-symbol=_end server/riscv.out a.out

# from C to assembly
%.s : %.c
	@echo "****** compile the file called $< to an assembly file called $@"
	riscv32-unknown-elf-gcc -O1 -S $<
	@echo "****** conteúdo do arquivo assembly $@:"
	grep -v "^[[:space:]]\." $@

binmake: binmake.cc
	gcc -o binmake binmake.cc

remote.bin: remote.cpp communicator.o gui.o
	$(CXX) $(CFLTK) remote.cpp communicator.o gui.o -o remote.bin $(BOOST) $(LFLTK)

spike-gui.bin: spike-gui.cpp communicator.o 
	$(CXX) $(CFLTK) -o spike-gui.bin $< communicator.o $(BOOST) $(LFLTK)

sim_socket.o: sim_socket.cpp
	$(CXX) -std=c++11 $(CTHREAD) -c $<

gui.o: gui.cpp gui.h
	$(CXX) $(CFLTK) -c $<

communicator.o: communicator.cpp communicator.h
	$(CXX) -std=c++11  $(CTHREAD) -c communicator.cpp


######################################################################

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.h.gch *.o *.bin *.log *.dmp *.vpd core a.out *.objdump binmake

isa-clean::
	-rm -f a.out *.s *.c *.objdump *.101

