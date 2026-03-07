# Project Overview

**Pocketcreature** is an interactive fiction and battle game.\
The game is developed for the **DTEK-V board** as part of the **KTH
course IS1200 Computer Hardware Engineering (Spring 2025)**.

The player explores nine unique rooms, collects items, catches
creatures, and battles gym leaders to ultimately defeat the final boss.

Gameplay logic, story, and hardware I/O are implemented entirely in
**C** and run directly on the **DTEK-V board with the I/O shield
attached**.

------------------------------------------------------------------------

# Running the Game on the DTEK-V Board

## 1. Connect the Board

Connect the **DTEK-V board** to your computer and download the project
folder.

## 2. Open Terminal

Open the terminal in your **DTEK-V development environment**.

## 3. Navigate to the Project Folder

``` bash
cd [path to folder]
```

## 4. Compile the Program

``` bash
make
```

This compiles all `.c` and `.S` files and links them into the executable
binary for the DTEK-V board.

If compilation succeeds, a new file will appear:

    main.bin

## 5. Prepare the Board

Make sure the **DTEK-V board** is connected via USB.

Press **BTN0** on the board to reset before loading a new program.

## 6. Run the Game

``` bash
dtekv-run main.bin
```

------------------------------------------------------------------------

# Optional Commands

## Clean Build Files

Remove object and temporary files from the folder:

``` bash
make clean
```

## JTAG UART Terminal

Find and run the **JTAG UART Terminal**.

The terminal displays all game text such as:

-   Story
-   Menus
-   Dialogue

It usually opens automatically, but if it does not you can start it
manually.

If the terminal freezes or fails to connect, run:

``` bash
pkill jtag
jtagconfig
```

------------------------------------------------------------------------

# Controls

  Control             Function
  ------------------- ------------------------------------------------------
  SW1--SW4            Navigate menus and rooms. Toggle up and down to use.
  LEDs                Indicate current room
  7‑Segment Display   Shows player and enemy HP

⚠️ **Important:** Switch 0 is not initialized.

------------------------------------------------------------------------

# Gameplay

Follow the tutorial at game start (**Professor Maple**) to learn how to:

-   Catch creatures
-   Battle enemies

### Goal

Defeat all **gym leaders** and the **final boss in room 9**.
