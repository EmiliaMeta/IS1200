Project Overview

Pocketcreature is an interactive fiction and battle game. 
The game is developed for the DTEK-V board as part of the KTH course IS1200 Computer Hardware Engineering (Spring 2025).
The player explores nine unique rooms, collects items, catches creatures, and battles gym leaders to ultimately defeat the final boss.
Gameplay logic, story, and hardware I/O are implemented entirely in C and run directly on the DTEK-V board with the I/O shield attached.

Running the Game on the DTEK-V Board:

1. Connect the DTEK-V board to your computer. Download the project folder.

2. Open the terminal on your DTEK-V environment.

3. In the terminal, navigate to the project folder by writing:
       
   cd [path to folder]

4.Compile the program in terminal by writing

        make

- This compiles all .c and .S files and links them into the executable binary for the DTEK-V board.
  If the compilation succeeds, you will see a new file: main.bin

5. Prepare the Board. Make sure the DTEK-V board is connected via USB.

 Press BTN0 on the board to reset before loading a new program.

6. Run the game by writing in terminal:
        dtekv-run main.bin

OPTIONAL: 
7. Clean Build Files to remove object and temporary files from the folder by writing in terminal
        make clean

8. Find and run the JTAG UART Terminal.
 The JTAG UART terminal displays all game text (story, menus, dialogue).
 It usually opens automatically, but if it does not, you can start it manually.
 
If the terminal freezes or fails to connect, write in terminal:
        pkill jtag
        jtagconfig


Use the following controls during gameplay:

Control	Function

SW1–SW4: Navigate menus and rooms. Toggle up and down to use. Important: Switch 0 is not initialized.

LEDs: Indicate current room

7-Segment Display:	Show player and enemy HP

Follow the tutorial at game start (Professor Maple) to learn how to catch creatures and battle enemies.

The goal is to defeat all gym leaders and the final boss in room 9.