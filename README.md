# LabarcFPGASimulatorDesktop
![Alt text](MainWindow.png?raw=true "Main Window")
## Instalação
### Ubuntu (testado no 16.04, 17.10 e 18.04)

Baixe o `verilator_3.906-1` por este link: http://cz.archive.ubuntu.com/ubuntu/pool/universe/v/verilator/verilator_3.906-1_amd64.deb

Instale o package

Agora execute o seguinte, no terminal:

```
sudo apt-get update
sudo apt-get install verilator=3.906-1 libfltk1.3-dev make g++ libpng-dev zlib1g-dev
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
```
### CentOS 7
```
yum -y --enablerepo=epel install verilator fltk-devel make gcc-c++ libpng-devel zlib-devel
```
### Windows 10
Download and start https://sourceforge.net/projects/mingw/files/Installer/mingw-get-setup.exe/download

In the MinGW Installation Manager select:
mingw-developper-toolkit
mingw32-base
mingw32-gcc-g++
msys-base

Create shortcut to
C:\MinGW\msys\1.0\msys.bat
and put it on the desktop

Double-click on the shortcut.
This opens a terminal window and creates the directory
C:\MinGW\msys\1.0\home\<your name>

copiar no Windows Explorer o arquivo FlexLexer.h de
MinGW/msys/1.0/include para MinGW/include

Download fltk-1.3.4-2-source.tar.gz from http://www.fltk.org
and copy the file to
C:\MinGW\msys\1.0\home\<your name>

In the terminal window type:
tar xf fltk-1.3.4-2-source.tar.gz

Entre no diretório fltk-1.3.4-2.

conforme https://groups.google.com/forum/#!topic/fltkgeneral/u8FMN02tdXA
usando vim ou WordPad adicione a linha:
#include <winerror.h>
somewhere at the top of src/Fl_win32.cxx.

Dê o comando:
./configure

Then edit the generated file makeinclude so it reads:
```
LDLIBS          =  -lole32 -luuid -lcomctl32 -lws2_32
GLDLIBS         = -lglu32 -lopengl32  -lole32 -luuid -lcomctl32 -lws2_32
```

Dê os comandos:
```
make
make install
```

para termos um editor simples:
cp test/editor.exe /usr/local/bin/

volte para a raiz do seu home:
cd

faça download de https://www.veripool.org/ftp/verilator-3.922.tgz
and copy the file to
C:\MinGW\msys\1.0\home\<your name>

In the terminal window type:
```
tar xf verilator-3.922.gz
cd verilator-3.922
sed -i 's/, 0777//g' examples/tracing_c/sim_main.cpp
./configure
make
make install
```
(ignorar a mensagem de erro "preserving permissions ...: Permission denied")
## Utilização
Vá até a pasta `LabarcFPGASimulatorDesktop` e abra o arquivo `top.sv`, edite ele como quiser, para rodar o simulador basta dar 'make'.

### Vídeo mostrando o passo a passo
https://youtu.be/mrfyZOujHHI
