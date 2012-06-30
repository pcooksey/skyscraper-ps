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
/**
*  Author: Philip Cooksey
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
    public:
        typedef vector< vector<SkyScraper> > Board;
        typedef vector< pair<int,int> > Group_pairs;
    private:
        /// Puzzle board with the col & row numbers and the board number
        Board puzzle;
        Group_pairs col;
        Group_pairs row;
        int number;
        ///bool to tell if the puzzle has been uploaded yet.
        bool loaded;

        ///These two function start the program
        void init(int num);
        ///This iteraters through the columns and cross checks every box
        bool checkInside();

        ///Cross checks the values of the top, bottom, left, and right
        void crossCheck(int column, int find);
        ///Checks if the row and column where they meet still hold true
        ///to the constraints on the top, bottom, left, and right.
        bool crossCheckEntry(int row, int column);
        ///Finds if by temporarily placing @findNum can the visibility be equal to @maxValue
        int visibility(int findNum, int place, int maxValue, vector<SkyScraper*> objects);
        ///Finds if it is possbile for @object to have a visibility of @maxValue
        int visibility(int maxValue, vector<SkyScraper*> objects);
        ///Returns the visibility score a row of numbers
        ///Ex: 3 2 4 1 = 2 visabiliy from the right
        int visableScore(vector<SkyScraper*> objects);
        template<typename type>
        int visableScore(const vector<type>& objects);
        ///Returns a row of pointers that can be fliped
        vector<SkyScraper*> getRow(int row, bool flip=false);
        ///Returns a column of pointers that can be fliped
        vector<SkyScraper*> getColumn(int column, bool flip=false);
        ///Checks if @num is the only one in the box of numbers
        ///@returns -1 if not the only one otherwise @returns index
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
        Puzzle():loaded(false){}
        Puzzle(int num);
        Puzzle(const Puzzle& other);

        /** Gets fileName from command prompt and then calls
        *  and then calls loadFile(fileName); */
        bool loadFile();
        ///Loads the file from the fileName give
        bool loadFile(string fileName);
        ///Checks if the outside values are valid
        bool checkOutside();
        ///Function to solve the puzzle
        bool solve();
        ///Checks to see if every box has found a number
        bool complete();
        ///Using @correct assumes the puzzle is complete
        bool correct();
        ///Prints puzzle. Change @loopPrint to true if you
        ///want to see it print everytime it minus a value
        void print(bool loopPrint = false);

        /** Public function to access variables */
        const int& size() { return number;};
        const Group_pairs& columnPairs() { return col;};
        const Group_pairs& rowPairs() { return row;};
        int entry(int row, int column);

};

#endif
