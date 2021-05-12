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

DIVIDE_BY=$(shell grep parameter top.sv | grep divide_by | grep -oP '(?<!\d)\d*(?!\d)' )

default: $(HDL_SIM) sim_socket.o remote.bin
	$(VERILATOR) $(WARN) -cc --exe +1800-2012ext+sv top.sv veri.cpp ../sim_socket.o $(VBOOST)
	$(MAKE) -j 2 -C obj_dir -f Vtop.mk
	obj_dir/Vtop $(DIVIDE_BY)

call: gui.o
	$(VERILATOR) $(WARN) -cc --exe +1800-2012ext+sv top.sv sim_main.cpp ../gui.o $(FLTK)
	$(MAKE) -j 2 -C obj_dir -f Vtop.mk
	obj_dir/Vtop

remote.bin: remote.cpp gui.o
	$(CXX) $(CFLTK) remote.cpp gui.o -o remote.bin $(BOOST) $(LFLTK)

sim_socket.o: sim_socket.cpp
	$(CXX) -std=c++11 $(CTHREAD) -c sim_socket.cpp

gui.o: gui.cpp gui.h
	$(CXX) $(CFLTK) -c gui.cpp

# from assembly to object dump
%.objdump : $(wildcard *.s) $(sort $(patsubst %.c,%.s,$(wildcard *.c)))
	riscv32-unknown-elf-gcc -nostartfiles -T$$RISCV/link.ld $^
	riscv32-unknown-elf-objdump -s -j .text | egrep " [0-9a-f]{4} [0-9a-f]{8}" | cut -b7-41 > $@

# from C to assembly
%.s : %.c
	riscv32-unknown-elf-gcc -O1 -S $<



######################################################################

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.h.gch *.o *.bin *.log *.dmp *.vpd core a.out *.objdump

