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

#include "Puzzle.hh"
using namespace std;

Puzzle::Puzzle(const Puzzle& other)
{
    number = other.number;
    col = other.col;
    row = other.row;
    puzzle = Board(other.puzzle.begin(), other.puzzle.end());
}

void Puzzle::init(int num)
{
    number = num;
    puzzle = vector< vector<SkyScraper> >(number, vector<SkyScraper>(number, number));
}

bool Puzzle::solve()
{
    outside();
    return true;
}

bool Puzzle::outside()
{
    /// Check if top and bottom is >= 6, can't happen
    for(unsigned int i=0; i<col.size(); i++)
    {
        int top = col[i].first;
        int bottom = col[i].second;
        if(top+bottom > number+1) return false;
    }
    /// Check if left and right is >= 6, can't happen
    for(unsigned int i=0; i<row.size(); i++)
    {
        int left = row[i].first;
        int right = row[i].second;
        if(left+right > number+1) return false;
    }
    bool check = checkOutside();
    return check;
}

bool Puzzle::checkOutside()
{
    ///Decending from number to 1
    for(int find=number; find>0; find--)
    {
        for(int column=0; column<number; column++)
        {
            crossCheck(column,find);
        }
    }
    return true;
}

void Puzzle::crossCheck(int column, int find)
{
    int top, bottom, left, right;
    top = col[column].first;
    bottom = col[column].second;
    ///Searchs each box in the column for number
    for(int y=0; y<number; y++)
    {
        left = row[y].first;
        right = row[y].second;
        //int TopValue = visableMax(find,y,top,getColumn(column));
        int TopValue = visibility(find,y,top,getColumn(column));
        if(TopValue<top)
        {
            remove(y, column, find);
        }
        else
        {
            //int BottomValue = visableMax(find,flipValue(y),bottom,getColumn(column,true));
            int BottomValue = visibility(find,flipValue(y),bottom,getColumn(column,true));
            if(BottomValue<bottom)
            {
                remove(y, column, find);
            }
            else
            {
                //int LeftValue = visableMax(find,column,left,getRow(y));
                //int RightValue = visableMax(find,flipValue(column),right,getRow(y,true));
                int LeftValue = visibility(find,column,left,getRow(y));
                int RightValue = visibility(find,flipValue(column),right,getRow(y,true));
                if((LeftValue<left ) || (RightValue<right))
                {
                    remove(y, column, find);
                }
            }
        }
        print();
        vector<SkyScraper*> columnBox = getColumn(column);
        int index = checkOnly(find, columnBox);
        if(index!=-1)
        {
            columnBox[index]->set(find);
            minusRow(index,find);
            print();
            break;
        }
        ///Uncomment the rest for a pause
        print(); ///cin.sync(); cin.get();
    }

    bool again = true;
    while(again)
    {
        again = false;
        for(int i=0; i<number; i++)
        {
            vector<SkyScraper*> RowBox = getRow(i);
            int index = checkOnly(find, RowBox);
            if(index!=-1 && !RowBox[index]->found())
            {
                RowBox[index]->set(find);
                minusCol(index,find);
                again = true;
            }
         }
    }
}

int Puzzle::visableMax(int findNum, int place, int maxValue, vector<SkyScraper*> objects)
{
    if(objects.size()!=(unsigned int)number)
        exit(-1);
    ///Set used to keep track of numbers that are used
    std::set<int> found;
    vector<int> box(number, 0);
    ///Checks to see if number can be placed otherwise return -2
    if(objects[place]->isPossible(findNum))
        box[place] = findNum;
    else
        return -2;
    ///Add numbers already found to box and found set
    found.insert(findNum);
    for(int i=1; i<=number; i++)
    {
        int index = checkOnly(i,objects);
        if(index!=-1)
        {
            box[index] = i;
            found.insert(i);
        }
    }
    ///Going from 1 to number
    for(int find=1, object = 0; find<=number && object<number; find++)
    {
        while(objects[object]->found() || box[object]!=0)
        {
            if(objects[object]->number()>=find)
                find = objects[object]->number()+1;
            if(box[object]>=find)
                find = box[object]+1;
            object++;
            if(object>=number) break;
        }
        if(found.find(find)==found.end() && object<number && find<=number)
        {
            if(objects[object]->isPossible(find) && box[object]==0)
            {
                box[object] = find;
                object++;
                found.insert(find);
            }
        }
    }
    for(int find=number; find>0; find--)
    {
        if(found.find(find)==found.end())
        {
            for(int tempObj = (find-1); tempObj>=0; tempObj--)
            {
                if(objects[tempObj]->isPossible(find) && box[tempObj]==0)
                {
                    box[tempObj] = find;
                    break;
                }
            }
        }
    }
    int maxScore = visableScore(box);
    int minScore = visableMin(findNum,place, maxValue, objects);
    if(minScore>maxScore)
    {
        swap(maxScore,minScore);
    }
    if(maxScore<=maxValue)
    {
        return maxScore;
    }
    else
    {
        if(minScore<=maxValue)
        {
            return maxScore;
        }
        else
        {
            ///Must be zero for 0 side numbers
            return 0;
        }
    }
}

