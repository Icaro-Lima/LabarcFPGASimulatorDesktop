# Labarc FPGA Simulator
<img src="http://lad.ufcg.edu.br/loac/uploads/OAC/MainWindow.png">

The simulator can be installed on Your PC (no internet link needed after installation)
or accessed by means of a web browser interface (no installation required).

## How to install the simulator on Your desktop

### For all operating systems

Installation of this typefont is recomended:
http://lad.ufcg.edu.br/loac/uploads/OAC/led_counter-7.ttf

### Ubuntu 18.04 to 19.10

Execute the following commands in a terminal window:

```
sudo apt-get update
sudo apt-get install libfltk1.3-dev make g++ libpng-dev zlib1g-dev git flex bison libfl-dev libboost-all-dev
wget https://www.veripool.org/ftp/verilator-3.922.tgz
tar xf verilator-3.922.tgz
cd verilator-3.922
./configure
make
sudo make install
cd ..
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
```

#### Video showing step by step
http://lad.ufcg.edu.br/loac/uploads/OAC/icaro_install.mp4

### Ubuntu 20.04 and above

Execute the following commands in a terminal window:

```
sudo apt-get update
sudo apt-get install verilator libfltk1.3-dev make g++ libpng-dev zlib1g-dev git flex bison libfl-dev libboost-all-dev
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
```

### CentOS 7

Execute the following commands in a terminal window:

```
yum -y --enablerepo=epel install verilator fltk-devel make gcc-c++ libpng-devel zlib-devel boost-devel
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
```

### Android

Install *VNC Viewer* and *Userland*. Inside *Userland*, choose *Ubuntu*.
Inside Ubuntu, follow instruction above for Ubuntu 18.04.

Restrictions: maximum 32 bit variables.
Only the rightmost 8 digits of the LCD display (`lcd_a` and `lcd_b`) are working.

### Windows 10

Download MSYS2 from https://www.msys2.org/
and install.

Open an MSYS2 terminal window and execute the following commands:
```
pacman -Syu
pacman -Sy
pacman -Su
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
```

When asked which packages of MinGW You want to install, answer "1-19". Do not answer "all".

When finished, close the MSYS2 terminal window and open a new MinGW terminal window.
Execute the following commands:

**Caution: You must change the terminal window from MSYS2 to MinGW**
```
pacman -S git make mingw-w64-x86_64-verilator mingw-w64-x86_64-fltk mingw-w64-x86_64-boost
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
```

Restrictions: maximum 32 bit variables.
Only the rightmost 8 digits of the LCD display (`lcd_a` and `lcd_b`) are working.

### Mac OS (Testado no High Sierra)
Baixe o _homebrew_ na sua máquina (se já tiver, não precisa baixar novamente)
```
ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)" < /dev/null 2> /dev/null
```
Após isso basta executar os seguintes comandos:
```
brew install verilator
brew install fltk
```

### Mac Os Mojave Workaround
Existe uma issue na biblioteca fltk no macOS Mojave, em que o simulador fica completamente vazio ao realizar `make`. O problema foi corridor por [este commit](https://github.com/fltk/fltk/commit/f76d2a2bf8c35c0c313f05bbd6deda49dd344efc). Caso este seja o seu caso, existe um *workaround* possível:

1. Remover qualquer versão existente da FLTK
`brew uninstall fltk`

2. Clonar o repositório com a versão nova de FLTK
`git clone https://github.com/fltk/fltk.git`

3. Navegue até o repositório
`cd fltk`

4. Realizar checkout no commit com o fix da issue
`git checkout f76d2a2bf8c35c0c313f05bbd6deda49dd344efc`

5. Instale
`make clean && make install`

OBS.: Não há uma previsão de quando haverá uma nova atualização no pacote disponível no Homebrew com o fix dessa issue.

## How to use the desktop simulator

Change into the directory  `LabarcFPGASimulatorDesktop` and open the file `top.sv`. Edit it as You wish. In order to run the simulation type `make`.

### Display refresh frequency

The display is updated once per second, indepenetly from the frequency of the `clk_2` input. Only if the frequency of `clk_2` is equal or less than 0.5&nbsp;Hz, it can be observerd correctly. If the frequency of `clk_2` is greater than 1&nbsp;Hz, the display may skip some changes which would be visible on the display of the real FPGA board.

### RISC-V control signals on LCD

The rightmost character of the first line shows the signal called `Branch`. `_` means `0` and `*` means `1`.
The next character to the left shows `MemWrite`.
The rightmost character of the second line shows `RegWrite` asnd the adjacent character to the left shows `MemtoReg`.

### RISC-V register file

Only the first 16 registers are shown. The remaining registers can be used but are not shown.

## How to use the simulador via an internet browser

Use the link http://lad.ufcg.edu.br/hdl/simulate.php
and upload Your files. The GUI will appear in the browser.

# How to use the remote FPGA Lab

There are 20 FPGA boards similar to this one, available for simulataneous use: 
<img src="http://lad.ufcg.edu.br/loac/uploads/OAC/nano.jpg" width="400">

Use the link http://lad.ufcg.edu.br/hdl/remote.php
and upload Your files. After completing synthesis, the GUI will appear in the browser.
The GUI reflects the position of the switches, the LEDs and the LCD of the board You are using. 

Close the window or tab of the browser to end the session and make the FPGA board available for another users.
After a timeout of several minutes the FPGA is disconnected from Your session automatically.

### Video showing simulator and remote access step by step

http://lad.ufcg.edu.br/loac/uploads/OAC/remote_FPGA.mp4

# RISC-V ISA Simulator

The RISC-V toolchain 
É necessário ter instalado a toolchain RISC-V, incluíndo o comando `spike`. Aparentemente não é possivel instalar-lo no Windows.

## Utilização do simulador ISA

Vá até a pasta `LabarcFPGASimulatorDesktop` e coloque arquivos em C (`.c`) ou arquivos em assembly (`.s`), ou um arquivo de código de máquina (`.101`) nele. Exemplos podem ser encontradas na sub-pasta `server`. Dê o comando:

```
make isa
```

Ao colocar um arquivo de código de máquina, arquivos em C e arquivos em assembly serão ignorados.

Para limpar todos os arquivos relacionados ao simulador, inclusive arquivos fonte, dê o comando

```
make isa-clean
```

## Utilização do simulador ISA remotamente

Navegue para http://lad.ufcg.edu.br/isa

# Publicações

Í. Lima and E. Melcher, “Um Simulador Didático para o Ensino de SystemVerilog”, Workshop sobre Educação em Arquitetura de Computadores, 2018:
http://lad.dsc.ufcg.edu.br/lad/uploads/Lad/WEAC_2018.pdf

Anais do WCAS 20211 com artigo "Remote FPGA Lab for Distance Learning" nas páginas 27-30:
https://bit.ly/wcas21

Vídeo da apresentação: https://youtu.be/PMbBgx9OTNU
