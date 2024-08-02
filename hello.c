
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
#include <peekpoke.h>

#define length 128


#define NES_MAPPER 4		// Mapper 4 (MMC3)
#define NES_PRG_BANKS 32	// # of 16KB PRG banks
#define NES_CHR_BANKS 32	// # of 8KB CHR banks
//#resource "nesbanked.cfg"
#define CFGFILE nesbanked.cfg

// link the pattern table into CHR ROM
//#link "chr_generic.s"

#define MMC_MODE 0x00

#define MMC3_SET_REG(r,n)\
	POKE(0x8000, MMC_MODE|(r));\
	POKE(0x8001, (n));

#define MMC3_CHR_0000(n) MMC3_SET_REG(0,n)
#define MMC3_CHR_0800(n) MMC3_SET_REG(1,n)
#define MMC3_CHR_1000(n) MMC3_SET_REG(2,n)
#define MMC3_CHR_1400(n) MMC3_SET_REG(3,n)
#define MMC3_CHR_1800(n) MMC3_SET_REG(4,n)
#define MMC3_CHR_1C00(n) MMC3_SET_REG(5,n)
#define MMC3_PRG_8000(n) MMC3_SET_REG(6,n)
#define MMC3_PRG_A000(n) MMC3_SET_REG(7,n)

#define MMC3_MIRROR(n) POKE(0xa000, (n))

//; WRAM_OFF $40
//; WRAM_ON $80
//; WRAM_READ_ONLY $C0
#define MMC3_WRAM_DISABLE() POKE(0xA001, 0x40)
#define MMC3_WRAM_ENABLE() POKE(0xA001, 0x80)
#define MMC3_WRAM_READ_ONLY() POKE(0xA001, 0xC0)

#pragma code-name(push, "CODEA_0")
#pragma rodata-name(push, "CODEA_0")
void function_A_0()
{
  char stringA[24] = "";
  ppu_off();
  sprintf(stringA, "Code A 0");
  vram_adr(NTADR_A(2,25));
  vram_write(stringA, strlen(stringA));
  ppu_on_all();
}
#pragma code-name(pop)
#pragma rodata-name(pop)


#pragma code-name(push, "CODEA_30")
#pragma rodata-name(push, "CODEA_30")

void function_A_30()
{
  char stringA[24] = "";
  ppu_off();
  sprintf(stringA, "Code A 30");
  vram_adr(NTADR_A(2,25));
  vram_write(stringA, strlen(stringA));
  ppu_on_all();
}
#pragma code-name(pop)
#pragma rodata-name(pop)

#pragma code-name(push, "CODEB_0")
#pragma rodata-name(push, "CODEB_0")
void function_B_0()
{
  char stringA[24] = "";
  ppu_off();
  sprintf(stringA, "Code B 0");
  vram_adr(NTADR_A(2,24));
  vram_write(stringA, strlen(stringA));
  ppu_on_all();
}
#pragma code-name(pop)
#pragma rodata-name(pop)

#pragma code-name(push, "CODEB_30")
#pragma rodata-name(push, "CODEB_30")
void function_B_30()
{
  char stringA[24] = "";
  ppu_off();
  sprintf(stringA, "Code B 30");
  vram_adr(NTADR_A(2,24));
  vram_write(stringA, strlen(stringA));
  ppu_on_all();
}
#pragma code-name(pop)
#pragma rodata-name(pop)


int *heaporg = (int*)&_heaporg;
int *heapptr = (int*)&_heapptr;
int *heapend = (int*)&_heapend;

void setHeap()
{
  heaporg[0] = 0x7000; //heaporg
  heapptr[0] = heaporg[0]; //heapptr
  heapend[0] = 0x8000; //heapend
  memset((int*)heaporg[0], 0, heapend[0] - heaporg[0]); 
}

int heap_avail(void)
{
  char *t;
  char stringA[24] = "";
  int x = 1;

  while(1)
  {
    if (!(t=malloc(x))) break;
    free(t);
    ++x;
  }

  ppu_off();
  sprintf(stringA, "heap starts: $%4x", heaporg[0]);
  vram_adr(NTADR_A(2,3));
  vram_write(stringA, strlen(stringA));
  sprintf(stringA, "heap ends:   $%4x", heapend[0]);
  vram_adr(NTADR_A(2,4));
  vram_write(stringA, strlen(stringA));
  sprintf(stringA, "heap avail:   %4u bytes",x - 1);
  vram_adr(NTADR_A(2,2));
  vram_write(stringA, strlen(stringA));
  sprintf(stringA, "Last %d bytes stored @ $%4x",length, heapptr[0]);
  vram_adr(NTADR_A(2,7));
  vram_write(stringA, strlen(stringA));
  ppu_on_all();
  return x - 1;
}

char pad;
// main function, run after console reset
void main(void) {
  MMC3_WRAM_ENABLE();
  setHeap();
  ppu_off();

  // set palette colors
  pal_col(0,0x02);	// set screen to dark blue
  pal_col(1,0x14);	// fuchsia
  pal_col(2,0x20);	// grey
  pal_col(3,0x30);	// white
  heap_avail();
  
  MMC3_PRG_8000(0);
  MMC3_PRG_A000(31);
  function_A_0();
  function_B_30();
  MMC3_PRG_8000(30);
  MMC3_PRG_A000(61);
  //function_A_30();
  function_B_30();

  // enable PPU rendering (turn on screen)
  //ppu_on_all();

  // infinite loop
  while (1)
  {

    byte i = 0;
    char *dataA = malloc(length);
    for (i = 0; i < length; ++i)
      dataA[i] = i + heapptr[0];


    ppu_off();
    vram_adr(NTADR_A(2,8));
    vram_write(dataA, length - 1);
    ppu_on_all();
    heap_avail();


    if (heapend[0] - (heapptr[0] + length) < 0)
      break;

    pad = pad_poll(0);
    if (pad & PAD_START)
      break;

  }
  main();
}
