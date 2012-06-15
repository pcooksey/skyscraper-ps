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
    while(!puzzles.empty())
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
                ///If the puzzle is still partially correct continue search
                if(partiallyCorrect(back))
                    puzzles.push_back(back);
            }
        }
        puzzles.pop_front();
    }
}

void PartialSolver::print()
{
    list<Puzzle>::iterator puzzle = incorrect.begin();
    for(;puzzle!=incorrect.end(); puzzle++)
    {
        cout<<"Incorrect"<<endl;
        puzzle->print(true);
    }
    puzzle = solved.begin();
    for(;puzzle!=solved.end(); puzzle++)
    {
        cout<<"Correct"<<endl;
        puzzle->print(true);
    }
    cout<<"Size Correct: "<<solved.size()<<endl;
    cout<<"Size Incorrect: "<<incorrect.size()<<endl;
}

void PartialSolver::boundaryFinder()
{
    for(list<Puzzle>::iterator it = solved.begin(); it!=solved.end(); it++)
    {
        stringstream boundkey, temp;
        for(int x=0; x<number; x++)
        {
            boundkey << (*it).visableScore((*it).getColumn(x));
            temp << (*it).visableScore((*it).getColumn(x, true));
        }
        boundkey <<","<< temp.str() <<",";
        temp.str("");
        for(int y=0; y<number; y++)
        {
            boundkey << (*it).visableScore((*it).getRow(y));
            temp << (*it).visableScore((*it).getRow(y, true));
        }
        boundkey <<","<< temp.str();
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
    ofstream file("BoundaryKeys.csv"), uniqueFile("UniqueKeys.csv");
    file<<"Top,Bottom,Left,Right,#"<<endl;
    map<string,int>::iterator it = boundMap.begin();
    for(; it!=boundMap.end(); it++)
    {
        int num = (*it).second;
        file<<(*it).first<<", "<<num<<endl;
        totalNum += num;
        totalKeys++;
        if(num==1)
        {
            totalUnique++;
            uniqueFile<<(*it).first<<endl;
        }
        if(num>upperLimit)
            upperLimit = num;

    }
    file.close();
    file.open("BoundaryStats.csv");
    file<<"Total Number of Latin Squares, "<<totalNum<<endl
        <<"Total Number of Boundaries, "<<totalKeys<<endl
        <<"Total Unique Boundaries, "<<totalUnique<<endl
        <<"Upper Limit of Branching, "<<upperLimit<<endl;
    file.close();
}

bool PartialSolver::partiallyCorrect(Puzzle& puzzle)
{
    for(int row=0; row<number; row++)
    {
        if(puzzle.row[row].first!=0)
        {
            vector<SkyScraper*> left = puzzle.getRow(row);
            if(partiallyFinished(left) &&
                puzzle.row[row].first!=puzzle.visableScore(left))
            {
                return false;
            }
        }
        if(puzzle.row[row].second!=0)
        {
            vector<SkyScraper*> right = puzzle.getRow(row,true);
            if(partiallyFinished(right) &&
                puzzle.row[row].second!=puzzle.visableScore(right))
            {
                return false;
            }
        }
    }
    for(int col=0; col<number; col++)
    {
        if(puzzle.col[col].first!=0)
        {
            vector<SkyScraper*> top = puzzle.getColumn(col);
            if(partiallyFinished(top) &&
                puzzle.col[col].first!=puzzle.visableScore(top))
            {
                return false;
            }
        }
        if(puzzle.col[col].second!=0)
        {
            vector<SkyScraper*> bottom = puzzle.getColumn(col,true);
            if(partiallyFinished(bottom) &&
                puzzle.col[col].second!=puzzle.visableScore(bottom))
            {
                return false;
            }
        }
    }
    return true;
}

bool PartialSolver::partiallyFinished(const vector<SkyScraper*>& path)
{
    bool passed = true;
    vector<SkyScraper*>::const_iterator it;
    for(it = path.begin(); it!=path.end(); it++)
    {
        if(!(*it)->found())
        {
            passed = false;
            break;
        }
        if(*(*it)==number)
            break;
    }
    return passed;
}
