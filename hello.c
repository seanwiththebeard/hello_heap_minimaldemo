
/*
A simple "hello world" example.
Set the screen background color and palette colors.
Then write a message to the nametable.
Finally, turn on the PPU to display video.
*/

#include "neslib.h"
#include <stdio.h>
#include <stdlib.h>
#include <_heap.h>
#include <string.h>
//extern unsigned*          __heaporg;

#define NES_MAPPER 4		// Mapper 4 (MMC3)
#define NES_PRG_BANKS 4		// # of 16KB PRG banks
#define NES_CHR_BANKS 1		// # of 8KB CHR banks
//#resource "nesbanked.cfg"
#define CFGFILE nesbanked.cfg

void heap_avail(void)
{
  int x;
  char *t;
  char stringA[24] = "";
  x=10;
  while(1)
  {
    t=malloc(x);
    if ( !t ) break;
    free(t);
    x+=10;
    sprintf(stringA, "heap avail: %u bytes",x);
    vram_adr(NTADR_A(2,2));		// set address
    vram_write(stringA, strlen(stringA));
  }
  if ( x > 10 ) 
    x-=10;
  else
    x=0;


}

// link the pattern table into CHR ROM
//#link "chr_generic.s"

// main function, run after console reset
void main(void) {
  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white

  // write text to name table
  vram_adr(NTADR_A(2,2));		// set address
  vram_write("HELLO, WORLD!", 13);	// write bytes to video RAM
  
  heap_avail();
  
  // enable PPU rendering (turn on screen)
  ppu_on_all();

  // infinite loop
  while (1) ;
}
