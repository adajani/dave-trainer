# Dave Trainer

## Updates
Version 1:
  - Added basic cheats: score, crown, level, jet, life and gun.
  - Trigger the cheats using keyboard hook.
  - Borland C >= 3 support only.

## Requirements

Dave trainer has been developed in DOS (DOSBox 0.74). The Borland C compiler version 3
has been used to compile the source code.

Borland C compiler should have the following settings before compiling:
- Standard EXE output.
- Model tiny.
- Remove debugging info.
- Optimizations:
	> Supress redundant loads.
	> Jump optimization.
	> Register variables : automatic.
	> Optimize for size.
- Linker:
	> No stack warning.

Finally, You have to get your own copy of Dave, and the trainer should be located
in the same game folder.

## Trainer activation
To activate the trainer you must press ~ (telda). Once the cheat being activated, you
will notice a red pixel in the bottom left of the screen as well as a beep sound.

## Trainer cheat codes
Press any of the following keys while playing:
* 1-9 Select next level when enter the door.
* Q   Get crown.
* W   Set score to 99999.
* E   Infinite lives (255).
* R   Get gun.
* T   Get jet.

