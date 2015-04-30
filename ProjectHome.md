### Skyscraper puzzle solver ###
A program that can solve any general size Skyscraper puzzle. The code is written in C++ and is open source. The program can generate all possible puzzles up to storage space on the computer. A graphical user interface allows the user to create their own puzzle or use pre-created puzzles. The program also generates all of the possible combinations within each row.

<img src='http://skyscraper-ps.googlecode.com/files/Skyscraper-ps-gui.jpg' alt='Logo' />

### Downloads: ###

<b>Featured: Skyscraper-ps-gui-1.6.msi</b>: Windows installer for GUI version 1.6. New features added are displaying all possible rows and columns when double clicking side constraints and the mouse scroll wheel is now used to change numbers in the puzzle.

<b>Deprecated: Skyscraper-ps-gui-1.5.msi</b>: Windows installer with graphical user interface

<b>Deprecated: skyscraper-ps-1.4.msi</b>: Windows installer for command line tool

<b>Deprecated: skyscraper-ps-1.4.zip</b>: Removed finding min and max when the brute force solver could just find the boundary condition number itself. This increased the speed slightly as it gave the puzzle solver more information about the puzzle. Fixed partial solver finding incorrect solution by fixing the puzzle correct function. Now checks to see if each number is the only number in each row and column. Increased the speed for finding partial solutions by skipping any boundary condition with a value of 0 as it gives the solver no information. Correct boundary finder function to display information correctly.