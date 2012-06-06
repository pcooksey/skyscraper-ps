/**
*  Copyright (c) 2012, CSUMB
*  All rights reserved.
*
*  Redistribution and use in source and binary forms, with or
*  without modification, are permitted provided that the following conditions are met:
*
*  Redistributions of source code must retain the above copyright notice,
*  this list of conditions and the following disclaimer. Redistributions
*  in binary form must reproduce the above copyright notice, this list of
*  conditions and the following disclaimer in the documentation and/or other
*  materials provided with the distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
*  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
*  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
*  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef PUZZLE
#define PUZZLE

#include "SkyScraper.hh"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
using namespace std;

class Puzzle
{
    friend class PartialSolver;
    private:
        typedef vector< vector<SkyScraper> > Board;
        typedef vector< pair<int,int> > Group_pairs;
        /// Puzzle board with the col & row numbers and the board number
        Board puzzle;
        Group_pairs col;
        Group_pairs row;
        int number;

        ///These two function start the program
        void init(int num);
        ///Checks if the outside values are valid
        bool outside();
        ///This iteraters through the columns and cross checks every box
        bool checkOutside();

        ///Cross checks the values of the top, bottom, left, and right
        void crossCheck(int column, int find);
        ///Finds the max visabiliy of a row or column
        int visableMax(int findNum, int place, int maxValue, vector<SkyScraper*> objects);
        ///Finds the min visibility of a row or column
        int visableMin(int findNum, int place, int maxValue, vector<SkyScraper*> objects);
        ///Finds the max and min visibility for checking if @findNum can be placed
        int visibility(int findNum, int place, int maxValue, vector<SkyScraper*> objects);
        ///Test to check visableMin for being the smallest
        int checkMin(vector<SkyScraper*>& objects, vector<int>& box, int findNum);
        ///Returns the visibility score a row of numbers
        ///Ex: 3 2 4 1 = 2 visabiliy from the right
        int visableScore(vector<int>& box);
        int visableScore(vector<SkyScraper*> objects);
        int visableScore(vector<SkyScraper> objects);
        ///Returns a row of pointers that can be fliped
        vector<SkyScraper*> getRow(int row, bool flip=false);
        ///Returns a column of pointers that can be fliped
        vector<SkyScraper*> getColumn(int column, bool flip=false);
        ///Checks if @num is the only one in the box of numbers
        int checkOnly(int num, vector<SkyScraper*>& box);


        ///Sets a number @num for box at @row & @column
        bool set(int row, int column, int num);
        ///Removes a possible number @num from box at @row & @column
        bool remove(int row, int column, int num);
        ///Removes @num from all boxes in row @row
        void minusRow(int row, int num);
        ///Removes @num from all boxes in column @col
        void minusCol(int col, int num);
        ///Remove for the visibility function
        void remove(int num, vector<SkyScraper>& objects);
        ///Flips a value for positioning in a row or column
        int flipValue(int value);

    public:
        Puzzle(){};
        Puzzle(const Puzzle& other);

        ///Loads the file with the puzzle
        bool loadFile();
        ///Function to solve the puzzle
        bool solve();
        ///Checks to see if every box has found a number
        bool complete();
        ///Using @correct assumes the puzzle is complete
        bool correct();
        ///Prints puzzle. Change @loopPrint to true if you
        ///want to see it print everytime it minus a value
        void print(bool loopPrint = false);

};

#endif
