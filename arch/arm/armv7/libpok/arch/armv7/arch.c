/*
 * arch.c
 *
 *  Created on: 29 juil. 2019
 *      Author: Alexy
 */


#include "stddef.h"

void pok_arch_idle (void)
{
   while (1);
}

size_t libja_mem_get_alignment(size_t obj_size)
{
   if(obj_size <= 1) {
      return 1;
   }
   else if(obj_size < 4) {
      return 2;
   }
   else if(obj_size < 8) {
      return 4;
   }
   else if(obj_size < 16) {
      return 8;
   }
   else {
      return 16;
   }
}