int Puzzle::visableMin(int findNum, int place, int maxValue, vector<SkyScraper*> objects)
{
    if(objects.size()!=(unsigned int)number)
        exit(-1);
    ///Set used to keep track of numbers that are used
    std::set<int> found;
    vector<int> box(number, 0);
    ///Checks to see if number can be placed otherwise return -2
    if(objects[place]->isPossible(findNum))
        box[place] = findNum;
    else
        return -2;
    ///Add numbers already found to box and found set
    found.insert(findNum);
    for(int i=1; i<=number; i++)
    {
        int index = checkOnly(i,objects);
        if(index!=-1)
        {
            box[index] = i;
            found.insert(i);
        }
    }
    ///Going from number to 1
    for(int find=number, object = 0; find>0 && object<number; find--)
    {
        while(objects[object]->found() || box[object]!=0)
        {
            if(objects[object]->number()<=find)
                find = objects[object]->number()-1;
            if(box[object]<=find)
                find = box[object]-1;
            object++;
            if(object>=number) break;
        }
        if(found.find(find)==found.end() && object<number && find>0)
        {
            for(int tempObj=object; tempObj<number; tempObj++)
            {
                if(objects[tempObj]->isPossible(find) && box[tempObj]==0)
                {
                    box[tempObj] = find;
                    if(tempObj==object)
                        object++;
                    found.insert(find);
                    break;
                }
            }
        }
    }
    for(int find=number; find>0; find--)
    {
        if(found.find(find)==found.end())
        {
            for(int tempObj = 0; tempObj<number; tempObj++)
            {
                if(objects[tempObj]->isPossible(find) && box[tempObj]==0)
                {
                    box[tempObj] = find;
                    break;
                }
            }
        }
    }
    return checkMin(objects,box,findNum);
}

int Puzzle::visibility(int findNum, int place, int maxValue, vector<SkyScraper*> objects)
{
    vector<SkyScraper> start(number,number);
    if(objects.size()!=(unsigned int)number)
        exit(-1);
    ///Checks to see if number can be placed otherwise return -2
    if(objects[place]->isPossible(findNum))
    {
        start[place].set(findNum);
        remove(findNum, start);
    }
    else
        return -2;

    int nMax(1), nMin(number);
    ///Checking for values that are already found
    for(int i=1; i<=number; i++)
    {
        int index = checkOnly(i,objects);
        if(index!=-1)
        {
            start[index].set(i);
            remove(i, start);
        }
    }
    list< vector<SkyScraper> > graph;
    graph.push_back(start);
    while(!graph.empty())
    {
        bool done = true;
        vector<SkyScraper>& path = graph.front();
        for(int i=0; i<number; i++)
        {
            if(path[i]==number)
                break;
            if(!path[i].found())
            {
                const list<int>& possible = path[i].isPossible();
                list<int>::const_iterator it = possible.begin();
                for(; it!=possible.end(); it++)
                {
                    vector<SkyScraper> temp(path);
                    temp[i].set((*it));
                    remove((*it), temp);
                    graph.push_back(temp);
                }
                done = false;
                break;
            }
        }
        if(done)
        {
            int nTemp = visableScore(path);
            nMax = max(nMax,nTemp);
            nMin = min(nMin,nTemp);
            if(nMax>=maxValue && nMin<=maxValue)
                return nMax;
        }
        graph.pop_front();
    }

    if(nMax>=maxValue && nMin<=maxValue)
    {
        return nMax;
    }
    else
    {
        ///Must be zero for 0 side numbers
        return 0;
    }
}

int Puzzle::checkMin(vector<SkyScraper*>& objects, vector<int>& box, int findNum)
{
    int top = box[0];
    for(int object=1; object<number; object++)
    {
        if(!objects[object]->found() && box[object]!=findNum)
        {
            if(box[object]==number)
                return visableScore(box);
            else if(box[object]>top && box[object]!=findNum)
            {
                for(int test = object+1; test<number; test++)
                {
                    if(box[test]<top
                       && objects[test]->isPossible(box[object])
                       && objects[object]->isPossible(box[test]))
                    {
                        swap(box[object], box[test]);
                    }
                }
            }
        }
    }
    return visableScore(box);
}

int Puzzle::visableScore(vector<int>& box)
{
    vector<int>::iterator start = box.begin(),
                          end = box.end();
    int value(0), score(0);
    for(; start!=end; start++)
    {
        if((*start)>value)
        {
            value = (*start);
            score++;
        }
        if(value==number)
        {
            return score;
        }
    }
    /// Should never reach here
    return score + (value-number);
}

int Puzzle::visableScore(vector<SkyScraper*> objects)
{
    vector<SkyScraper*>::iterator start = objects.begin(),
                                 end = objects.end();
    int value(0), score(0);
    for(; start!=end; start++)
    {
        if((*start)->number()>value)
        {
            value = (*start)->number();
            score++;
        }
        if(value==number)
        {
            return score;
        }
    }
    /// Should never reach here
    return 0;
}

