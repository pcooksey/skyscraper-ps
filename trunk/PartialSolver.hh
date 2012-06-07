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
        list<Puzzle> puzzles;
        list<Puzzle> solved;
        list<Puzzle> incorrect;
        int number;
        int variable;
        list< pair<int,int> > points;
        map<string,int> boundMap;

        void findPoints(const Puzzle::Board& board);

        void checkRow(int row, const Puzzle::Board& board);
        void checkColumn(int col, const Puzzle::Board& board);
        /// Functions used to check if the board is partially correct as a
        /// way of pruning the search.
        bool partiallyCorrect(Puzzle& puzzle);
        bool partiallyFinished(const vector<SkyScraper*>& path);

    public:
        PartialSolver(const Puzzle& puzzle);

        void solve();
        void print();

        /// Used for puzzles with zeros in the boundaries
        void boundaryFinder();
};

#endif
