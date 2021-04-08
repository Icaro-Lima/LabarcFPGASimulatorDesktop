# Labarc FPGA Simulator and Remote LAB
![Alt text](MainWindow.png?raw=true "Main Window")

Work inspired from https://fpga-vision-lab.h-brs.de/weblab/login

## Instalação

### Para todos os sistemas operacionais

Recomenda-se a instalação desta fonte tipográfica:
http://lad.dsc.ufcg.edu.br/loac/uploads/OAC/led_counter-7.ttf

### Ubuntu 18.04 até 20.04

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

#### Vídeo mostrando o passo a passo
http://lad.dsc.ufcg.edu.br/loac/uploads/OAC/icaro_install.mp4

### Ubuntu 20.10

(ainda em análise)
```
sudo apt-get update
sudo apt-get install verilator libfltk1.3-dev make g++ libpng-dev zlib1g-dev git flex bison libfl-dev libboost-all-dev
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

Follow steps 1. to 8. in https://www.msys2.org


At step 6, first execute
```
pacman -Sy
```
before
```
pacman -Su
```

At step 7, when asked to choose, answer "1-19" instead of "all"

Abre uma janela de terminal MinGW (não MSys2) e execute o seguinte:
```
pacman -S git mingw-w64-x86_64-verilator mingw-w64-x86_64-fltk mingw-w64-x86_64-boost
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
sudo apt-get install libfltk1.3-dev make g++ libpng-dev zlib1g-dev git libboost-all-dev
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

Vá até a pasta `LabarcFPGASimulatorDesktop` e abra o arquivo `top.sv`, edite ele como quiser, para rodar o simulador basta dar `make`.

No caso do RISC-V, o caractere mais a direita na primeira linha do LCD visualiza o valor do sinal `Branch`,
sendo que `_` significa `0` e `*` significa `1`. O caractere imediatamente a esquerda visualiza o sinal `MemWrite`. O caractere mais a direita na segunda  linha visualiza o sinal `RegWrite` e o caractere imediatamente a esquerda deste visualiza o sinal `MemtoReg`. Somente os primeiros 16 registradores aparecem na GUI. Os demais registradores podem ser usados normalmente, mas não são visualizados na GUI.

## Usar o simulador remotamente

Navegue para http://lad.ufcg.edu.br/hdl/simulate.php
e faça upload do arquivo `top.sv`. No final da compilação, a GUI aparece na janela do browser.

## Usar uma placa FPGA remotamente

Navegue para http://lad.ufcg.edu.br/hdl/remote.php
e faça upload do arquivo `top.sv`. No final da síntese, a GUI aparece na janela do browser.

Feche a aba ou janela do navegador para encerrar o uso da placa FPGA e liberar-la para outro usuário. Caso não fizer isso, depois de alguns minutos a placa FPGA será liberada assim mesmo.

#### Vídeo mostrando o passo a passo

Este vídeo está desatualizado.

http://lad.dsc.ufcg.edu.br/loac/uploads/OAC/remote_FPGA.mp4
