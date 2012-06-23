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

#ifndef PARTIALSOLVER
#define PARTIALSOLVER

#include "Puzzle.hh"
#include <list>
#include <map>
#include <sstream>
#include <iostream>

using namespace std;

class PartialSolver
{
    private:
        /**
        *   List containers that store the puzzles
        *   @puzzles are not finished, @solved are correct,
        *   and @incorrect are puzzles finished incorrectly.
        */
        list<Puzzle> puzzles;
        list<Puzzle> solved;
        list<Puzzle> incorrect;
        /**
        *   @number is the variable => n in nxn board.
        */
        int number;
        /**
        *   @variable is the current number to be placed in
        *   the top @puzzle with the @points being the different
        *   positions in the board.
        */
        int variable;
        list< pair<int,int> > points;
        /**
        *   @boundMap stores all the @solved puzzles boundary conditions
        *   as a key(string) so that we can see if it is unique or not shown
        *   by the int.
        */
        map<string,int> boundMap;
        /**
        *   Finds the @points and @variable for the const board.
        */
        void findPoints(const Puzzle::Board& board);
        /**
        *   Finds the @points for the @variable in either a row
        *   or a column.
        */
        void checkRow(int row, const Puzzle::Board& board);
        void checkColumn(int col, const Puzzle::Board& board);
        /**
        *   Function used to check if the board is partially correct as a
        *   way of pruning the search. It check to see if the row or column
        *   is @pariallyFinished and then sees if the visiability is correct.
        */
        bool partiallyCorrect(Puzzle& puzzle);
        /**
        *   Check if the path has the numbers found up to the highest number.
        *   Ex:  2,1,4,? is partially finished because from the right persepctive
        *   we don't care about seeing over the 4.
        */
        int partiallyFinished(const vector<SkyScraper*>& path);

    public:
        /**
        *   Default constructor copys partial puzzle.
        */
        PartialSolver(const Puzzle& puzzle);
        /**
        *   Solved the puzzle by breath-first searching the possible partial puzzles.
        */
        void solve();
        /**
        *   Prints the incorrect puzzles and then the correct puzzles.
        */
        void print();
        /**
        *   Finds the boundary conditions for all of the solved puzzles. This is mostly
        *   used getting data on all possible puzzles for some n.
        */
        void boundaryFinder();
};

#endif
