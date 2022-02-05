#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <direct.h>
#include "menu.c"

int main()
{

  affichage1();
  printf("                                    appyer sur entre pour continuer");
  getch();
  menu();
  return 0;
}
