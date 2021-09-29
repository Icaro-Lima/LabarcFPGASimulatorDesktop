int main(int argc,char *argv[]) {
   register int x,y;
   x = argv[1][0]-'0';       // primeira letra do primeiro argumento convertida para valor entre 0 e 9
   y = argv[2][0]-'0';       // primeira letra do segundo argumento
   return x<<y;
}
