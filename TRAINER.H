//  trainer.h
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

#ifndef __TRAINER_H
#define __TRAINER_H

	#ifndef __TURBOC__
		#error "Turbo C compiler is required to compile Dave trainer"
	#endif

	#if __TURBOC__ < 0x0300
		#error "Turbo C compiler version must be >= 0x0300"
	#endif

	#ifdef __cplusplus
		#define __CPPARGS ...
	#else
		#define __CPPARGS
	#endif

	#define CF 1 /* Carry flag */
	#define KEYBOARD_INTR 9 /* The clock tick interrupt */
	#define TRAINER_INTR 0x88

	#define SCREEN_LAST_LINE (199 * 320)

	/* Keyboard scancode */
	#define SCANCODE_1 2
	#define SCANCODE_2 3
	#define SCANCODE_3 4
	#define SCANCODE_4 5
	#define SCANCODE_5 6
	#define SCANCODE_6 7
	#define SCANCODE_7 8
	#define SCANCODE_8 9
	#define SCANCODE_9 0xa
	#define SCANCODE_0 0xb
	#define SCANCODE_TELDA 0x29
	#define SCANCODE_Q 0x10
	#define SCANCODE_W 0x11
	#define SCANCODE_E 0x12
	#define SCANCODE_R 0x13
	#define SCANCODE_T 0x14

	#define TUX_WIDTH (21*2)
	#define TUX_HEIGHT (12)

	#define DAVE_EXE_PATH "dave.exe"

	void interrupt keyboardHandler(__CPPARGS);
	void interrupt (*keyboardOldHandler)(__CPPARGS);

	void installTrainer();
	void uninstallTrainer();
	void runDave();
	void splashScreen();
	void byeScreen();
	void panic(char *msg);
	void initTextScreen();
	void pressAnyKey();
	void music(int pitch, int time);
#endif