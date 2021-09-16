# Labarc FPGA Simulator
<img src="http://lad.ufcg.edu.br/loac/uploads/OAC/MainWindow.png">

## Instalação do simulador no desktop

### Para todos os sistemas operacionais

Recomenda-se a instalação desta fonte tipográfica:
http://lad.ufcg.edu.br/loac/uploads/OAC/led_counter-7.ttf

### Ubuntu 18.04 até 19.10

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
http://lad.ufcg.edu.br/loac/uploads/OAC/icaro_install.mp4

### Ubuntu 20.04 e acima

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

### Android

Instale VNC Viewer e Userland. Dentro do Userland, escolhe Ubuntu.
Dentro do Ubuntu, siga as instruções acima para Ubuntu 18.04.

Limitação: variáveis de no máximo 32 bits. Do LCD (lcd_a e lcd_b), só funcionam os 8 dígitos hexadecimais da direita.

### Windows 10

Baixe o MSYS2 através do link: https://www.msys2.org/

Após concluir a instalação, abra o terminal MSYS2 e execute os seguintes comandos:
```
pacman -Syu
pacman -Sy
pacman -Su
pacman -S --needed base-devel mingw-w64-x86_64-toolchain
```

No passo anterior, quando questionado quais pacotes do MinGW deseja instalar, digite: "1-19", ao invés de "all".

Após concluído, feche o terminal e abra um novo, agora do **MinGW**, e execute os seguintes comandos:

**Nota: perceba que o terminal não é o mesmo em que você executou os últimos comandos, o Msys2.**
```
pacman -S git make mingw-w64-x86_64-verilator mingw-w64-x86_64-fltk mingw-w64-x86_64-boost
git clone https://github.com/Icaro-Lima/LabarcFPGASimulatorDesktop.git
cd LabarcFPGASimulatorDesktop
```

Limitação: variáveis de no máximo 32 bits. Do LCD (lcd_a e lcd_b), só funcionam os 8 dígitos hexadecimais da direita.

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

## Utilização do simulador no desktop

Vá até a pasta `LabarcFPGASimulatorDesktop` e abra o arquivo `top.sv`, edite ele como quiser, para rodar o simulador basta dar `make`.

### Frequencia de atualização do display

A GUI se atualiza a cada segundo, independentemente da frequência do sinal `clk_2`. Só se a frequencia do sinal `clk_2` for igual ou menor a 0.5&nbsp;Hz, este sinal pode ser observado corretamente na GUI. Se a frequencia do sinal `clk_2` for maior do que 1&nbsp;Hz, a visualização da GUI pode estar pulando mudanças rápidas que eventualmente poderiam ser vistas no display da placa física.

### Sinais de controle do RISC-V

No caso do RISC-V, o caractere mais a direita na primeira linha do LCD visualiza o valor do sinal `Branch`,
sendo que `_` significa `0` e `*` significa `1`. O caractere imediatamente a esquerda visualiza o sinal `MemWrite`. O caractere mais a direita na segunda  linha visualiza o sinal `RegWrite` e o caractere imediatamente a esquerda deste visualiza o sinal `MemtoReg`. Somente os primeiros 16 registradores aparecem na GUI. Os demais registradores podem ser usados normalmente, mas não são visualizados na GUI.

## Utilização do simulador remotamente

Navegue para http://lad.ufcg.edu.br/hdl/simulate.php
e faça upload do arquivo `top.sv`. No final da compilação, a GUI aparece na janela do browser.

Se, além do arquivo `top.sv`, mais arquivos forem necessários, coloque eles, junto com o arquivo `top.sv`, dentro de um arquivo `top.zip` e faça upload do arquivo `top.zip` no lugar do `top.sv`.

# Remote FPGA Lab

Temos conectadas atualmente 20 placas FPGA como esta, para uso simultâneo:  
<img src="http://lad.ufcg.edu.br/loac/uploads/OAC/nano.jpg" width="400">

Navegue para http://lad.ufcg.edu.br/hdl/remote.php
e faça upload do arquivo `top.sv`. No final da síntese, a GUI aparece na janela do browser.

Se, além do arquivo `top.sv`, mais arquivos forem necessários, coloque eles, junto com o arquivo `top.sv`, dentro de um arquivo `top.zip` e faça upload do arquivo `top.zip` no lugar do `top.sv`.

Feche a aba ou janela do navegador para encerrar o uso da placa FPGA e liberar-la para outro usuário. Caso não fizer isso, depois de alguns minutos a placa FPGA será liberada assim mesmo.

### Vídeo mostrando o passo a passo

O vídeo mostra a utilização do simulador no desktop seguida da utilização de uma FPGA remota, depois é feita uma alteração do SystemVerilog, com nova simulação e utilização de FPGA remota. 

http://lad.ufcg.edu.br/loac/uploads/OAC/remote_FPGA.mp4

# RISC-V ISA Simulator

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
