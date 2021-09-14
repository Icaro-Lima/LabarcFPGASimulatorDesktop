void __attribute__ ((naked)) main() { // naked significa desconsiderar a pilha, nao precisa salvar nada na pilha
   volatile int * const io = (int *)(0x3F*4); // apontador para entrada/saida
   // o apontador aponta para algo que é volátil, ou seja, algo que pode mudar fora do controle do software
                  // o valor do apontador é constante, ou seja, o endereço não pode mudar
   int x,y;
   x = *io;                  // primeiro pega x de SWI[4:0]
   y = *io;                  // no clock seguinte y
   *io = x<<y;               // resultado vai para saida - LED[4:0]
}
