# Wolverine

This is a C++ program for "狼人杀" in Chinese. I don't really know how to translate this name correctly.

Warning: This program is now only functional for 6-9, or 12 players, but you can make it work for 10 and 11 by manually editing Distrib.lrs, as explained below.

The logger is now complete and has been embedded into master. Work on master resumed.

## File information

Clean.bash and Execute.h have been deprecated.

Distrib.lrs: Records the number of different identities for different numbers of players. Will be updated after I complete the remaining code on other files.

Player.h: Interface and implementation for class Player.

LRS.cpp: Main program.

Logger.h: Implementations of logging functions.

## About Distrib.lrs

This version of Distrib.lrs will be the standard, and PRs concerning additions will be welcome.

Basic format:

x(Total number of players) x(Number of wolverines) x(Number of villagers) [W][P][H][G](Presence of powers)

Inside "Presence of powers", W, P, H, G each stands for Witch, Predictor, Hunter, and Guard. Add one space between each.

The number of players should be put in a increasing order one by one, with no blank lines in between.


## Error code solutions

1: Enter a correct number of players (currently between 6 and 12)

2: Put Distrib.lrs it into the place where you are starting your Shell (user home folder mostly)

3: Wrong format of Distrib.lrs. Please follow the format above.

4: There are letters other than W, P, H, G in Distrib.lrs. Please check your file.

## Current to-dos (Not exactly ordered by importance)

1. Making use of macOS "say" utility to enhance experience.

2. A better input capability. Sometimes I just can't fully understand how C++ handles CLI input.
