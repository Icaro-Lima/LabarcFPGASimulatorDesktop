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

RVLDFL=-nostartfiles -T/usr/local/riscv/link.ld

default: $(HDL_SIM) sim_socket.o remote.bin
	$(VERILATOR) $(WARN) -cc --exe +1800-2012ext+sv top.sv veri.cpp ../sim_socket.o $(VBOOST)
	$(MAKE) -j 2 -C obj_dir -f Vtop.mk
	obj_dir/Vtop $(DIVIDE_BY)

isa: a.out spike-gui.bin
	@echo "****** gravar um arquivo de comando a.cmd para avançar o pc até o main"
	echo -n "until pc 0 " >a.cmd
	riscv32-unknown-elf-objdump -j .text -t a.out | grep " main" | cut -d' ' -f1 >>a.cmd
	@echo "****** gravar um arquivo com o executável disassemblado"
	riscv32-unknown-elf-objdump -d -j .text | sed -n '/<main>:/,/<exit>:/p' | sed '/<exit>:/d' | sed "s/  *\t/\t/g" >disa.txt
	@echo "****** chamar o simulador spike"
	rm -f q.log
	( while [ $$(cat q.log 2>/dev/null | wc -l) -eq 0 ]; do sleep 0.2; done; ./spike-gui.bin $$(cut -d' ' -f8 q.log) ) &
	spike -d -s --cmd=a.cmd $(PK) a.out $(ARGS) | tee q.log
	echo $$?

# from assembly to elf
a.out : $(wildcard *.s) $(sort $(patsubst %.c,%.s,$(wildcard *.c)))
	@echo "****** chamar assembler para transformar arquivo(s) assembly em arquivo executável a.out"
	riscv32-unknown-elf-gcc $(RVLDFL) $^
	@if ! [ -z "$(RVLDFL)" ]; then echo "****** A opção -nostartfiles serve para excluir do executável arquivos necessários somente para rodar o executável com Linux. A opção -T\$RISCV/link.ld serve para colocar o main no endereço 0x80000000, ao agrado do spike."; fi

# from C to assembly
%.s : %.c
	@echo "****** compilar arquivo $< para arquivo assembly"
	riscv32-unknown-elf-gcc -O1 -S $<
	@echo "****** conteúdo do arquivo assembly $@:"
	grep -v "^[[:space:]]\." $@
	$(eval RVLDFL := )
	$(eval PK := pk)

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

# from binary to object dump
%.objdump : a.out
	riscv32-unknown-elf-objdump -s -j .text | egrep "( [0-9a-f]{8}){5}" | cut -b11-45 > $@


######################################################################

maintainer-copy::
clean mostlyclean distclean maintainer-clean::
	-rm -rf obj_dir *.h.gch *.o *.bin *.log *.dmp *.vpd core a.out *.objdump

