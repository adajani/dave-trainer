//  trainer.cpp
//
//  Author:
//       Ahmad Dajani <eng.adajani@gmail.com>
//
//  Copyright (c) 2020 Ahmad Dajani
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "trainer.h"
#include <dos.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>

/* reduce heaplength and stacklength to make a smaller program in memory */
extern unsigned _heaplen = 1024;
extern unsigned _stklen  = 512;

static unsigned char tuxAscii[TUX_HEIGHT][TUX_WIDTH] = {
	{' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,'#', 8,'#', 8,'O',15,'#', 8,'O',15,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,'#', 8,'#',14,'#',14,'#',14,'#',14,'#',14,'#', 8,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,' ', 0,' ', 0,'#', 8,'#', 8,'#',15,'#',15,'#',14,'#',14,'#',14,'#',15,'#',15,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,' ', 0,'#', 8,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,'#', 8,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,' ', 0,'#', 8,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#', 8,'#', 8,'#', 8,' ', 0,' ', 0,' ', 0},
	{' ', 0,' ', 0,'#',14,'#',14,'#', 8,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#', 8,'#', 8,'#',14,' ', 0,' ', 0},
	{'#',14,'#',14,'#',14,'#',14,'#',14,'#',14,'#', 8,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#',15,'#', 8,'#',14,'#',14,'#',14,'#',14,'#',14,'#',14},
	{'#',14,'#',14,'#',14,'#',14,'#',14,'#',14,'#',14,'#', 8,'#',15,'#',15,'#',15,'#',15,'#', 8,'#',14,'#',14,'#',14,'#',14,'#',14,'#',14,'#',14,'#',14},
	{' ', 0,' ', 0,'#',14,'#',14,'#',14,'#',14,'#',14,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,'#', 8,'#',14,'#',14,'#',14,'#',14,'#',14,' ', 0,' ', 0}
};

static unsigned char far *screenPointer;
static unsigned char far *crown;
static unsigned long far *score;
static unsigned char far *level;
static unsigned char far *life;
static unsigned char far *gun;
static unsigned char far *jet;

void main() {
	//@TODO check md5 to ensure the trainer will works with offset
	//@SEE https://www.netlib.org/crc/md5sum.c
	splashScreen();
	atexit(uninstallTrainer);
	installTrainer();
	runDave();
	byeScreen();
}

void music(int pitch, int time) {
	sound(pitch);
	delay(time);
	nosound();
}

void pressAnyKey() {
	_AX = 0;
	asm int 0x16
}

void initTextScreen() {
	_AX = 3;
	asm int 0x10
}

void panic(char *msg) {
	initTextScreen();
	fprintf(stderr, "Error: %s :'(\n", msg);
	exit(-1);
}

void byeScreen() {
	textcolor(LIGHTGREEN);
	cprintf("I would rather spend one lifetime with you, than face all the ages of the world alone. Love you, my dear Ayah <3\n");
}

void splashScreen() {
	unsigned char far *VgaTextBuffer = (unsigned char far *)MK_FP(0xb800, 0);
	register unsigned int xpos = (160-TUX_WIDTH);
	register unsigned int ypos = 0;
	register unsigned int offset;

	initTextScreen();

	for(register unsigned y=0; y<TUX_HEIGHT; y++) {
		for(register unsigned x=0; x<TUX_WIDTH; x+=2) {
			offset = (ypos+y) * (80*2) + (x + xpos);
			VgaTextBuffer[offset] = tuxAscii[y][x];
			VgaTextBuffer[offset+1] = tuxAscii[y][x+1];
		}
	}

	textcolor(WHITE);
	cprintf("Welcome to Dave trainer v1 (c)2020 \n\r\n"
		"Written by Ahmad Dajani (eng.adajani@gmail.com)\r\n"
	);

	textcolor(LIGHTRED);
	cprintf("\nDisclaimer:\r\n"
		"This SOFTWARE PRODUCT is provided by THE PROVIDER\r\n"
		"'as is' and 'with all faults.' THE PROVIDER makes\r\n"
		"no representations or warranties of any kind\r\n"
		"concerning the safety, suitability, lack of viruses,\r\n"
		"inaccuracies, typographical errors, or other\r\n"
		"harmful components of this SOFTWARE PRODUCT.\r\n"
	);

	textcolor(LIGHTBLUE);
	cprintf("\n\nTo enable the trainer press ~ then one of the following:\r\n"
		"\t 1-9 Select next level when enter the door\r\n"
		"\t Q   Get crown\r\n"
		"\t W   Set score to 99999\r\n"
		"\t E   Infinite lives (255)\r\n"
		"\t R   Get gun\r\n"
		"\t T   Get jet\r\n"
	);

	textcolor(LIGHTGREEN + BLINK);
	cprintf("\n\nPress any key to play...\r\n");

	pressAnyKey();
}

void runDave() {
	char filename[]={DAVE_EXE_PATH};
	char parameter[]={""};

	struct REGPACK reg;

	reg.r_ax = 0x4b00; //0x4b01 (load and create psp)
	reg.r_dx = FP_OFF(filename);
	reg.r_ds = FP_SEG(filename);
	reg.r_bx = FP_OFF(parameter);
	reg.r_es = FP_SEG(parameter);
	intr(0x21, &reg);

	if (reg.r_flags & CF) {
		/* DOS error found in 
		 e.g: 8 no memory
		*/
		switch(reg.r_ax) {
			case 2: panic("Dave not found");
			case 8: panic("Insufficient memory");
			default: panic("DOS");
		}
	}
}

void installTrainer() {
	/* is the trainer installed ? */
	if(getvect(TRAINER_INTR)) {
		panic("Other programs are using trainer resources");
	}

	/* installing the trainer */
	screenPointer = (unsigned char far *)MK_FP(0xA000, 0);
	
	disable();
	keyboardOldHandler = getvect(KEYBOARD_INTR); /*get the timer ISR address*/
	setvect(KEYBOARD_INTR, keyboardHandler); /*set our function address*/
	setvect(TRAINER_INTR, keyboardOldHandler); /* save the old handler in out interrupt vector */
	enable();
}

void uninstallTrainer() {
	/* is the trainer installed ? */
	if(getvect(TRAINER_INTR) == NULL) {
		panic("Other programs are using trainer resources");
	}

	disable();
	keyboardOldHandler = getvect(TRAINER_INTR);
	setvect(KEYBOARD_INTR, keyboardOldHandler);
	setvect(TRAINER_INTR, NULL);
	enable();
}

void interrupt keyboardHandler(__CPPARGS) {
	static unsigned int init = 0;
	static unsigned int DavedataSegment;
	unsigned char key = inportb(0x60);
	/* enable the trainer */
	if(key == SCANCODE_TELDA && !init) {
		asm {
			push ax
			mov ax, word [bp + 6]
			mov DavedataSegment, ax
			pop ax
		}
		init = 1;
		music(275, 300);
		screenPointer[SCREEN_LAST_LINE] = 12;
		level = (unsigned char far *)MK_FP(DavedataSegment, 0x56F4);
		crown = (unsigned char far *)MK_FP(DavedataSegment, 0x4F88);
		score = (unsigned long far *)MK_FP(DavedataSegment, 0x4C60);
		life = (unsigned char far *)MK_FP(DavedataSegment, 0x56EE);
		gun = (unsigned char far *)MK_FP(DavedataSegment, 0x4F84);
		jet = (unsigned char far *)MK_FP(DavedataSegment, 0x4F8A);
	}

	if(init) {
		screenPointer[SCREEN_LAST_LINE] = 12;
		if(key >= SCANCODE_1 && key <= SCANCODE_9) {
			*level = key - 1;
			screenPointer[SCREEN_LAST_LINE + 2] = key;
		}
		else if(key == SCANCODE_Q) {
			*crown = 1;
			screenPointer[SCREEN_LAST_LINE + 4] = 10;
		}
		else if(key == SCANCODE_W) {
			*score = 99999;
			screenPointer[SCREEN_LAST_LINE + 6] = 10;
		}
		else if(key == SCANCODE_E) {
			*life = 0xff;
			screenPointer[SCREEN_LAST_LINE + 8] = 10;
		}
		else if(key == SCANCODE_R) {
			*gun = 1;
			screenPointer[SCREEN_LAST_LINE + 10] = 10;
		}
		else if(key == SCANCODE_T) {
			*jet = 1;
			screenPointer[SCREEN_LAST_LINE + 12] = 10;
		}
	}

	/* call the old interrupt handler */
	keyboardOldHandler();
}