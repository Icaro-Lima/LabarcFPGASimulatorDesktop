int lupi(register int x, register int y) {
   register int s=0;
   do {
      s = s+x+y;
      x = x-y;
   } while (x>0);
   return s;
}

