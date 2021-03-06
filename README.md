### Skyscraper puzzle solver ###
A program that can solve any general size Skyscraper puzzle. The code is written in C++ and is open source. The program can generate all possible puzzles up to storage space on the computer. A graphical user interface allows the user to create their own puzzle or use pre-created puzzles. The program also generates all of the possible combinations within each row.

<img src='http://skyscraper-ps.googlecode.com/files/Skyscraper-ps-gui.jpg' alt='Logo' />

### Install for Linux ###

Please [download](https://www.qt.io/download-open-source/) qt creator

And install it on your system

Download source code into some file location, lets say /home/skyscraper-ps

cd /home/skyscraper-ps

Run <b> qmake skyscraper-ps.pro </b>

Run <b> make </b>

Run <b> ./skyscraper-ps </b>

### Skyscraper-ps ###

Load a puzzle or create a puzzle to start. Then click <b>solve</b> to reduce the problem
using a constraint based solver. This does not always solve the solution completely.
This may be because the puzzle has multiple solutions, not solvable, or the constraints
where not enough to solve the problem. The parital solver will do a graph based
search to find the possible solution(s). 

If there are multiple solutions you will find two files in the directory.

<b>BoundaryKeys.csv</b>: This has all the possible boundary solutions for the puzzle.
There can be multiple latin square solutions with the same boundary numbers, which is 
indicated by the number next to the solutions in the file.

<b>BoundaryStats.csv</b>: This shows some stats on the boundary solutions.
Total number of Laing Squares.
Total number of Boundaries.
Total Unique Boundaries.
Upper Limit of Branching (max number of latin squares with the same boundaries).

### Versions ###

<b> Skyscraper-ps Gui 1.6 </b> New features added are displaying all possible rows and columns when double clicking side constraints and the mouse scroll wheel is now used to change numbers in the puzzle.

<b> Skyscraper-ps Gui 1.5 </b> Graphical user interface

<b> Skyscraper-ps Gui 1.4 </b> Removed finding min and max when the brute force solver could just find the boundary condition number itself. This increased the speed slightly as it gave the puzzle solver more information about the puzzle. Fixed partial solver finding incorrect solution by fixing the puzzle correct function. Now checks to see if each number is the only number in each row and column. Increased the speed for finding partial solutions by skipping any boundary condition with a value of 0 as it gives the solver no information. Correct boundary finder function to display information correctly.
 
