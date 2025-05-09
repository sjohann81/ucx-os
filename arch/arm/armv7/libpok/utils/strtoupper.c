#include <utils.h>

int chr_toupper(int c)
{
  if (c >= 'a' && c <= 'z') return 'A' + c - 'a';

  return c;
}

void strtoupper(char* s)
{
  int i=0;
  while(s[i]){
     s[i]=chr_toupper(s[i]);
     i++;
     }
}
