# micro_rts

## Compilation instructions

This project depends on library `allegro` (version `5.2.*`). To install it, follow instructions in the
[official guide](https://github.com/liballeg/allegro_wiki/wiki/Quickstart).

### Build dependencies

To build this project you will need:
- `cmake`
- `pkg-config`

In root directory of the project run:

```
cmake .
make
```

Binary `micro_rts` should now be ready to run!

## Gameplay

The big structure on the left is your base. The big structure on the right is the enemy base. Destroy the enemy base to 
win, and don't let them destroy yours!

### Bases and income

Smaller buildings similar to your base are the auxiliary bases. They provide income to their owner
(5 credits/s by default). You can see your current balance and income in the top left corner.

### Units
Press `Q` to buy a unit. Each unit will cost you 100 credits. You can give them commands using controls 
explained on the title screen. 

### Overdrive
By pressing `E` you can overdrive a group of units, giving them massive buffs to movement and attack speed. 
But be careful! Overdriving units costs 5 credits/s per unit. If you run out of credits, all your units will lose their
overdrives and take heavy damage.  
This ability will be crucial on higher difficulties - if your enemy has twice your income, only quick hands and good 
overdrives can save you from defeat!
