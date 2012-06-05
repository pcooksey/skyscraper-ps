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

#include "SkyScraper.hh"

SkyScraper::SkyScraper(int num)
{
    foundNumber = 0;
    for(int i=1; i<=num; i++)
    {
        possible.push_back(i);
    }
}

SkyScraper::SkyScraper(const SkyScraper& other)
{
    foundNumber = other.foundNumber;
    possible = list<int>(other.possible.begin(),other.possible.end());
}

bool SkyScraper::checkFound()
{
    if(!foundNumber)
    {
        if(possible.size()==1)
        {
            foundNumber = possible.front();
            possible.clear();
            return true;
        }
    }
    return false;
}

void SkyScraper::init(int num)
{
    for(int i=0; i<num; i++)
    {
        possible.push_back(i);
    }
}

bool SkyScraper::remove(int num)
{
    if(!foundNumber)
    {
        possible.remove(num);
        return checkFound();
    }
    return false;
}

bool SkyScraper::set(int num)
{
    if(!found())
    {
        if(isPossible(num))
        {
            foundNumber = num;
            possible.clear();
            return true;
        }
    }
    else if(num==foundNumber)
    {
        return true;
    }
    return false;
}

bool SkyScraper::found()
{
    return foundNumber!=0;
}

bool SkyScraper::operator==(int number)
{
    return foundNumber==number;
}

bool SkyScraper::isPossible(int num) const
{
    if(num==foundNumber)
    {
        return false;
    }
    list<int>::const_iterator it;
    for(it = possible.begin(); it!=possible.end(); it++)
    {
        if((*it)==num)
        {
            return true;
        }
    }
    return false;
}

int SkyScraper::number()
{
    return foundNumber;
}

void SkyScraper::printPossible()
{
    cout<<"(";
    list<int>::iterator it;
    for(it = possible.begin(); it!=possible.end(); it++)
    {
        cout<<(*it)<<",";
    }
    cout<<")";
}