int Puzzle::visableScore(vector<SkyScraper> objects)
{
    vector<SkyScraper>::iterator start = objects.begin(),
                                 end = objects.end();
    int value(0), score(0);
    for(; start!=end; start++)
    {
        if((*start).number()>value)
        {
            value = (*start).number();
            score++;
        }
        if(value==number)
        {
            return score;
        }
    }
    /// Should never reach here
    return 0;
}

vector<SkyScraper*> Puzzle::getRow(int row, bool flip)
{
    vector<SkyScraper*> box(number);
    int offset = (flip)?number-1:0;
    for(int i=0; i<number; i++)
    {
        ///Absolute for the different between the number
        box[i] = &puzzle[row][abs(i-offset)];
    }
    return box;
}

vector<SkyScraper*> Puzzle::getColumn(int column, bool flip)
{
    vector<SkyScraper*> box(number);
    int offset = (flip)?number-1:0;
    for(int i=0; i<number; i++)
    {
        ///Absolute for the different between the number
        box[i] = &puzzle[abs(i-offset)][column];
    }
    return box;
}

int Puzzle::flipValue(int value)
{
    return (number-1)-value;
}

int Puzzle::checkOnly(int num, vector<SkyScraper*>& box)
{
    int count=0;
    vector<SkyScraper*>::iterator it = box.begin(),
                                  end = box.end();
    int index=0;
    for(int i=0; it!=end; it++, i++)
    {
        if((*it)->isPossible(num)|| *(*it)==num)
        {
            count++;
            index = i;
        }
    }
    return (count!=1)?-1:index;
}

bool Puzzle::remove(int row, int column, int num)
{
    bool only = puzzle[row][column].remove(num);
    if(only)
    {
        minusCol(column, puzzle[row][column].number());
        minusRow(row, puzzle[row][column].number());
    }
    return only;
}

bool Puzzle::set(int row, int column, int num)
{
    bool pass = puzzle[row][column].set(num);
    if(pass)
    {
        minusCol(column, num);
        minusRow(row, num);
    }
    return pass;
}

void Puzzle::minusRow(int row, int num)
{
    for(int i=0; i<number; i++)
    {
        if(puzzle[row][i].remove(num))
        {
            minusRow(row, puzzle[row][i].number());
            minusCol(i, puzzle[row][i].number());
        }
    }
}

void Puzzle::minusCol(int col, int num)
{
    for(int i=0; i<number; i++)
    {
        if(puzzle[i][col].remove(num))
        {
            minusRow(i, puzzle[i][col].number());
            minusCol(col, puzzle[i][col].number());
        }
    }
}

void Puzzle::remove(int num, vector<SkyScraper>& objects)
{
    for(int i=0; i<number; i++)
    {
        objects[i].remove(num);
    }
}

void Puzzle::print(bool loopPrint)
{
    if(loopPrint)
    {
        for(unsigned int x=0; x<col.size(); x++)
        {
            for(unsigned int y=0; y<row.size(); y++)
            {
                cout<<puzzle[x][y].number();
                puzzle[x][y].printPossible();
                cout<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }
}

bool Puzzle::loadFile()
{
    ifstream input;
    string name;
    cout<<"Enter in File Name: ";
    cin>>name;
    input.open(name.c_str());
    //input.open("puzzle6_3.txt");
    if(input.fail())
    {
        return false;
    }
    int cnum;
    input>>cnum;
    ///Creates the puzzle
    init(cnum);

    vector<int> top;
    vector<int> bottom;
    for(int i=0; i<number; i++)
    {
        int temp;
        input >> temp;
        top.push_back(temp);
    }

    for(int i=0; i<number; i++)
    {
        int right, left;
        input>>right;
        for(int col=0; col<number; col++)
        {
            input.get();
            char peek;
            int intMid;
            peek = input.peek();
            if(isdigit(peek))
            {
                input>>intMid;
                set(i, col, intMid);
            }
            else
            {
                input.get();
            }
        }
        input>>left;
        row.push_back( make_pair(right,left));
    }
    for(int i=0; i<number; i++)
    {
        int num;
        input>>num;
        bottom.push_back(num);
    }
    for(unsigned int i=0; i<top.size(); i++)
    {
        col.push_back(make_pair(top[i],bottom[i]));
    }

    input.close();
    return true;
}

bool Puzzle::correct()
{
    for(int column=0; column<number; column++)
    {
        int top = col[column].first;
        int bottom = col[column].second;
        if(top!=visableScore(getColumn(column)) && top!=0)
            return false;
        else if(bottom!=visableScore(getColumn(column,true)) && bottom!=0)
            return false;
    }

    for(int rowNum=0; rowNum<number; rowNum++)
    {
        int left = row[rowNum].first;
        int right = row[rowNum].second;
        if(left!=visableScore(getRow(rowNum)) && left!=0)
            return false;
        else if(right!=visableScore(getRow(rowNum,true)) && right!=0)
            return false;
    }
    return true;
}

bool Puzzle::complete()
{
    for(int row=0; row<number; row++)
    {
        for(int col=0; col<number; col++)
        {
            if(!puzzle[row][col].found())
                return false;
        }
    }
    return true;
}
