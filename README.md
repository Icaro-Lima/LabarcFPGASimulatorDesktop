# LabarcFPGASimulatorDesktop
![Alt text](MainWindow.png?raw=true "Main Window")
## Instalação

### Para todos os sistemas operacionais

Recomenda-se a instalação desta fonte tipográfica: https://www.dafont.com/led-counter-7.font

Antes de poder usar uma placa FPGA remotamente, você precisa enviar seu arquivo '.ssh/id_rsa.pub'
para o professor.
Se você não tiver este arquivo no seu computador, segue as instruções no primeiro
quadro rosa no link http://www.linuxproblem.org/art_9.html

### Ubuntu 18.04 ou acima

Execute o seguinte, no terminal:

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

### CentOS 7

Execute o seguinte, no terminal:
```
yum -y --enablerepo=epel install verilator fltk-devel make gcc-c++ libpng-devel zlib-devel boost-devel
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
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
```
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
```

### Windows 10 (Alternativa)

1. [Baixe](https://sourceforge.net/projects/xming/) e Instale o Xming

2. Após a instalação, abra o XLauch e coloque 0 no número de display. Avance todos os passos.  
    Caso sinta dificuldade nessa parte, acesse esse [link](https://virtualizationreview.com/articles/2018/01/30/hands-on-with-wsl-graphical-apps.aspx) ou esse [vídeo](https://youtu.be/k4vFBxOFD3E)
    
3. Baixe e Instale o Ubuntu 18.04 (20.04 comprovadamente não funciona) como subsistema do Windows na [Microsoft Store](https://www.microsoft.com/store/productId/9N9TNGVNDL3Q)

4. Inicie o Ubuntu, escolha um nome de usuário e uma senha. Em seguida, entrará em modo de terminal.

5. Vamos baixar e instalar o verilator, no terminal:
```
wget http://old-releases.ubuntu.com/ubuntu/pool/universe/v/verilator/verilator_3.906-1_amd64.deb
sudo dpkg -i verilator_3.906-1_amd64.deb
```

6. Execute os seguintes comandos no terminal:
```
sudo apt-get update
sudo apt-get install libfltk1.3-dev make g++ libpng-dev zlib1g-dev git
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
```

Lembre-se, sempre antes de executar o simulador, faça o passo "2" e digite no Ubuntu: export DISPLAY=:0. Esse comando é responsável por mostrar graficamente janelas dos programas que estão rodando no Ubuntu.

A pasta LabarcFPGASimulatorDesktop se encontra em: 
```
C:\Users\<seu_usuario>\AppData\Local\Packages\CanonicalGroupLimited.Ubuntu18.04onWindows_79rhkp1fndgsc\LocalState\rootfs\home
```
Lá você pode editar o top.sv, e para executar é só ir no bash do Ubuntu, entrar na pasta e dar `make`

> Dica: Você pode instalar o Geany e nele editar o top.sv e com as teclas shift + f9 executar o simulador diretamente. 	

> No terminal:

> Para instalar: `sudo apt-get install geany`

> Para executar, entre na pasta do simulador e coloque: `geany top.sv`

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

## Utilização
Para simular, vá até a pasta `LabarcFPGASimulatorDesktop` e abra o arquivo `top.sv`, edite ele como quiser, para rodar o simulador basta dar 'make'.

Para usar uma placa FPGA remotamente, navegue para http://lad.dsc.ufcg.edu.br/hdl/sintese.php
e faça upload do arquivo `top.sv`. No final da síntese deve aparecer uma instrução que
pede dar o comando './remote' com dois argumentos. Este comando deve ser dado dentro da pasta
`LabarcFPGASimulatorDesktop`. É preciso rodar o simulador antes de usar uma placa FPGA remotamente.

### Vídeo mostrando o passo a passo
https://youtu.be/mrfyZOujHHI (desatualizado)
