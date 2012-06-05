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

#include "PartialSolver.hh"

PartialSolver::PartialSolver(const Puzzle& puzzle)
{
    puzzles.push_back(Puzzle(puzzle));
    number = puzzle.number;
}

void PartialSolver::checkRow(int row, const Puzzle::Board& board)
{
    for(int col =0; col<number; col++)
    {
        if(board[row][col].isPossible(variable))
        {
            points.push_back(make_pair(row,col));
        }
    }
}

void PartialSolver::checkColumn(int col, const Puzzle::Board& board)
{
    for(int row =0; row<number; row++)
    {
        if(board[row][col].isPossible(variable))
        {
            points.push_back(make_pair(row,col));
        }
    }
}

void PartialSolver::findPoints(const Puzzle::Board& board)
{
    int counter(0);
    ///Start by finding lowest amount of a number
    for(int stop=2; stop<=number; stop++)
    {
        ///iterate through the numbers in decending order
        for(int num=number; num>0; num--)
        {
            counter = 0;
            ///Check each column
            for(int col=0; col<number; col++)
            {
                for(int row=0; row<number; row++)
                {
                    if(board[row][col].isPossible(num))
                    {
                        counter++;
                    }
                }
                if(counter==stop)
                {
                    variable = num;
                    checkColumn(col, board);
                    return;
                }
            }
            counter = 0;
            ///Check each row
            for(int row=0; row<number; row++)
            {
                for(int col=0; col<number; col++)
                {
                    if(board[row][col].isPossible(num))
                    {
                        counter++;
                    }
                }
                if(counter==stop)
                {
                    variable = num;
                    checkRow(row, board);
                    return;
                }
            }
        }
    }
}

void PartialSolver::solve()
{
    Puzzle& front = puzzles.front();
    ///Clear points to find points for next puzzle
    points.clear();
    findPoints(front.puzzle);
    list< pair<int,int> >::iterator it = points.begin();
    for(;it!=points.end(); it++)
    {
        int row = (*it).first;
        int col = (*it).second;
        Puzzle back = Puzzle(front);
        back.set(row, col, variable);
        back.solve();
        if(back.complete())
        {
            if(back.correct())
            {
                solved.push_back(back);
            }
            else
            {
                incorrect.push_back(back);
            }
        }
        else
        {
            puzzles.push_back(back);
        }
    }
    puzzles.pop_front();
    if(!puzzles.empty())
    {
        solve();
    }
}

void PartialSolver::print()
{
    list<Puzzle>::iterator puzzle = solved.begin();
    for(;puzzle!=solved.end(); puzzle++)
    {
        cout<<"Correct"<<endl;
        puzzle->print(true);
    }
    puzzle = incorrect.begin();
    for(;puzzle!=incorrect.end(); puzzle++)
    {
        cout<<"Incorrect"<<endl;
        puzzle->print(true);
    }
    cout<<"Size Correct: "<<solved.size()<<endl;
    cout<<"Size Incorrect: "<<incorrect.size()<<endl;
}

void PartialSolver::boundaryFinder()
{
    map<string,int> boundMap;
    for(list<Puzzle>::iterator it = solved.begin(); it!=solved.end(); it++)
    {
        stringstream boundkey, temp;
        for(int x=0; x<number; x++)
        {
            boundkey << (*it).visableScore((*it).getRow(x));
            temp << (*it).visableScore((*it).getRow(x, true));
        }
        boundkey << temp.str();
        temp.str("");
        for(int y=0; y<number; y++)
        {
            boundkey << (*it).visableScore((*it).getColumn(y));
            temp << (*it).visableScore((*it).getColumn(y, true));
        }
        boundkey << temp.str();
        string key = boundkey.str();
        if(boundMap.find(key )!=boundMap.end())
        {
            boundMap[key]++;
        }
        else
        {
            boundMap[key] = 1;
        }
    }

    int totalKeys(0), totalNum(0), totalUnique(0), upperLimit(0);
    ofstream file("BoundaryKeys.txt");
    map<string,int>::iterator it = boundMap.begin();
    for(; it!=boundMap.end(); it++)
    {
        int num = (*it).second;
        file<<(*it).first<<": "<<num<<endl;
        totalNum += num;
        totalKeys++;
        if(num==1)
            totalUnique++;
        if(num>upperLimit)
            upperLimit = num;

    }
    file<<"Total Number: "<<totalNum<<endl
        <<"Total Keys: "<<totalKeys<<endl
        <<"Total Unique: "<<totalUnique<<endl
        <<"Upper Limit: "<<upperLimit<<endl;
    file.close();
}
